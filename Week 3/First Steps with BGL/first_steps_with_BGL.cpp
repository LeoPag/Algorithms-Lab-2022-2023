#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int sum_weight_mst;
edge_desc e;
weight_map weights;

void dijkstra_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  int furthest_distance = 0;
    for(int k = 1; k < n; k++){
      if(dist_map[k] > furthest_distance){
        furthest_distance = dist_map[k];
      }
    }
  cout << furthest_distance << endl;
}


void kruskal( const weighted_graph &G){
  vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  sum_weight_mst = 0;
  for (vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
    sum_weight_mst += weights[*it];
  }
  cout << sum_weight_mst << " ";
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; cin >> test_cases;
  for(int i = 0; i < test_cases; i++){
    int n, m; cin >> n; cin >> m;
    weighted_graph G(n);
    weight_map weights = boost::get(boost::edge_weight, G);

    for(int j = 0; j < m; j++){
      int source, target, w; cin >> source; cin >> target; cin >> w;
      e = boost::add_edge(source,target,G).first; weights[e] =  w;
    }
    kruskal(G);
    dijkstra_dist (G,0);
  }
  return 0;
}
