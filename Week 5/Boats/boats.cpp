#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Boat{
  int length, ring;

  Boat(int l, int r): length(l), ring(r){}
};

bool operator < (const Boat& b1, const Boat& b2) {
    return (b1.ring < b2.ring);
}

typedef vector<Boat> VB;
int n;

void boats(){
  VB boats;
  for(int i = 0; i < n; i++){
    int l,r; cin >> l >>r;
    Boat b(l,r);
    boats.push_back(b);
  }

  sort(boats.begin(), boats.end());

  int count_boats = 0;
  const int MIN = -1000000;
  int last_boat_end = MIN;
  int second_to_last_boat_end = MIN;

  for(auto b: boats){
    if(b.ring >= last_boat_end){
      second_to_last_boat_end = last_boat_end;
      last_boat_end = max(b.ring, last_boat_end + b.length);
      count_boats++;
    }
    else{
      int new_last_end = max(b.ring, second_to_last_boat_end + b.length);
      last_boat_end = min(last_boat_end, new_last_end);
    }
  }
  cout << count_boats << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n;
    boats();
  }
}
