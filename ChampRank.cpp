#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class rankingList
{
    public:
    using vit = std::vector<std::pair<int, int>>::iterator;

    rankingList() = delete;
    rankingList(int champ, const std::vector<std::string>& vec);
    
    void erasePosition(int pos);
    void swapElements(int a);
    vit getElementPosition(int pos);
    vit getElementID(int pos);

    private:
    std::vector<std::pair<int, int>> array;
};

rankingList::rankingList(int champ, const std::vector<std::string>& vec)
{
    int max = 0;
    for(const auto& e: vec)
    {
        if(std::all_of(e.begin(), e.end(), ::isdigit))
        {
            int tmp = std::stoi(e);
            if(tmp > max) max = tmp;
        }
    }
    if(champ > max) max = champ;
    array.resize(max);

    for(int i = 1; i <= max; ++i)
        array[i-1] = std::make_pair(i, i);
}

void rankingList::erasePosition(int pos)
{
    auto p = array.erase(getElementID(pos));
    while(p != array.end())
    {
        p->second--;
        p++;
    }
} 

void rankingList::swapElements(int a)
{
    vit pa = getElementID(a);
    vit pb = getElementPosition(pa->second - 1);
    std::swap(pa->second, pb->second);
}

rankingList::vit rankingList::getElementPosition(int pos)
{
    auto retIt = std::find_if(array.begin(), array.end(), [&](const std::pair<int, int>& p) 
        {return (p.second == pos);});
    return retIt;
}

rankingList::vit rankingList::getElementID(int pos)
{
    auto retIt = std::find_if(array.begin(), array.end(), [&](const std::pair<int, int>& p) 
        {return (p.first == pos);});
    return retIt;
}

void printEvents(int champion, const std::vector<std::string>& ev)
{
    std::cout << champion << ": ";
    for(const auto& e: ev)
        std::cout << e << " ";
    std::cout << "\n";
}

int ChampionRank(int champion,const std::string &events) 
{
    if(events.empty())
        return champion;

    std::vector<std::string> filteredEvents {};
    std::string buffer = "";
    std::istringstream is(events);

    while(is >> buffer)
    {
        filteredEvents.push_back(buffer);
    }

    printEvents(champion, filteredEvents);

    auto pos = std::find(filteredEvents.begin(), filteredEvents.end(), std::to_string(champion));
    if(pos != filteredEvents.end() && *(pos + 1) == "I")
    {
        return -1;
    }

    rankingList ranking(champion, filteredEvents);
    size_t len = filteredEvents.size();
    for(size_t i = 0; i < len; ++i)
    {
        if(filteredEvents[i] == "I")
        {
            int tmp = std::stoi(filteredEvents[i-1]);
            if(tmp == champion) return -1;
            ranking.erasePosition(tmp);
        }
        else if(filteredEvents[i] == "O")
        {
            int tmp = std::stoi(filteredEvents[i-1]);
            ranking.swapElements(tmp);
        }
    }

    return ranking.getElementID(champion)->second;
}

int main (int argc, char *argv[])
{
    int res = ChampionRank(4, "1 I 4 O 2 I");
    std::cout << "Result: " << res << "\n";
    return 0;
}
