
#include "Graph.h"
#include <numeric>

using namespace std;


template<typename T>
using CutType = std::pair<std::vector<T>, std::vector<T>>;


// typedefs
using Graph = AdjacencyList<int>;
using Cut = CutType<int>;



void contraction(Graph g) {

    Graph S;

    // Record set S(v) that have been contracted into v
    // Initialize S(v) = v for each v
    for(int ) {

    }

    




      iSecret = rand() % 10 + 1;

}



int main(int argc, char* argv[]) {

    srand(time(NULL));

    vector<int> nodes(3);
    iota(nodes.begin(), nodes.end(), 10);


    Graph l = make_complete_graph(nodes);

 
    cout << l;


    printf("main\n");
}
