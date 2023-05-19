#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <set>

using namespace std;
int n,m,k;

typedef tuple<int,int,int> TRIS; //min_temp + max_temp + parent;
typedef vector<TRIS> VTRIS;
typedef vector<VTRIS> VVTRIS;
typedef vector<int> VI;
typedef set<int> SI;

void new_york(){
  VVTRIS dp_table(n);
  VI parent(n);
  for(int i = 0; i < n; i++){
    int temp; cin >> temp;
    TRIS new_t(temp, temp, i);
    dp_table[i].push_back(new_t);
  }
  for(int i = 0; i < n-1; i++){
    int from, to; cin >> from >> to;
    parent[to] = from;
  }
  parent[0] = 0;

  const int INF = 10000000;
  int s = 2;
  int count_entrance = 0;
  while(s <= m){
    for(int i = 0; i < n; i++){
      int pred = parent[get<2>(dp_table[i][count_entrance])];
      if(get<2>(dp_table[i][count_entrance]) != 0){
        int min_t = min(get<0>(dp_table[i][count_entrance]), get<0>(dp_table[pred][count_entrance]));
        int max_t = max(get<1>(dp_table[i][count_entrance]), get<1>(dp_table[pred][count_entrance]));
        int new_e = get<2>(dp_table[pred][count_entrance]);
        TRIS new_t(min_t, max_t, new_e);
        dp_table[i].push_back(new_t);
      }
      else{
        TRIS new_t(-INF, INF, 0);
        dp_table[i].push_back(new_t);
      }
    }
    count_entrance += 1;
    s = s*2;
  }
  s = s / 2;

  int power = 12;
  while(power >= 0){
    while(m-s >= pow(2,power)){
      for(int i = 0; i < n; i++){
        int pred = parent[get<2>(dp_table[i][count_entrance])];
        if(get<2>(dp_table[i][count_entrance]) != 0){
          int min_t = min(get<0>(dp_table[i][count_entrance]), get<0>(dp_table[pred][power]));
          int max_t = max(get<1>(dp_table[i][count_entrance]), get<1>(dp_table[pred][power]));
          int new_e = get<2>(dp_table[pred][power]);
          TRIS new_t(min_t, max_t, new_e);
          dp_table[i].push_back(new_t);
        }
        else{
          TRIS new_t(-INF, INF, 0);
          dp_table[i].push_back(new_t);
        }
      }
      count_entrance += 1;
      s = s + pow(2,power);
    }
    power--;
  }
  SI critical_points;
  for(int i = 0; i < n; i++){
    if(get<1>(dp_table[i][count_entrance]) - get<0>(dp_table[i][count_entrance]) <= k) {
      critical_points.insert(get<2>(dp_table[i][count_entrance]));
    }
  }
  if(critical_points.empty()) cout << "Abort mission" << endl;
  else {
    for(auto c: critical_points) cout << c << " ";
    cout << endl;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> k;
    new_york();
  }
  return 0;
}
