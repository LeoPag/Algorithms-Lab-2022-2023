#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>

using namespace std;
int n;
typedef vector<int> VI;
typedef set<int> SI;

void beach_bars(){

  const int INF = 10000000;
  VI parasols(n);
  SI best_bars;
  for(int i = 0; i < n; i++) cin >> parasols[i];

  sort(parasols.begin(), parasols.end());

  int bar = parasols[0];
  int left_end = parasols[0];
  int right_end = parasols[0];

  int max_parasols = 0;
  int j = 0; int k = 0;

  int min_longest_distance = INF;
  int curr_parasols = max_parasols;
  int curr_longest_distance = 0;

  while(bar < parasols[n-1]){
    bar++;
    while(j < n-1){
      j++;
      if(parasols[j] - bar <= 100){
        right_end = parasols[j];
        curr_parasols++;
      }
      else{
        j--;
        break;
      }
    }
    right_end = parasols[j];
    while((bar - left_end > 100) & (left_end < bar)){
      k++;
      left_end = parasols[k];
      curr_parasols--;
    }
    curr_parasols = j-k+1;
    curr_longest_distance = max(right_end - bar, bar - left_end);

    if((curr_parasols == max_parasols) & (curr_longest_distance == min_longest_distance)){
      best_bars.insert(bar);
    }
    else if ((max_parasols == curr_parasols)  & (curr_longest_distance < min_longest_distance)){
      best_bars.clear();
      best_bars.insert(bar);
      min_longest_distance = curr_longest_distance;
    }
    else if(curr_parasols > max_parasols){
      best_bars.clear();
      best_bars.insert(bar);
      max_parasols = curr_parasols;
      min_longest_distance = curr_longest_distance;
    }
  }

  cout << max_parasols << " " << min_longest_distance << endl;
  for(auto b: best_bars) cout << b << " ";
  cout << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n;
    beach_bars();
  }
}
