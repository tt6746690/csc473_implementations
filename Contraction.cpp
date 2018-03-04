/*
    O(n^2) implementation of contraction algorithm for finding global min-cut in undirected graphs.
*/
#include "Macro.h"
#include <numeric>
#include <iostream>
#include <vector>
#include <random>
#include <iterator>
#include <forward_list>
#include <utility>
#include <cassert>


using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vvi graph_t;
typedef vector<forward_list<int>> vli;
typedef pair<forward_list<int>, forward_list<int>> plili;
typedef discrete_distribution<int> dist;


void init_graph(graph_t& g, int dim) {
    g.resize(dim);
    for(int i = 0; i < dim; ++i) {
        g[i].resize(dim);
    }
}

void add_edge(graph_t& g, int i, int j) {
    g[i][j] = 1;
    g[j][i] = 1;
}

void print_graph(const graph_t& g) {
    for (int i = 0; i < g.size(); ++i) {
        for (int j = 0; j < g[i].size(); ++j) {
            printf("%d ", g[i][j]); 
        }
        printf("\n");
    }
}

void print_cut(const plili& cut) {
    printf("[0] ");
    for(auto v : cut.first) {
        printf("%d ", v);
    }
    printf("\n[1] ");
    for(auto v : cut.second) {
        printf("%d ", v);
    }
    printf("\n");
}

int graph_size(const vli& g) {
    int size = 0;
    for(int i = 0; i < g.size(); ++i) {
        if(!g[i].empty()) size += 1;
    }
    return size;
}


plili contraction(graph_t g) {

    default_random_engine gen;
    gen.seed(time(NULL));

    // Initialize supernodes with S(v) = {v} 
    vli supernodes(g.size());
    for (int i = 0; i < g.size(); ++i) {
        supernodes[i].push_front(i);
    }

    // O(n^2) populate array of degrees 
    vi d(g.size());
    for(int i = 0; i < g.size(); ++i) {
        for(int j = 0; j < g[i].size(); ++j) {
            d[i] += g[i][j];
        }
    }

    // Terminates when graph contain just 2 nodes
    while(graph_size(supernodes) > 2) {

        // O(n) pick a random edge (i,j) with probability A[i,j]/m 
        dist d1(d.begin(), d.end());
        auto i = d1(gen);
        dist d2(g[i].begin(), g[i].end());
        auto j = d2(gen);

        // O(n) Add j-column to i-th column 
        for (int k = 0; k < g.size(); ++k) {
            g[k][i] += g[k][j];
        }
        // O(n) Add j-th row to i-th row
        for (int k = 0; k < g.size(); ++k) {
            g[i][k] += g[j][k];
        }
        // O(n) Zero out j-th row and column 
        for (int k = 0; k < g.size(); ++k) {
            g[j][k] = 0;
            g[k][j] = 0;
        }
        // O(1) update degree with i representing the supernode for (i,j)
        d[i] += d[j] - g[i][j];
        d[j] = 0;

        // O(1) update supernodes
        supernodes[i].splice_after(supernodes[i].before_begin(), supernodes[j]);
    }

    vi sind;
    for(int i = 0; i < supernodes.size(); ++i) {
        if (!supernodes[i].empty()) sind.push_back(i);
    }
    assert(sind.size() == 2);
    return make_pair(supernodes[sind[0]], supernodes[sind[1]]);
}



int main(int argc, char* argv[]) {


    vector<int> nodes(10);
    iota(nodes.begin(), nodes.end(), 10);

    graph_t g;
    init_graph(g, nodes.size());

    for(int i = 0; i < nodes.size(); ++i) {
        for(int j = 0; j < nodes.size(); ++j) {
            if (i != j) add_edge(g, i, j);
        }
    }

    print_graph(g);
    auto c = contraction(g);
    print_cut(c);

}
