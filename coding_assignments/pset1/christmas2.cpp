#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <array>
using namespace std;

ifstream infile;

struct node{
  node  *parent = NULL, *left = NULL, *right = NULL;
  int val = 0;
  int j = 0;
};


      //inserts value x returns position of the last node in cartesian
node *insert(node *last, int count, int x, int j){

          if(count ==0 && x > last->val){
              node *newbie;
              newbie -> val = x;
              newbie -> j = j;
              last ->right = newbie;
              return newbie;
          }
          else if(count>0 && x > (last->val)){
            node *newbie;
            newbie -> val = x;
            newbie -> j = j;
            newbie->left = last -> right;
            last->right = newbie;
            newbie->left = NULL;
            newbie ->right = NULL;
            return newbie;
          }
          else if(last->parent!=NULL)
               insert(last->parent, x, j);
          else {            //if last = root
            //make newbie the new root,
            newbie->left = last;
            newbie ->right = NULL;
            return NULL;       //last did not change
          }
}

node *find(node *t, int j ,int key){

        node *h;
        if(t==NULL) return NULL;      //not found
        int v = t->val;
        if(v<key) return NULL;       // min heap
        if (v == key)
             return t;
        else{
          h = find(t->left, j, key);
          if((h->j)>0) return h;
        }
        h = find(t->right, j, key);
        if((h->j)>0) return h;
        else return NULL;

}



int main(int argc, char **argv){

  int N, K;
  infile.open(argv[1]);
  infile >> N >> K;
  int pref[K+1];
  pref[0] = 0; //dummy
  int temp, delta, res = K+1;
  infile>>pref[1];

  for(int i=2; i<K+1; i++){
      infile>>temp;
      pref[i] = pref[i-1]+temp;
      if(pref[i]==K && i < res)
          res = i;
  }
  infile.close();

  vector<vector<int>> *s = new vector<vector<int>>[K];



   //............build n cartesian trees in O(n*n).......................
  node **T = new node*[N];
  for(int i =1; i<K; i++){
      node *last, *root = last;
      for(int j=i+1; j<K+1; j++){
        temp = pref[j] - pref[i-1];
        delta = j-i+1;
        if(temp > K) break;             //i am interested only in s(i,j)<=K
        last = insert(last, temp, j);
        if(last == NULL ) last = root;
      }
      T[i] = root;
  }
/*..............................................................*/

//..........search for (k,m) such that s(k,m) = K - s(i,j).......
  node *r;
  int c;

  for(int i=1; i<K; i++){
    for(int j = i+1; j<K+1; j++){
        c = 0;
        temp = K-s[i][j];
        for(int q = j+1; j<N; ){       //search for s(q,m): q>j
            r = find(T[q], q, temp);
            if(r!=NULL){
                c = (r->j -q +1) + (j-i+1);        //total length
                res = min(res, c);
            }
         }
    }
  }
  return 0;
}
