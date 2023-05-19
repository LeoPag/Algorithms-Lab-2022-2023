#include <iostream>
#include <cmath>
#include <vector>
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
int b,s,p;

void fleetrace(){
  ///Build graph
  graph G(1+b+s+1);
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  for(int i = 0; i < p; i++){
    int boat, sailor, coeff; cin >> boat >> sailor >> coeff;
    adder.add_edge(1 + boat, 1 + b + sailor, 1, -coeff + 50);
  }
  for(int boat_index = 0; boat_index < b; boat_index++){
    adder.add_edge(0, 1 + boat_index , 1, 0);
    adder.add_edge(1 + boat_index, 1 + b + s , 1, 50); // Fake link to target to maximize flow;
  }
  for(int sailor_index = 0; sailor_index < s; sailor_index++){
    adder.add_edge(1 + b + sailor_index, 1 + b + s, 1, 0);
  }
  ///MCMF
  boost::successive_shortest_path_nonnegative_weights(G, 0, 1 + b + s);
  int cost = boost::find_flow_cost(G);
  int true_cost = (50 * b) - cost;
  cout << true_cost << endl;
  return;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++) {
    cin >> b >> s >> p;
    fleetrace();
  }
  return 0;
}
