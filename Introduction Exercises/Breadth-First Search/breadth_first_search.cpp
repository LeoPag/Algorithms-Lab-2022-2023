#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

int count_distance;
VI distances;
VI bool_visited;
VVI adjacency_list;

void display(VI to_display){
for (int elem: to_display){
  cout << elem << " ";
}
cout << endl;
}


VVI build_adjacency_list(int n, int m){

  VVI adjacency_list(n);

  for(int j = 0; j < m; j++){
    int node1; cin >> node1;
    int node2; cin >> node2;
    adjacency_list[node1].push_back(node2);
    adjacency_list[node2].push_back(node1);
  }

  return adjacency_list;
}

void breadth_first_search(VVI adjacency_list,VI to_visit){
  VI next_to_visit;
  bool new_elem_found = false;
  for (int node:to_visit){
    if(bool_visited[node] == 0){
      distances[node] = count_distance;
      bool_visited[node] = 1;
      for(int next:adjacency_list[node]){
        next_to_visit.push_back(next);
        new_elem_found = true;
      }
    }
  }
  count_distance += 1;
  if(new_elem_found == true){
    breadth_first_search(adjacency_list,next_to_visit);
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; cin >> test_cases;
  int n,m,start_node;

  for (int i = 0; i < test_cases; i++){
  cin >> n;
  cin >> m;
  cin >> start_node;

  bool_visited = VI(n,0);
  distances = VI(n,-1);
  distances[start_node] = 0;
  bool_visited[start_node] = 1;
  count_distance = 1;
  auto adjacency_list = build_adjacency_list(n,m);

  breadth_first_search(adjacency_list, adjacency_list[start_node]);
  display(distances);
  }
  return 0;
}
