#include <cstddef>
#include <ios>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

#define IDX_SIZE 8
#define LINE_SIZE 16

std::string hexdump(const std::vector<char> &data)
{
    if(data.empty())
        return {};

    size_t len = data.size();
    size_t byteCount = (len / LINE_SIZE) > 0 ? (len / LINE_SIZE + 1) : 1;
    if(byteCount > 1 && len%LINE_SIZE == 0) byteCount--;

    size_t currentPos = 0;
    std::ostringstream os;
    os << std::hex;

    auto lineCode = [](size_t val) {
        std::ostringstream os;
        os << std::hex << std::setw(IDX_SIZE) << std::setfill('0') << val << "0";
        return os.str();
    };
    
    for(size_t i = 0; i < byteCount; ++i)
    {
        os << lineCode(i) << ": ";
        std::string tempStr = "";
        tempStr.reserve(LINE_SIZE);

        for(size_t i = 0; i < LINE_SIZE; ++i) 
        {
            (currentPos < len)
                ? os << static_cast<size_t>(data[currentPos])
                : os << "  ";

            if(i < LINE_SIZE - 1) os << " ";
            else os << "  ";

            size_t tmp = (size_t)(data[currentPos]);
            if(currentPos < len)
            {
                if(tmp >= 32 && tmp <= 126)
                    tempStr.push_back((char)tmp);
                else
                    tempStr.push_back('.');
            }

            currentPos++;
        }
        
        os << tempStr;
        if(i != byteCount - 1) os << "\n";
    }

    return os.str();
}

std::vector<char> dehex(const std::string &text)
{
    static constexpr size_t startingPos = 10;

    std::vector<std::string> strings;
    auto it1 = text.begin();
    for(auto it2 = text.begin(); it2 != text.end(); ++it2)
    {
        if(*it2 == '\n')
        {
            strings.push_back(text.substr(it1-text.begin() + startingPos, it2-it1-LINE_SIZE-1));
            it1 = it2;
        }
        if(it2 == (text.end() - 1))
        {
            strings.push_back(text.substr(it1-text.begin() + startingPos, it2-it1-LINE_SIZE-1));
        }
    }
    std::vector<char> ans;
    for(const auto& e:strings)
    {
        std::istringstream is(e);
        is >> std::dec;
        std::string buffer;

        while(is >> buffer)
        {
            size_t tmp = std::stoul(buffer);
            ans.push_back((char)(tmp));
        }
    }

    return ans;
}

std::vector<char> strToArray(const std::string& str)
{
    size_t len = str.length();
    std::vector<char> arr;
    arr.resize(len);
    for(size_t i = 0; i < len; ++i)
        arr[i] = str[i];
    return arr;
}

int main (int argc, char *argv[]) 
{
    std::string name = "Guilherme Ferraz de Andrade";
    std::string dump = hexdump(strToArray(name));
    std::cout << dump << "\n";
    return 0;
}
