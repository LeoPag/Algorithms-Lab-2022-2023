#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> VI;

int n;
VI bits;
/*Runs in linear time - At every iteration j it finds the even pairs ending with <i,j> with i = 1,2,...,j*/
void even_pairs(){
  int evenpairs, evenpairs_with_end_index_just_before_i, oddpairs_with_end_index_just_before_i;
  if(bits[0] == 0){
    evenpairs = 1;
    evenpairs_with_end_index_just_before_i = 1;
    oddpairs_with_end_index_just_before_i = 0;
  }
  else{
    evenpairs = 0;
    evenpairs_with_end_index_just_before_i = 0;
    oddpairs_with_end_index_just_before_i = 1;
  }
  for(int i = 1; i < n; i++){
    int new_pairs = i + 1;
    if(bits[i] == 0){
      evenpairs = evenpairs + evenpairs_with_end_index_just_before_i + 1;
      evenpairs_with_end_index_just_before_i = evenpairs_with_end_index_just_before_i + 1;
      oddpairs_with_end_index_just_before_i = new_pairs - evenpairs_with_end_index_just_before_i;
    }
    else{
      evenpairs = evenpairs + oddpairs_with_end_index_just_before_i;
      oddpairs_with_end_index_just_before_i = evenpairs_with_end_index_just_before_i + 1;
      evenpairs_with_end_index_just_before_i = new_pairs - oddpairs_with_end_index_just_before_i;

    }
  }
  cout << evenpairs << endl;
}
int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; cin >> test_cases;
  for (int i = 0; i< test_cases; i++){
    cin >> n;
    bits = VI(n);
    for(int j = 0; j < n; j++){
      int new_bit; cin >> new_bit;
      bits[j] = new_bit;
    }
    even_pairs();
  }
}
