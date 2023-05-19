#include <iostream>
#include <vector>

using namespace std;
int n;

typedef vector<int> VI;
typedef vector<VI> VVI;

//Take as input the row index and counts the even pairs -- Linear time;
int count_even_pairs(const VI& v){
  int new_even_pairs = 0; int even_pairs = 0;
  int new_odd_pairs = 0;
  for(int i = 0; i < n; i++){
    if(v[i] % 2 == 0){
      new_even_pairs = 1 + new_even_pairs;
      even_pairs += new_even_pairs;
    }
    else if(v[i] % 2 == 1){
      int temp = new_odd_pairs;
      new_odd_pairs = 1 + new_even_pairs;
      new_even_pairs = temp;
      even_pairs += new_even_pairs;
    }
  }
  return even_pairs;
}

void even_matrices(){
   //Read input and build matrix;
   VVI matrix(n,VI(n,0));
   for(int i = 0; i < n; i++){
     for(int j = 0; j < n; j++){
       int bit; cin >> bit;
       matrix[i][j] = bit;
     }
   }

   VVI cumulative_sum(n,VI(n,0));
   int even_matrices = 0;
   for(int i = 0; i < n; i++){
     for (int j = 0; j <= i; j++){
       for(int k = 0; k < n; k++){
        cumulative_sum[j][k] +=  matrix[i][k];
       }
       even_matrices += count_even_pairs(cumulative_sum[j]);
     }
   }
   cout << even_matrices << endl;
}
int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n;
    even_matrices();
  }
}
