#include <iostream>
#include <string>

void length(std:: string a,std::string b){
  std::cout << a.length() << ' ' << b.length() << std::endl;
}

void concat(std:: string a,std::string b){
  std::cout << a + b << std::endl;
}

std::string reverse(std::string str){
  long length = str.length();
  std::string reversed_string(length, ' ');
  for (long i = 0; i < length; i++){
    reversed_string[i] = str[str.length() - 1 - i];
  }
  return reversed_string;
}

void reverse_and_switch(std:: string a,std::string b){
  std::string c; c = reverse(a);
  std::string d; d = reverse(b);
  char temp = c[0];
  c[0] = d[0];
  d[0] = temp;
  std::cout << c << " " << d << std::endl;
}

int main(){
  int test_cases;
  std::ios_base::sync_with_stdio(false);
  std::cin >> test_cases;

  for(int i = 0; i < test_cases; i++){
    std::string a; std::cin >> a;
    std::string b; std::cin >> b;

    length(a,b);
    concat(a,b);
    reverse_and_switch(a,b);

  }
  return 0;
}
