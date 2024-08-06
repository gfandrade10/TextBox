#include <iostream>
#include <unordered_set>
#include <string>
#include <utility>
#include <vector>

std::pair<bool, std::string> find_next_word(std::vector<std::string>& word_list, const std::string& word)
{
    auto comp_words = [](const std::string& word1, const std::string& word2) ->bool
    {
        if(word1.size() != 4 || word2.size() != 4)
            return false;

        size_t diff_count = 0;
        std::unordered_set<char> letters_set;

        for(size_t i = 0; i < 4; i ++)
        {
            letters_set.insert(word1[i]);
            letters_set.insert(word2[i]);
            diff_count += (word1[i] != word2[i]);
        }
        if(diff_count == 1 && letters_set.size() == 5)
            return true;
        else 
            return false;
    };    

    for(auto it = word_list.begin(); it != word_list.end(); it++)
    {
        if(comp_words(*it, word))
        {
            std::string str_copy = *it;
            word_list.erase(it);
            return {true, str_copy};
        }
    }
    return {false, ""};
}

void remove_input_word(std::vector<std::string>& list, const std::string& word)
{
    for(auto it = list.begin(); it != list.end(); )
    {
        if(*it == word)
            list.erase(it);
        else 
            it++;
    }
}

int mutations(const std::vector<std::string> &alice, const std::vector<std::string> &bob, std::string word, int turn)
{
    std::vector<std::string>  alice_copy = alice;
    std::vector<std::string>  bob_copy = bob;
    std::vector<std::string>* loop_list = (turn == 0) ? &alice_copy : &bob_copy;

    bool alice_success = (turn == 0);
    bool bob_success = (turn == 1);
    bool first_round_flag = true;

    std::pair<bool, std::string> next;

    remove_input_word(alice_copy, word);
    remove_input_word(bob_copy, word);

    do
    {
        loop_start:
        next = find_next_word(*loop_list, word);

        if(next.first)
        {
            word = next.second;
            if(loop_list == &alice_copy)
                alice_success = true;
            else 
                bob_success = true;
        }
        else
        {
            if(loop_list == &alice_copy)
                alice_success = false;
            else 
                bob_success = false;
        }

        loop_list = (loop_list == &alice_copy ? &bob_copy : &alice_copy);

        if(first_round_flag)
        {
            first_round_flag = false;
            goto loop_start;
        }
    } 
    while (alice_success && bob_success);

    if(!alice_success && !bob_success)
        return -1;
    else 
        return (alice_success) ? 0 : 1;
}

int main()
{
    const std::vector<std::string> alice {"plat", "rend", "bear", "soar", "mare", "pare", "flap", "neat", "clan", "pore"};
    const std::vector<std::string> bob   {"boar", "clap", "mere", "lend", "near", "peat", "pure", "more", "plan", "soap"};
    int result = mutations(alice, bob, "flap", 1);
    std::cout << result << "\n\n";
}
