#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <utility>
#include <iostream>
#include <vector> 
#include <string>
#include <forward_list>
#include <algorithm>


template<typename T>
class AdjacencyList  {
public:
    using EdgeList = std::forward_list<T>;
    using AdjList  = std::vector<EdgeList>;
    using NodeList = std::vector<T>;
public:
    
    AdjacencyList();
    AdjacencyList(const std::vector<T>& nodes);

    // Proxy methods for underlying AdjList's methods
    EdgeList& operator[](int i);
    const EdgeList& operator[](int i) const;
    int size() const { return l.size(); }

    // Add edge (i, j) to the Graph
    void AddEdge(int i, int j); 
    // Number of nodes n in the Graph, O(1)
    int V_size() const { return size(); }
    // Number of edges m in the Graph, O(mn)
    int E_size() const;
public:
    AdjList  l;
    NodeList nodes;
};


// Generate a complete graph of size num_nodes
template<typename T>
AdjacencyList<T> make_complete_graph(std::vector<T>& nodes) {
    AdjacencyList<T> g(nodes);
    for(int i = 0; i < nodes.size(); ++i) {
        for(int j = 0; j < nodes.size(); ++j) {
            if (i != j) g.AddEdge(i, j);
        }
    }
    return g;
}

// impls

template<typename T>
AdjacencyList<T>::AdjacencyList(const std::vector<T>& nodes) {
    this->nodes = nodes;
    l.resize(nodes.size());
}

template<typename T>
auto AdjacencyList<T>::operator[](int i) -> EdgeList& {
    if (i >= l.size()) { l.resize(i+1); }
    return l[i];
}

template<typename T>
auto AdjacencyList<T>::operator[](int i) const -> const EdgeList& { return l[i]; }

template<typename T>
int AdjacencyList<T>::E_size() const { 
    int num_edges;
    std::for_each(l.begin(), l.end(), [&num_edges](const EdgeList& x){
        num_edges += x.size();
    });
    return num_edges;
}

template<typename T>
void AdjacencyList<T>::AddEdge(int i, int j) { 
    EdgeList& edgelist = operator[](i);
    edgelist.push_front(j);
}


template<typename T>
std::ostream& operator<<(std::ostream& os, const AdjacencyList<T>& list) {
    for (int i = 0; i < list.size() ; ++i) {
        os << " [" + std::to_string(i) + "] ";
        for(const auto& v: list[i]) { os << std::to_string(v) + " "; };
        os << '\n';
    }
    return os;
}


#endif // __GRAPH_H__