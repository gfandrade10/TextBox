#include <boost/unordered/unordered_flat_map.hpp>
#include <iostream>
#include <string>

int main()
{
    #define BoostFlatMap boost::unordered_flat_map

    using Bit = BoostFlatMap<std::string, size_t>::const_iterator;
    BoostFlatMap<std::string, size_t> MyMap;

    MyMap["Gui"]++;
    MyMap["Gui"]++;
    MyMap["Pit"]++;

    for(Bit it = MyMap.begin(); it != MyMap.end(); ++it)
        std::cout << "{" << it->first << ", " << it->second << "}\n";
    
    #undef BoostFlatMap
    return 0;
}
