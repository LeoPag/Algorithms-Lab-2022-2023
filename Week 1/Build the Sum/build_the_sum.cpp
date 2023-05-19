#include <iostream>

using namespace std;


void build_the_sum(){
  int n; cin >> n;
  int sum = 0;
  for(int j = 0; j < n; j++){
    int new_integer; cin >> new_integer;
    sum = sum + new_integer;
  }
  cout << sum << endl;
}


int main(){
  int test_cases; cin >> test_cases;
  for(int i = 0; i < test_cases; i++){
    build_the_sum();
  }
  return 0;
}
