#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;
int n,q;

typedef unordered_map<string,int> MSI;
typedef vector<int> VI;
typedef vector<string> VS;
typedef vector<VI> VVI;
typedef vector<pair<int,int>> VPAIR; //query id -- max_age
typedef vector<VPAIR> VVPAIR;

VS names;
VI ages;
VVI adj_list;
VS answers;
VI path;
VVPAIR queries_for_index;

void dfs(int root){
  path.push_back(root);
  for(auto query: queries_for_index[root]){

    //Binary search
    int start = 0;
    int end = path.size() - 1;
    while(start < end){
      int middle = (start + end) / 2;
      if(ages[path[middle]] <= query.second){
        end = middle;
      }
      else{
        start = middle + 1;
      }
    }
    answers[query.first] = names[path[end]];
  }

  for(int child: adj_list[root]){
    dfs(child);
  }
  path.pop_back();
}


void evolution(){
  MSI species_index;
  names = VS(n);
  ages = VI(n);
  int max_age_id = 0;
  int max_age = -1;
  for(int i = 0; i <n; i++){
    string species; int age;
    cin >> species >> age;
    species_index.insert(make_pair(species,i));
    names[i] = species;
    ages[i] = age;
    if(age > max_age){
      max_age = age;
      max_age_id = i;
    }
  }

  adj_list = VVI(n);
  for(int i = 0; i < n-1; i++){
    string child, parent; cin >> child >> parent;
    adj_list[species_index[parent]].push_back(species_index[child]);
  }

  answers = VS(q);
  queries_for_index = VVPAIR(n);
  for(int i = 0; i <q; i++){
    string s; int b; cin >> s >> b;
    queries_for_index[species_index[s]].push_back(make_pair(i,b));
  }

  dfs(max_age_id);

  for(int i = 0; i < q; i++){
    cout << answers[i] << " ";
  }
  cout << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i<t;i++){
    cin >> n >> q;
    evolution();
  }
  return 0;
}
