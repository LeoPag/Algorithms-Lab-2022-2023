#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<int> VI;

void display(VI v){
  for(int elem: v){
    cout << elem << " ";
  }
  cout << endl;
}

VI get_vector(){
  int n;  cin >> n;
  VI v(n);
  for(int j = 0; j < n; j++){
    int new_elem; cin >> new_elem;
    v[j] = new_elem;
  }

  return v;
}

void display_sorted(VI v, int bool_order){
  if(bool_order == 1){
  sort(v.begin(), v.end(), greater<int>());
  }
  else{
  sort(v.begin(), v.end());
  }
  display(v);
}


int main(){
  int test_cases; cin >> test_cases;
  for(int i =0; i < test_cases; i++){
    VI v = get_vector();
    int bool_order; cin >> bool_order;
    display_sorted(v,bool_order);
  }
  return 0;
}
