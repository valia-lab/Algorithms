#include <stdio.h>
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;
set<int> to_explore = {0}; //min priority queue as for nodes to explore
long int *Cost = new long int[10005];
vector<long int> d;
int  N;


void Dikjstra(){
  int v;
  int equiv;
  long int next, old;
  while (!to_explore.empty()){
    v = *to_explore.begin();
    to_explore.erase(v);
    //relax
    for(int i = 1; i < N; i++){
        next = Cost[v] + d[i];
        equiv = next % d[0];
        old = Cost[equiv];
        if(old > next || old<0){
            Cost[equiv] = next;                  //keep the smallest int with i, s.t i%di = equiv
            to_explore.insert(equiv);            //equiv was updated, may improve routes to other nodes as well
        }
    }
  }
  return;
}

int main(){

  int Q;
  scanf("%d%d", &N, &Q);
  long int temp;
  for(int i=0; i<N; i++){
      scanf("%ld", &temp);
      d.push_back(temp);
  }

  sort(d.begin(), d.end());
  for(int i=1; i<d[0]; i++){
      Cost[i] = -2;    //initialise to dummy val
  }

  Cost[0] = 0;
  Dikjstra();
  //..................QUERIES....................
  while(Q>0){
    Q--;
    scanf("%ld", &temp);
    if(Cost[temp % d[0]] <= temp) printf("YES\n");
    else printf("NO\n");
  }
  delete[] Cost;
  return 0;
}
