#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Segment_2 S;
typedef K::Point_2 P;
typedef K::Ray_2 R;

void hit(const int n){
  long x_start_ray, y_start_ray, x_end_ray, y_end_ray;
  cin >> x_start_ray >> y_start_ray >> x_end_ray >> y_end_ray;
  P start_ray(x_start_ray, y_start_ray);
  P end_ray(x_end_ray, y_end_ray);
  R laser(start_ray,end_ray);
  bool has_hit = false;
  for(int i = 0; i < n; i++){
    long x_start_obs, y_start_obs, x_end_obs, y_end_obs;
    cin >> x_start_obs >> y_start_obs >> x_end_obs >> y_end_obs;
    if(has_hit == false){
      P start_obs(x_start_obs, y_start_obs);
      P end_obs(x_end_obs, y_end_obs);
      S obstacle(start_obs, end_obs);
       if (CGAL::do_intersect(laser,obstacle)){
         has_hit = true;
       }
    }
  }
  if (has_hit == true){
    cout << "yes" << "\n";
  }
  else{
    cout << "no" << "\n";
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int n; cin >> n;
  while(n != 0){
    hit(n);
    cin >> n;
  }
  return 0;
}
