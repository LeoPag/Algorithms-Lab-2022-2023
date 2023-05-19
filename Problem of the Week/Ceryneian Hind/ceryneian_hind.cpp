#include <iostream>
#include <vector>
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
int n,m;
typedef vector<int> VI;

void ceryneian_hind(){
  int pos_sum = 0;
  VI scores(n);

  graph G(1 + n + 1); //source + n + target
  edge_adder adder(G);

  const int v_source = n;
  const int v_target = n+1;

  for(int i = 0; i <n; i++) {
    cin >> scores[i];
    if(scores[i] >= 0){
      pos_sum += scores[i];
      adder.add_edge(v_source, i, scores[i]);
    }
    else adder.add_edge(i, v_target, -scores[i]);
  }

  for(int i = 0; i < m; i++){
    int from, to; cin >> from >> to;
    adder.add_edge(from, to, pow(2,30));
  }
  long flow = boost::push_relabel_max_flow(G, v_source, v_target);
  long best_score = pos_sum - flow;
  if(best_score > 0) cout << best_score << endl;
  else cout << "impossible" << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m;
    ceryneian_hind();
  }
  return 0;
}
