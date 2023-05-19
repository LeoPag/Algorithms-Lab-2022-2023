#include <iostream>
#include <vector>
#include <tuple>
using namespace std;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef tuple<int,int,int> TRIS;

int n;
const int INF = 1000000;

TRIS dfs(const VI& costs, const VVI& adj_list, int start_node){
  int selected = costs[start_node];
  int covered = 0;
  int uncovered = 0;
  int best_child_diff = INF;
  for(int child: adj_list[start_node]){
    TRIS child_state = dfs(costs, adj_list, child);
    best_child_diff = min(best_child_diff, get<0>(child_state)-get<1>(child_state));
    selected += get<2>(child_state);
    uncovered += get<1>(child_state);
  }
  covered = uncovered + best_child_diff;
  covered = min(selected, covered);
  uncovered = min(covered, uncovered);
  TRIS state(selected, covered, uncovered);
  return(state);
}

void chariot_race(){
  VVI adj_list(n);
  for(int i = 0; i < n-1; i++){
    int from, to; cin >> from >> to;
    adj_list[from].push_back(to);
  }

  VI costs(n);
  for(int i = 0; i < n; i++) cin >> costs[i];

  cout << get<1>(dfs(costs, adj_list, 0)) << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n;
    chariot_race();
  }
  return 0;
}
