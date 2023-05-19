#include <iostream>
#include <map>
#include <string>
#include <set>

void display(std::string key_to_print, std::multimap <std::string, int> m){
  auto pair = m.equal_range(key_to_print);
  if (pair.first == pair.second){
    std::cout << "Empty";
  }
  else{
    std::set <int> s;
    for (auto it = pair.first; it != pair.second; it++){
    s.insert(it->second);
    }
    for (int elem:s){
      std::cout << elem << " ";
    }
  }
  std::cout<<std::endl;
}

void single_test_case(){
  int q; std::cin >> q;
  std::multimap<std::string,int> m;

  for (int j = 0; j < q; j++){
    int value; std::cin >> value;
    std::string key; std::cin >> key;
    if (value != 0){
      m.insert(std::make_pair(key,value));
    }
    else{
      m.erase(key);
    }
  }
  std::string key_to_print; std::cin >> key_to_print;
  display(key_to_print,m) ;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; std::cin >> test_cases;

  for(int i = 0; i < test_cases; i++){
    single_test_case();
  }
  return 0;
}
