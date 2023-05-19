#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>

void read_and_print_line(){

  int value1; std::cin>>value1;
  long value2; std::cin >>value2;
  std::string value3; std::cin >>value3;
  double value4; std::cin >> value4;
  value4 = (round(value4*100)) / 100;
  char space = ' ';

  std::cout.precision(2);
  std::cout << std::fixed << value1 << space <<  value2 << space << value3 << space << value4 << std::endl;
}

int main(){
  int test_cases;
  std::ios_base::sync_with_stdio(false);
  std::cin>>test_cases;
  for(int i = 0; i < test_cases; i++){
    read_and_print_line();

  }
  return 0;
}
