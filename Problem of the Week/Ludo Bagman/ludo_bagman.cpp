// ========
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
int e,w,m,d,p,l;

void ludo_bagman(){
    // Create graph, edge adder class and propery maps
    graph G(1 + 1 + e + w + 1 + 1); //source + choice_east_node + east + west + choice_west + target
    edge_adder adder(G);
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto rc_map = boost::get(boost::edge_residual_capacity, G);
    const int v_source = e+w;
    const int v_choice_east = e+w+1;
    const int v_choice_west = e+w+2;
    const int v_target = e+w+3;

    for(int e_index = 0; e_index < e; e_index++){
      adder.add_edge(v_source, e_index, l, 0);//Connect source to east nodes for easy matches
      adder.add_edge(v_choice_east, e_index, p-l*e, 0);//Connect east_choice to east_nodes
    }

    adder.add_edge(v_source, v_choice_east, p - l*e, 0); //Connect source to choice east with residual capacity (free choice matches)

    for(int w_index = 0; w_index < w; w_index++){
      adder.add_edge(e + w_index, v_target , l, 0);//Connect west nodes to target for easy matches
      adder.add_edge(e+w_index, v_choice_west, p-l*w, 0);//Connect west_nodes to west_choice
    }

    adder.add_edge(v_choice_west, v_target , p - l*w, 0); //Connect choice west to target with residual capacity (free choice matches)

    for(int i = 0; i < m; i++){ //Matches edges -- Bipartite matching
      int east_index,west_index, risk; cin >> east_index >> west_index >> risk;
      adder.add_edge(east_index, e+west_index, 1, risk);
    }

    for(int i = 0; i < d; i++){ //Connect choice edges for each difficult matchings.
      int east_index,west_index, risk; cin >> east_index >> west_index >> risk;
      adder.add_edge(v_choice_east, v_choice_west, 1, risk);
    }

    // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
    boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
    int cost = boost::find_flow_cost(G);
    int s_flow = 0;
    out_edge_it e, eend;
    for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
        s_flow += c_map[*e] - rc_map[*e];
    }
    if(s_flow == p) cout << cost << endl;
    else cout << "No schedule!" << endl;

}


int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t; // Read the number of test cases
    for (int i = 0; i < t; ++i){
      cin >> e >> w >> m >> d >> p >> l;
      ludo_bagman(); // Solve a particular test case
    }
    return 0;
 }
