#include <iostream>
// BGL include
#include <boost/graph/adjacency_list.hpp>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>
// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};
using namespace std;
int m,n,c,k;


void knights(){
  graph G(1 + 2*m*n + 1); //source + real_nodes + fake_nodes + target
  edge_adder adder(G);

  const int v_source = 2*m*n;
  const int v_target = 1 + 2*m*n;

  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      adder.add_edge(i*m + j, (m*n) + i*m + j, c);//Connecting real nodes to fake nodes with capacity C
      if(((i == 0) && (j == 0)) or ((i == n-1) &&(j == 0)) or ((i == 0) && (j == m-1)) or ((i == n-1) && (j == m-1))){
        adder.add_edge(m*n + i*m + j, v_target, 2);//Connect corner fake nodes to target (exit)
      }
      else if((i == 0) or( i == n-1) or (j == 0) or (j == m-1)){
        adder.add_edge(m*n + i*m + j, v_target, 1); //Connect boundary edges (not corners) fake nodes to target (exit)
      }
      if(i > 0) adder.add_edge(m*n + i*m + j, (i-1)*m + j, 1);
      if(i < n-1)adder.add_edge(m*n + i*m + j, (i+1)*m + j, 1);
      if(j > 0) adder.add_edge(m*n + i*m + j, i*m + j - 1, 1);
      if(j < m-1) adder.add_edge(m*n + i*m + j, i*m + j + 1, 1);
    }
  }
  for(int i = 0; i < k; i++){
    int col, row; cin >> col >> row;
    adder.add_edge(v_source, row*m + col, 1);
  }
  long flow = boost::push_relabel_max_flow(G, v_source, v_target);
  cout << flow << "\n";
}


int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t; // Read the number of test cases
    for (int i = 0; i < t; ++i){
        cin >> m >> n >> k >> c;
        knights(); // Solve a particular test case

     }
}
