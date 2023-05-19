#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <queue>
#include <cmath>
using namespace std;
int n,m;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef std::priority_queue<pair<long,int>> max_heap; // Default behaviour

typedef vector<long> VL;
typedef vector<pair<int, long>> VPAIR;
typedef vector<VPAIR> VVPAIR;

void h1n1(){

  ///Build DT
  std::vector<K::Point_2> pts;
  pts.reserve(n);

  for (int i = 0; i < n; ++i) {
    long x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }

  // construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());

  //Label each face with an index;
  int face_index = 0;
  for(auto f = t.all_faces_begin(); f != t.all_faces_end(); f++){
    if(t.is_infinite(f)) f->info() = 0;
    else {
      face_index++;
      f->info() = face_index;
    }
  }

  //Build adjacency list
  VVPAIR adj_list(face_index + 1);
  for(auto f = t.all_faces_begin(); f != t.all_faces_end(); f++){
    int from = f->info();
    for(int i = 0; i < 3; i++){
      int to_i = f->neighbor(i)->info();
      long len_i = t.segment(make_pair(f,i)).squared_length();
      adj_list[from].push_back(make_pair(to_i, len_i));
    }
  }

  //Find max distance for each face via max_heap calls;
  const long INFINITE = pow(2,52);

  VL max_distances(face_index + 1, -1);
  max_heap H;
  H.push(make_pair(INFINITE, 0));

  while(H.empty() == false){
    auto p =H.top(); H.pop();
    int curr_face_index = p.second;
    long max_f_dist = p.first;
    if(max_distances[curr_face_index] != -1) continue;
    max_distances[curr_face_index] = max_f_dist ;
    for(auto neigh: adj_list[curr_face_index]){
      if(max_distances[neigh.first] == -1){
        long max_window = min(neigh.second, max_f_dist);
        H.push(make_pair(max_window, neigh.first));
      }
    }
  }

  cin >> m;

  //Read and respond to query points;
  for(int i = 0; i < m; i++){
    long x,y,d; cin >> x >> y >> d;
    K::Point_2 query_point(x,y);
    auto nearest_neighbour = t.nearest_vertex(query_point);
    if(CGAL::squared_distance(nearest_neighbour->point(), query_point) < d){
      cout << "n";
    }
    else{
      int face_of_point = (t.locate(query_point))->info();
      if(max_distances[face_of_point] >= 4 * d) cout << "y";
      else cout << "n";
    }
  }
   cout << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  cin >> n;
  while(n!= 0){
    h1n1();
    cin >> n;
  }
  return 0;
}
