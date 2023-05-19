#include <iostream>
#include <vector>

using namespace std;
int n,k,w;

typedef vector<int> VI;
typedef vector<VI> VVI;


void greyjoy(){
  VI costs(n);

  VVI dp_table(w);
  VVI cost_branches(w);

  for(int i = 0; i < n; i++) {
    cin >> costs[i];
  }

  const int  pyke_value = costs[0];

  //Dp_table stores cumulative cost with increasing distance from Pyke
  for(int i = 0; i < w; i++){
    int l; cin >> l;
    VI cost_branch(l);
    VI dp_table_branch(l);
    for(int j = 0; j < l; j++){
      int island; cin >> island;
      cost_branch[j] = costs[island];
      if(j == 0) dp_table_branch[j] = 0;
      else dp_table_branch[j] = dp_table_branch[j-1] + costs[island];
    }
    dp_table[i] = dp_table_branch;
    cost_branches[i] = cost_branch;
  }


  int max_islands = 0;

  // -- Sliding window on one branch --
  for(int i = 0; i < w; i++){
    int sum = pyke_value;
    int start_window = 0;
    int end_window = 0;
    while(start_window < cost_branches[i].size()){
      if(sum == k){
        max_islands = max(max_islands, end_window - start_window + 1);
        sum -= cost_branches[i][start_window];
        start_window++;
        if(start_window == cost_branches[i].size()) break;
        if(start_window > end_window) {
          sum += cost_branches[i][start_window];
          end_window = start_window;
        }
      }
      else if(sum < k){
        end_window++;
        if(end_window == cost_branches[i].size()) break;
        else sum += cost_branches[i][end_window];
      }
      else if(sum > k){
        sum -= cost_branches[i][start_window];
        start_window++;
        if(start_window == cost_branches[i].size()) break;
        if(start_window > end_window){
          sum += cost_branches[i][start_window];
          end_window = start_window;
        }
      }
    }
  }

  // -- Max number maps each sum to k to max islands to get that number , use it as a lookup table
  VI max_number(k+1,0);
 int new_target = k - pyke_value;

 if(new_target > 0){
   for(int i = 0; i < w; i++){
    for(int j = 0; j < dp_table[i].size(); j++){
      if(dp_table[i][j] > new_target) break;
        int complementary_to_target = max_number[new_target - dp_table[i][j]];
        if (complementary_to_target > 0){
           max_islands = max(max_islands, complementary_to_target + j + 1);
        }
      }
    for(int j = 0; j < dp_table[i].size(); j++){
      if(dp_table[i][j] > new_target) break;
      max_number[dp_table[i][j]] = max(max_number[dp_table[i][j]],j);
      }
    }
 }
  cout << max_islands << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> k >> w;
    greyjoy();
  }
  return 0;
}
