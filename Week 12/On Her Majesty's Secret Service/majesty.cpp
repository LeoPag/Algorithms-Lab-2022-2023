// STL includes
#include <iostream>
#include <vector>
#include <cmath>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

//DIJKSTRA GRAPH DEFINITIONS
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

#include <boost/graph/max_cardinality_matching.hpp>

//MAXIMAL MATCHING GRAPH DEFINITIONS
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
typedef vector<int> VI;
typedef vector<VI> VVI;
int n,m,a,s,c,d;

void majesty(){

  //Build graph G for Dijkstra;
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);

  edge_desc e;

  for(int i = 0; i < m; i++){
    char w; int x,y,z; cin >> w >> x >> y >> z;
    if(w == 'S'){
      e = boost::add_edge(x, y, G).first; weights[e]=z;  //Slope
    }
    else if(w == 'L'){
      e = boost::add_edge(x, y, G).first; weights[e]=z;  //Lift
      e = boost::add_edge(y, x, G).first; weights[e]=z;
    }
  }

  VI agent_positions(a);
  VI shelter_positions(s);
  for(int i = 0; i< a; i++) cin >> agent_positions[i];
  for(int i = 0; i < s; i++) cin >> shelter_positions[i];

  //Compute min travel time between each agent and ecah shelter via Dijstra SP algo;
  VVI agent_to_shelter(a);
  for(int i = 0; i< a; i++){
    int n = boost::num_vertices(G);
    std::vector<int> dist_map(n);
    boost::dijkstra_shortest_paths(G, agent_positions[i],
      boost::distance_map(boost::make_iterator_property_map(
        dist_map.begin(), boost::get(boost::vertex_index, G))));

    for(int j = 0; j < s; j++){
      int dist = dist_map[shelter_positions[j]];
      if((dist < 0) or (dist > pow(10,9))) dist = pow(10,9);
      agent_to_shelter[i].push_back(dist);
    }
  }

  //Binary search on the answer using maximal matching
  int high_end = pow(10,9) -1;
  int low_end = d;
  int best = pow(10,9) -1;
  while(low_end <= high_end){
    graph F(a + c * s); //Init new graph F;
    int mid = floor((high_end + low_end) / 2);
    for(int i = 0; i < a; i++){
      for(int j = 0; j < s; j++){
        for(int k = 0; k < c; k++){
          if(agent_to_shelter[i][j] <= mid-(k+1) * d){ //First agent can get first into shelter in time only if he is faster than 2d;
            boost::add_edge(i, a + (c*j) + k, F);
        }
        }
      }
    }
    int max_mm = maximum_matching(F);
    if(max_mm == a){
      best = min(best,mid);
      high_end = mid - 1;
    }
    else{
      low_end = mid + 1;
    }
  }
  cout << best << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> a >> s >> c >> d;
    majesty();
  }
}
