#include "RenderTree.h"

RenderTree::RenderTree()
: nodes({Node(0, 0, nullptr)}) // initialize with a first Node respresenting entity 0
, NONE(nodes[0])
{}

// returns 0 if not found
uint RenderTree::getNodeIndex(uint id)
{
    for (uint i=1; i<nodes.size(); ++i)
    {
        if (nodes[i].id == id) return i;      
    }
    return 0;
}

// returns NONE if not found
Node& RenderTree::getNodeFromIndex(uint index)
{
    if (index >= nodes.size()) return NONE;
    return nodes[index];   
}

// returns NONE (nodes[0]) if not found
Node& RenderTree::getNode(uint id)
{
    for (Node& n : nodes) 
    {
        if (n.id == id) return n;   
    }
    return NONE;
}

uint RenderTree::createNode(uint id)
{
    if ((id == 0) or (getNode(id) != NONE)) [[unlikely]]
    {
        throw std::invalid_argument("RenderTree: node with id #" 
            + std::to_string(id) + " already exists");
    }
    
    uint index = nodes.size();
    nodes.push_back(Node(id, index, this));  
    return index;
}

// cid: child id
// pid: parent id
void RenderTree::addChildTo(uint cid, uint pid)
{
    Node& p = getNode(pid);
    Node& c = getNode(cid);
    
    if (p == NONE) [[unlikely]]
        throw std::invalid_argument("RenderTree: addChildTo: node with id #" 
        + std::to_string(pid) + " doesn't exists");
    if (c == NONE) [[unlikely]]
        throw std::invalid_argument("RenderTree: addChildTo: node with id #" 
        + std::to_string(cid) + " doesn't exists");
    
    p.addChild(c);
}

std::vector<uint> RenderTree::topologicalSort()
{
    res = {};
    // std::cout << "--------" << std::endl;
    // build in_nodes
    for (Node& n : nodes)
    {
        if (n == NONE) continue;
        if (n.per_mark == true) continue;
        visit(n);
        
    }
    
    return res;
}

void RenderTree::visit(Node& n)
{
    if (n.per_mark == true) return;
    if (n.tmp_mark == true)
    {
        std::cout << "Cycle detected while sorting, aborting" << std::endl;   
        exit(-1);
    }
    n.tmp_mark = true;
    
    for (uint i : n.childs)
    {
        Node& m = getNodeFromIndex(i);
        // std::cout << n.id << " -> " << m.id << std::endl;
        visit(m);   
    }
    
    n.tmp_mark = false;
    n.per_mark = true;
    res.push_back(n.id);
}

