#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef vector<int> VI;
int n;
int minimum, maximum, length;
int best_length;
VI occurrences;
VI window;
//Creating a struct with 2 arguments: the position and the word type
struct Word{
  int word_pos; int word_ref;
  Word(int wp, int wr) : word_pos(wp), word_ref(wr) {}
};

typedef vector<Word> VW;
VW all_pos;

bool operator < (const Word& w1, const Word& w2){
  return(w1.word_pos < w2.word_pos);
}

void search_snippets(){
  for(VW::iterator it = all_pos.begin(); it != all_pos.end(); it++){
    Word w = *it;
    window[w.word_ref] = w.word_pos;
    minimum = *min_element(window.begin(),window.end());
    maximum = *max_element(window.begin(),window.end());
    length = maximum - minimum + 1;
    if(length < best_length){
      best_length = length;
    }
  }
  cout << best_length << endl;
}

void read_input(){
  for(int i = 0; i < n; i++){
    for(int j = 0; j < occurrences[i]; j++){
      int new_pos; cin >> new_pos;
      if(j == 0){
        window[i] =  new_pos;
      }
      Word new_word(new_pos,i);
      all_pos.push_back(new_word);
    }
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  int test_cases; cin >> test_cases;
  for(int i = 0; i<test_cases; i++){
    cin >>n;
    occurrences = VI(n,-1);
    window = VI(n,-1);
    for(int j = 0; j < n; j++){
      int occurrence; cin>>occurrence;
      occurrences[j] = occurrence;
    }
    read_input();
    sort(all_pos.begin(),all_pos.end());
    minimum = *min_element(window.begin(),window.end());
    maximum = *max_element(window.begin(),window.end());
    length = maximum - minimum + 1;
    best_length = length;
    search_snippets();
    all_pos.clear();
  }
  return 0;
}
