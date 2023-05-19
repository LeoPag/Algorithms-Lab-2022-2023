#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <vector>
using namespace std;

typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;
typedef EK::Point_2 P;
typedef EK::Segment_2 S;
typedef EK::Ray_2 R;

int n;
typedef vector<S> VS;
double floor_to_double(const EK::FT& x){
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void first_hit(){
  long x,y,a,b; cin >> x >> y >> a >> b;
  P start(x,y); P ray_dir(a,b);
  R laser(start, ray_dir);
  S clip_ray(start,ray_dir); //does not matter
  P best_intersection;
  bool has_hit = false;
  EK::FT min_distance;
  long x_start, y_start, x_end, y_end;
  VS obstacles;
  for(int i = 0; i < n; i++){
    cin >> x_start >> y_start >> x_end >> y_end;
    P start_obs(x_start,y_start);
    P end_obs(x_end,y_end);
    S obs(start_obs, end_obs);
    obstacles.push_back(obs);
  }
  random_shuffle(obstacles.begin(), obstacles.end());
  for(int i = 0; i < n; i++){
    if(has_hit == false){
      if(CGAL::do_intersect(laser,obstacles[i])){
        has_hit = true;
        auto o = CGAL::intersection(laser,obstacles[i]);
        if (const P* op = boost::get<P>(&*o)){
          best_intersection = *op;
        }
        else if(const S* os = boost::get<S>(&*o)){
          if(CGAL::squared_distance(start,os->source()) < CGAL::squared_distance(start,os->target())){
          best_intersection = os->source();
          }
          else{
          best_intersection = os->target();
          }
        }
        min_distance = CGAL::squared_distance(start,best_intersection);
        S new_clip_ray(start,best_intersection);
        clip_ray = new_clip_ray;
      }
    }
    else{
     if(CGAL::do_intersect(clip_ray,obstacles[i])){
       auto o = CGAL::intersection(laser,obstacles[i]);
        P intersection;
        if (const P* op = boost::get<P>(&*o)){
          intersection = *op;
        }
        else if(const S* os = boost::get<S>(&*o)){
          if(CGAL::squared_distance(start,os->source()) < CGAL::squared_distance(start,os->target())){
            intersection = os->source();
          }
          else{
             intersection = os->target();
          }
         }
        EK::FT distance = CGAL::squared_distance(start,intersection);
        if(distance < min_distance){
          min_distance = distance;
          best_intersection = intersection;
          S new_clip_ray(start,best_intersection);
          clip_ray = new_clip_ray;
        }
      }
    }
  }
  if(has_hit == false){
    cout << "no" << endl;
  }
  else{
    cout << setprecision(100);
    cout << floor_to_double(best_intersection.x()) << " " << floor_to_double(best_intersection.y()) << "\n";
  }
}
int main(){
  std::ios_base::sync_with_stdio(false);
  cin >> n;
  while(n != 0){
    first_hit();
    cin >> n;
  }
}
