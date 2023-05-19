#include <iostream>
#include <vector>

void display_vector(std::vector<int> v){

  if (v.size() < 1){
  std::cout << "Empty" << std::endl;
  }
  else{
    for(int j: v){
      std::cout << j << ' ';
    }
    std::cout << std::endl;
  }

}


void single_case(){

  int n; std::cin >> n;
  std::vector<int> v;
  for (int i = 0; i<n; i++){
    int new_elem; std::cin >> new_elem;
    v.push_back(new_elem);
  }

  int d; std::cin >> d;
  v.erase(v.begin() + d);

  int a; std::cin >> a;
  int b; std::cin >> b;

  if (a < b){
  v.erase(v.begin() + a, v.begin() + b + 1);
  }

  else {
  v.erase(v.begin()+ b, v.begin() + a + 1);
  }

  display_vector(v);
}

int main(){
  int test_cases;
  std::ios_base::sync_with_stdio(false);
  std::cin >> test_cases;
  for(int  j = 0; j < test_cases; j++){
    single_case();
  }
  return 0;
}
