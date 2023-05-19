#include <iostream>
#include <stack>
#include <unordered_map>
#include <set>
#include <vector>
#include <stack>
#include <algorithm>

std::vector<int> timestamps_discovery;
std::vector<int> timestamps_closed;
std::vector<int> visited;
std::vector<std::vector<int>> adj_list;

int time_counter;

void display_int_vector(std::vector<int> int_vector){
     for (int k: int_vector){
       std::cout << k << " ";
     }
     std::cout << std::endl;
}


std::vector<std::vector<int>> build_adjacency_list(int n, int m){

  std::vector<std::vector<int>>  adjacency_list(n);

  for(int j = 0; j < m; j++){
    int node1; std::cin >> node1;
    int node2; std::cin >> node2;
    adjacency_list[node1].push_back(node2);
    adjacency_list[node2].push_back(node1);
  }
  return adjacency_list;

}


void depth_first_search(const std::vector<std::vector<int>> adjacency_list, int start_node){

  if(visited[start_node] == 0){
    timestamps_discovery[start_node] = time_counter;
    visited[start_node] = 1;
  }
  time_counter += 1;
  std::vector<int> current = adjacency_list[start_node];
  std::sort(current.begin(), current.end());
  for(int j: current){
    if(visited[j] == 0){
    depth_first_search(adjacency_list,j);
    }
  }
  timestamps_closed[start_node] = time_counter;
  time_counter += 1;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; std::cin >> test_cases;

  for (int i = 0; i < test_cases; i++){
    int n; std::cin >> n;
    int m; std::cin >> m;
    int start_node; std::cin >> start_node;

    time_counter = 0;
    timestamps_discovery = std::vector<int>(n,-1);
    timestamps_closed = std::vector<int>(n,-1);
    visited = std::vector<int> (n,0);

    auto adjacency_list = build_adjacency_list(n,m);

    depth_first_search(adjacency_list, start_node);
    display_int_vector(timestamps_discovery);
    display_int_vector(timestamps_closed);
  }
  return 0;
  }
