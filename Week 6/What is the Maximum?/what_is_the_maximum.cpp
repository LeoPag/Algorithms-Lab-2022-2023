#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <cmath>

using namespace std;
typedef CGAL::Gmpz ET;
typedef int IT;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
int p,a,b;


void optimize(){
  cin >> a >> b;
  if(p == 1){
    Program lp (CGAL::SMALLER, true, 0, false, 0);
    const int X = 0;
    const int Y = 1;
    lp.set_a(X, 0, 1); lp.set_a(Y, 0, 1); lp.set_b(0, 4); // x + y <= 4
    lp.set_a(X, 1,  4); lp.set_a(Y, 1, 2); lp.set_b(1, a*b);  //  4x + 2y  <= ab
    lp.set_a(X, 2, -1); lp.set_a(Y, 2, 1); lp.set_b(2, 1); // -x+y <= 1
    // objective function
    lp.set_c(X, a);lp.set_c(Y, -b);
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_optimal()){
      int to_print = floor((CGAL::to_double(-s.objective_value())));
      if(to_print == -0){
        cout << 0 << endl;
      }
      else{
        cout << to_print << endl;
      }
    }
    else if (s.is_unbounded()) {
      cout << "unbounded" << endl;
    }
    else if (s.is_infeasible()) {
      cout << "no" << endl;
    }
  }
  else{
    Program lp (CGAL::SMALLER, false, 0, true, 0);
    const int X = 0;
    const int Y = 1;
    const int Z = 2;
    lp.set_a(X, 0, -1); lp.set_a(Y, 0, -1); lp.set_a(Z, 0, 0); lp.set_b(0, 4); // -x -y <= 4
    lp.set_a(X, 1,  -4); lp.set_a(Y, 1, -2); lp.set_a(Z,1, -1); lp.set_b(1, a * b); // -4x -2y -z <= ab
    lp.set_a(X, 2, 1); lp.set_a(Y, 2, -1); lp.set_a(Z, 2, 0); lp.set_b(2, 1); // x-y <= 1
    lp.set_c(X, a);lp.set_c(Y, b);lp.set_c(Z, 1);
    Solution s = CGAL::solve_linear_program(lp, ET());
    if (s.is_optimal()){
      int to_print = ceil((CGAL::to_double(s.objective_value())));
      if(to_print == -0){
        cout << 0 << endl;
      }
      else{
        cout << to_print << endl;
      }
    }
    else if (s.is_unbounded()) {
      cout << "unbounded" << endl;
    }
    else if (s.is_infeasible()) {
      cout << "no" << endl;
    }
  }
}
int main(){
  cin >> p;
  while(p != 0){
    optimize();
    cin >> p;
  }
  return 0;
}
