#include <iostream>
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;
int n;

struct Bomb {
  int t;
  int id;
};

// Linearly order bombs by increasing explosion time
bool operator < (const Bomb& b1, const Bomb& b2) {
    return (b1.t < b2.t);
}

typedef vector<int> VI;
typedef vector<Bomb> VB;
typedef stack<int> SI;

void octopussy(){

  VI deactivated(n,0);
  VB bombs(n);

  for(int i = 0; i < n; i++){
    int exp_time; cin >> exp_time;
    Bomb new_bomb({exp_time, i});
    bombs[i] = new_bomb;
  }

  sort(bombs.begin(), bombs.end());

  int clock_time = 0;

  for(int i = 0; i < n; i++){
    auto current_bomb = bombs[i];
    int current_id = current_bomb.id;
    if(deactivated[current_id] == 1) continue;
    int current_exploding_time = current_bomb.t;
    SI children;
    children.push(current_id);
    int time_for_current_bomb = 0;
    while(children.empty() == false){
      int child = children.top(); children.pop();
      deactivated[child] = 1;
      time_for_current_bomb++;
      int next1 = child*2 + 1;
      int next2 = child*2 + 2;
      if(next1 < n){
        if(deactivated[next1] == 0) children.push(next1);
      }
      if(next2 < n){
        if(deactivated[next2] == 0) children.push(next2);
      }
    }
    clock_time += time_for_current_bomb;
    if(clock_time > current_exploding_time){
      cout << "no" << endl;
      return;
    }
  }
  cout << "yes" << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i< t; i++){
    cin >> n;
    octopussy();
  }
  return 0;
}
