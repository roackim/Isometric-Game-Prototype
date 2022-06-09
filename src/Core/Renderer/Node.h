#pragma once

#include <vector>

typedef unsigned int uint;

class RenderTree;

class Node
{
public:
    void display();
    
friend class RenderTree; 
private:
    std::vector<uint> parents;
    std::vector<uint> childs;
    std::vector<uint> specials; // used to break cycles
    
    const uint id; // entity id
    const uint index;
    
    bool tmp_mark = false;  // temporary mark (used for topological sorting)
    bool per_mark = false;  // permanent mark (used for topological sorting)
    
    Node(uint id_, uint index_, RenderTree* tree_);
    bool operator==(const Node& n);
    bool operator!=(const Node& n);
    
    void addChild(Node& n);
    void addparent(Node& n);
    
    Node& getDescendant(uint id_);
    Node& getAscendant(uint id_);
    
    
private:
    // pointer to the tree container
    RenderTree* tree;
};