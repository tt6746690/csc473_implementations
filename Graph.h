#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <iostream>
#include <vector> 
#include <string>
#include <forward_list>


template<typename T>
class adjacency_list  {
public:
    using EdgeList = std::forward_list<T>;
    using NodeList = std::vector<EdgeList>;
    NodeList l;
public:
    EdgeList& operator[](int i);
    const EdgeList& operator[](int i) const;
    int size() const { return l.size(); }
};


template<typename T>
auto adjacency_list<T>::operator[](int i) -> EdgeList& {
    if (i >= l.size()) { l.resize(i+1); }
    return l[i];
}

template<typename T>
auto adjacency_list<T>::operator[](int i) const -> const EdgeList& { return l[i]; }



template<typename T>
std::ostream& operator<<(std::ostream& os, const adjacency_list<T>& list) {
    for (int i = 0; i < list.size() ; ++i) {
        os << " [" + std::to_string(i) + "] ";
        for(const auto& v: list[i]) { os << std::to_string(v); };
        os << '\n';
    }
    return os;
}


#endif // __GRAPH_H__