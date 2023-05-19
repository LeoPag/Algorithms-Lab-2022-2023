#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

int n;

struct Bike{
  CGAL::Gmpq m,q;
  int index;

  Bike(CGAL::Gmpq m, CGAL::Gmpq q, int index) : m(m), q(q), index(index){}
};

// Linearly order bikes by increasing q
bool operator < (const Bike& b1, const Bike& b2) {
    return (b1.q < b2.q);
}

typedef vector<Bike> VB;
typedef vector<int> VI;

void motorcycles(){

  VB bikes;

  for(int i = 0; i < n; i++){
    long y0,x1,y1; cin >> y0 >> x1 >> y1;
    CGAL::Gmpq angular_coeff((y1 - y0),x1);
    Bike new_bike(angular_coeff, y0, i);
    bikes.push_back(new_bike);
  }

  sort(bikes.begin(), bikes.end());
  VI forever_into_sunrise(n,1);
  Bike ref_bike = bikes[0];
  //First linear pass
  for(int i = 1; i < n; i++){
    CGAL::Gmpq curr_m = bikes[i].m;
    CGAL::Gmpq ref_m = ref_bike.m;
    if((CGAL::abs(curr_m) < CGAL::abs(ref_m)) or ((CGAL::abs(curr_m) == CGAL::abs(ref_m)) & (curr_m > ref_m))){
      ref_bike = bikes[i];
    }
    else if(curr_m < ref_m){
      int idx = bikes[i].index;
      forever_into_sunrise[idx] = 0;
    }
  }
  ref_bike = bikes[n-1];
  for(int i = n-2; i >= 0; i--){
    CGAL::Gmpq curr_m = bikes[i].m;
    CGAL::Gmpq ref_m = ref_bike.m;
    if(CGAL::abs(curr_m) <= CGAL::abs(ref_m)) {
      ref_bike = bikes[i];
    }
    else if(curr_m > ref_m){
      int idx = bikes[i].index;
      forever_into_sunrise[idx] = 0;
    }
  }

  for(int i = 0; i < n; i++){
    if(forever_into_sunrise[i]) cout << i << " ";
  }
  cout << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n;
    motorcycles();
  }
  return 0;
}
