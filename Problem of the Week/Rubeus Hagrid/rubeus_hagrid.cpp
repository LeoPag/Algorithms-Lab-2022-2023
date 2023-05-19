#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
int n;

typedef vector<int> VI;

struct Node {
    long id, time_to_close, distance, subtree_size;
    // This constructor is convenient for a concise initialisation. It can also
    // be omitted and the member variables be set manually.
    Node(long i, long t, long d, long st) : id(i), time_to_close(t), distance(d), subtree_size(st){}
};

// Linearly order nodes by increasing time_to_close / subnode_size ratio
bool operator < (const Node& n1, const Node& n2) {
    return (n1.time_to_close * n2.subtree_size < n2.time_to_close * n1.subtree_size);
}

typedef vector<Node> VN;
typedef vector<VN> VVN;
VVN adj_list;
VI gallons;
long max_gallons, clock_time;

pair<int,int> dfs_explore(int start_node){
  long new_time_to_close = 0;
  long new_subtree_size = 1;
  for(auto neigh = adj_list[start_node].begin(); neigh != adj_list[start_node].end(); neigh++){
    auto next = dfs_explore(neigh->id);
    long neigh_time_to_close = next.first;
    long neigh_subtree_size = next.second;
    neigh->time_to_close = neigh_time_to_close + 2 * neigh->distance;
    neigh->subtree_size = neigh_subtree_size;
    new_time_to_close  += neigh_time_to_close + 2 * neigh->distance;
    new_subtree_size += neigh_subtree_size;
  }

  return make_pair(new_time_to_close, new_subtree_size);
}

void dfs_exploit(int start_node){
  max_gallons += gallons[start_node] - clock_time;
  sort(adj_list[start_node].begin(), adj_list[start_node].end());
  for(auto neigh = adj_list[start_node].begin(); neigh != adj_list[start_node].end(); neigh++){
    clock_time += neigh->distance;
    dfs_exploit(neigh->id);
    clock_time += neigh->distance;
  }
}


void rubeus_hagrid(){
  gallons = VI(n+1, 0); //Index 0 denotes start
  for(int i = 1; i <= n; i++) cin >> gallons[i];

  adj_list = VVN(n+1);
  for(int i = 0; i < n; i++){
    long from, to, distance;
    cin >> from >> to >> distance;
    Node new_node(to, 0, distance,1);
    adj_list[from].push_back(new_node);
  }

  //First exploration
  dfs_explore(0);

  //Second exploration according to closing time/sub_node size ratio
  max_gallons = 0;
  clock_time = 0;

  dfs_exploit(0);

  cout << max_gallons << endl;
}

int main() {
    std::ios_base::sync_with_stdio(false); // Always!
    int t; std::cin >> t; // Read the number of test cases
    for (int i = 0; i < t; ++i){
        cin >> n;
        rubeus_hagrid(); // Solve a particular test case
    }
    return 0;
 }
