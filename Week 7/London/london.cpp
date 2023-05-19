#include <iostream>
#include <vector>
// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
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
typedef vector<int> VI;
typedef vector<VI> VVI;
int h,w;


void london(){
  VI occurrences(26,0);
  VI sums_front_letters(26,0);
  VI front_page;

  string note; cin >> note;
  int len_note = 0;
  for(char c:note){
    int char_index = c - 'A';
    occurrences[char_index] += 1;
    len_note += 1;
  }
  graph G(26);
  edge_adder adder(G);
  char letter;
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  VVI front_letters;
  for(int i = 0; i < 26; i++){
    VI new_vec(26,0);
    front_letters.push_back(new_vec);
  }
  for(int i = 0; i < h; i++){
    for(int j = 0; j < w; j++){
      cin >> letter;
      front_page.push_back(letter - 'A');
      sums_front_letters[letter - 'A'] += 1;
    }
  }
  for(int i = 0; i < h; i++){
    for(int j = w-1; j >= 0; j--){
      cin >> letter;
      front_letters[front_page[i*w + j]][letter - 'A'] += 1;
    }
  }

  for(int i = 0; i < 26; i++){
    adder.add_edge(v_source, i, sums_front_letters[i]);
    for(int j = 0; j < 26; j++){
       adder.add_edge(i, j, front_letters[i][j]);
    }
    adder.add_edge(i, v_sink, occurrences[i]);
  }

  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);

  if(flow == len_note){
    cout << "Yes" << endl;
  }
  else{
    cout << "No" << endl;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; cin >> test_cases;
  for(int i = 0; i < test_cases; i++){
    cin >> h >> w;
    london();
  }
  return 0;
}
