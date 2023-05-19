#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Gmpq.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Finite_faces_iterator  Face_iterator;
typedef K::Point_2 P;
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;
typedef CGAL::Gmpq QT;
// program and solution types
typedef CGAL::Quadratic_program<QT> Program;
typedef CGAL::Quadratic_program_solution<QT> Solution;

using namespace std;
int a, s, b, max_energy;

typedef vector<K::FT> VFT;
typedef vector<P> VP;
typedef vector<int> VI;

void strikesback(){

  //Get particles
  VP particles(a);
  VI densities(a);
  for(int i = 0; i < a; i++){
    int x,y,d; cin >> x >> y >> d;
    P new_part(x,y);
    particles[i] = new_part;
    densities[i] = d;
  }

  //Get shot centers
  VP shots_centers(s);
  for(int i = 0; i < s; i++){
    int x,y; cin >> x >> y;
    P new_shot_cen(x,y);
    shots_centers[i] = new_shot_cen;
  }

  //Build DT with bounty hunters
  // read points
  VP hunters;
  hunters.reserve(b);
  for (int i = 0; i < b; ++i) {
    int x, y; cin >> x >> y;
    hunters.push_back(K::Point_2(x, y));
  }
  // construct triangulation
  Triangulation t;
  t.insert(hunters.begin(), hunters.end());

  VFT rmax(s, pow(2,30));
  if(b > 0){
  //Get Rmax for each shot by quering the shots in the DT, finding NN
  for(int i = 0; i < s; i++){
    auto nearest_neighbour = t.nearest_vertex(shots_centers[i]);
    rmax[i] = CGAL::squared_distance(nearest_neighbour->point(), shots_centers[i]);
  }
  }
  //Setup LP
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 0, false, 0);

  //LP has s decision_variables : each shot energy
  //Lp has a + 1 constraints: a to destroy the particles + 1 to respect the total energy boundary;

  //Enforce total energy constraint
  for(int j = 0; j < s; j++){
    lp.set_a(j, 0, 1);
  }
  lp.set_b(0, max_energy);

  for(int i = 0; i < a; i++){
    for(int j = 0; j < s; j++){
      K::FT distance = CGAL::squared_distance(shots_centers[j], particles[i]);
      if(distance <= 1) distance = 1;
      if(distance <= rmax[j]){
        QT coeff(1,distance);
        lp.set_a(j, i+1, -coeff);
      }
    }
    lp.set_b(i+1,-densities[i]);
  }

  // solve the program, using ET as the exact type
  Solution s = CGAL::solve_linear_program(lp, QT());
  if (s.is_optimal()) cout << "y" << endl;
  else cout << "n" << endl;

}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t; // Read the number of test cases
    for (int i = 0; i < t; ++i){
      cin >> a >> s >> b;
      cin >> max_energy;
      strikesback(); // Solve a particular test case
    }
    return 0;
 }
