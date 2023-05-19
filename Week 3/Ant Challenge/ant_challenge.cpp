#include <iostream>
#include <vector>

using namespace std;

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;
typedef boost::graph_traits<weighted_graph>::edge_iterator              edge_it;

typedef vector<int> VI;
typedef vector<weighted_graph> VG;
typedef vector<weight_map> VMAP;
weighted_graph final_graph;
weight_map weights = boost::get(boost::edge_weight, final_graph);
VI hives;
VG store_graphs;
VMAP store_weights;
edge_desc e;
edge_it e_beg,e_end;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

void kruskal(const weighted_graph &G, int l, const weighted_graph &final_graph) {
  vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

  for (vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {

    boost::tie(e_beg, e_end) = boost::edges(final_graph);
    for (boost::tie(e_beg, e_end) = boost::edges(final_graph); e_beg != e_end; ++e_beg){ //Update the minimum
      if((boost::source(*e_beg, final_graph) == boost::source(*it, store_graphs[l])) &
          (boost::target(*e_beg, final_graph) == boost::target(*it, store_graphs[l])) &
          (weights[*e_beg] > store_weights[l][*it])){
        weights[*e_beg] = store_weights[l][*it];
      }
    }
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; cin >> test_cases;
  for(int i = 0; i < test_cases; i++){
    int n,m,s,a,b; cin >> n; cin >>m; cin >> s;cin >> a; cin >> b;
    weighted_graph final_graph(n);
    store_graphs = VG(s);
    store_weights = VMAP(s);
    hives = VI(s);
    for(int j = 0; j < m; j++){
      int source, target; cin >> source; cin>>target;
      e = boost::add_edge(source,target,final_graph).first; weights[e] = 10000000; //INIT with really large value
      for(int k = 0; k < s; k++){
        int w; cin >> w;
        store_weights[k] = boost::get(boost::edge_weight,store_graphs[k]);
        e = boost::add_edge(source,target,store_graphs[k]).first; store_weights[k][e] =  w;
      }
    }
    for(int m = 0; m < s; m++){
      int hive; cin >> hive;
      hives[m] = hive;
    }
    for(int l = 0; l < s; l++){
      kruskal(store_graphs[l],l, final_graph); //Find the private network of each species via MST kruskal algo
    }// Final graph is created by finding minimum edges of private networks
    cout << dijkstra_dist(final_graph, a, b) << "\n"; //dijkstra finds shortes path
  }
  return 0;
}
