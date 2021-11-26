#include <iostream>
#include <stdio.h>
#include <tuple>
#include <algorithm>
#include <fstream>
#include <list>
using namespace std;
ifstream infile;

int tuple_partition(tuple<int, int, int> *a, int first, int last){

    if(first>=last) return last;
    int left = first;
    int right = last;
    int pivot = get<0>(a[(last + first)/2]);
    while(true){
      while(get<0>(a[left]) < pivot && left<right) left++;
      while(get<0>(a[right])> pivot && right>left) right--;
      if(left>=right) return right;
      swap(a[left], a[right]);
      left++;
      right--;
  }
  return right;
}

void tuple_quicksort(tuple<int, int, int> *a, int first,int last){

  if(first>=last) return;
  int limit = tuple_partition(a, first, last);
  tuple_quicksort(a, first, limit); //everyone is <= pivot here
  tuple_quicksort(a, limit+1, last);
}

struct Set{
  int parent;
  int real_parent;
  int rank;
  int w;            //length of portal that connects with parent
};

int find(struct Set *uf, int u){

      list<int> path;

      if(uf[u].parent == u) return u;     //root
      else while(uf[u].parent != u){
        path.push_back(u);
        u = uf[u].parent;
      }

      //path compression
      for(const auto& p : path)
          uf[p].parent = u;
      /*...........................................*/
      path.clear();
      return u;
}


void Union(struct Set *uf, int u, int v, int w){
  int pu = find(uf, u), pv = find(uf, v);
  int ranku = uf[pu].rank, rankv = uf[pv].rank;
  if(ranku > rankv){
     uf[pv].real_parent = pu;
     uf[pv].parent = pu;
     uf[pv].w = w;
  }
  else if(ranku<rankv){
    uf[pu].real_parent = pv;
    uf[pu].parent = pv;
    uf[pu].w = w;
  }
  else{ //if they have the same rank choose pv as  parent and increase rank
    uf[pu].parent = pv;
    uf[pu].w = w;
    uf[pv].rank++;
    if(uf[pu].real_parent==0) //first time we see an edge involving pu
        uf[pu].real_parent = pv;
  }
}

int main(int argc, char **argv){

    infile.open(argv[1]);
    int N, M, i;
    infile >> N >> M;
//    cout<<N<<M<<endl;
    int *C = new int[N+1];
    C[0] = -1;//dummy

    Set *uf = new Set[N+1];
    for(i=1; i<N+1; i++){
  //    cout<<i<<" ";
        infile>>C[i];
        uf[i].parent = i;     //initiliase Union find
        uf[i].real_parent = 0;
        uf[i].rank = 0;
        uf[i].w = 0;
    }

  //  cout<<"\n";
    infile.get();
    tuple<int, int, int> *portal = new tuple<int, int, int>[M];
    int t1, t2, w;
    for(i=0; i<M; i++){
      infile>>t1>>t2>>w;
      portal[i] = make_tuple(w, t1, t2);
    }
    infile.close();

    tuple_quicksort(portal, 0, M-1);       //sort portals by length
    int u,v;
    for(i=M-1; i>=0; i--){             // examine portals in descending order
      u = get<1>(portal[i]);
      v = get<2>(portal[i]);
      if(find(uf, u) != find(uf, v)){
            Union(uf, u, v, get<0>(portal[i]));
      }
    }

    int mini;
    int max = get<0>(portal[M-1]) +1;
    int res = max;
    int q, flag, root, edge;


    for(i=1; i<N+1; i++){
    //  printf("edge number %d ", i);
      if(C[i]!= i){
        q = C[i];
        flag = 0;
        mini = max;
        //***************************compute path C[i] --> i *************************************
        // if i and c[i] are not in the same branch compute : path i-> root and path root-> C[i]
        while(q!=i){ //find path to i , and the min portal length in this path
          edge = uf[q].w;
          if(edge==0) break;   //root
          mini = min(mini, edge);
          q = uf[q].real_parent;
          if(uf[q].parent==q && q !=i){       //we are in the root and root is not our destination
              flag = 1;
              root = q;
              break;
          }
       }
  //     printf("ok2\n");
       if(flag){  //compute path from i to root == path from root to i
         q = i;
         while(q!=root){ //find path from i to root , and the min portal length in this path
           mini = min(mini, uf[q].w);
           q = uf[q].real_parent;
         }
        }
        //***************************************************************************************
       }

      if(mini<res) res = mini;        //compare with results for other paths
    }
    cout<<res<<endl<<flush;
    return 0;


}
