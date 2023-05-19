#include <iostream>
#include <vector>

using namespace std;
int n,m,k;

typedef vector<int> VI;
typedef vector<pair<int,int>> VMEMO;
typedef vector<VMEMO> VVMEMO;

void lord_voldemort(){
  VI horcruxes(n);
  for(int i = 0; i < n; i++) cin >> horcruxes[i];

  int i = 0; int j = 0;
  int sum = horcruxes[0];
  VI intervals(n,-1);

  //SLIDING WINDOW -- FIND INTERVALS; INTERVALS[start] = END
  while(j < n){
    if(sum < k){
      j++;
      if(j < n) sum += horcruxes[j];
    }
    else if(sum == k){
      intervals[i] = j;
      sum -= horcruxes[i];
      i++;
      if(i>j){
        j = i;
        if(j < n) sum += horcruxes[j];
      }
    }
    else{
      sum -= horcruxes[i];
      i++;
      if(i>j){
        j = i;
        if(j < n) sum += horcruxes[j];
      }
    }
  }
  //memo[a][i] contains a pair with max horcruxes killed using at most a magicians and horcruxes from i onwards;
  VVMEMO dp(m+1,VMEMO(n+1,{0,0}));

  for(int a = 1; a <= m; a++){
    for(int i = n-1; i >= 0; i--){
      dp[a][i] = dp[a][i+1];

      int j = intervals[i];

      if((j != -1) && ((dp[a-1][j+1].second + j - i + 1 > dp[a][i].second))){
        dp[a][i].second = dp[a-1][j+1].second + j - i + 1;
        dp[a][i].first = dp[a-1][j+1].first + 1;
      }
    }
  }

  if(dp[m][0].first != m) cout << "fail" << endl;
  else cout << dp[m][0].second << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> k;
    lord_voldemort();
  }
  return 0;
}
