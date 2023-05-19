#include <iostream>
#include <vector>
#include <set>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

using namespace std;
int n,m;
typedef vector<int> VI;
typedef vector<long> VL;
typedef vector<K::Point_2> VP;
typedef set<int> SI;

void light_the_stage(){
  VP participants(m);
  VL radii(m);
  //Get participants
  for(int i = 0; i < m; i++){
    int x,y; long r; cin >> x >> y >> r;
    participants[i] = K::Point_2(x,y);
    radii[i] = r;
  }
  //Get height
  long h; cin >> h;
  //
  VP lights(n);
  for(int i = 0; i < n; i++){
    int x,y; cin >> x >> y ;
    lights[i] = K::Point_2(x,y);
  }

  //Check if one participant survives - Build complete DT
  Triangulation t;
  t.insert(lights.begin(), lights.end());
  VI winners;
  for(int i = 0; i < m; i++){
    auto nn = t.nearest_vertex(participants[i]);
    if((radii[i] + h) * (radii[i] + h) <= CGAL::squared_distance(nn->point(), participants[i])) winners.push_back(i);
  }
  if(winners.size() > 0){
    for(int w: winners) cout << w << " ";
    cout << endl;
    return;
  }

  //One by one elimination as lights turn on
  VI still_in_game(m,1);
  VI elimination_round(m,-1);
  int max_elimination_round  = -1;
  int count_out = 0;
  for(int i = 0; i < n; i++){
    if(count_out == m) break;
    for(int j = 0; j < m; j++){
      if(count_out == m) break;
      if(still_in_game[j] == 0) continue;
      if((radii[j] + h) * (radii[j] + h) <= CGAL::squared_distance(lights[i], participants[j])) {
        elimination_round[j] = i;
        max_elimination_round  = i;
      }
      else {
        still_in_game[j] = 0;
        count_out++;
      }
    }
  }
  for(int i = 0; i < m; i++){
    if(elimination_round[i] == max_elimination_round) cout << i << " ";
  }
  cout << endl;
}

int main(){
   std::ios_base::sync_with_stdio(false);
   int t; cin >> t;
   for(int i = 0; i < t; i++){
     cin >> m >> n;
     light_the_stage();
   }
   return 0;
}
