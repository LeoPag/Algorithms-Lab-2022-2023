#include <iostream>
#include <vector>

using namespace std;
int n;

typedef vector<int> VI;
typedef vector<VI> VVI;

void dhl(){

  const int OUT_OF_BOUNDS = 10000000;

  VI a_stack(n);
  VI b_stack(n);

  for(int i = 0; i < n; i++) cin >> a_stack[i];
  for(int i = 0; i < n; i++) cin >> b_stack[i];

  VVI dp_table(n+1,VI(n+1,0));
  for(int i = 1; i <= n; i++) dp_table[i][0] = OUT_OF_BOUNDS;
  for(int i = 1; i <= n; i++) dp_table[0][i] = OUT_OF_BOUNDS;

  for(int i = 1; i <= n; i++){
    for(int j = 1; j <= n; j++){
      dp_table[i][j] = min(min(dp_table[i-1][j], dp_table[i][j-1]), dp_table[i-1][j-1]);
      dp_table[i][j] += ((a_stack[i-1] - 1) * (b_stack[j-1] - 1));
    }
  }
  cout << dp_table[n][n] << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n ;
    dhl();
  }
  return 0;
}
