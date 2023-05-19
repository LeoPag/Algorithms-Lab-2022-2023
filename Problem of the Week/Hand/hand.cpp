#include <iostream>
#include <algorithm>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb; //Store component in the vertex
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;

using namespace std;
int n,k,f0;
long s0;

typedef vector<int> VI;
typedef vector<pair<K::Point_2, int>> VP;
typedef queue<Triangulation::Vertex_handle> QV;

struct Edge{
  Triangulation::Vertex_handle v1, v2;
  K::FT length;

  Edge(Triangulation::Vertex_handle vert1, Triangulation::Vertex_handle vert2,K::FT l) : v1(vert1), v2(vert2), length(l) {}
  };

// Linearly order edges by increasing length
bool operator < (const Edge& e1, const Edge& e2) {
    return (e1.length < e2.length);
}

typedef vector<Edge> VE;

VI clusters;

int get_max_families(){
  if(k == 1){
    return clusters[1] + clusters[2] + clusters[3] + clusters[4];
  }
  else if(k == 2){
    return clusters[2] + clusters[3] + clusters[4] + clusters[1] / 2;
  }
  else if(k == 3){
    int matched_2_with_1 = min(clusters[2], clusters[1]);
    int to_3_from_2 = (clusters[2] - matched_2_with_1) / 2;
    int to_3_from_1 = (clusters[1] - matched_2_with_1) / 3;
    return clusters[4] + clusters[3] + matched_2_with_1 + to_3_from_2 + to_3_from_1;
  }

  else if(k == 4){
    int matched_3_with_1 = min(clusters[1], clusters[3]);
    int remaining_3 = clusters[3] - matched_3_with_1;
    int remaining_1 = clusters[1] - matched_3_with_1;
    int to_4_from_2 = (clusters[2] + remaining_3) / 2;
    int remaining_2 = (clusters[2] + remaining_3) % 2;
    int to_4_from_1 = (remaining_1 + remaining_2 * 2) / 4;
    return clusters[4] + matched_3_with_1 + to_4_from_2 + to_4_from_1;
  }
}

void hand(){
  // read points
  VP cpoints;
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    cpoints.push_back(make_pair(K::Point_2(x, y),i)); //-1 means vertex is alone
  }
  // construct triangualation
  Triangulation t;
  t.insert(cpoints.begin(), cpoints.end());

  VE edges;

  //Get all edges from DT
  for(auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++){
    auto v1 = e->first->vertex((e->second + 1) % 3);
    auto v2 = e->first->vertex((e->second + 2) % 3);
    Edge new_edge(v1,v2,t.segment(e).squared_length());
    edges.push_back(new_edge);
  }
  //Sort edges according to length
  sort(edges.begin(), edges.end());

  //----QUESTION 1------
  boost:: disjoint_sets_with_storage <> uf1(n); //Union find
  clusters = VI(5,0);
  clusters[1] = n;
  VI components(n,1);
  K::FT s;
  for(auto e: edges){
    auto v1 = e.v1;
    auto v2 = e.v2;
    auto c1 = uf1.find_set(v1->info());
    auto c2 = uf1.find_set(v2->info());
    if(c1 != c2){
      uf1.link(c1,c2);
      int joined_components = min(4,components[c1] + components[c2]);
      clusters[components[c1]]--;
      clusters[components[c2]]--;
      clusters[joined_components]++;
      components[c1] = components[c2] = joined_components;

      if((get_max_families() < f0)){
        s = e.length;
        break;
      }
    }
  }

   // --QUESTION 2 --
  boost:: disjoint_sets_with_storage <> uf2(n); //Union find
  clusters = VI(5,0);
  clusters[1] = n;
  components = VI(n,1);
  for(auto e: edges){
    auto v1 = e.v1;
    auto v2 = e.v2;
    auto c1 = uf2.find_set(v1->info());
    auto c2 = uf2.find_set(v2->info());
    if(e.length >= s0) break;
    if(c1 != c2){
      uf2.link(c1,c2);
      int joined_components = min(4,components[c1] + components[c2]);
      clusters[components[c1]]--;
      clusters[components[c2]]--;
      clusters[joined_components]++;
      components[c1] = components[c2] = joined_components;
    }
  }
  int f = get_max_families();

  cout << setprecision(100) << s << " " << f << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i< t; i++){
    cin >> n >> k >> f0 >> s0;
    hand();
  }
  return 0;
}
