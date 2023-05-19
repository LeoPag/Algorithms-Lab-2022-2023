#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

typedef boost::graph_traits<graph>::edge_iterator edge_it;
// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

using namespace std;
int n,m,l;

void casino_royale(){
  graph G(1 + n + 1); //source + stations + target
  //Index 0 denotes source node while index 1 + n denotes target node;
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  auto w_map = boost::get(boost::edge_weight, G);
  for(int i = 0; i < 1 + n; i++){
    adder.add_edge(i, i + 1, l, 0 + 129);
  }
  for(int i = 0; i < m; i++){
  int start_station, end_station, priority;
  cin >> start_station >> end_station >> priority;
    adder.add_edge(1 + start_station, 1 + end_station, 1, -priority + 129* (end_station - start_station));
  } // We make cost proportional to the number of stops to keep up with the extended graph representation.

  boost::successive_shortest_path_nonnegative_weights(G, 0, 1+n);
  int cost = boost::find_flow_cost(G);

  cout << (129 * (1 + n) * l) - cost << endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> l;
    casino_royale();
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t;cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> l;
    casino_royale();
  }
}
