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
using namespace std;
typedef vector<int> VI;
typedef vector<VI> VVI;
// program and solution types
typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int n,m;
VI mins;
VI maxs;
VI prices;
VVI nutrients_in_foods;

void diet(){
  mins = VI(n);
  maxs = VI(n);
  prices = VI(m);
  nutrients_in_foods = VVI(m);

  int min, max;
  for(int i = 0; i < n; i++){
    cin >> min >> max;
    mins[i] = min;
    maxs[i] = max;
  }
  int price,nut;
  for(int i = 0; i < m; i++){
    cin >> price;
    prices[i] = price;
    for(int j = 0; j < n;j++){
      cin >> nut;
      nutrients_in_foods[i].push_back(nut);
    }
  }
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      lp.set_a(j,i, nutrients_in_foods[j][i]);
    }
    lp.set_b(i,maxs[i]);
  }
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      lp.set_a(j,i + n, -nutrients_in_foods[j][i]);
    }
    lp.set_b(i + n,-mins[i]);
  }
  for(int j = 0; j < m; j++){
    lp.set_c(j, prices[j]);
  }
  Solution s = CGAL::solve_linear_program(lp, ET());
  if(s.is_infeasible()){
    cout << "No such diet." << endl;
  }
  else if(s.is_optimal()){
    cout << floor(CGAL::to_double(s.objective_value()))<< endl;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  cin >> n >> m;
  while((n != 0) or (m != 0)){
    diet();
    cin >> n >> m;
  }
  return 0;
}
