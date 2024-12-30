#include <cstddef>
#include <cstdio>
#include <exception>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <utility>

#define PRINT_VS(X) {               \
    for(const auto& it: X)          \
        std::cout << it << "\n";    \
}

//defining scoreHolder data structure
struct scoreHolder 
{
    scoreHolder()
        : x(0), y(0), score(0.0f) {}

    scoreHolder(const scoreHolder& sc)
        : x(sc.x), y(sc.y), score(sc.score) {}

    scoreHolder(scoreHolder&& sc)
        : x(std::move(sc.x)), y(std::move(sc.y)), score(std::move(sc.score)) {}

    scoreHolder(size_t X, size_t Y, size_t SCORE)
        : x(X), y(Y), score(SCORE) {}

    template<typename T>
    scoreHolder(const T& list, 
                typename std::enable_if<std::is_same<T, std::initializer_list<size_t>>::value>::type* = nullptr)
    {
        if(list.size() == 3)
        {
            x = *(list.begin());
            y = *(list.begin() + 1);
            score = *(list.begin() + 2);
        }
        else
            throw std::invalid_argument("Initializer list must have exactly three elements.");
    }

    ~scoreHolder() = default;
    
    scoreHolder& operator=(const scoreHolder& sc)
    {
        this->x = sc.x;
        this->y = sc.y;
        this->score = sc.score;
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& output, const scoreHolder& sc)
    {
        output << sc.x << " " << sc.y << " " << sc.score;
        return output;
    }

    size_t x;
    size_t y;
    float score;
};
//end of scoreHolder

std::vector<std::string> findMoshpits(const std::vector<std::string>& dance_floor)
{
    auto danceFloor(dance_floor);
    size_t xMax = dance_floor.begin()->length();
    size_t yMax = dance_floor.size();
    auto isValidPosition = [&](int x, int y) ->bool {
        return (x >= 0 && y >= 0 && x <= ((int)xMax - 1) && y <= ((int)yMax - 1));
    };

    for(size_t y = 0; y < yMax; y++)
    {
        for(size_t x = 0; x < xMax; ++x)
        {
            if(danceFloor[y][x] == ' ')
            {
                if(isValidPosition(x+1, y) && (danceFloor[y][x+1] == ' ')
                    && isValidPosition(x, y+1) && (danceFloor[y+1][x] == ' ')
                    && isValidPosition(x+1, y+1) && (danceFloor[y+1][x+1] == ' '))
                {
                    danceFloor[y][x]     = '*';
                    danceFloor[y][x+1]   = '*';
                    danceFloor[y+1][x]   = '*';
                    danceFloor[y+1][x+1] = '*';
                }
            }
        }
    }
    
    size_t counter = 1;
    while(counter != 0)
    {
        counter = 0;
        for(size_t y = 0; y < yMax; y++)
        {
            for(size_t x = 0; x < xMax; ++x)
            {
                if(danceFloor[y][x] == ' ')
                {
                    if(isValidPosition(x+1, y) && danceFloor[y][x+1] == '*')
                    {
                        danceFloor[y][x] = '*';
                        counter++;
                    }
                    else if(isValidPosition(x, y+1) && danceFloor[y+1][x] == '*')
                    {
                        danceFloor[y][x] = '*';
                        counter++;
                    }
                }
                else if(danceFloor[y][x] == '*')
                {
                    if(isValidPosition(x+1, y) && danceFloor[y][x+1] == ' ')
                    {
                        danceFloor[y][x+1] = '*';
                        counter++;
                    }
                    else if(isValidPosition(x, y+1) && danceFloor[y+1][x] == ' ')
                    {
                        danceFloor[y+1][x] = '*';
                        counter++;
                    }
                }
            }
        }
    }
    return danceFloor;
}

std::pair<size_t, size_t> best_place(const std::vector<std::string>& dance_floor)
{
    if(dance_floor.empty())
        return {};

    size_t xMax = dance_floor.begin()->length();
    size_t yMax = dance_floor.size();

    std::vector<scoreHolder> placeScores;
    placeScores.reserve(xMax * yMax);

    auto isValidPosition = [&](int x, int y) ->bool {
        return (x >= 0 && y >= 0 && x <= ((int)xMax - 1) && y <= ((int)yMax - 1));
    };

    isValidPosition(0,0);
    std::vector<std::string> danceFloor = findMoshpits(dance_floor);

    for(size_t y = 0; y < yMax; ++y)
    {
        for (size_t x = 0; x < xMax; ++x)
        {
            float score = 0.0f;
            
        }
    }

    PRINT_VS(dance_floor);
    puts("");
    PRINT_VS(danceFloor);

    return {999, 999};
}

int main (int argc, char *argv[]) 
{
    try
    {
        std::vector<std::string> dance_floor = {"gbvKq  JfiM I", "q jecl   fvoX", "L  Foa   ygKT"};
        best_place(dance_floor);
        return 0;
    }
    catch(std::exception& e)
    {
        throw std::runtime_error(e.what());
    }
}
