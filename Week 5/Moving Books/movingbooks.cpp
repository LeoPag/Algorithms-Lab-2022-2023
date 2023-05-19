#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;
int n,m;

typedef vector<int> VI;
typedef multiset<int> SI;

void moving_books(){
  VI strengths(n);
  SI weights;
  int max_s = 0;
  for(int i = 0; i < n; i++) {
    int s; cin >> s; strengths[i] = -s;
    max_s = max(max_s, s);
  }
  int max_w = 0;
  for(int i = 0; i < m; i++){
    int w; cin >> w; weights.insert(-w);
    max_w = max(max_w, w);
  }
  if(max_w > max_s) {
    cout << "impossible" << endl;
    return;
  }

  sort(strengths.begin(), strengths.end());

  int total_time = 0;

  while(weights.empty() == false){
    for(int s = 0; s < n; s++){
      auto it = weights.lower_bound(strengths[s]);
      if(it != weights.end()) weights.erase(it);
      else break;
    }
    total_time += 3;
  }
  cout << total_time - 1 << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m;
    moving_books();
  }
}
