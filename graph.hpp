
#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <cmath>
#include <algorithm>

// C++ is somewhat obnoxious here:  You can't do a circular
// reference, so we declare all the classes we will use all up here
// and then declare the bodies later.

// Additionally, since we are doing things with parameterized typing
// on how we name the graph nodes, basically ALL the code needs to
// be in the header, as C++'s parameterization is effectively textual
// substitution: replacing the typename T with whatever type is actually
// used in the code.


template <class T>
class graph_node;
template <class T>
class graph_edge;
template <class T>
class graph;

// The primary class for a Graph.
//
// This implementation uses an adjacency list within each node, and the graph
// itself has a name->node mapping.

// We have to be careful here in distinguishing between std::shared_ptr and
// std::weak_ptr, in order to prevent cycles in the data structure.  The idea:
// The graph class itself has a std::shared_ptr to each node in the graph.
// Thats fine, now when the graph is removed all the nodes will be removed.

// And each node has a shared_ptr to each edge which starts or ends
// at this node, which again is fine as when a node is deleted it will
// delete its edges.

// The problem comes with the link from the edges back to the nodes.  
// If we used std::shared_ptr here, this would mean a node could never
// be deleted because an edge would refer to it, and the edge wouldn't be deleted
// because a node was referring to it, creating a cyclic structure.

// Thus we have the pointers in the edges be typed as std::weak_ptr instead.  A
// weak pointer is a "safe" placeholder.  As is, the weak pointer does not count
// as a reference for cleanup purposes, so an edge won't keep a node around needlessly.
// But a weak pointer can't also be accessed directly, instead one needs to
// convert it to a shared_pointer by either calling lock() (which returns a shared
// pointer or nullptr if the underlying object has been freed), or by calling 
// std::shared_ptr's constructor on a weak-pointer (which will throw a std::bad_weak_ptr
// exception if the data was already freed)

template <class T>
class graph : std::enable_shared_from_this<graph<T>>
{
private:
    std::unordered_map<T, std::shared_ptr<graph_node<T>>> nodes{};
    friend graph_edge<T>;
    friend graph_node<T>;

public:
    void create_node(T name)
    {
        if (nodes.contains(name))
        {
            throw std::domain_error("Node already exists");
        }
        nodes[name] = std::make_shared<graph_node<T>>(name);
    }

    void create_link(T start, T end, double weight)
    {
        if (!nodes.contains(start) || !nodes.contains(end))
        {
            throw std::domain_error("Node does not exist");
        }
        auto edge = std::make_shared<graph_edge<T>>(nodes[start], nodes[end],
                                                    weight);
        for (auto edge : nodes[start]->out_edges){
            // Our edge->end node is a weak_ptr, so we need
            // to convert it to a shared pointer 
            // before we access its fields.  Note that
            // we aren't bothering to check that the
            // conversion worked.  It would only fail if
            // someone mucked with our data structures so
            // crashing immediately afterwards with a null pointer
            // exception is Just Fine.
            auto end_node = edge->end.lock();
            if (end_node->name == end){
                throw std::domain_error("Edge already exists");
            }
        }                                                    
        nodes[start]->out_edges.insert(edge);
        nodes[end]->in_edges.insert(edge);
    }
};

// The class for the edge.  Its pretty simple, with
// just a reference to the starting node, the ending node
// and the weight on the edge.
template <class T>
class graph_edge : std::enable_shared_from_this<graph_edge<T>>
{

public:
    const double weight;
    // The big thing however is the start and edge references
    // are weak pointers rather than shared_ptrs so as to not
    // create a cycle in the reference counting system
    const std::weak_ptr<graph_node<T>> start;
    const std::weak_ptr<graph_node<T>> end;

    graph_edge(std::weak_ptr<graph_node<T>> startIn,
               std::weak_ptr<graph_node<T>> endIn,
               double weightIn) : weight(weightIn), start(startIn), end(endIn)
    {

        if (weight <= 0)
        {
            throw std::domain_error("Weights must be positive");
        }
    }
};

// And the class for the node.  This is an adjacency list approach, where
// each node has an unordered set of outward edges and a corresponding unordered
// set of inward edges.  For our traversal we are only using the outEdges, but
// we include both to enable this class to support other graph operations.
template <class T>
class graph_node : std::enable_shared_from_this<graph_node<T>>
{
private:
    std::unordered_set<std::shared_ptr<graph_edge<T>>> out_edges{};
    std::unordered_set<std::shared_ptr<graph_edge<T>>> in_edges{};
    friend graph<T>;
    friend graph_edge<T>;

public:
    const T name;

    explicit graph_node(T nameIn) : name(nameIn)
    {
    }
};


#endif