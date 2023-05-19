#include <iostream>
#include <vector>

using namespace std;
int n,m,r,b;

typedef vector<int> VI;
typedef vector<VI> VVI;

int the_great_game(){
  const int OUT_OF_SCALE = 10000000;
  VVI adj_list(n+1);
  //Build adj_list
  for(int i = 0; i < m; i++){
    int from, to; cin >> from >> to;
    adj_list[from].push_back(to);
  }

  VI best_distance(n+1,0);
  VI best_neighbours(n+1,0);
  VI worst_distance(n+1, 0);
  VI worst_neighbours(n+1,0);
  for(int k = n-1; k >= 1; k--){ //iterate through nodes from closer to further
    int min_dist = OUT_OF_SCALE;
    int best_neighbour = OUT_OF_SCALE;
    for(int neigh: adj_list[k]){
      if( 1 + worst_distance[neigh] < min_dist){
        min_dist = 1 + worst_distance[neigh];
        best_neighbour = neigh;
      }
    }
    best_distance[k] = min_dist;
    best_neighbours[k] = best_neighbour;

    int max_dist = -1;
    int worst_neighbour = OUT_OF_SCALE;
    for(int neigh: adj_list[k]){
      if(1 + best_distance[neigh] > max_dist){
        max_dist = max(max_dist, 1 + best_distance[neigh]);
        worst_neighbour = neigh;
      }
    }
    worst_distance[k] = max_dist;
    worst_neighbours[k] = worst_neighbour;
  }

  int count_moves = 0;
  while (true){
    if(count_moves%2 == 0){
      r = best_neighbours[r];
      b = best_neighbours[b];
      if(r == n) return 0;
      else if(b == n) return 1;
    }
    else{
      r = worst_neighbours[r];
      b = worst_neighbours[b];
      if(b == n) return 1;
      else if(r == n) return 0;
    }
    count_moves += 1;
  }
}

int main(){
    std::ios_base::sync_with_stdio(false);
    int t; cin >> t;
    for(int i = 0; i < t; i++){
      cin >> n >> m >> r >> b;
      cout << the_great_game() << endl;
    }
}
