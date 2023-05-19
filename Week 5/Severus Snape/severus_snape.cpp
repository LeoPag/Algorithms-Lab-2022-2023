#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
long n,m,a,b,P,H,W;

typedef vector<int> VI;
typedef vector<long> VL;
typedef vector<VL> VVL;
typedef vector<VVL> VVVL;

int severus_snape(){

  VL powers(n);
  VI happiness(n);
    VL wits(m);
  for(int i = 0; i < n; i++) cin >> powers[i] >> happiness[i];
  for(int i = 0; i < m; i++) cin >> wits[i];

  const long MIN = -pow(2,51);
  //dp[i][j][h] contains max power achievable when using exactly j potions among the first i and getting at least h happiness
  VVVL dp(n+1, VVL(n+1, VL(H+1, MIN)));

  //Base case j = 1;
  for(int i = 1; i <= n; i++){
    for(int h = 0; h <= H; h++){
      dp[i][1][h] = dp[i-1][1][h];
      if((happiness[i-1] >= h) & (powers[i-1] > dp[i][1][h])) dp[i][1][h] = powers[i-1];
    }
  }

  for(int j = 2; j <= n; j++){
    for(int i = j; i <= n; i++){
      for(int h = 0; h <= H; h++){
        int idx = max(0, h-happiness[i-1]);
        dp[i][j][h] = max(dp[i-1][j][h], dp[i-1][j-1][idx] + powers[i-1]);
      }
    }
  }

  sort(wits.begin(), wits.end(), greater<long>());
  long current_wit = 0;
  for(int i = 0; i < m; i++){
    current_wit += wits[i];
    if(current_wit >= W){
      long buffer = current_wit - W;
      long max_a  = n;
      if(a > 0) max_a = min(buffer / a, n);
      for(int j = 1; j <= max_a; j++){
        long extra_p = (i+1) * b;
        if(dp[n][j][H] >= P + extra_p) return (i+1) + j;
      }
    }
  }
  return -1;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> a >> b >> P >> H >> W;
    cout << severus_snape() << endl;;
  }
  return 0;
}
