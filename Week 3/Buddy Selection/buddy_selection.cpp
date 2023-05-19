#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <set>
#include <algorithm>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef set<string> SS;
typedef vector<SS> VSS;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

int n,c,f;
graph G;
VSS characteristichs;

void maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map
  const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  bool all_matched = true;
  for (int i = 0; i < n; ++i) {
    if (mate_map[i] == NULL_VERTEX ){
      all_matched = false;
      break;
    }
  }
  if(all_matched == true){
    cout << "not optimal" << endl;
  }
  else{
    cout << "optimal" << endl;
  }
}
// Buddies nodes are connected only if they share f+1 characteristichs
void build_graph(){
  G = graph(n);
  characteristichs = VSS(n);
  for(int j = 0; j < n; j++){ /*Read char*/
    for(int k = 0; k < c; k++){
      string keyword; cin >> keyword;
      characteristichs[j].insert(keyword);
    }
  }
  for(int z = 0; z < n; z++){
    for(int w = z+1; w < n; w++){
        vector<string> v_intersection;
        set_intersection(characteristichs[z].begin(), characteristichs[z].end(),
                          characteristichs[w].begin(), characteristichs[w].end(),
                          back_inserter(v_intersection));
        if(v_intersection.size() >= f+1){
          boost::add_edge(z, w, G);
        }
    }
  }
}
int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; cin >> test_cases;
  for(int i = 0; i < test_cases; i++){
    cin >> n; cin>> c; cin >> f;
    build_graph();
    maximum_matching(G);
  }
return 0;
}
