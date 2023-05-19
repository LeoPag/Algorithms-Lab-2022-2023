#include <iostream>

int main(){

  int test_cases;

  std::ios_base::sync_with_stdio(false);
  std::cin>>test_cases;

  for(int i = 0; i < test_cases; i++){

    int number_of_integers;
    std::cin >> number_of_integers;

    int sum = 0;
    for(int  j = 0; j < number_of_integers; j++){
      int new_integer;
      std::cin >> new_integer;
      sum = sum + new_integer;
    }
    std::cout << sum << std::endl;

  }
  return 0;

}
