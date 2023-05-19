#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <algorithm>
#include <cmath>

using namespace std;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Nail {
    ET x_coord, y_coord;
    Nail(ET x, ET y) : x_coord(x), y_coord(y){}
};

// Linearly order edges by increasing x; if tiebreak decreasing y;
bool operator < (const Nail& n1, const Nail& n2) {
  if(n1.x_coord != n2.x_coord){
    return (n1.x_coord < n2.x_coord);
  }
  else{
    return(n1.y_coord > n2.y_coord);
  }
}

typedef vector<Nail> VNAIL;
VNAIL new_nails;
VNAIL old_nails;

int n,m;
ET h,w;

void read_input(){
  ET x_c,y_c;
  for(int i = 0; i < n; i++){
    cin >> x_c >> y_c;
    Nail new_nail(x_c,y_c);
    new_nails.push_back(new_nail);
  }
  for(int j = 0; j < m; j++){
    cin >> x_c >> y_c;
    Nail old_nail(x_c,y_c);
    old_nails.push_back(old_nail);
  }
}
void suez(){
  sort(new_nails.begin(),new_nails.end());
  Program lp (CGAL::SMALLER, true, 1, false, 0);
  int count = 0;
  for(int i = 0; i < n; i++){
    for(int j = i + 1; j < n; j++){
      ET cand1 = ((abs(new_nails[i].y_coord - new_nails[j].y_coord) * 2 )/ h);
      ET cand2 = (((new_nails[j].x_coord - new_nails[i].x_coord) * 2) / w);
      lp.set_a(i,count,1);lp.set_a(j, count, 1);lp.set_b(count,max(cand1,cand2));
      count += 1;
    }
  }
  const int MAX = pow(2,30);
  for(int i = 0; i < n; i++){
    ET thresh = MAX;
    for(int j = 0; j < m; j++){
      ET cand1 = ((abs(new_nails[i].x_coord - old_nails[j].x_coord) - (w / 2)) * (2 / w));
      ET cand2 = ((abs(new_nails[i].y_coord - old_nails[j].y_coord) - (h / 2)) * (2 / h));
      ET new_thresh = max(cand1,cand2);
      if(new_thresh < thresh){
        thresh = new_thresh;
      }
    }
    lp.set_u(i, true, thresh);
  }
  for(int i = 0; i < n; i++){
    lp.set_c(i, -1);
  }
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  cout << setprecision(25);
  cout << ceil(CGAL::to_double(-s.objective_value() * (h+w) * 2)) << endl;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; cin >>test_cases;
  for(int i = 0; i < test_cases; i++){
    cin >> n >> m >> h >> w;
    read_input();
    suez();
    new_nails.clear();
    old_nails.clear();
  }
  return 0;
}
