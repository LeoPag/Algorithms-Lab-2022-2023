#include <iostream>
#include <vector>

using namespace std;

long n,m,x,k;

typedef vector<long> VL;
typedef vector<VL> VVL;

void san_francisco(){
  VVL adj_list(n);
  VVL weights(n);
  for(int i = 0; i < m; i++){
    int from, to , w; cin >> from >> to >> w;
    adj_list[from].push_back(to);
    weights[from].push_back(w);
  }

  VVL dp_table(n,VL(k+1, 0));

 for(int j = 1; j <= k; j++){
    for(int i = 0; i < n; i++){
      if(adj_list[i].size()== 0){//Weayaya
        dp_table[i][j] = dp_table[0][j];
      }
      else{
        for(int l = 0; l < adj_list[i].size(); l++){
          dp_table[i][j] = max(dp_table[i][j], dp_table[adj_list[i][l]][j-1] + weights[i][l]);
        }
      }
    }
  }
  for(int i = 0; i <= k; i++){
    if(dp_table[0][i] >= x){
      cout << i << endl;
      return;
    }
  }
  cout << "Impossible"<< endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> x >> k;
    san_francisco();
  }
}
