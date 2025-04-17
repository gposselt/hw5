#ifndef WORDLE_H
#define WORDLE_H

#ifndef RECCHECK
#include <set>
#include <string>
#endif
#include <vector>

/**
 * @brief Returns the list of all legal words in the provided dictionary
 *        that meet the criteria provided in the `in` and `floating`
 *        inputs.
 * 
 * @param [in] in - string of fixed, correct characters and `-`s to indicate
 *                  the length of string and locations that must be filled in
 * @param [in] floating  - Characters that must be used somewhere in the word
 * @param [in] dict - Dictionary of strings of legal words
 * @return std::set<std::string> - Set of all words that meet the criteria
 */
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict);

struct word_filter {
    virtual ~word_filter() = default;

    virtual bool operator()(const std::string& word) const = 0;
};

struct location_filter : word_filter {
    /**
     * Override to make it easier to code :3
     * @param word The word test to match
     * @return false iff the word could not possibly be the wordle word according to this filter
     */
    bool operator()(const std::string& word) const override {
        for (size_t i = 0; i < word.size(); i++) {
            char letter = word[i];

            if (!(*this)(i, letter)) {
                return false;
            }
        }
        return true;
    }

    /**
     *
     * @param locationInWord The location in a test word that this filter is getting applied at
     * @param letterAtLocation The letter at that location in the test word
     * @return false iff the word could not possibly be the wordle word according to this filter
     */
    virtual bool operator()(size_t locationInWord, char letterAtLocation) const = 0;
};

struct static_location_filter : location_filter {
    size_t location;
    char letter;

    static_location_filter(size_t location, char letter) : location(location), letter(letter) {}

    bool operator()(size_t locationInWord, char letterAtLocation) const override {
        if (locationInWord != this->location) {
            return true;
        }

        return letterAtLocation == letter;
    }
};

struct floating_letter_filter : word_filter {
    char letter;
    int occurrences;
    const std::vector<static_location_filter*>& filters;

    floating_letter_filter(char letter, int occurrences, const std::vector<static_location_filter*>& locations_to_avoid) : letter(letter), occurrences(occurrences), filters(locations_to_avoid) {}

    bool operator()(const std::string& word) const override {
        int letter_count = 0;

        for (size_t i = 0; i < word.size(); i++) {
            const char c = word[i];

            bool cont = false;

            for (const static_location_filter* locFilter : filters) {
                if (locFilter->location == i && (*locFilter)(i, c)) {
                    cont = true;
                    break;
                }
            }

            if (cont) {
                continue;
            }

            if (c == letter) {
                letter_count++;
            }
        }

        return letter_count >= occurrences;
    }

};

struct word_size_filter : word_filter {
    size_t size;

    explicit word_size_filter(size_t wordSize) : size(wordSize) {}

    bool operator()(const std::string& word) const override {
        return word.size() == size;
    }

};

#endif