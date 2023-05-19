#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
int n,k;

typedef vector<int> VI;

void deck_of_cards(){

  VI values(n);
  for(int i = 0; i < n; i++) cin >> values[i];

  int i = 0; int j = 0;
  int sum = values[0];
  pair<int,int> best_pair = make_pair(0,0);
  int best_value = abs(sum - k);
  while(j < n){
    if(sum < k){
      j++; sum += values[j];
    }
    else if (sum == k){
      best_value = 0;
      best_pair.first = i;
      best_pair.second = j;
      break;
    }
    else{
      sum -= values[i]; i++;
      if(i > j){
        j = i;
        sum += values[j];
      }
    }
    if(abs(sum - k) < best_value){
      best_value = abs(sum - k);
      best_pair.first = i;
      best_pair.second = j;
    }
  }
  cout << best_pair.first << " " << best_pair.second << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> k;
    deck_of_cards();
  }
}
