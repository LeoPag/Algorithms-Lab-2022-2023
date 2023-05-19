#include <iostream>
#include <vector>
#include <algorithm>
// BGL includes
#include <boost/graph/biconnected_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>
using namespace std;
struct edge_component_t
{
  enum
  {
    num = 555
  };
  typedef boost::edge_property_tag kind;
} edge_component;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property,
        boost::property< edge_component_t, int> > graph;
typedef boost::graph_traits<graph>::edge_iterator edge_it;

struct Bridge{
  int edge1; int edge2;
  Bridge(int e1, int e2) : edge1(e1), edge2(e2) {}
};

bool operator < (const Bridge& b1, const Bridge& b2){
  if(b1.edge1 != b2.edge1){
    return(b1.edge1< b2.edge1);
  }
  else{
    return(b1.edge2< b2.edge2);
  }
}

typedef vector<Bridge> VB;
typedef vector<VB> VVB;

int n,m;

void important_bridges(){
  graph G(n);
  edge_it e_beg, e_end;
  int u, v;
  for(int j = 0; j < m; j++){
    cin >>u; cin >> v;
    boost::add_edge(u, v, G);
  }
  boost::property_map< graph, edge_component_t >::type component = get(edge_component, G);
  int num_comps = biconnected_components(G, component);
  int minimum,maximum;
  VVB all_bridges(num_comps);
  for(boost::tie(e_beg, e_end) = edges(G); e_beg != e_end; ++e_beg){
    minimum = min(source(*e_beg, G),target(*e_beg, G));
    maximum = max(source(*e_beg, G),target(*e_beg, G));
    Bridge edge(minimum,maximum);
    all_bridges[component[*e_beg]].push_back(edge);
  }
  int count_critical = 0;
  VB critical_bridges;
  for(auto v: all_bridges){
   if(v.size() == 1){
     count_critical += 1;
     critical_bridges.push_back(v[0]);
    }
  }
  sort(critical_bridges.begin(), critical_bridges.end());
  cout << count_critical << endl;
  for(auto bridge: critical_bridges){
    cout << bridge.edge1 << " " << bridge.edge2 << endl;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; cin >> test_cases;
  for(int i = 0; i < test_cases; i++){
    cin >> n >> m;
    important_bridges();
  }
  return 0;
}
