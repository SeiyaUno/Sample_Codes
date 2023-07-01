// Digraph.hpp
//
// ICS 46 Winter 2022
// Project #5: Rock and Roll Stops the Traffic
//
// This header file declares a class template called Digraph, which is
// intended to implement a generic directed graph.  The implementation
// uses the adjacency lists technique, so each vertex stores a linked
// list of its outgoing edges.
//
// Along with the Digraph class template is a class DigraphException
// and a couple of utility structs that aren't generally useful outside
// of this header file.
//
// In general, directed graphs are all the same, except in the sense
// that they store different kinds of information about each vertex and
// about each edge; these two types are the type parameters to the
// Digraph class template.

#ifndef DIGRAPH_HPP
#define DIGRAPH_HPP

#include <exception>
#include <functional>
#include <list>
#include <map>
#include <utility>
#include <vector>

#include <stack>
#include <queue>

#include <iostream>

//**********************************
//constraints on the vertex info and edge info
//EdgeInfo has to be compatible with == operator

// DigraphExceptions are thrown from some of the member functions in the
// Digraph class template, so that exception is declared here, so it
// will be available to any code that includes this header file.

class DigraphException : public std::runtime_error
{
public:
    DigraphException(const std::string& reason);
};


inline DigraphException::DigraphException(const std::string& reason)
    : std::runtime_error{reason}
{
}



// A DigraphEdge lists a "from vertex" (the number of the vertex from which
// the edge points), a "to vertex" (the number of the vertex to which the
// edge points), and an EdgeInfo object.  Because different kinds of Digraphs
// store different kinds of edge information, DigraphEdge is a struct template.

template <typename EdgeInfo>
struct DigraphEdge
{
    int fromVertex;
    int toVertex;
    EdgeInfo einfo;

    // DigraphEdge(int fV, int tV, EdgeInfo eI) {
    //     fromVertex = fV;
    //     toVertex = tV;
    //     einfo = eI;
    // }

    // DigraphEdge(const DigraphEdge& dE) {
    //     fromVertex = dE.fromVertex;
    //     toVertex = dE.toVertex;
    //     einfo = dE.einfo;
    // }

    //  DigraphEdge() {
        
    // }

    // DigraphEdge& operator=(const DigraphEdge& dE) {
    //     fromVertex = dE.fromVertex;
    //     toVertex = dE.toVertex;
    //     einfo = dE.einfo;
    // }

    bool operator==(const DigraphEdge &other) {
        return fromVertex == other.fromVertex && toVertex == other.toVertex && einfo == other.einfo;
    }
};



// A DigraphVertex includes two things: a VertexInfo object and a list of
// its outgoing edges.  Because different kinds of Digraphs store different
// kinds of vertex and edge information, DigraphVertex is a struct template.

template <typename VertexInfo, typename EdgeInfo>
struct DigraphVertex
{
    VertexInfo vinfo;
    std::list<DigraphEdge<EdgeInfo>> edges;
};



// Digraph is a class template that represents a directed graph implemented
// using adjacency lists.  It takes two type parameters:
//
// * VertexInfo, which specifies the kind of object stored for each vertex
// * EdgeInfo, which specifies the kind of object stored for each edge
//
// You'll need to implement the member functions declared here; each has a
// comment detailing how it is intended to work.
//
// Each vertex in a Digraph is identified uniquely by a "vertex number".
// Vertex numbers are not necessarily sequential and they are not necessarily
// zero- or one-based.

template <typename VertexInfo, typename EdgeInfo>
class Digraph
{
public:
    // The default constructor initializes a new, empty Digraph so that
    // contains no vertices and no edges.
    Digraph();

    // The copy constructor initializes a new Digraph to be a deep copy
    // of another one (i.e., any change to the copy will not affect the
    // original).
    Digraph(const Digraph& d);

    // The move constructor initializes a new Digraph from an expiring one.
    Digraph(Digraph&& d) noexcept;

    // The destructor deallocates any memory associated with the Digraph.
    ~Digraph() noexcept;

    // The assignment operator assigns the contents of the given Digraph
    // into "this" Digraph, with "this" Digraph becoming a separate, deep
    // copy of the contents of the given one (i.e., any change made to
    // "this" Digraph afterward will not affect the other).
    Digraph& operator=(const Digraph& d);

    // The move assignment operator assigns the contents of an expiring
    // Digraph into "this" Digraph.
    Digraph& operator=(Digraph&& d) noexcept;

    // vertices() returns a std::vector containing the vertex numbers of
    // every vertex in this Digraph.
    std::vector<int> vertices() const;

    // edges() returns a std::vector of std::pairs, in which each pair
    // contains the "from" and "to" vertex numbers of an edge in this
    // Digraph.  All edges are included in the std::vector.
    std::vector<std::pair<int, int>> edges() const;

    // This overload of edges() returns a std::vector of std::pairs, in
    // which each pair contains the "from" and "to" vertex numbers of an
    // edge in this Digraph.  Only edges outgoing from the given vertex
    // number are included in the std::vector.  If the given vertex does
    // not exist, a DigraphException is thrown instead.
    std::vector<std::pair<int, int>> edges(int vertex) const;

    // vertexInfo() returns the VertexInfo object belonging to the vertex
    // with the given vertex number.  If that vertex does not exist, a
    // DigraphException is thrown instead.
    VertexInfo vertexInfo(int vertex) const;

    // edgeInfo() returns the EdgeInfo object belonging to the edge
    // with the given "from" and "to" vertex numbers.  If either of those
    // vertices does not exist *or* if the edge does not exist, a
    // DigraphException is thrown instead.
    EdgeInfo edgeInfo(int fromVertex, int toVertex) const;

    // addVertex() adds a vertex to the Digraph with the given vertex
    // number and VertexInfo object.  If there is already a vertex in
    // the graph with the given vertex number, a DigraphException is
    // thrown instead.
    void addVertex(int vertex, const VertexInfo& vinfo);

    // addEdge() adds an edge to the Digraph pointing from the given
    // "from" vertex number to the given "to" vertex number, and
    // associates with the given EdgeInfo object with it.  If one
    // of the vertices does not exist *or* if the same edge is already
    // present in the graph, a DigraphException is thrown instead.
    void addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo);

    // removeVertex() removes the vertex (and all of its incoming
    // and outgoing edges) with the given vertex number from the
    // Digraph.  If the vertex does not exist already, a DigraphException
    // is thrown instead.
    void removeVertex(int vertex);

    // removeEdge() removes the edge pointing from the given "from"
    // vertex number to the given "to" vertex number from the Digraph.
    // If either of these vertices does not exist *or* if the edge
    // is not already present in the graph, a DigraphException is
    // thrown instead.
    void removeEdge(int fromVertex, int toVertex);

    // vertexCount() returns the number of vertices in the graph.
    int vertexCount() const noexcept;

    // edgeCount() returns the total number of edges in the graph,
    // counting edges outgoing from all vertices.
    int edgeCount() const noexcept;

    // This overload of edgeCount() returns the number of edges in
    // the graph that are outgoing from the given vertex number.
    // If the given vertex does not exist, a DigraphException is
    // thrown instead.
    int edgeCount(int vertex) const;

    // isStronglyConnected() returns true if the Digraph is strongly
    // connected (i.e., every vertex is reachable from every other),
    // false otherwise.
    bool isStronglyConnected() const;

    // findShortestPaths() takes a start vertex number and a function
    // that takes an EdgeInfo object and determines an edge weight.
    // It uses Dijkstra's Shortest Path Algorithm to determine the
    // shortest paths from the start vertex to every other vertex
    // in the graph.  The result is returned as a std::map<int, int>
    // where the keys are vertex numbers and the value associated
    // with each key k is the predecessor of that vertex chosen by
    // the algorithm.  For any vertex without a predecessor (e.g.,
    // a vertex that was never reached, or the start vertex itself),
    // the value is simply a copy of the key.
    std::map<int, int> findShortestPaths(
        int startVertex,
        std::function<double(const EdgeInfo&)> edgeWeightFunc) const;


private:
    // Add whatever member variables you think you need here.  One
    // possibility is a std::map where the keys are vertex numbers
    // and the values are DigraphVertex<VertexInfo, EdgeInfo> objects.
    std::map<int, DigraphVertex<VertexInfo, EdgeInfo>> graph;   //int is for vertex num
    int totalVertex;
    int totalEdge;

    std::vector<bool> check; //vector for isStronglyConnected

    // You can also feel free to add any additional member functions
    // you'd like (public or private), so long as you don't remove or
    // change the signatures of the ones that already exis

};

struct shortPathInfo {
    bool pathKnown;
    double length;
    int predecessor;

    shortPathInfo() {
        pathKnown = false;
        length = std::numeric_limits<double>::infinity();
    }
};

//struct for representing a weight of each key
struct pqWeight {
    int key;
    double weight;
};

//for priority queue
class Compare {
public:
    bool operator() (pqWeight a, pqWeight b) {
        return a.weight > b.weight;
    }
};



// You'll need to implement the member functions below.  There's enough
// code in place to make them compile, but they'll all need to do the
// correct thing instead.

template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph()
{
    totalVertex = 0;
    totalEdge = 0;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(const Digraph& d)
{
    totalVertex = d.totalVertex;
    totalEdge = d.totalEdge;

    for (auto [key, value] : d.graph) {
        graph.emplace(key, value);
    }
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::Digraph(Digraph&& d) noexcept
{
    totalVertex = d.totalVertex;
    totalEdge = d.totalEdge;

    for (auto [key, value] : d.graph) {
        graph.emplace(key, value);
    }

    d.totalVertex = 0;
    d.totalEdge = 0;
    d.graph.clear();
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>::~Digraph() noexcept
{
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(const Digraph& d)
{
    if(this == &d) {    //if it is self-assigning
        return *this;
    }

    totalVertex = d.totalVertex;
    totalEdge = d.totalEdge;

    for (auto [key, value] : d.graph) {
        graph.emplace(key, value);
    }

    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
Digraph<VertexInfo, EdgeInfo>& Digraph<VertexInfo, EdgeInfo>::operator=(Digraph&& d) noexcept
{
    totalVertex = d.totalVertex;
    totalEdge = d.totalEdge;

    for (auto [key, value] : d.graph) {
        graph.emplace(key, value);
    }

    d.totalVertex = 0;
    d.totalEdge = 0;
    d.graph.clear();

    return *this;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<int> Digraph<VertexInfo, EdgeInfo>::vertices() const
{
    std::vector<int> vct;

    for (auto [key, value] : graph) {
        vct.push_back(key);
    }
    return vct;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges() const
{
    std::vector<std::pair<int, int>> vct;

    //iterate graph and iterate the list of edge on each vertex
    for(auto [key, value] : graph) {
        for(auto edge : value.edges) {
            std::pair<int, int> p = {edge.fromVertex, edge.toVertex};
            vct.push_back(p);
        }
    }
    return vct;
}


template <typename VertexInfo, typename EdgeInfo>
std::vector<std::pair<int, int>> Digraph<VertexInfo, EdgeInfo>::edges(int vertex) const
{
    std::vector<std::pair<int, int>> vct;

    if(graph.find(vertex) == graph.end()) {
        throw DigraphException{"(edges with int) No such a vertex exists"};
    }
    else {
        //iterate a list of edges
        for(auto edge : graph.at(vertex).edges) {
            std::pair<int, int> p = {edge.fromVertex, edge.toVertex};
            vct.push_back(p);
        }
    }

    return vct;
}


template <typename VertexInfo, typename EdgeInfo>
VertexInfo Digraph<VertexInfo, EdgeInfo>::vertexInfo(int vertex) const
{
    if(graph.find(vertex) == graph.end()) {
        throw DigraphException{"(vertexInfo) No such a vertex exists"};
    }
    else {
        return graph.at(vertex).vinfo;
    }
}


template <typename VertexInfo, typename EdgeInfo>
EdgeInfo Digraph<VertexInfo, EdgeInfo>::edgeInfo(int fromVertex, int toVertex) const
{
    EdgeInfo info;
    bool notFound = true;
    //check if an edge exists
    if(graph.find(fromVertex) == graph.end() || graph.find(toVertex) == graph.end()) {
        throw DigraphException{"(edgeInfo) At least one of the vertex does not exist"};
    }
    else {
        for(auto edge : graph.at(fromVertex).edges) {
            if(edge.fromVertex == fromVertex && edge.toVertex == toVertex) {
                info = edge.einfo;
                notFound = false;
            }
        }
    }

    //if any edge was not found
    if(notFound) {
        throw DigraphException{"(edgeInfo) An edge does not exist"};
    }

    return info;
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addVertex(int vertex, const VertexInfo& vinfo)
{
    if(graph.find(vertex) != graph.end()) {
        throw DigraphException{"(addVertex) A vertex with a vertex number already exists"};
    }
    else {
        //if the vertex does not exist, add a new one
        DigraphVertex<VertexInfo, EdgeInfo> v;
        v.vinfo = vinfo;
        graph.emplace(vertex, v);
        totalVertex++;
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::addEdge(int fromVertex, int toVertex, const EdgeInfo& einfo)
{
    bool found = false;
    DigraphEdge<EdgeInfo> edge;
    edge.fromVertex = fromVertex;
    edge.toVertex  = toVertex;
    edge.einfo = einfo;

    //check if an edge alreay exists
    for(auto info : graph[fromVertex].edges) {
        if(info == edge) {
            found = true;
            break;
        }
    }

    if(found) {
        throw DigraphException{"(addEdge) An edge already exists"};
    }
    else {
        graph[fromVertex].edges.emplace_back(edge);
        totalEdge++;
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeVertex(int vertex)
{
    if(graph.find(vertex) == graph.end()) {
        throw DigraphException{"(removeVertex) No such a vertex exists"};
    }

    //erase the vertex
    totalEdge -= edges(vertex).size();
    graph.erase(vertex);
    totalVertex--;

    //erase incoming edges
    for(auto v : graph) {
        typename std::list<DigraphEdge<EdgeInfo>>::iterator it = v.second.edges.begin();
        while(it != v.second.edges.end()) {
            if((*it).toVertex == vertex) {
                it = v.second.edges.erase(it);
                totalEdge--;
            }
            else {
                it++;
            }
        }
    }
}


template <typename VertexInfo, typename EdgeInfo>
void Digraph<VertexInfo, EdgeInfo>::removeEdge(int fromVertex, int toVertex)
{
    bool notRemoved = true;

    if(graph.find(fromVertex) == graph.end() || graph.find(toVertex) == graph.end()) {
        throw DigraphException{"(removeEdge) At least one of the vertex does not exist"};
    }
    else {
        //remove an edge
        typename std::list<DigraphEdge<EdgeInfo>>::iterator it = graph.at(fromVertex).edges.begin();
        while(it != graph.at(fromVertex).edges.end()) {
            if((*it).toVertex == toVertex && (*it).fromVertex == fromVertex) {
                it = graph.at(fromVertex).edges.erase(it);
                totalEdge--;
                notRemoved = false;
                break;
            }
            else {
                it++;
            }
        }
    }

    if(notRemoved) {
        //if removal was not done
        throw DigraphException{"(removeEdge) An edge does not exist"};
    }
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::vertexCount() const noexcept
{
    return totalVertex;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount() const noexcept
{
    return totalEdge;
}


template <typename VertexInfo, typename EdgeInfo>
int Digraph<VertexInfo, EdgeInfo>::edgeCount(int vertex) const
{
    return edges(vertex).size();
}


template <typename VertexInfo, typename EdgeInfo>
bool Digraph<VertexInfo, EdgeInfo>::isStronglyConnected() const
{
    for(auto m : graph) {   //visit each vertex and check
        
        //each element in this map corresponds to graph
        std::map<int, bool> vertexCheck;

        for(auto mp : graph) {
            vertexCheck.emplace(mp.first, false);
        }

        //stack to keep track of unvisited vertices
        std::stack<int> unvisited;

        //check if all the vertices are reachable from one vertex
        unvisited.push(m.first);
        while(!unvisited.empty()) {
            int v = unvisited.top();
            unvisited.pop();
            vertexCheck.at(v) = true;

            for(auto l : graph.at(v).edges) {
                if(!vertexCheck.at(l.toVertex)) {
                    unvisited.push(l.toVertex);
                }
            }
        }

        for(auto mpAfter : vertexCheck) {
            if(!mpAfter.second) {   //if there are any unvisited vertex
                return false;
            }
        }
    }

    return true;
}


template <typename VertexInfo, typename EdgeInfo>
std::map<int, int> Digraph<VertexInfo, EdgeInfo>::findShortestPaths(
    int startVertex,
    std::function<double(const EdgeInfo&)> edgeWeightFunc) const
{
    if(graph.find(startVertex) == graph.end()) {
        throw DigraphException{"(removeVertex) No such a vertex exists"};
    }

    std::map<int, int> shortestPath;

    //Create a map of struct with bool flag, lenght, and predecessor
    std::map<int, shortPathInfo> vPathInfo;
    for(auto k : graph) {
        shortPathInfo sp;
        vPathInfo.emplace(k.first, sp);
    }

    std::priority_queue<pqWeight, std::vector<pqWeight>, Compare> pq;
    pqWeight obj;
    obj.key = startVertex;
    obj.weight = 0;
    vPathInfo.at(startVertex).length = obj.weight;
    pq.push(obj);
    shortestPath.emplace(startVertex, startVertex);

    while(!pq.empty()) {
        pqWeight vtx = pq.top();
        pq.pop();

        if(!vPathInfo.at(vtx.key).pathKnown) {  //if the shortest path is not known
            vPathInfo.at(vtx.key).pathKnown = true;
            shortestPath.emplace(vtx.key, vPathInfo.at(vtx.key).predecessor);

            for(auto l : graph.at(vtx.key).edges) {
                if(vPathInfo.at(l.toVertex).length > vPathInfo.at(vtx.key).length + edgeWeightFunc(l.einfo)) {
                    vPathInfo.at(l.toVertex).length = vPathInfo.at(vtx.key).length + edgeWeightFunc(l.einfo);
                    vPathInfo.at(l.toVertex).predecessor = vtx.key;

                    //enqueue toVertex into pq with weight(length)
                    obj.key = l.toVertex;
                    obj.weight = vPathInfo.at(l.toVertex).length;
                    pq.push(obj);
                }
            }
        }
    }

    //check if all the vertices are reached
    for(auto v : graph) {
        if(shortestPath.find(v.first) == shortestPath.end()) {
            shortestPath.emplace(v.first, v.first);
        }
    }

    return shortestPath;
}



#endif

