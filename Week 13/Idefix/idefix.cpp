#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

using namespace std;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int,K> Vb; //vertex index
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
typedef K::Point_2 P;
typedef vector<pair<P,int>> VPINT;
typedef vector<P> VP;
typedef vector<int> VI;
int n,m,k; long s;

//Define Edge struct:
//If v_end == -1, edge connects tree to bone
struct Edge {
    int v_start, v_end;
    K::FT length;
    // This constructor is convenient for a concise initialisation. It can also
    // be omitted and the member variables be set manually.
    Edge(int st, int e, K::FT l) : v_start(st), v_end(e), length(l){}
};

// Linearly order edges by increasing length
bool operator < (const Edge& e1, const Edge& e2) {
    return (e1.length < e2.length);
}

typedef vector<Edge> VE;

void idefix(){
// read points
  VPINT trees(n);
  for(int i = 0; i <n; i++){
    int x,y; cin >> x >> y;
    P tree(x,y);
    trees[i] = make_pair(tree,i);
  }

  VP bones(m);
  for(int i = 0; i < m; i++){
    int x,y; cin >> x >> y;
    P bone(x,y);
    bones[i] = bone;
  }

  //construct triangulation
  Triangulation t;
  t.insert(trees.begin(), trees.end());

  boost::disjoint_sets_with_storage<> uf1(n);

  VI bones_to_tree(n);
  VE edges;

  // -------- QUESTION 1 -------
  for(int i = 0; i < m; i++){
    auto nearest_tree = t.nearest_vertex(bones[i]);
    auto dist_bone_tree = CGAL::squared_distance(nearest_tree->point(), bones[i]);
    if(4 *dist_bone_tree<= s){
      bones_to_tree[nearest_tree->info()]++;
    }
    Edge e1(nearest_tree->info(), -1, 4 * dist_bone_tree);
    edges.push_back(e1);
  }

  for(auto e = t.finite_edges_begin(); e != t.finite_edges_end(); e++){
    auto v1 = e->first->vertex((e->second + 1) % 3);
    auto v2 = e->first->vertex((e->second + 2) % 3);

    auto dist_between_trees = CGAL::squared_distance(v1->point(), v2->point());
    if(dist_between_trees <= s){
      auto c1 = uf1.find_set(v1->info());
      auto c2 = uf1.find_set(v2->info());
      if(c1 != c2){
        uf1.link(c1,c2);
        int new_k = bones_to_tree[c1] + bones_to_tree[c2];
        bones_to_tree[uf1.find_set(v1->info())] = new_k;
      }
    }
    Edge e2(v1->info(), v2->info(), dist_between_trees);
    edges.push_back(e2);
  }

  //Find maximum number of bones in connected componenents
  int max_bones = 0;
  for(auto it: bones_to_tree){
    max_bones = max(max_bones, it);
  }


  //Reinitialize Union Find to solve second question;
  VI bones_to_tree_2(n);
  boost::disjoint_sets_with_storage<> uf2(n);

  //Sort edges by length
  sort(edges.begin(), edges.end());

  int current_bones = 0;
  K::FT last_edge = 0;

    // -------- QUESTION 2 -------
  for(auto edge:edges) {

    //Bone-Tree
    if(edge.v_end == -1){
      bones_to_tree_2[uf2.find_set(edge.v_start)] += 1;
    }

    //Tree-Tree
    else{
      auto c1 = uf2.find_set(edge.v_start);
      auto c2 = uf2.find_set(edge.v_end);
      if(c1 != c2){
        uf2.link(c1,c2);
        int new_k = bones_to_tree_2[c1] + bones_to_tree_2[c2];
        bones_to_tree_2[uf2.find_set(edge.v_start)] = new_k;
      }
    }
    current_bones = max(current_bones, bones_to_tree_2[uf2.find_set(edge.v_start)]);
    last_edge = edge.length;

    if(current_bones >= k) break;
  }

    // -------- PRINTOUT -------
  cout << setprecision(50) << max_bones << " " << last_edge << endl;
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int t; cin >> t;
  for(int i = 0; i < t; i++){
    cin >> n >> m >> s >> k;
    idefix();
  }
}
