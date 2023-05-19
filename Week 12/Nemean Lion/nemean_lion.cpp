#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

using namespace std;
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef vector<K::FT> VK;
int n; K::FT s, h;

struct Boundary{
  K::FT b; int is_min;
  Boundary(K::FT b, int is_min) : b(b), is_min(is_min) {}
};

// Linearly order boundaries by increasing distance
bool operator < (const Boundary& b1, const Boundary& b2) {
  if(b1.b == b2.b){
    return (b1.is_min < b2.is_min);
  }
  else return (b1.b < b2.b);
}

typedef vector<Boundary> VB;

void nemean_lion(){
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

  int a2,a3,as,amax;
  a2 = a3 = as = amax = 0;

  VK distances2;
  for(auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++){
    distances2.push_back(t.segment(e).squared_length());
  }

  VK distances3;
  for(auto f = t.finite_faces_begin(); f != t.finite_faces_end(); f++){
    auto c = t.dual(f);
    auto v = f->vertex(0);
    distances3.push_back(CGAL::squared_distance(c, v->point()));
  }
  sort(distances2.begin(), distances2.end());
  sort(distances3.begin(), distances3.end());
  auto ref_dist2 = distances2[0];
  int j = 0;
  while(distances2[j] == ref_dist2){
    a2++;
    j++;
  }

  K::FT ref_dist3 = distances3[0];
  int k = 0;

  while(distances3[k] == ref_dist3){
    a3++;
    k++;
  }

  VB boundaries;
  for(auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++){
    auto o = t.dual(e);
    auto v1 = e->first->vertex((e->second + 1) % 3);
    auto len = t.segment(e).squared_length();
    K::Segment_2 seg; K::Ray_2 ray;
    //Inside Convex Hull
    if(assign(seg,o)){
      auto e1 = seg.source();
      auto e2 = seg.target();
      auto distv1 = CGAL::squared_distance(v1->point(),e1);
      auto distv2 = CGAL::squared_distance(v1->point(),e2);
      //Voronoi intersects DT edge;
      if(CGAL::do_intersect(seg, t.segment(e))){
        auto max_d = max(distv1, distv2);
        if((max_d >= s) & (len <= 4*s))as ++;
        boundaries.push_back({len, 0});
        boundaries.push_back({4 *max_d, 1});
      }
      //Voronoi does not intersect DT edge;
      else{
        auto min_d = min(distv1, distv2);
        auto max_d = max(distv1, distv2);
        if((min_d <= s) & (s <= max_d)) as++;
        boundaries.push_back({4 * min_d, 0});
        boundaries.push_back({4 * max_d, 1});
      }
    }
    //Convex Hull
    else if(assign(ray,o)){
      auto e1 = ray.source();
      auto distv1 = CGAL::squared_distance(v1->point(),e1);
      //Voronoi intersects DT edge;
      if(CGAL::do_intersect(ray, t.segment(e))){
        if(len <= 4*s) as ++;
        boundaries.push_back({len, 0});
        boundaries.push_back({pow(2,51), 1});
      }
      //Voronoi does not intersect DT edge
      else {
        if(distv1 <= s) as++;
        boundaries.push_back({4 * distv1, 0});
        boundaries.push_back({pow(2,51), 1});
      }
    }
  }

  sort(boundaries.begin(), boundaries.end());

  int a = 0;
  for(auto b: boundaries){
    if(b.is_min == 0) a++;
    else a--;
    amax = max(a,amax);
  }

  cout << a2 << " " << a3 << " " << as << " " << amax << endl;
}
int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> s >> h;
    nemean_lion();
  }
}
