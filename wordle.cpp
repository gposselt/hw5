#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <map>
#include <set>
#endif

#include "wordle.h"

#include <vector>

#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

std::string strtolower(const std::string& str, std::string output = {},  size_t i = 0) {

    if (i >= str.size() ) return output;

    char c = str[i];

    output += static_cast<char>(tolower(c));

    return strtolower(str, output, i + 1);
}


void addLocationFilters(const std::string& in, std::vector<static_location_filter*>& location_filters, size_t i = 0) {
    if (i >= in.size()) return;

    const char c = in[i];
    if (c == '-') return addLocationFilters(in, location_filters, i + 1); //skip empty characters

    location_filters.push_back(new static_location_filter{i, c});

    return addLocationFilters(in, location_filters, i + 1);
}

void addFloatingLetterOccurances(const std::string& floating, std::map<char, int>& floating_letter_occurances, std::size_t i = 0) {
    if (i >= floating.size()) return;

    char c = floating[i];

    if (floating_letter_occurances.find(c) == floating_letter_occurances.end()) {
        floating_letter_occurances.insert(std::make_pair(c, 0));
    }

    floating_letter_occurances[c] = floating_letter_occurances[c] + 1;

    return addFloatingLetterOccurances(floating, floating_letter_occurances, i + 1);
}


void addFloatingLetterFilters(
    std::vector<word_filter*>& filters,
    std::vector<static_location_filter*>& location_filters,
    std::map<char, int>& floating_letter_occurances,
    std::map<char, int>::iterator& it
    ) {

    if (it == floating_letter_occurances.end()) return;

    pair<const char, int> letter = *it;
    floating_letter_filter* filter = new floating_letter_filter{letter.first, letter.second, location_filters};

    filters.push_back(filter);

    ++it;

    addFloatingLetterFilters(filters, location_filters, floating_letter_occurances, it);
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

    addLocationFilters(in, location_filters);

    std::map<char, int> floating_letter_occurances{};

    addFloatingLetterOccurances(floating, floating_letter_occurances);

    std::map<char, int>::iterator floating_letters = floating_letter_occurances.begin();
    addFloatingLetterFilters(filters, location_filters, floating_letter_occurances, floating_letters);

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
