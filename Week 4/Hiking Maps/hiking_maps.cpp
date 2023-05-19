#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Triangle_2 T;
typedef K::Line_2 L;

using namespace std;

typedef vector<P> VP;
typedef vector<L> VL;
typedef vector<int> VI;
typedef vector<VI> VVI;

void hiking_maps(){

  int n,m; cin >> m >> n;
  int x_path,y_path;
  VP path;
  for(int j = 0; j < m; j++){
    cin >> x_path >> y_path;
    P new_point(x_path,y_path);
    path.push_back(new_point);
  }
  int x_start, y_start, x_end,y_end;
  vector<bool> is_in_triangle(m);
  VI interval(m-1,-1);
  int found_first = 0; int best_length = n;
  int maximum, minimum, length;
  for(int i = 0; i < n; i++){
    VL triangle_lines;
    for(int k = 0; k < 3; k++){
      cin >> x_start >> y_start >> x_end >> y_end;
      P start_line(x_start,y_start);
      P end_line(x_end,y_end);
      L triangle_line(start_line,end_line);
      triangle_lines.push_back(triangle_line);
    }
    auto o1 = CGAL::intersection(triangle_lines[0],triangle_lines[1]);
    auto o2 = CGAL::intersection(triangle_lines[0],triangle_lines[2]);
    auto o3 = CGAL::intersection(triangle_lines[1],triangle_lines[2]);

    P corner1 = *boost::get<P>(&*o1);
    P corner2 = *boost::get<P>(&*o2);
    P corner3 = *boost::get<P>(&*o3);

    T map(corner1, corner2, corner3);

    for(int j = 0; j < m; j++){
      if (CGAL::do_intersect(map,path[j])) {
        is_in_triangle[j] = true;
        if(j>=1){
          if(is_in_triangle[j-1] == true){
            if(interval[j-1] == -1){
              found_first += 1;
            }
             interval[j-1] = i;
          }
        }
      }
      else{
        is_in_triangle[j] = false;
      }
    }
    if(found_first == m-1){
      maximum = *max_element(interval.begin(), interval.end());
      minimum = *min_element(interval.begin(), interval.end());
      length = maximum - minimum + 1;
      if(length < best_length){
      best_length = length;
      }
    }
  }
  cout << best_length << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int c; cin >> c;
  for(int i = 0; i < c; i++){
    hiking_maps();
  }
  return 0;
}
