#include <iostream>
#include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <cmath>
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;
int n,d;

void inball(){
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  int a,b;
  const int R = d;
  lp.set_l(R, true, 0);
  for(int i = 0; i < n; i++){
    int norm = 0;
    for(int j = 0; j < d; j++){
      cin >> a;
      norm = norm + (a * a);
      lp.set_a(j, i, a);
    }
    lp.set_a(R,i,sqrt(norm));  //Point-Hyperplane distance formula comes up handy here!!!
    cin >> b;
    lp.set_b(i,b);
  }
  lp.set_c(R,-1);
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_optimal()){
    cout << floor(CGAL::to_double(-s.objective_value()))<< endl;
  }
  else if((s.is_unbounded())){
    cout << "inf" << endl;
  }
  else if(s.is_infeasible()){
    cout << "none" << endl;
  }
}

int main(){
  cin >> n >> d;
  while(n!= 0){
    inball();
    cin >> n >> d;
  }
  return 0;
}
