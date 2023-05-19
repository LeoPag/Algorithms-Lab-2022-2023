#include <iostream>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <tuple>
#include<cmath>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq) typedef CGAL::Gmpz ET;
typedef CGAL::Gmpz ET;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq) typedef CGAL::Gmpz ET;
// program and solution types
using namespace std;
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef tuple<int,int,int,int,int> PENTA;
typedef vector<PENTA> VPENTA;
typedef vector<int> VI;

int n;

void augean_stables(){
  VPENTA rooms(n);
  //Setup linear program
  for(int i = 0; i < n; i++){
    int f,c,k,l,m; cin >> f >> c >> k >> l >> m;
    PENTA room(f,c,k,l,m);
    rooms[i] = room;
  }
  VI cumulative_a(25,0);
  VI cumulative_p(25,0);
  int sum_a = 0;
  for(int i = 1; i < 25; i++){ //Index 0 means no work on Alphaeius
    int a_i; cin >> a_i; sum_a += a_i;
    cumulative_a[i] = sum_a;
  }
  int sum_p = 0;
  for(int i = 1; i < 25; i++){ //Index 0 means no work on Peneius
    int p_i; cin >> p_i; sum_p += p_i;
    cumulative_p[i] = sum_p;
  }
  const int h1 = 0;
  const int h2 = 1;
  const int h3 = 2;

  //Check if impossible by trying cleaning with max hours
  Program lp (CGAL::SMALLER, true, 0, true, 1);
  for(int i = 0; i < n; i++){
    int fi = get<0>(rooms[i]);
    int ci = get<1>(rooms[i]);
    int ki_first = get<2>(rooms[i]) + (cumulative_a[24]*cumulative_a[24]);
    int li_first = get<3>(rooms[i]) + (cumulative_p[24]*cumulative_p[24]);
    int mi_first = get<4>(rooms[i]) + (cumulative_a[24]*cumulative_p[24]);

    lp.set_a(h1, i,  -ki_first); lp.set_a(h2, i, -li_first); lp.set_a(h3, i, -mi_first);lp.set_b(i, ci - fi);
  }

   // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_infeasible()) {
     cout << "Impossible!"<< endl;
  return;}

  //Binary search
  int lower_bound_sum = 0;
  int upper_bound_sum = 48;
  VI max_pi_for_ai(25,-1); //For each a_index with store the max p_index that did not solve the problem
  while(lower_bound_sum < upper_bound_sum){
    bool found_sol = false;
    int middle_sum = (lower_bound_sum + upper_bound_sum) / 2;
    for(int a_index = 0; a_index <= middle_sum; a_index++){
      int p_index = middle_sum - a_index;
      if((p_index < 0) or (p_index > 24)) continue;
      if((a_index < 0) or (a_index > 24)) continue;
      if((max_pi_for_ai[a_index] > p_index)) continue;
       // create an LP with Ax <= b, lower bound 0 and no upper bounds
      Program lp (CGAL::SMALLER, true, 0, true, 1);
      const int h1 = 0;
      const int h2 = 1;
      const int h3 = 2;
      for(int i = 0; i < n; i++){
        int fi = get<0>(rooms[i]);
        int ci = get<1>(rooms[i]);
        int ki_first = get<2>(rooms[i]) + (cumulative_a[a_index]*cumulative_a[a_index]);
        int li_first = get<3>(rooms[i]) + (cumulative_p[p_index]*cumulative_p[p_index]);
        int mi_first = get<4>(rooms[i]) + (cumulative_a[a_index]*cumulative_p[p_index]);

        lp.set_a(h1, i,  -ki_first); lp.set_a(h2, i, -li_first); lp.set_a(h3, i, -mi_first);lp.set_b(i, ci - fi);
      }

      // solve the program, using ET as the exact type
      Solution s = CGAL::solve_linear_program(lp, ET());

      if (s.is_optimal()) {
        upper_bound_sum = middle_sum;
        found_sol = true;
        break;
      }
      else if(s.is_infeasible()){
        max_pi_for_ai[a_index] = p_index;
      }
    }
    if (found_sol == false) lower_bound_sum = middle_sum + 1;
  }
  cout << lower_bound_sum << endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n;
    augean_stables();
  }
  return 0;
}
