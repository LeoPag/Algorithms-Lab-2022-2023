#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

using namespace std;
long n,m,c;

// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq) typedef CGAL::Gmpz ET;
// program and solution types
typedef CGAL::Gmpz ET;
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<long,K> Vb; //radius of contour line
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<pair<int,int>> VPAIRINT;
typedef K::Point_2 P;
typedef vector<P> VP;

double floor_to_double(const CGAL::Quotient<ET>& x)
{
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

typedef set<int> SI;
typedef vector<SI> VSI;

void world_cup(){
  VP warehouses(n);
  VPAIRINT supply_alcohol(n);
  Triangulation t;
  //Read warehouses data
  VP dt_points;
  for(int i = 0; i < n; i++){
    int x,y,s,a; cin >> x >> y >> s >> a;
    P warehouse(x,y);
    warehouses[i] = warehouse;
    supply_alcohol[i] = make_pair(s,a);
    dt_points.push_back(warehouse);
  }

  VP stadiums(m);
  VPAIRINT demand_limit(m);
  //Read stadium data
  for(int i = 0; i < m; i++){
    int x,y,d,u; cin >> x >> y >> d >> u;
    P stadium(x,y);
    stadiums[i] = stadium;
    demand_limit[i] = make_pair(d,u);
    dt_points.push_back(stadium);
  }
  t.insert(dt_points.begin(), dt_points.end());

  //Read revenues;
  VVI revenues(n);
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      int revenue; cin >> revenue;
      revenues[i].push_back(revenue);
    }
  }
  VP useful_contour_points;
  VI useful_contour_radiuses;

  //Find only useful contour lines. If nearest neighbour is further than r, then line does not contain any warehouse nor stadium
  for(int i = 0; i < c; i++){
    int xc, yc; long rc;
    cin >> xc >> yc >> rc;
    P center_contour_line(xc,yc);
    auto nearest_neighbour = t.nearest_vertex(center_contour_line);
    if(CGAL::squared_distance(nearest_neighbour->point(), center_contour_line) < rc * rc){
      useful_contour_points.push_back(center_contour_line);
      useful_contour_radiuses.push_back(rc);
    }
  }

  int n_lines = int(useful_contour_points.size());
  VSI contour_for_warehouse(n);
  VSI contour_for_stadium(m);

  //Find contour set for each warehouse
  for(int i = 0; i < n; i++){
    for(int k = 0; k < n_lines; k++){
      if(CGAL::squared_distance(useful_contour_points[k], warehouses[i]) < useful_contour_radiuses[k] * useful_contour_radiuses[k]){
        contour_for_warehouse[i].insert(k);
      }
    }
  }
  //Find contour set for each stadium
  for(int j = 0; j < m; j++){
    for(int k = 0; k < n_lines; k++){
      if(CGAL::squared_distance(useful_contour_points[k], stadiums[j]) < useful_contour_radiuses[k] * useful_contour_radiuses[k]){
        contour_for_stadium[j].insert(k);
      }
    }
  }
  //Create LP program using CGAL formulation and solver
  //Create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 0, false, 0);

  //We have n x m decision variables.
  //The variables from 0 ... m-1 represent liters from warehouse 0 to all stadiums and so on;
  //variables from (ix(m-1)) + (0 .... m-1) represent liters from warehouse i to all stadiums;

  //Set constraints on supply from each warehouse; There are n of these constraints;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      lp.set_a(i*m + j, i, 1);
    }
    lp.set_b(i,supply_alcohol[i].first);
  }

  //Set offset for constraint counter;
  int offset = n;

 //Set constraints on demand for each stadium; -- Greater Inequality
  for(int j = 0; j < m; j++){
    for(int i = 0; i < n; i++){
      lp.set_a(i*m + j, offset + j,-1);
    }
    lp.set_b(offset + j, -(demand_limit[j].first));
  }
  offset = n + m;

   //Set constraints on demand for each stadium; -- Smaller Inequality
   for(int j = 0; j < m; j++){
    for(int i = 0; i < n; i++){
      lp.set_a(i*m + j, offset + j,1);
    }
    lp.set_b(offset + j, (demand_limit[j].first));
  }
  offset = n + m + m;

  //Set constraints on alcohol quantity;
  for(int j = 0; j < m; j++){
    for(int i = 0; i < n; i++){
      lp.set_a(i*m + j, offset + j, supply_alcohol[i].second);
    }
    lp.set_b(offset + j, 100 * demand_limit[j].second);
  }

  //Set cost function
  for(int i = 0; i < n; i++){
    for(int j = 0; j < m; j++){
      VI union_set, intersection_set;
      set_union(contour_for_warehouse[i].begin(), contour_for_warehouse[i].end(), contour_for_stadium[j].begin(), contour_for_stadium[j].end(),back_inserter(union_set));
      set_intersection(contour_for_warehouse[i].begin(), contour_for_warehouse[i].end(), contour_for_stadium[j].begin(), contour_for_stadium[j].end(), back_inserter(intersection_set));
      lp.set_c(i*m + j, -(100 * revenues[i][j] - (union_set.size() - intersection_set.size())));
    }
  }
  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, ET());

  if (s.is_infeasible()) {
    cout << "RIOT!" << endl;
  }
  if (s.is_optimal()) {
    cout << fixed << setprecision(0) <<floor_to_double(-s.objective_value() / 100) << endl;
  }

}
int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> c;
    world_cup();
  }
  return 0;
}
