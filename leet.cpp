#include <iostream>
#include <vector>

template <class T>
void printVector(const std::vector<T>& vec)
{
    for(const auto& e: vec)
        std::cout << e << " ";
    std::cout << "\n";
}

int main (int argc, char *argv[])
{
    std::vector<int> v = {1,2,3};
    printVector(v);
    return 0;
}
