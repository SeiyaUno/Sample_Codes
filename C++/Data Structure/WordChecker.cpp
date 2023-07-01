// WordChecker.cpp
//
// ICS 46 Winter 2022
// Project #4: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"

#include <iostream>


WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}

//start from this one
bool WordChecker::wordExists(const std::string& word) const
{
    if(words.contains(word)) {
        return true;
    }
    else {
        return false;
    }
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    // Nonsensical code because the compiler requires the member variables
    // 'words' to be used somewhere, or else it becomes a warning (which
    // turns into an error).
    //words.isImplemented();

    std::vector<std::string> suggestions;
    AVLSet<std::string> keepUnique; //check this before adding string to suggestions
    std::string temp;

    //sawp adjacent characters
    for(int i = 0; i < word.size() - 1; i++) {
        //sawp
        temp = swapAdjacent(word, i);
        if(temp != "" && !keepUnique.contains(temp)) {
            keepUnique.add(temp);
            suggestions.push_back(temp);
        }
    }

    //insert alphabet
    for(int i = 0; i < word.size() + 1; i++) {
        for(int s = 0; s < 26; s++) {
            temp = insertCharBtw(word, i, ALPHABET[s]);
            if(temp != "" && !keepUnique.contains(temp)) {
                keepUnique.add(temp);
                suggestions.push_back(temp);
            }
        }
    }

    //delete each character
    for(int i = 0; i < word.size(); i++) {
        temp = word;
        temp.erase(i, 1);
        temp = goThroughWords(temp);
        if(temp != "" && !keepUnique.contains(temp)) {
            keepUnique.add(temp);
            suggestions.push_back(temp);
        }
    }
    
    //replace each character
    for(int i = 0; i < word.size(); i++) {
        for(int s = 0; s < 26; s++) {
            temp = word;
            temp.replace(i, 1, ALPHABET[s]);
            temp = goThroughWords(temp);
            if(temp != "" && !keepUnique.contains(temp)) {
                keepUnique.add(temp);
                suggestions.push_back(temp);
            }
        }
    }

    //split a word
    for(int i = 0; i < word.size() - 1; i++) {
        temp = splitWord(word, i);
        if(temp != "" && !keepUnique.contains(temp)) {
            keepUnique.add(temp);
            suggestions.push_back(temp);
        }
    }
    
    return suggestions;
}

//This function returns empty string if words does not contain w
std::string WordChecker::goThroughWords(const std::string w) const {
    if(words.contains(w)) {
        return w;
    }
    else {
        return "";
    }
}

std::string WordChecker::swapAdjacent(const std::string word, int index) const
{
    std::string str = word;

    //sawp adjacent character at index and index + 1
    char temp = str[index];
    str[index] = str[index + 1];
    str[index + 1] = temp;

    return goThroughWords(str);
}

std::string WordChecker::insertCharBtw(const std::string word, int index, std::string s) const
{
    std::string str = word;
    if(index == word.size()) {
        str.append(s);
    }
    else {
        str.insert(index, s);
    }

    return goThroughWords(str);
}

//Split word between index and index + 1
std::string WordChecker::splitWord(const std::string word, int length) const
{
    std::string temp1 = word;
    std::string temp2 = temp1.substr(0, length); //temp2 will contain the first part

    temp1.erase(0,temp2.size());                //temp1 will contain the second part

    temp2 = goThroughWords(temp2);
    temp1 = goThroughWords(temp1);

    if(temp2 != "" && temp1 != "") {
        return temp2 + " " + temp1;
    }
    else {
        return "";
    }
}