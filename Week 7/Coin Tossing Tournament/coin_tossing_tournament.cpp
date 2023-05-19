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
using namespace std;
typedef vector<int> VI;
int n,m;
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


void coin_tossing_tournament(){
  graph G(m+n);
  edge_adder adder(G);
  int a,b,c,s;
  for(int i = 0; i < m; i++){
    cin >> a >> b >> c;
    if(c == 1){
      adder.add_edge(i, m+a, 1);
    }
    else if(c==2){
      adder.add_edge(i, m+b, 1);
    }
    else{
      adder.add_edge(i, m+a, 1);
      adder.add_edge(i, m+b, 1);
    }
  }
  // Add special vertices source and sink
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  VI scores(n);
  for(int i = 0; i < m; i++){
    adder.add_edge(v_source, i, 1);
  }
  int sum = 0;
  for(int i = 0; i < n; i++){
    cin >> s;
    scores[i] = s;
    adder.add_edge(m+i, v_sink, s);
    sum = sum +s;
  }

  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  if((flow == sum) & (sum == m)){
    cout << "yes" << endl;
  }
  else{
    cout << "no" << endl;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; cin >> test_cases;
  for(int i = 0; i < test_cases; i++){
    cin >> n >> m;
    coin_tossing_tournament();
  }
  return 0;
}
