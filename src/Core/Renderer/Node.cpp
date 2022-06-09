#include "Node.h"
#include "RenderTree.h"

Node::Node(uint id_, uint index_, RenderTree* tree_)
: id(id_)
, index(index_)
, tree(tree_)
{}

bool Node::operator==(const Node& n)
{
    return id == n.id;
}

bool Node::operator!=(const Node& n)
{
    return id != n.id;
}


void Node::display()
{
    for (uint i : childs)
    {
        Node& n = tree->getNodeFromIndex(i);
        std::cout << id << " -> " << n.id << std::endl;
        n.display();
    }
    
    for (uint i : specials)
    {
        Node& n = tree->getNodeFromIndex(i);
        std::cout << id << " ~> " << n.id << std::endl;
    }
}

// assumes n != NONE
void Node::addChild(Node& n)
{
    // verify n isn't already a child of this node
    for (uint i : childs)
    {
        if (i == n.index)
        {
            // std::cout << "Node #" << index << " already has #" << n.id << " as a child" << std::endl;        
            return;
        }
    }
    
    if (getAscendant(n.id) == tree->NONE) [[likely]]
    {
        childs.push_back(n.index);
        n.parents.push_back(index);
    }
    else [[unlikely]] // Cycle detected
    {
        // check if node n is listed as weak-linked
        for (uint i : specials) 
            if (i == n.index) return; // already weak-linked nothing more to do   
        // otherwise add it
        specials.push_back(n.index);
        std::cout << "ERROR: Cycle forming when linking " << id << " and " << n.id << std::endl;
    }
}


Node& Node::getDescendant(uint id_)
{
    for (uint i : childs)
    {
        Node& n = tree->getNodeFromIndex(i);
        if (n.id == id_) return n;
        else return n.getDescendant(id_); // recusrive call
    }
    return tree->NONE;
}

Node& Node::getAscendant(uint id_)
{
    for (uint i : parents)
    {
        Node& n = tree->getNodeFromIndex(i);
        if (n.id == id_) return n;
        else return n.getAscendant(id_); // recusrive call
    }
    return tree->NONE;
}
