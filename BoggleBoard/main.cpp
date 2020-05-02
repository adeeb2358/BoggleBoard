//
//  main.cpp
//  BoggleBoard
//
//  Created by adeeb mohammed on 02/05/20.
//  Copyright © 2020 adeeb mohammed. All rights reserved.
//

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

/*
    to solve the boggle board we have been dividing the board into two parts
    first part storing the words to search in a trie tree and the depth first
    search graph traversal for constructing the words in the tree
    two words can use same characters in same location, but a single word
    cant use same characters.
    the space complexity of the algorithm is (w*s) w is the number of words
    s is the size of the largest word
    time complexity is (ws + (nm * 8 ^ s)
 */


class TrieNode{
public:
    std::unordered_map<char, TrieNode*> children;
    std::string word = "";
};

class Trie{
public:
    TrieNode *root;
    char endSymbol;
    void addString(std::string str){
        auto node = this->root;
        for(auto ch : str){
            if(node->children.find(ch) == node->children.end()){
                node->children.insert({ch,new TrieNode()});
            }
            node = node->children[ch];
        }
        node->children.insert({this->endSymbol,nullptr});
        node->word = str;
        //std::cout << node->word  << std::endl;
    }
    Trie(){
        this->root = new TrieNode();
        this->endSymbol = '*';
    }
};

// this is a function for retrieving all the unvisited neigbours
// there are maximum 8 numbers that can be adjascent to each node
std::vector<std::vector<int>> getNieghbours(int i,
                   int j,
                   std::vector<std::vector<char>> &board,
                   std::vector<std::vector<bool>> &isVisited
                   ){
    std::vector<std::vector<int>> neighbours;
    
    // we take the neighbours in the clockwise direction
    
    // 1st neioghbour
    if(i > 0 && j > 0){
        neighbours.push_back({i-1,j-1});
    }
    // second
    if(i < board.size() - 1 && j < board.at(0).size() - 1){
        neighbours.push_back({i+1,j+1});
    }
    // third
    if( j < board.at(0).size() - 1){
        neighbours.push_back({i,j+1});
    }
    // fourth
    if(i < board.size() - 1 ){
        neighbours.push_back({i+1,j});
    }
    //fifth
    if(i > 0 ){
        neighbours.push_back({i-1,j});
    }
    //6th
    if(j > 0){
        neighbours.push_back({i,j-1});
    }
    //7th
    if(i < board.size() - 1 && j > 0){
        neighbours.push_back({i+1,j-1});
    }
    //8th
    if(i > 0 && j < board.at(0).size() - 1){
        neighbours.push_back({i-1,j+1});
    }
    
    return neighbours;
}

void exploreEachCharacter(int i,
                          int j,
                          std::vector<std::vector<char>> &board,
                          TrieNode *trieNode,
                          std::vector<std::vector<bool>> &isVisited,
                          std::unordered_map<std::string,bool> &finalWords
                          ){
    
    // if these nodes are already visited then we dont want to conitue
    if(isVisited.at(i).at(j)){
        return;
    }
    char ch =  board.at(i).at(j);
    if (trieNode->children.find(ch) == trieNode->children.end()) {
        return;
    }
    isVisited.at(i).at(j) = true;
    trieNode = trieNode->children.at(ch);
    if (trieNode->children.find('*') != trieNode->children.end()) {
        //std::cout << trieNode->word << std::endl;
        finalWords.insert({trieNode->word,true});
    }
    auto neighbours = getNieghbours(i, j, board, isVisited);
    for (auto neighbour : neighbours) {
        exploreEachCharacter(neighbour.at(0),
                             neighbour.at(1),
                             board,
                             trieNode,
                             isVisited,
                             finalWords);
    }
    // at the end mark every node as unvisited for look of another word
    isVisited.at(i).at(j) = false;
}

std::vector<std::string> boggleBoard(std::vector<std::vector<char>> board,
                                     std::vector<std::string> words){
    std::vector<std::string> finalWordsArray;
    // insert all of our input string into the trie node
    Trie trie;
    for (auto str : words) {
        trie.addString(str);
    }
    // map for finding the found out words
    std::unordered_map<std::string,bool> finalWords;
    // visited matrix information
    std::vector<std::vector<bool>> isVisited(board.size(),
            std::vector<bool>(board.at(0).size(),false));
    
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board.at(0).size(); ++j) {
            exploreEachCharacter(i, j, board, trie.root, isVisited, finalWords);
        }
    }
    for (auto it : finalWords) {
        finalWordsArray.push_back(it.first);
    }
    return finalWordsArray;
}


int main(int argc, const char * argv[]) {
    std::cout << "Boggle board to find out the matching words" << std::endl;
    std::vector<std::vector<char>> board =
    {{'t', 'h', 'i', 's', 'i', 's', 'a'},
     {'s', 'i', 'm', 'p', 'l', 'e', 'x'},
     {'b', 'x', 'x', 'x', 'x', 'e', 'b'},
     {'x', 'o', 'g', 'g', 'l', 'x', 'o'},
     {'x', 'x', 'x', 'D', 'T', 'r', 'a'},
     {'R', 'E', 'P', 'E', 'A', 'd', 'x'},
     {'x', 'x', 'x', 'x', 'x', 'x', 'x'},
     {'N', 'O', 'T', 'R', 'E', '-', 'P'},
     {'x', 'x', 'D', 'E', 'T', 'A', 'E'}};
    std::vector<std::string> words =
    {
        "this",
        "is",
        "not",
        "a",
        "simple",
        "boggle",
        "board",
        "test",
        "REPEATED",
        "NOTRE-PEATED"
    };
    
    auto result = boggleBoard(board, words);
    for(auto word   : result){
        std::cout << word << std::endl;
    }
    return 0;
}
