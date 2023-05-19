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
int N,M,S;

void real_estate_market(){
  graph G(1 + N + M + S + 1);// N+M+S + source + target
  //Node 0 is source, while node 1 + N + M + S denotes target;
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  for(int i = 0; i < S; i++){
    int state_limit; cin >> state_limit;
    adder.add_edge(1 + N + M + i, 1 + N + M + S , state_limit, 0); // edges between states and target
  }
  for(int i = 0; i < M; i++){
    int state_id; cin >> state_id;
    adder.add_edge(1 + N + i, 1 + N + M + state_id - 1 , 1 , 0); // edges between sites and states
  }
  for(int i = 0; i < N; i++){
    for(int j = 0; j < M; j++){
      int cost; cin >> cost; // edges between players and sites
      adder.add_edge(1 + i, 1 + N + j, 1, -cost + 100); // adding a constant to the costs to make them all positive
    }
    adder.add_edge(0, 1 + i, 1, 0); //edges between source and players
  }

    boost::successive_shortest_path_nonnegative_weights(G, 0, 1 + N + M + S);
    // Iterate over all edges leaving the source to sum up the flow values.
    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(0,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];
    }
    int cost = boost::find_flow_cost(G);
    std::cout << s_flow <<" " << -cost + (100*s_flow) << "\n"; // 12
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int T; cin >> T;
  for(int i=0; i < T; i++ ){
    cin >> N >> M >> S;
    real_estate_market();
  }
}
