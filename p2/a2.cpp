////////////////////////////////////////////////////////////////////////////////
// File Name:      a2.cpp
//
// Author:         Gerald
// CS email:       gerald@cs.wisc.edu
//
// Description:    The source file for a2.
//
// IMPORTANT NOTE: THIS IS THE ONLY FILE THAT YOU SHOULD MODIFY FOR A2.
//                 You SHOULD NOT MODIFY any of the following:
//                   1. Name of the functions/methods.
//                   2. The number and type of parameters of the functions.
//                   3. Return type of the functions.
////////////////////////////////////////////////////////////////////////////////

#include "a2.hpp"
#include "trim.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

void cleanData(std::ifstream &inFile, std::ofstream &outFile,
               std::unordered_set<std::string> &stopwords) {
    // TODO: Implement this method.
    // # of lines of code in Gerald's implementation: 13
    // Do the following operations on each review before
    // storing it to the output file:
    //   1. Replace hyphens with spaces.
    //   2. Split the line of text into individual words.
    //   3. Remove the punctuation marks from the words.
    //   4. Remove the trailing and the leading whitespaces in each word.
    //   5. Remove the empty words.
    //   6. Remove words with just one character in them. You should NOT remove
    //      numbers in this step because if you do so, you'll lose the ratings.
    //   7. Remove stopwords.
    string line;
    while (getline(inFile, line)){
        vector<string> v1, v2;
        replaceHyphensWithSpaces(line);

        splitLine(line,v1);

        removePunctuation(v1,v2);

        removeWhiteSpaces(v2);

        removeEmptyWords(v2);

        removeSingleLetterWords(v2);

        removeStopWords(v2,stopwords);

        for (auto it : v2){
          outFile << it << " ";
        }
        outFile << endl;
    }
}

void fillDictionary(std::ifstream &newInFile,
                    std::unordered_map<std::string, std::pair<long, long>> &dict) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 20
    string line;
    while (getline(newInFile, line)){
      stringstream ss (line); //Cast string to stream
      int rating;
      string w;
      ss >> rating; //Read one word by one word
      while (ss>>w){
        if (dict.find(w) == dict.end()){ // NOT FOUND
            dict[w] = make_pair(rating, 1);
        }
        else { //FOUND
            dict[w] = make_pair(dict[w].first + rating, dict[w].second + 1);
        }
      }
    }
}


void fillStopWords(std::ifstream &inFile,
                   std::unordered_set<std::string> &stopwords) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    string word;
    while (getline(inFile, word)){
        stopwords.insert(word);
    }
}

void rateReviews(std::ifstream &testFile,
                 std::unordered_map<std::string, std::pair<long, long>> &dict,
                 std::ofstream &ratingsFile) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 20
    string line;
    ratingsFile << setprecision(2) << fixed;
    while (getline(testFile, line)){
      if (line.empty()){
        ratingsFile << "2.00" << endl;
        continue;
      }
      stringstream ss (line);
      string w;
      int count = 0;
      double rating = 0;
      while (ss>>w){
        if (dict.find(w) == dict.end()){ // NOT FOUND
            rating = rating + 2;
        }
        else { // FOUND
            rating = rating + (double)dict[w].first/dict[w].second;
        }
        count++;
      }
      ratingsFile << rating/count << endl;
    }
}

void removeEmptyWords(std::vector<std::string> &tokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    auto lambda = [](string s) {return s.empty();} ;
    tokens.erase(remove_if(tokens.begin(), tokens.end(), lambda), tokens.end());
}

void removePunctuation(std::vector<std::string> &inTokens,
                       std::vector<std::string> &outTokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 10
    for (std::vector<string>::iterator it = inTokens.begin() ; it != inTokens.end(); ++it){
      string text = *it;
      for (int i = 0, len = text.size(); i < len; i++)
      {
        if (ispunct(text[i]))
        {
            text.erase(i--, 1);
            len = text.size();
        }
      }
      outTokens.push_back(text);
    }
}

void removeSingleLetterWords(std::vector<std::string> &tokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    auto lambda = [](string text) {
      return (text.length() == 1 && text != "0" && text != "1" && text != "2"
              && text != "3" && text != "4" && text != "5" && text != "6"
              && text != "7" && text != "8" && text != "9");
    } ;
    tokens.erase(remove_if(tokens.begin(), tokens.end(), lambda), tokens.end());
}

void removeStopWords(std::vector<std::string> &tokens,
                     std::unordered_set<std::string> &stopwords) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    auto lambda = [stopwords](string s) {
      return (stopwords.find(s) != stopwords.end());
    };
    tokens.erase(remove_if(tokens.begin(), tokens.end(), lambda), tokens.end());
}

void removeWhiteSpaces(std::vector<std::string> &tokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    // You may want to use the trim() method from the trim.*pp files in a2.
    for (auto it = tokens.begin(); it != tokens.end(); it++){
      *it = trim(*it);
    }
}

void replaceHyphensWithSpaces(std::string &line) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    size_t found;
    do{
      found = line.find('-');
      if (found!=std::string::npos)
        line.replace(found, 1, 1, ' ');
    } while (found!=std::string::npos);
}

void splitLine(std::string &line, std::vector<std::string> &words) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 10
    size_t pos = 0;
    string token;
    while ((pos = line.find(' ')) != std::string::npos) {
      token = line.substr(0, pos);
      words.push_back(token);
      line.erase(0, pos + 1);
    }
    words.push_back(line);
}
