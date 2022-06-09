#pragma once

#include <iostream>
#include <vector>

typedef unsigned int uint;

#include "Node.h"


class RenderTree
{
public:
    std::vector<Node> nodes;
    Node& NONE; 
    
    RenderTree();
    
    Node& getNodeFromIndex(uint index); // return node from index
    Node& getNode(uint id);     // return node from id
    uint getNodeIndex(uint id); // return index from id
    uint createNode(uint id);   // create node and return its index
    
    void addChildTo(uint cid, uint pid);
    void visit(Node& n);
    
  
    std::vector<uint> topologicalSort();
    
private:
    std::vector<uint> res;
};

// L ← Empty list that will contain the sorted nodes
// while exists nodes without a permanent mark do
//     select an unmarked node n
//     visit(n)

// function visit(node n)
//     if n has a permanent mark then
//         return
//     if n has a temporary mark then
//         stop   (not a DAG)

//     mark n with a temporary mark

//     for each node m with an edge from n to m do
//         visit(m)

//     remove temporary mark from n
//     mark n with a permanent mark
//     add n to head of L