#include <vector>
#include <iostream>

using namespace std;

int n;
typedef vector<int> VI;

void dominoes(){

  VI heights(n);
  for(int i = 0; i < n; i++) cin >> heights[i];

  int trigger_height = heights[0];
  int trigger_index = 0;
  int fallen_dominoes = 1;

  for(int i = 1; i < n; i++){
    int distance_from_trigger = i - trigger_index;
    if(distance_from_trigger < trigger_height){
      fallen_dominoes++;
      if(heights[i] > trigger_height - distance_from_trigger){
        trigger_index = i;
        trigger_height = heights[i];
      }
    }
  }
  cout << fallen_dominoes << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n;
    dominoes();
  }
  return 0;
}
