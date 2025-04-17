#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"

#include <vector>

#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

std::string strtolower(const std::string& str) {
    std::string output{};

    for (char c : str) {
        output += static_cast<char>(tolower(c));
    }

    return output;
}


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here

    std::vector<word_filter*> filters{};

    filters.push_back(new word_size_filter{in.size()});


    std::vector<static_location_filter*> location_filters{};
    for (size_t i = 0; i < in.size(); i++) {
        const char c = in[i];
        if (c == '-') continue; //skip empty characters

        location_filters.push_back(new static_location_filter{i, c});
    }

    std::map<char, int> floating_letter_occurances{};

    for (const char& a : floating) {
        char c = a;


        if (floating_letter_occurances.find(c) == floating_letter_occurances.end()) {
            floating_letter_occurances.insert(std::make_pair(c, 0));
        }

        floating_letter_occurances[c] = floating_letter_occurances[c] + 1;

    }

    for (pair<const char, int> letter : floating_letter_occurances) {
        floating_letter_filter* filter = new floating_letter_filter{letter.first, letter.second, location_filters};

        filters.push_back(filter);
    }

    std::vector<word_filter*>::iterator before_floating = filters.begin();
    ++before_floating;
    filters.insert(before_floating, location_filters.begin(), location_filters.end());

    std::set<std::string> toRemove{};
    for (const std::string& word : dict) {
        for (const word_filter* filter : filters) {
            if (!(*filter)(word)) {
                toRemove.insert(word);
                break;
            }
        }
    }


    std::set<std::string> result{};


    for (const auto & it : dict) {

        if (toRemove.find(it) == toRemove.end()) {

            std::string a = strtolower(it);

            if (a != it) {
                continue;
            }

            result.insert(strtolower(it));
        }

    }

    return result;
}

// Define any helper functions here
