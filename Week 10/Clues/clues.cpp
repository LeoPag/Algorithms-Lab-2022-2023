#include <iostream>
#include <vector>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<pair<int,int>,K> Vb; //Color + Component
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef vector<K::Point_2> VP;
typedef queue<Triangulation::Vertex_handle> QVH;
int n,m; long r;

void clues(){
  long squared_r = r*r;

  // read points
  VP pts;
  pts.reserve(n);
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }

  // construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());

  for(auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++){
    v->info() = make_pair(0,0);
  }

  //-----2COLOR GRAPH + Connected Components - BFS ----
  VP reds, blacks;
  int component_index = 0;
  bool interference = false;
  for(auto v = t.finite_vertices_begin(); v != t.finite_vertices_end(); v++){
    if(interference == true) break;
    if(v->info().first != 0) continue;
    QVH q; q.push(v);
    component_index++;
    v->info() = make_pair(1, component_index);
    while(q.empty() == false){
      if(interference == true) break;
      auto curr_v = q.front(); q.pop();
      int curr_col = -(curr_v ->info()).first;
      Triangulation::Vertex_circulator c= t.incident_vertices(curr_v);
      curr_v->info().second = component_index;
      if(curr_v->info().first == 1) reds.push_back(curr_v->point());
      else if(curr_v->info().first == -1) blacks.push_back(curr_v->point());
      do {
        if (t.is_infinite(c)) continue;
        if(CGAL::squared_distance(c->point(), curr_v->point()) <= squared_r){
          if(c->info().first * curr_col == -1) interference = true;
          else{
            if(c->info().first == 0) q.push(c);
            c->info().first = curr_col;
          }
        }

      } while (++c != t.incident_vertices(curr_v));
    }
  }

  //Check the DT edges of the red graph and the black graph. They must me limited under squared_r
  Triangulation t_red, t_black;
  t_red.insert(reds.begin(), reds.end());
  t_black.insert(blacks.begin(), blacks.end());

  for(auto e = t_red.finite_edges_begin(); e != t_red.finite_edges_end(); e++){
    if (interference == true) break;
    if((t_red.segment(e).squared_length()) <= squared_r) interference = true;
  }

  for(auto e = t_black.finite_edges_begin(); e != t_black.finite_edges_end(); e++){
    if (interference == true) break;
    if((t_black.segment(e).squared_length()) <= squared_r) interference = true;
  }

  //If network has interference, no query is admissible;
  if(interference == true){
    for(int i = 0; i < m; i++){
      int trash; cin >> trash>> trash>> trash>> trash;
      cout << "n";
    }
    cout << endl;
    return;
  }

   //-----Establish routing on query couples by analyzing the connected components----
  for(int i = 0; i < m; i++){
    int x_holmes, y_holmes , x_watson, y_watson;
    cin >> x_holmes >> y_holmes >> x_watson >> y_watson;
    K::Point_2 P_Holmes(x_holmes, y_holmes);
    K::Point_2 P_Watson(x_watson, y_watson);
    auto nearest_h = t.nearest_vertex(P_Holmes);
    auto nearest_w = t.nearest_vertex(P_Watson);

    //Direct communication
    if(CGAL::squared_distance(P_Holmes, P_Watson) <= squared_r) cout << "y";

    else if((CGAL::squared_distance(nearest_h->point(), P_Holmes) > squared_r)or (CGAL::squared_distance(nearest_w->point(), P_Watson) > squared_r)){
      cout << "n";
    }
    else nearest_h->info().second == nearest_w->info().second ? cout << "y" : cout << "n";
  }
  cout << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> r;
    clues();
  }
  return 0;
}
