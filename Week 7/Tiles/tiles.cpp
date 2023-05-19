// STL includes
#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>
using namespace std;
int w , h;

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

void tiles(){
  //Build graph
  VVI bool_to_tile;
  char indicator;
  int count_to_tile = 0;
  for(int i = 0; i < h; i++){
    VI bool_row_to_tile(w,0);
    for(int j = 0; j < w; j++){
      cin >> indicator;
      if(indicator == '.'){
        bool_row_to_tile[j] = 1;
        count_to_tile += 1;
      }
    }
    bool_to_tile.push_back(bool_row_to_tile);
  }
  if(count_to_tile % 2 == 1){
    cout << "no" << endl;
    return;
  }
  graph G(h*w);
  //Setting horizontal edges when adjacent spaces are to tile on the same row
  for(int i = 0; i < h;i++){
    for(int j = 0; j < w - 1;j++){
      if((bool_to_tile[i][j] == 1) & (bool_to_tile[i][j + 1] == 1)){
        boost::add_edge(i*w + j, i*w + j + 1, G);
      }
    }
  }
  //Setting vertical edges when adjacent spaces are to tile on the same column
   for(int j = 0; j < w; j++){
    for(int i = 0; i < h - 1;i++){
      if((bool_to_tile[i][j] == 1) & (bool_to_tile[i+1][j] == 1)){
        boost::add_edge(i*w + j, (i+1)*w + j, G);
      }
    }
  }
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map
  const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();
  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  if(matching_size == count_to_tile / 2){
    cout << "yes" << endl;
  }
  else{
    cout << "no" << endl;
  }
}
int main(){
  std::ios_base::sync_with_stdio(false);
  int n; cin >> n;
  for(int i = 0; i < n; i++){
    cin >> w >> h;
    tiles();
  }
  return 0;
}
