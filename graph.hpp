
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
// on how we name the Graph nodes, basically ALL the code needs to
// be in the header, as C++'s parameterization is effectively textual
// substitution: replacing the typename T with whatever type is actually
// used in the code.

template <class T>
class GraphNode;
template <class T>
class GraphEdge;
template <class T>
class Graph;

template <class T>
struct DijkstraIterationStep;
template <class T>
class DijkstraTraversal;
template <class T>
struct DijkstraTraversalIterator;

// The primary class for a Graph.

// This implementation uses an adjacency list within each node (so each node has
// a set of all edges to or from that node, with each edge knowing which
// nodes it starts and ends with), and the Graph
// itself has a name->node mapping.

// We have to be careful here in distinguishing between std::shared_ptr and
// std::weak_ptr, in order to prevent cycles in the data structure.  The idea:
// The Graph class itself has a std::shared_ptr to each node in the Graph.
// Thats fine, now when the Graph is removed all the nodes will be removed.

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
class Graph : std::enable_shared_from_this<Graph<T>>
{
private:
    // We store the name->node mapping as an unordered map.
    // The unordered map class doesn't guarentee any order
    // when iterating over the contents but it is fast: O(1)
    // expected to insert new data.

    std::unordered_map<T, std::shared_ptr<GraphNode<T>>> nodes{};

    // A set of friend declarations.
    friend GraphEdge<T>;
    friend GraphNode<T>;
    friend DijkstraTraversalIterator<T>;

    // Done so the constructor can't be called except
    // by the make_shared factory function create().
    // As a result we will never have to worry about
    // Graph objects that aren't automatically managed
    // by std::shared_ptr reference counting.
    struct Private
    {
        explicit Private() = default;
    };

public:
    // The constructor doesn't do anything since the
    // default constructor for the map creates everything
    // just fine.
    Graph(Private)
    {
    }

    // This is an example of a static "Factory" function
    // that creates instances of Graph objects as shared
    // pointers.
    static std::shared_ptr<Graph<T>> create()
    {
        return std::make_shared<Graph<T>>(Private());
    }

    // Create a new node in the graph named "name".
    // This does a fair bit of checking, making sure
    // that the name wasn't already used to create a node.
    void create_node(T name)
    {
        if (nodes.contains(name))
        {
            throw std::domain_error("Node already exists");
        }
        nodes[name] = std::make_shared<GraphNode<T>>(name);
    }

    // Creates a link between to nodes.  There can only exist
    // one link from a given start to a given end, and each link has a
    // weight that is used in the traversal.
    void create_link(T start, T end, double weight)
    {
        // Make sure that the nodes actually exist.
        if (!nodes.contains(start) || !nodes.contains(end))
        {
            throw std::domain_error("Node does not exist");
        }

        // The edge constructor uses std::weak_ptr, which the
        // shared_ptrs are automatically converted to.
        auto edge = std::make_shared<GraphEdge<T>>(nodes[start], nodes[end],
                                                   weight);
        for (auto edge : nodes[start]->out_edges)
        {
            // Our edge->end node is a weak_ptr, so we need
            // to convert it back to a shared pointer
            // before we access its fields.  Note that
            // we aren't bothering to check that the
            // conversion worked.  It would only fail if
            // someone mucked with our data structures so
            // crashing immediately afterwards with a null pointer
            // exception is Just Fine.
            auto end_node = edge->end.lock();
            if (end_node->name == end)
            {
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
class GraphEdge : std::enable_shared_from_this<GraphEdge<T>>
{

public:
    const double weight;
    // The big thing however is the start and edge references
    // are weak pointers rather than shared_ptrs so as to not
    // create a cycle in the reference counting system
    const std::weak_ptr<GraphNode<T>> start;
    const std::weak_ptr<GraphNode<T>> end;

    GraphEdge(std::weak_ptr<GraphNode<T>> startIn,
              std::weak_ptr<GraphNode<T>> endIn,
              double weightIn) : weight(weightIn), start(startIn), end(endIn)
    {
        // This algorithm needs positive weights to work...
        if (weight <= 0)
        {
            throw std::domain_error("Weights must be positive");
        }
    }
};

// And the class for the node.  This is an adjacency list approach, where
// each node has an unordered set of outward edges and a corresponding unordered
// set of inward edges.  For our traversal we are only using the outEdges, but
// we include both to enable this class to support other Graph operations.
template <class T>
class GraphNode : std::enable_shared_from_this<GraphNode<T>>
{
private:
    std::unordered_set<std::shared_ptr<GraphEdge<T>>> out_edges{};
    std::unordered_set<std::shared_ptr<GraphEdge<T>>> in_edges{};
    friend Graph<T>;
    friend GraphEdge<T>;
    friend DijkstraTraversalIterator<T>;

public:
    const T name;

    explicit GraphNode(T nameIn) : name(nameIn)
    {
    }
};

/*
 * This class is used to return step in the iteration:
 * it contains a pointer to the node, the distance to this node from
 * the start, and the prior node on the path (if this isn't the starting
 * node).
 */
template <class T>
struct DijkstraIterationStep
{
public:
    std::shared_ptr<GraphNode<T>> current;
    double distance = HUGE_VAL;
    std::shared_ptr<GraphNode<T>> previous = nullptr;

    explicit DijkstraIterationStep(std::shared_ptr<GraphNode<T>> node) : current(node)
    {
    }
};

// This is the heart of the calculation.  In C++ iterators are somewhat
// complex:  the root object needs to support begin() and end() which return
// the iteration objects themselves.  The iteration object for the beginning needs
// to support ++ (increment), * (get the current element) and != (is it at the end).
//
// The end object is effectively a dummy in this case, but it needs to be supported
// anyway.  It will create a needless empty map in the process, but ah well.
//
// Basically, in C++, a loop like

// for(auto a : b) { ... }

// gets converted into:
// auto iterStart = b.begin()
// auto iterEnd = b.end()
// while(iterStart != iterEnd; ) {
//   auto a = *iterStart;
//   ...
//   iterStart++;}

// This means that * will be called for each time through the loop
// and ++ will be called just before the ending is checked.

template <class T>
struct DijkstraTraversalIterator : std::input_iterator_tag
{
    friend DijkstraTraversal<T>;

private:
    // The working set maps GraphNodes (as shared ptrs) to the
    // associated iteration information (which contains the node, the distance,
    // and the prior node.)
    std::unordered_map<std::shared_ptr<GraphNode<T>>,
                       std::shared_ptr<DijkstraIterationStep<T>>>
        working_set;
    std::shared_ptr<DijkstraIterationStep<T>> current_node = nullptr;
    const std::shared_ptr<Graph<T>> working_graph;

    // The private constructor for the iterator.  If its the end it does nothing.
    // If it is the beginning it creates the working set and initializes all the
    // distances to +infinity, except for the start which it initializes to zero.

    // Once done it calls the intnernal iteration function once so that current_node
    // will be pointing to the first node in the traversal (which is the start node).
    // and the first iteration of the calculation will be executed.
    DijkstraTraversalIterator(std::shared_ptr<Graph<T>> graph_ptr, T start, bool is_beginning) : working_graph(graph_ptr)
    {
        // Only do the work for the beginning iterator.  The end iterator
        // is effectively a dummy.
        if (is_beginning)
        {
            if (!working_graph->nodes.contains(start))
            {
                throw std::logic_error("Unable to find the node");
            }
            // Iterator for maps return an object where the .first field is the key and
            // the .second field is the value.  So for this it is the name and the
            // GraphNode object itself.
            for (auto itr : working_graph->nodes)
            {
                auto element = std::make_shared<DijkstraIterationStep<T>>(itr.second);
                if (itr.first == start)
                {
                    element->distance = 0;
                }
                working_set[itr.second] = element;
            }
            // Does a single step of the iterator so we are all queued up
            // at the first element.
            this->iter();
        }
    }

    // And this is the heart of the iteration step.  It clears the current node
    // and first finds the closest node remaining in the working set which is the
    // new current node (or returns if nothing to do).
    //
    // If the distance for the current node is still +infinity it sets the current to
    // nullptr and returns.
    //
    // It removes that node from the working set and then for each outbound edge it looks
    // up the destination.  If that destination is in the working set, it checks the
    // distance.  If the new distance would be less it reduces the distance and updates
    // the previous node on the record.
    void iter()
    {
        current_node = nullptr;
        if (working_set.size() == 0)
        {
            return;
        }
        // The iterator for a map automatically has first as the key
        // which is the node itself and
        // second as the value, which in this case is a DijkstraTraversal
        for (auto itr : working_set)
        {
            if (current_node == nullptr)
            {
                current_node = itr.second;
            }
            if (itr.second->distance < current_node->distance)
            {
                current_node = itr.second;
            }
        }
        working_set.erase(current_node->current);
        if (current_node->distance == HUGE_VAL)
        {
            current_node = nullptr;
            return;
        }
        for (auto itr : current_node->current->out_edges)
        {
            // start and end are weak pointers so lets make the
            // shared version for the actual work here.
            auto start = itr->start.lock();
            auto end = itr->end.lock();
            if (working_set.contains(end))
            {
                auto distance = current_node->distance + itr->weight;
                if (distance < working_set[end]->distance)
                {
                    working_set[end]->distance = distance;
                    working_set[end]->previous = current_node->current;
                }
            }
        }
    }

public:
    // The ++ operator is the part that calls the iterator to
    // make sure the current node is available.
    void operator++()
    {
        iter();
    }

    // And the * operator returns the current node.
    std::shared_ptr<DijkstraIterationStep<T>> operator*()
    {
        return current_node;
    }

    // And this is "is there still data left".  The contract for the
    // iterator says that ++ is called AFTER the data is accessed and before
    // this is called, so we know it will be executed in the loop in order: If there is
    // no data left the != operation will be checked before the next call
    // to *.
    bool operator!=(DijkstraTraversalIterator &)
    {
        return current_node != nullptr;
    }
};

//
// And this is the basic shell for the above iterator.
// The constructor accepts the graph and the starting node,
// while the object itself returns the iterator using begin()
// and end().
//
// The reason why C++ requires returing TWO iterators, while
// just about every other language with iteration primatives uses
// one is because C++ bears a lot of old legacy.  Iterators were initially
// designed to do things like iterate over an array's internal storage,
// and the start and end were just pointers to the first element and one plus
// the last element, and the ++ was just doing pointer arithmatic.
template <class T>
class DijkstraTraversal
{

public:
    const std::shared_ptr<Graph<T>> working_graph;
    const T start;
    DijkstraTraversal(std::shared_ptr<Graph<T>> g, T s) : working_graph(g), start(s)
    {
    }

    DijkstraTraversalIterator<T> begin()
    {
        return DijkstraTraversalIterator<T>(working_graph, start, true);
    }

    DijkstraTraversalIterator<T> begin() const
    {
        return DijkstraTraversalIterator<T>(working_graph, start, true);
    }

    DijkstraTraversalIterator<T> end()
    {
        return DijkstraTraversalIterator<T>(working_graph, start, false);
    }

    DijkstraTraversalIterator<T> end() const
    {
        return DijkstraTraversalIterator<T>(working_graph, start, false);
    }
};

#endif