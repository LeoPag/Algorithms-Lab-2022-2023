#include <iostream>
#include <vector>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

int maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  return matching_size;
}

using namespace std;

int n;
typedef vector<int> VI;
typedef vector<VI> VVI;

void placing_knights(){
  int free_cells = 0;
  VVI chessboard(n,VI(n,-1));
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      cin >> chessboard[i][j];
      if(chessboard[i][j] == 1) free_cells += 1;
    }
  }
  graph G(n*n);
  for(int i = 0; i <n; i++){
    for(int j = 0; j < n; j++){
      if(chessboard[i][j] == 0) continue;
      if((i-1 >= 0) & (j-2 >= 0)){
        if(chessboard[i-1][j-2] ==1) boost::add_edge(i*n + j, (i-1)*n + j -2, G);
      }
      if((i-1 >= 0) & (j + 2 < n)){
        if(chessboard[i-1][j+2] ==1) boost::add_edge(i*n + j, (i-1)*n + j + 2, G);
      }
      if((i+1 < n) & (j - 2 >= 0)){
        if(chessboard[i+1][j-2] ==1) boost::add_edge(i*n + j, (i+1)*n + j - 2, G);
      }
       if((i+1 < n) & (j + 2 < n)){
        if(chessboard[i+1][j+2] ==1) boost::add_edge(i*n + j, (i+1)*n + j + 2, G);
      }
       if((i-2 >= 0) & (j - 1 >= 0)){
        if(chessboard[i-2][j-1] ==1) boost::add_edge(i*n + j, (i-2)*n + j - 1, G);
      }
       if((i-2 >= 0) & (j + 1 < n)){
        if(chessboard[i-2][j+1] ==1) boost::add_edge(i*n + j, (i-2)*n + j + 1, G);
      }
      if((i+2 < n) & (j - 1 >= 0)){
        if(chessboard[i+2][j-1] ==1) boost::add_edge(i*n + j, (i+2)*n + j - 1, G);
      }
      if((i+2 < n) & (j + 1 < n)){
        if(chessboard[i+2][j+1] ==1) boost::add_edge(i*n + j, (i+2)*n + j + 1, G);
      }
    }
  }
  int mm = maximum_matching(G);
  cout << free_cells - mm << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n;
    placing_knights();
  }
  return 0;
}
