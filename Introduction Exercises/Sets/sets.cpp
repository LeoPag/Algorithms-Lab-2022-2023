#include <iostream>
#include <set>
#include <string>


void print_set(std::set<int> s){
  for (int elem:s){
    std::cout << elem << ' ';
  }
  std::cout << std::endl;
}


void single_test_case(){
  int q; std::cin >> q;
  std::set<int> s;

  for (int j = 0; j < q; j++){
    int bool_val; std::cin >> bool_val;
    int new_elem; std::cin >> new_elem;
    if(bool_val == 0){
      s.insert(new_elem);
    }
    else {
      std::set <int>::iterator it = s.find(new_elem);
      if (it != s.end()){
        s.erase(it);
      }
    }
  }
  if (s.size() < 1){
    std::cout << "Empty" << std::endl;
  }
  else{
    print_set(s);
  }
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; std::cin>>test_cases;
  for(int i = 0; i < test_cases; i++){
    single_test_case();
  }
  return 0;
}
