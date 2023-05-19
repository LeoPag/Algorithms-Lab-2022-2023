#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
long n,m,D,T;

typedef vector<long> VL;

struct Asterix_move{
  long d,t;

  Asterix_move(long d, long t) : d(d), t(t) {}
};

// Linearly order edges by time
bool operator < (const Asterix_move& m1, const Asterix_move& m2) {
    return (m1.t < m2.t);
}

typedef vector<Asterix_move> VAM;
VAM first_subset;
VAM second_subset;
VL extra_distances;

bool split_and_list(long extra){

  VAM M1;

  for (int s = 0; s < 1<<first_subset.size(); ++s) { // Iterate through all subsets
    long total_time_subset = 0;
    long total_distance_subset = 0;
    for (int i = 0; i < first_subset.size(); ++i) {
        if (s & 1<<i){// If i-th element in subset
          total_time_subset += first_subset[i].t;
          total_distance_subset += first_subset[i].d + extra;
        }
    }
    if((total_time_subset < T) && (total_distance_subset >= D))return true;
    Asterix_move subset1(total_distance_subset, total_time_subset);
    M1.push_back(subset1);
  }

  VAM M2;
  for (int s = 0; s < 1<<second_subset.size(); ++s) { // Iterate through all subsets
    long total_time_subset = 0;
    long total_distance_subset = 0;
    for (int i = 0; i < second_subset.size(); ++i) {
        if (s & 1<<i){// If i-th element in subset
          total_time_subset += second_subset[i].t;
          total_distance_subset += second_subset[i].d + extra;
        }
    }
    if((total_time_subset < T) && (total_distance_subset >= D))return true;
    Asterix_move subset2(total_distance_subset, total_time_subset);
    M2.push_back(subset2);
  }

  //Sort subset 2 by total_time
  sort(M2.begin(), M2.end());

  VL max_distance(M2.size());

  long max_d = 0;
  for(int i = 0; i < M2.size(); i++){
    max_d = max(max_d, M2[i].d);
    max_distance[i] =  max_d;
  }

  for(int i = 0; i < M1.size(); i++){

    //Binary search split&list
    int low = 0; int high = M2.size() - 1;
    while(low <= high){
      int middle = (low + high) / 2;
      if((max_distance[middle] + M1[i].d >= D) && (M1[i].t + M2[middle].t < T)) return true;
      else if(max_distance[middle] + M1[i].d >= D && (M1[i].t + M2[middle].t >= T)) high = middle-1;
      else if((max_distance[middle] + M1[i].d < D) && (M1[i].t + M2[middle].t >= T)) break;
      else if((max_distance[middle] + M1[i].d < D) && (M1[i].t + M2[middle].t < T)) low = middle+1;
    }
  }
  return false;
}


void asterix_the_gaul(){

  for(int i = 0 ; i < n; i++){
    long d,t; cin >> d >> t;
    Asterix_move new_move(d,t);

    if(i % 2 == 0) first_subset.push_back(new_move);
    else second_subset.push_back(new_move);
  }

  extra_distances = VL(m);
  for(int i = 0; i < m; i++){
    cin >> extra_distances[i];
  }

  //Try with 0 potion:
  if (split_and_list(0)) {
    cout << 0 << endl;
    return;
  }

  //Binary search on the potion values;
  int low = 0;
  int high = m-1;
  int min_pot = 10000000;
  while(low <= high){
    int middle = (low + high) / 2;
    if(split_and_list(extra_distances[middle])){
      min_pot = min(min_pot,middle+1);
      high = middle - 1;
    }
    else{
      low = middle + 1;
    }
  }
  if(min_pot == 10000000) cout << "Panoramix captured" << endl;
  else cout << min_pot << endl;
  return;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> D >> T;
    asterix_the_gaul();
    first_subset.clear();
    second_subset.clear();
    extra_distances.clear();
  }
  return 0;
}
