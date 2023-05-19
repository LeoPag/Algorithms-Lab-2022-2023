#include <iostream>
#include <vector>

using namespace std;

typedef vector<int> VI;

int n, subarrays_number;
VI coin_values;
VI dp_table_value_player;
VI dp_table_value_adversarial;

int value_player(int start, int end);
int value_adversarial(int start, int end);

int value_adversarial(int start, int end){
  int subarrays_starting_after_start_index = ( (n - start) * ( n - start + 1))/2;
  int dp_index = subarrays_number - subarrays_starting_after_start_index + end - start;

  if(dp_table_value_adversarial[dp_index] != -1){
    return dp_table_value_adversarial[dp_index];
  }
  else{
    if(start == end){
      dp_table_value_adversarial[dp_index] = 0;
      return(0);
    }
    else{
      int value_subarray = min(value_player(start + 1, end), value_player(start, end -1));
      dp_table_value_adversarial[dp_index] = value_subarray;
      return value_subarray;
    }
  }
}


int value_player(int start, int end){
  int subarrays_starting_after_start_index = ( (n - start) * ( n - start + 1))/2;
  int dp_index = subarrays_number - subarrays_starting_after_start_index + end - start;

  if(dp_table_value_player[dp_index] != -1){
    return dp_table_value_player[dp_index];
  }
  else{
    if(start == end){
      dp_table_value_player[dp_index] = coin_values[start];
      return(coin_values[start]);
    }
    else{
      int value_subarray = max(coin_values[start] + value_adversarial(start + 1, end), coin_values[end] + value_adversarial(start, end - 1));
      dp_table_value_player[dp_index] = value_subarray;
      return(value_subarray);
    }
  }
}


void burning_coins(){
  cout << value_player(0, n-1) << endl;
}

void read_input(){
  cin>>n;
  coin_values = VI(n,0);
  subarrays_number = (n*(n+1) / 2); /*The number of subarrays in an array of length n is computed with Gauss formula (n*(n+1) / 2)*/
  dp_table_value_player = VI(subarrays_number, - 1);
  dp_table_value_adversarial = VI(subarrays_number, - 1);
  for(int j= 0; j < n; j++){
    int new_value; cin>>new_value;
    coin_values[j] = new_value;
  }
}

int main(){
  int test_cases;
  cin>>test_cases;
  for(int i = 0; i < test_cases; i++){
    read_input();
    burning_coins();
  }
 return 0;
}
