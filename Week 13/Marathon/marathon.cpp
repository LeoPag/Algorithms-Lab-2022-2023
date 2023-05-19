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
int n,m,s,f;

void marathon(){

  int max_flow = 0; //Max_flow

  graph G(n+1); //nodes + source
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  const int v_source = n;
  for(int i = 0; i < m; i++){
    int int1, int2, cap, dist;
    cin >> int1 >> int2 >> cap >> dist;
    adder.add_edge(int1, int2, cap, dist);
    adder.add_edge(int2, int1, cap, dist);
    if((int1 == s) or (int2 == s)) max_flow += cap;
  }
  //Find shortest path by forcing flow to 1;
  adder.add_edge(v_source, s, 1, 0);
  boost::successive_shortest_path_nonnegative_weights(G, v_source, f);
  int min_dist = boost::find_flow_cost(G);

  //Force max_flow to max_participants;
  adder.add_edge(v_source, s, max_flow - 1, 0); //one already added;
  adder.add_edge(s, f, max_flow, min_dist+1);
  boost::successive_shortest_path_nonnegative_weights(G, v_source, f);
  int min_cost = boost::find_flow_cost(G);
  int max_part = ((min_dist + 1) * max_flow) - min_cost;
  cout << max_part << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> s >> f;
    marathon();
  }
  return 0;
}
