/*
    Find Global Min-Cut in Undirected Graphs
    - O(n^2) Karger's Contraction Algorithm
    - O(n^2 (log n)^2) Karger-Stein Min Cut Algorithm 
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
typedef forward_list<int> li;
typedef vector<li> vli;
typedef pair<li, li> plili;
typedef discrete_distribution<int> dist;


void init_graph(graph_t& g, int dim) {
    g.resize(dim);
    for(int i = 0; i < dim; ++i) {
        g[i].resize(dim);
    }
}

void add_edge(graph_t& g, int i, int j, int w = 1);
void add_edge(graph_t& g, int i, int j, int w) {
    g[i][j] = w;
    g[j][i] = w;
}

void print_graph(const graph_t& g) {
    for (int i = 0; i < g.size(); ++i) {
        for (int j = 0; j < g[i].size(); ++j) {
            printf("%d ", g[i][j]); 
        }
        printf("\n");
    }
}

void print_li(const li& l) {
    for(auto v : l) {
        printf("%d ", v);
    }
}

void print_cut(const plili& cut) {
    printf("[0] ");
    print_li(cut.first);
    printf("\n[1] ");
    print_li(cut.second);
    printf("\n");
}

void print_vli(const vli& ll) {
    for(auto i = 0; i < ll.size(); ++i) {
        printf("[%d] ", i); print_li(ll[i]); printf("\n");
    }
}

int graph_size(const vli& g) {
    int size = 0;
    for(int i = 0; i < g.size(); ++i) {
        if(!g[i].empty()) size += 1;
    }
    return size;
}



int cut_set_size(const graph_t& g, const plili& c) {
    int s = 0;
    for (auto u: c.first) {
        for (auto v: c.second) {
            s += g[u][v];
        }
    }
    return s;
}


// Given l, where l[i] = S(v_i), the supernodes contracted into i
// Return the cut set, assuming |supernodes| = 2
plili make_cut(const vli& l) {
    vi sind;
    for (int i = 0; i < l.size(); ++i) {
        if (!l[i].empty()) sind.push_back(i);
    }
    assert(sind.size() == 2);
    return make_pair(l[sind[0]], l[sind[1]]);
}


// O(n^2) algorithm that outputs global min cut with probability 1 / nC2
plili karger_min_cut(graph_t g) {

    random_device rd;

    // Initialize supernodes with S(v) = {v} 
    vli supernodes(g.size());
    for (int i = 0; i < g.size(); ++i) {
        supernodes[i].push_front(i);
    }

    // O(n^2) populate array of degrees 
    vi d(g.size());
    for (int i = 0; i < g.size(); ++i) {
        for (int j = 0; j < g[i].size(); ++j) {
            d[i] += g[i][j];
        }
    }

#ifdef __debug
    int iter = 1;
#endif 

    int i = 0, j = 0;   // (i,j) is a random edge 

    // Terminates when graph contain just 2 nodes
    while (graph_size(supernodes) > 2) {

        // O(n) pick a random edge (i,j) with probability A[i,j]/m 
        i = dist(d.begin(), d.end())(rd);
        j = dist(g[i].begin(), g[i].end())(rd);
        if (i == j) continue;
        
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
        
#ifdef __debug
    printf("Iteration %d: \n", iter);
    print_vli(supernodes);
    iter += 1;
#endif 
    }

    return make_cut(supernodes);
}




// Recursive routine, similar to karger's contraction algorithm
plili min_cut_rec(const graph_t& original_g, graph_t g, vi d, vli supernodes) {

#ifdef __debug
    int iter = 1;
#endif 
    int i = 0, j = 0, n = graph_size(supernodes);
    random_device rd;

    // base case: return the cut set
    if (n == 2) 
        return make_cut(supernodes);
    
    // recursive case, run contraction and recursively call min_cut twice independently
    while (graph_size(supernodes) > (n / sqrt(2))) {
        
        // O(n) pick a random edge (i,j) with probability A[i,j]/m 
        i = dist(d.begin(), d.end())(rd);
        j = dist(g[i].begin(), g[i].end())(rd);
        if (i == j) continue;
        
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
        
#ifdef __debug
    printf("Iteration %d: \n", iter);
    print_vli(supernodes);
    print_graph(g);
    iter += 1;
#endif 
    }

    plili c1 = min_cut_rec(original_g, g, d, supernodes);
    plili c2 = min_cut_rec(original_g, g, d, supernodes);

    return (cut_set_size(original_g, c1) > 
                cut_set_size(original_g, c2)) ? c1 : c2;
}

// O(n^2 (log n)^2) algorithm that outputs global min-cut with constant probability
plili karger_stein_min_cut(graph_t g) {

    // Initialize supernodes with S(v) = {v} 
    vli supernodes(g.size());
    for (int i = 0; i < g.size(); ++i) {
        supernodes[i].push_front(i);
    }

    // O(n^2) populate array of degrees 
    vi d(g.size());
    for (int i = 0; i < g.size(); ++i) {
        for (int j = 0; j < g[i].size(); ++j) {
            d[i] += g[i][j];
        }
    }

    return min_cut_rec(g, g, d, supernodes);
}



int main(int argc, char* argv[]) {


    graph_t g = {
        {1, 1, 1, 0, 0, 0, 0, 0},
        {1, 0, 1, 1, 0, 0, 0, 0},
        {1, 1, 0, 1, 0, 0, 0, 0},
        {1, 1, 1, 0, 1, 0, 0, 0},
        {0, 0, 0, 1, 0, 1, 1, 1},
        {0, 0, 0, 0, 1, 0, 1, 1},
        {0, 0, 0, 0, 1, 1, 0, 1},
        {0, 0, 0, 0, 1, 1, 1, 0},
    };

    li s1 = {4,5,6,7}, s2 = {0,1,2,3};
    plili mincut1 = {s1, s2}, 
          mincut2 = {s2, s1};

    printf("graph:\n");
    print_graph(g);
    printf("min-cut:\n");
    print_cut(mincut1);

    auto c = karger_stein_min_cut(g);

    double num_output_correct = 0, num_ran = 1000;
    for (int i = 0; i < num_ran; ++i) {
        auto c = karger_min_cut(g);
        printf("output:\n");
        print_cut(c);
        if (c == mincut1 || c == mincut2)
            num_output_correct += 1;
    }
    printf("success probability %f\n", num_output_correct / num_ran);
}
