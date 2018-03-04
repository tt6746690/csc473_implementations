
#include "Graph.h"
#include <numeric>

using namespace std;


int main(int argc, char* argv[]) {

    vector<int> v(10);
    iota(v.begin(), v.end(), 0);

    adjacency_list<int> l;
    l[0].push_front(1);
    cout << l;


    printf("main\n");
}
