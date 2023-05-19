#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
using namespace std;
int n,m;

void bistro(){
   // read points
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (int i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }
   // construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  cin >> m;
  for(int i = 0; i < m; i++){
    int cand_x, cand_y;
    cin >> cand_x >> cand_y;
    K::Point_2 p(cand_x,cand_y);
    auto neighbour_handle= t.nearest_vertex(p);
    auto vertex_neighbour =  (*neighbour_handle).point();
    std::cout << std::setprecision(100);
    K::FT d = CGAL::squared_distance(p,vertex_neighbour);
    cout << d << "\n";
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  cin >> n;
  while(n!= 0){
    bistro();
    cin >> n;
  }
  return 0;
}
