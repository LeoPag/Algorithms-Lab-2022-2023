#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

using namespace std;
int n,m,k,x,y;

void tracking(){
  //Build graph and save rivers;
  weighted_graph G(n * (k+1)); //Duplicate graph
  weight_map weights = boost::get(boost::edge_weight, G);

  edge_desc e;
  for(int i = 0; i < m; i++){
    int start, end, cost, is_river; cin >> start >> end >> cost >> is_river;
    for(int j = 0; j < k+1 ; j++){
      e = boost::add_edge(start + j*n, end + j*n, G).first; weights[e]=cost;
      e = boost::add_edge(end + j*n, start + j*n, G).first; weights[e]=cost;
    }
    if(is_river){
      for(int l = 0; l <= k; l++){
         e = boost::add_edge(start + l*n, end + (l+1)*n, G).first; weights[e]=cost;
         e = boost::add_edge(end + l*n, start + (l+1)*n, G).first; weights[e]=cost;
      }
    }
  }
  cout << dijkstra_dist(G, x, y + k*n) << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> k >> x >> y;
    tracking();
  }
  return 0;
}
