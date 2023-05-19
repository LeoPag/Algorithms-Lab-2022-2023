#include <iostream>
#include <vector>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef vector<pair<K::Point_2, int>> VIPOINT;
typedef queue<Triangulation::Vertex_handle> QVH;
int n; long r;

//This function compute max size of connected components via BFS
int max_cc_size(const VIPOINT& planets, int k){
  Triangulation t;
  t.insert(planets.begin() + k, planets.end());

  int max_component_size = 0;
  int component_index = 0;

  // --- BFS ---
  for(auto v = t.finite_vertices_begin(); v!= t.finite_vertices_end(); v++){
    if(v->info() != 0) continue;
    QVH q; q.push(v);
    int new_component_size = 0;
    component_index++;
    while(q.empty() == false){
      auto curr_v = q.front(); q.pop();
      curr_v->info() = component_index;
      new_component_size++;
      Triangulation::Vertex_circulator c = t.incident_vertices(curr_v);
      do {
        if (t.is_infinite(c)) continue;
        if((CGAL::squared_distance(curr_v->point(), c->point()) <= r*r) & (c->info() == 0)){
          q.push(c); c->info() = component_index;
        }
      } while (++c != t.incident_vertices(curr_v));
    }
    max_component_size = max(max_component_size, new_component_size);
  }
  return max_component_size;
}

void sith(){
  VIPOINT planets;
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    planets.push_back(make_pair(K::Point_2(x, y), 0));
  }

  //Binary search
  int left = 0; int right = n/2;
  int res = 0;
  while(left <= right){
    int k = (left + right) / 2;
    int c = max_cc_size(planets, k);
    res = max(res,min(k,c));
    if(k == c) break;
    else if(c > k) left = k + 1;
    else right = k - 1;
  }
  cout << res << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
     cin >> n >> r;
     sith();
  }
  return 0;
}
