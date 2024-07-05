#include <cmath>
#include <cstddef>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

class maze_solver
{
    public:
    
    maze_solver() { m_position.x = 0; m_position.y = 0; m_rows = 0; }
    ~maze_solver() = default;

    enum direction { up, down, left, right, null_dir };
    struct coord 
    { 
        coord() = default;
        coord(std::initializer_list<size_t> list)
        {
            x = *list.begin();
            y = *(list.begin() + 1);
        }
        coord (const coord& c)
        {
            x = c.x;
            y = c.y;
        }
        coord& operator=(const coord& c)
        {
            x = c.x;
            y = c.y;
            return *this;
        }
        bool operator==(const coord& c)
        {
            return (x == c.x && y == c.y);
        }
        bool operator!=(const coord& c)
        {
            return (x != c.x || y != c.y);
        }
        size_t x, y; 
    };

    void update_position(coord& pos, const direction& dir)
    {
        switch (dir) 
        {
            case(up):    { if (pos.x > 0) pos.x--; break; }
            case(down):  { if (pos.x + 1 < m_rows) pos.x++; break; }
            case(left):  { if (pos.y > 0) pos.y--; break; }
            case(right): { if (pos.y + 1 < m_rows) pos.y++; break; }        
            case(null_dir): break;
        }
    }

    direction inverse_direction(const direction& dir)
    {
        switch (dir) 
        {
            case(up):       {  return down;  }
            case(down):     {  return up;    }
            case(left):     {  return right; }
            case(right):    {  return left;  }
            case(null_dir): { return null_dir; break; }
        }
        return null_dir;
    }

    void track_movement(const coord& pos, bool flag)
    {
        m_trail.push_back(std::make_pair(pos, flag));
    }

    int get_trail_size()
    {
        return m_trail.size();
    }

    inline size_t crd (const coord& point) const noexcept
    { 
        return m_rows*point.x + point.y;
    }

    void string_to_maze(const std::string& str)
    {
        m_rows = std::sqrt(str.length());
        m_maze.resize(m_rows * m_rows);
        size_t i = 0;
        for(const auto& it: str)
        {
            switch(it) 
            {
                case '.'  :   { m_maze[i] = true; i++; break; }
                case 'W'  :   { m_maze[i] = false; i++; break; }
                case '\n' :   { continue; break; }
                default   :   { throw std::runtime_error("Invalid maze\n\n"); }
            }
        }
    }

    inline void track_coord(size_t x, size_t y, bool bifurcation)
    {
        m_trail.push_back(std::make_pair(coord({x,y}), bifurcation));
    }

    inline bool is_within_bounds(const coord& point) const noexcept
    {
        return point.x < m_rows && point.y < m_rows
           && point.x >= 0 && point.y >= 0;
    }

    void print_maze()
    {
        if(m_rows)
        {
            for(size_t i = 0; i < m_rows; i++)
            {
                for(size_t j = 0; j < m_rows; j++)
                    std::cout << m_maze[crd({i,j})] << " ";
                std::cout << "\n";
            }
            std::cout << "\n";
        }
        else
            throw std::runtime_error("Maze was not set.\n\n");
    }

    void set_new_direction(coord& point)
    {
        std::vector<direction> possibilities; 
        std::mt19937 seed(m_rd());
    
        if(is_within_bounds({ point.x, point.y + 1 }) && m_maze[crd({ point.x, point.y + 1 })])
            possibilities.push_back(right);

        if(is_within_bounds({ point.x + 1, point.y }) && m_maze[crd({ point.x + 1, point.y })])
            possibilities.push_back(down);

        if(is_within_bounds({ point.x, point.y - 1 }) && m_maze[crd({ point.x, point.y - 1 })])
            possibilities.push_back(left);

        if(is_within_bounds({ point.x - 1, point.y }) && m_maze[crd({ point.x - 1, point.y })])
            possibilities.push_back(up);

        if(possibilities.empty())
            { m_no_solution_flag = true; return; }
        
        if(m_start_flag)
        {
            m_bifurcation = true;
            m_start_flag = false;
        }
        else if(possibilities.size() == 2 && m_wipe_flag)
        {
            m_bifurcation = true;
            m_previous_dir = null_dir;
        }
        else if(possibilities.size() <= 2 && !m_wipe_flag)
            m_bifurcation = false;
        else
            m_bifurcation = true;

        bool right_flag = false;
        bool down_flag = false;

        for(const auto& it: possibilities)
        {
            if(it == right)
                right_flag = true;
            if(it == down)
                down_flag = true;
        }

        if(!m_bifurcation)
        {
            if(possibilities.size() == 1 && m_previous_dir == inverse_direction(possibilities[0]))
            {
                m_next_dir = inverse_direction(m_previous_dir);
                m_previous_dir = m_next_dir;
                track_movement(point, m_bifurcation);
                wipe_path(point);
                m_wipe_flag = true;
                return;
            }
            else
            {
                for(const auto& it: possibilities)
                {
                    if(it == m_previous_dir)
                        m_next_dir = it;
                    else if(it != inverse_direction(m_previous_dir))
                        m_next_dir = it;
                }
                m_previous_dir = m_next_dir;
                update_position(point, m_next_dir);
                track_movement(point, m_bifurcation);
                m_wipe_flag = false;
                return;
            }
        }
        else 
        {
            if(right_flag && down_flag)
            {
                std::uniform_int_distribution<int> gen(0,1);
                int decider = gen(seed);
                if(decider == 0)
                    m_next_dir = down;
                else
                    m_next_dir = right;
                m_previous_dir = m_next_dir;
                update_position(point, m_next_dir);
                track_movement(point, m_bifurcation);
                m_wipe_flag = false;
                return;
            }
            else if(down_flag)
            {
                m_next_dir = down;
                m_previous_dir = m_next_dir;
                update_position(point, m_next_dir);
                track_movement(point, m_bifurcation);
                m_wipe_flag = false;
                return;
            }
            else if(right_flag)
            {
                m_next_dir = right;
                m_previous_dir = m_next_dir;
                update_position(point, m_next_dir);
                track_movement(point, m_bifurcation);
                m_wipe_flag = false;
                return;
            }
            else
            {
                std::uniform_int_distribution<int> gen(0,(int)possibilities.size()-1);
                m_next_dir = possibilities[gen(seed)];
                m_previous_dir = m_next_dir;
                update_position(point, m_next_dir);
                track_movement(point, m_bifurcation);
                m_wipe_flag = false;
                return;
            }
        }
    }

    void wipe_path(coord& point)
    {
        if(!m_trail.empty())
        {
            m_maze[crd(point)] = false;
            m_trail.pop_back();
            point = m_trail.back().first;
            m_counter--;
        }
    }

    int solution()
    {
        std::vector<int> move_counter;
        std::mt19937 seed(m_rd());
        std::uniform_int_distribution<size_t> gen(0,1);
        m_end = {m_rows - 1, m_rows - 1};
        m_no_solution_flag = false;
        m_wipe_flag = false;
        m_start_flag = true;

        for(size_t i = 0; i < 250; i++)
        {
            m_counter = 0;
            m_previous_dir = null_dir;

            while(m_position != m_end && m_counter < (int)(m_rows * m_rows * m_rows))
            {
                set_new_direction(m_position);
                m_counter++;
            }
            m_position = {0,0};
            move_counter.push_back(m_counter);
            if(m_no_solution_flag)
                return  -1;
        }

        int res = *std::min_element(move_counter.begin(), move_counter.end());

        if(res > (int)(m_rows * m_rows) || move_counter.size() == 0)
            return - 1;
        else
            return res;
    }

    private:
    int m_counter;
    size_t m_rows;
    coord m_position;
    coord m_end;
    bool m_bifurcation;
    bool m_no_solution_flag;
    bool m_wipe_flag;
    bool m_start_flag;
    direction m_previous_dir;
    direction m_next_dir;
    std::vector<std::pair<coord, bool>> m_trail;
    std::vector<bool> m_maze;
    std::random_device m_rd;
};

int path_finder(const std::string& maze)
{
    maze_solver solver;
    solver.string_to_maze(maze);
    return solver.solution();
}

std::string file_to_string(std::string&& file)
{
    std::fstream file_reader;
    file_reader.open(file);
    std::string ans, buffer;
    while (std::getline(file_reader, buffer))
        ans += buffer + '\n';
    ans.pop_back();
    file_reader.close();
    return ans;
}

int main()
{
    std::string maze = file_to_string("maze.txt");    
    std::cout << maze << "\n\n";

    maze_solver obj;
    obj.string_to_maze(maze);
    obj.print_maze();
    std::cout << path_finder(maze) << "\n\n";
}
