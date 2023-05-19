#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef vector<K::FT> VKFT;

int n;

void germs(){
  K::FT l,b,r,t; cin >> l >> b >> r >> t;
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (int i = 0; i < n; ++i) {
    K::FT x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }
  VKFT radii;
  const long INF = pow(2,50);
  Triangulation q;
  q.insert(pts.begin(),pts.end());
  for(auto v = q.finite_vertices_begin(); v != q.finite_vertices_end(); v++){
    Triangulation::Edge_circulator c = q.incident_edges(v);
    K::FT min_edge = INF;
    do {
      if (q.is_infinite(c)) continue;
      min_edge = min(min_edge, q.segment(c).squared_length());
    } while (++c != q.incident_edges(v));
    K::FT min_border_hor = min(v->point().x() - l, r - v->point().x());
    K::FT min_border_ver = min(v->point().y() - b, t - v->point().y());
    K::FT min_border = min(min_border_hor, min_border_ver);
    K::FT min_border_sq = min_border * min_border;

    if(4 * min_border_sq < min_edge) radii.push_back(min_border_sq);
    else radii.push_back(min_edge / 4);
  }
  sort(radii.begin(), radii.end());
  K::FT first_dead_radius = radii[0];
  K::FT over_50_dead_radius = radii[n/2];
  K::FT last_dead_radius = radii[n-1];
  cout << ceil(sqrt(sqrt(first_dead_radius) - 0.5)) << " " << ceil(sqrt(sqrt(over_50_dead_radius) - 0.5)) << " " << ceil(sqrt(sqrt(last_dead_radius) - 0.5)) << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  cin >> n;
  while(n != 0){
    germs();
    cin >> n;
  }
  return 0;
}
