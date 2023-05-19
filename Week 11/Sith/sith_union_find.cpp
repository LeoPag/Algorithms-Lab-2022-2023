#include <iostream>
#include <vector>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include<boost/pending/disjoint_sets.hpp>
using namespace std;

long n,r;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb; //Point Index;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef queue<Triangulation::Vertex_handle> QVH;
typedef vector<int> VI;
typedef vector<P> VP;

void sith(){
  VI sizes(n,1);
  VP points;
  long r_squared = r*r;
  for(int i = 0; i < n; i++){
    int x_planet, y_planet; cin >> x_planet >> y_planet;
    K::Point_2 p(x_planet, y_planet);
    points.push_back(p);
  }
  boost::disjoint_sets_with_storage<>  UF(n); //Union Find data structure

  Triangulation t;
  int max_size = 1;
  auto v = t.insert(points[n-1]);
  v->info() = n-1;

  for(int k = n-2; k>=0;k--){
    auto curr_node = t.insert(points[k]);
    curr_node->info() = k;
    auto neigh = t.incident_vertices(curr_node);
    do {
      if(neigh == t.infinite_vertex()) continue;
       if(CGAL::squared_distance(curr_node->point(),neigh->point()) <= r_squared){
        auto c1 = UF.find_set(curr_node -> info()); //Find
        auto c2 = UF.find_set(neigh -> info());
        if(c1!=c2){
          UF.link(c1,c2);                          //Union
          auto new_size = sizes[c1] + sizes[c2];
          sizes[UF.find_set(c1)] = new_size;
        }
      }
    } while (++neigh != t.incident_vertices(curr_node));
    int max_connected_component_size = 0;
    for(int size: sizes) max_connected_component_size = max(max_connected_component_size, size);
    max_size = max(max_size, min(max_connected_component_size, k));
    if(max_connected_component_size > k) break;
  }
  cout << max_size << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> r;
    sith();
  }
}
