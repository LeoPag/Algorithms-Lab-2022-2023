// Includes
// ========
#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <map>
#include <tuple>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>
//GraphTypewithnestedinterioredgepropertiesforCostFlowAlgorithms
typedef boost::adjacency_list_traits<boost::vecS,boost::vecS,boost::directedS>traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator
// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

using namespace std;
int N,S;
typedef vector<int> VI;
typedef set<int> SI;
typedef vector<SI> VSI;
typedef map<pair<int,int>,int> MINTS;
typedef tuple<int,int,int,int,int> PENTAINT;
typedef vector<PENTAINT> VPI;

void car_sharing(){
  const int BASELINE_COST = 101;

  VI starting_cars(S);
  int total_cars = 0;
  for(int i = 0; i < S; i++) {
    cin >> starting_cars[i];
    total_cars += starting_cars[i];
  }

  //Read and store requests
  VPI requests;
  VSI unique_timestamps(S);
  for(int i = 0; i< N;i++){
    int si , ti , di , ai , pi;
    cin >> si >> ti >> di >> ai >> pi;
    PENTAINT new_request(si,ti,di,ai,pi);
    requests.push_back(new_request);
    unique_timestamps[si - 1].insert(di);
    unique_timestamps[ti - 1].insert(ai);
  }

  //Insert 0 and 100000 in every set to  make things easier
  for(int i = 0; i < S; i++){
    unique_timestamps[i].insert(0);
    unique_timestamps[i].insert(100000);
  }

  MINTS m;
  int count_vertices = 0;
  for(int i = 0; i < S; i++){
    for(auto it = unique_timestamps[i].begin(); it != unique_timestamps[i].end(); it++){
      m.insert(make_pair(make_pair(i,*it),count_vertices));
      count_vertices++;
    }
  }

  graph G(1 + count_vertices + 1); //Source + (times * stations) + target
  edge_adder adder(G);
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  auto w_map = boost::get(boost::edge_weight, G);
  const int v_source = 1 + count_vertices -1;
  const int v_target = 1 + count_vertices;

  //Add initial flow capacity from source to first t for each station
  //Add final flow capacity from last t for each station to target;
  for(int i = 0; i < S; i++){
    int min_e = *(unique_timestamps[i].begin());
    int max_e = *(unique_timestamps[i].rbegin());
    adder.add_edge(v_source, m[make_pair(i,min_e)], starting_cars[i],0);
    adder.add_edge(m[make_pair(i,max_e)], v_target, 100 * S,0);
  }

  for(int i = 0; i < S;i++){
    int first = 0 ;
    for(auto it = unique_timestamps[i].begin(); it != unique_timestamps[i].end(); it++){
      if(*it!=0){
        adder.add_edge(m[make_pair(i,first)], m[make_pair(i,*it)], 100 * S,BASELINE_COST * (*it - first));
        first = *it;
      }
    }
  }
  //Process request
  for(int i = 0; i < N; i++){
    auto req = requests[i];
    int curr_s = get<0>(req);
    int curr_t = get<1>(req);
    int curr_d = get<2>(req);
    int curr_a = get<3>(req);
    int curr_p = get<4>(req);
    adder.add_edge(m[make_pair(curr_s - 1,curr_d)], m[make_pair(curr_t - 1,curr_a)], 1,BASELINE_COST * (curr_a - curr_d) - curr_p);
  }

    // Min Cost Max Flow with successive_shortest_path_nonnegative_weights
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  int cost = boost::find_flow_cost(G);
  cout << (BASELINE_COST * (100000) * total_cars) - cost << endl;
}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t; // Read the number of test cases
    for (int i = 0; i < t; ++i){
      cin >> N >> S;
      car_sharing();
    }
    return 0;
 }
