#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

using namespace std;

int n;

typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;
typedef vector<K::Point_2> VP;

double ceil_to_double(const K::FT& x){
  double a = ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

void antenna(){

  VP pts;
  for (int i = 0; i < n; ++i){
    long x,y; cin >> x >> y;
    K::Point_2 p(x,y);
    pts.push_back(p);
  }
  Min_circle mc(pts.begin(), pts.end(), true);
  Traits::Circle c = mc.circle();
  K::FT radius = CGAL::sqrt(c.squared_radius());
  cout << setprecision(53);
  cout << ceil_to_double(radius) << endl;
}
int main(){
  std::ios_base::sync_with_stdio(false);
  cin >> n;
  while(n != 0){
    antenna();
    cin >> n;
  }
  return 0;
}
