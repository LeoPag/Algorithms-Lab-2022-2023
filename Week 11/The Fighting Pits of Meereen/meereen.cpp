#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
using namespace std;

//State variable are (p-q,i,memo north, memo south) where i denotes the last warrior taken into consideration
//DP table[p-q,i] maps every memory state at correspondent score.

struct Memory {
    // contains state encodings of the north and south queues. for example, 213 means the last 3 fighters were 2, 1 and 3
    int north_memory, south_memory;
    Memory(int m, int s) : north_memory(m), south_memory(s) {};
};

bool operator < (const Memory m1, const Memory m2) {
    if (m1.north_memory == m2.north_memory) {
        return m1.south_memory < m2.south_memory;
    } else {
        return m1.north_memory < m2.north_memory;
    }
}

int n,m,k;
typedef map<Memory,int> MEMO;
typedef vector<MEMO> VMEMO;
typedef vector<VMEMO> VVMEMO;
typedef set<int> SI;

int get_unique_warriors_in_memory(int memory){
  SI unique_warriors;
  while(memory != 0){
    int last_w = memory % 10;
    unique_warriors.insert(last_w);
    memory  = memory / 10;
  }
  return unique_warriors.size();
}


void meereen(){
  VVMEMO dp_table(n+1, VMEMO(23));    //  - 11 <= p-q <= 11 otherwise RIOT. 23 possibilities for state variable (p-q)
  dp_table[0][11][Memory(0,0)] = 0;            // Init dp_table; no score for no warriors; j = 11 -> p-q = 0;
  for(int i = 0; i < n; i++){
    int new_warrior; cin >> new_warrior;
    new_warrior += 1;                             // No zero as a warrior index --> makes state encoding easier;
    for(int j = 0; j < 23; j++){
      for(auto state: dp_table[i][j]){

        auto memory = state.first;
        auto score = state.second;
        int north_memory = memory.north_memory;
        int south_memory = memory.south_memory;

         // ADD new warrior to north entrance
        int new_north_memory = (north_memory * 10 + new_warrior);   //state encoding representation
        int unique_warriors = get_unique_warriors_in_memory(new_north_memory);
        new_north_memory = new_north_memory % int(pow(10, m-1));     //Memo encoding is at most m digits
        int new_difference_add_left = j - 11 + 1;        // Adding north yields p-q to increase by one.

        //Compute round score when adding north
        int new_north_score = 1000 * unique_warriors - pow(2, abs(new_difference_add_left));
        if(new_north_score > 0){ // NO RIOT
          dp_table[i+1][new_difference_add_left + 11][Memory(new_north_memory, south_memory)] =
          max(dp_table[i+1][new_difference_add_left + 11][Memory(new_north_memory, south_memory)],
            score +  new_north_score);  // Max operation is crucial to reduce representation; otherwise tree gets exponentially large;
        }


        //ADD new_warrior to south entrance
        int new_south_memory = (south_memory * 10 + new_warrior);   //state encoding representation
        unique_warriors = get_unique_warriors_in_memory(new_south_memory);
        new_south_memory = new_south_memory % int(pow(10, m-1)); // Memo encoding is at most m digits
        int new_difference_add_right = j - 11 - 1;        // Adding south yields p-q to decrease by one.

        //Compute round score when adding south
        int new_south_score = 1000 * unique_warriors - pow(2, abs(new_difference_add_right));
        if(new_south_score > 0){ // NO RIOT
          dp_table[i+1][new_difference_add_right + 11][Memory(north_memory, new_south_memory)] =
          max(dp_table[i+1][new_difference_add_right + 11][Memory(north_memory, new_south_memory)],
            score +  new_south_score);  // Max operation is crucial to reduce representation; otherwise tree gets exponentially large;
        }
      }
    }
  }
  int max_enthusiasm = 0;
  for(int PQ_difference = 0; PQ_difference < 23; PQ_difference++){
    for(auto memory: dp_table[n][PQ_difference]){
      max_enthusiasm = max(max_enthusiasm, memory.second);
    }
  }
  cout << max_enthusiasm << endl;
}


int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> k >> m;
    meereen();
  }
  return 0;
}
