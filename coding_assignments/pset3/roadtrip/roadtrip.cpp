// C++ implementation to find the
// maximum weighted edge in the simple
// path between two nodes in N-ary Tree

#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h>
#include <tuple>
#include <algorithm>
#include <list>
using namespace std;

const int N = 100005;
const int LG = 20;
//...........................
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
  else if(ranku < rankv){
    uf[pu].real_parent = pv;
    uf[pu].parent = pv;
    uf[pu].w = w;
  }
  else{ //if they have the same rank choose pv as  parent and increase rank
    uf[pu].parent = pv;
    uf[pu].w = w;
    uf[pv].rank++;
    if(uf[pu].real_parent == 0) //first time we see an edge involving pu
        uf[pu].real_parent = pv;
  }
}

int n;

// Function to traverse the nodes
// using the Depth-First Search Traversal
void dfs_lca(vector<vector<int> > dp, vector<vector<int> > mx, vector<int> level, vector<vector<pair<int, int> > > G, int a, int par, int lev)
{
    dp[0][a] = par;
    level[a] = lev;
    for (auto i : G[a]) {

        // Condition to check if its
        // equal to its parent then skip
        if (i.first == par)
            continue;
        mx[0][i.first] = i.second;

        // DFS Recursive Call
        dfs_lca(dp, mx, level, G, i.first, a, lev + 1);
    }
}

// Function to find the ansector
void find_ancestor(vector<vector<int> > dp, vector<vector<int> > mx)
{

    // Loop to set every 2^i distance
    for (int i = 1; i < LG; i++) {
        // Loop to calculate for
        // each node in the N-ary tree
        for (int j = 1; j <= n; j++) {
            dp[i][j]
                = dp[i - 1][dp[i - 1][j]];

            // Storing maximum edge
            mx[i][j]
                = max(mx[i - 1][j],
                      mx[i - 1][dp[i - 1][j]]);
        }
    }
}

int getMax(vector<vector<int> > dp, vector<vector<int> > mx, vector<int> level, int a, int b)
{
    // Swaping if node a is at more depth
    // than node b because we will
    // always take at more depth
    if (level[b] < level[a])
        swap(a, b);

    int ans = 0;

    // Difference between the depth of
    // the two given nodes
    int diff = level[b] - level[a];
    while (diff > 0) {
        int log = log2(diff);
        ans = max(ans, mx[log][b]);

        // Changing Node B to its
        // parent at 2 ^ i distance
        b = dp[log][b];

        // Subtracting distance by 2^i
        diff -= (1 << log);
    }

    // Take both a, b to its
    // lca and find maximum
    while (a != b) {
        int i = log2(level[a]);

        // Loop to find the 2^ith
        // parent that is different
        // for both a and b i.e below the lca
        while (i > 0
               && dp[i][a] == dp[i][b])
            i--;

        // Updating ans
        ans = max(ans, mx[i][a]);
        ans = max(ans, mx[i][b]);

        // Changing value to its parent
        a = dp[i][a];
        b = dp[i][b];
    }
    return ans;
}

// Function to compute the Least
// common Ansector
void compute_lca(vector<vector<int> > dp, vector<vector<int> > mx, vector<int> level, vector<vector<pair<int, int> > > G)
{
    dfs_lca(dp, mx, level, G, 1, 0, 0);
    find_ancestor(dp, mx);
}

// Driver Code
int main()
{
  int N, M, i, Q;
  scanf("%d ",&N);
  scanf("%d",&M);
  getchar();
  //...........................
  // Depths of Nodes
  vector<int> level(N);


  // Parent at every 2^i level
  vector<vector<int> > dp(LG, vector<int>(N));

  // Maximum node at every 2^i level
   vector<vector<int> > mx(LG, vector<int>(N));

  // Graph that stores destinations
  // and its weight
  vector<vector<pair<int, int>>> G(N);
  Set *uf = new Set[N+1];
  for(i=1; i<N+1; i++){

      uf[i].parent = i;     //initiliase Union find
      uf[i].real_parent = 0;
      uf[i].rank = 0;
      uf[i].w = 0;
  }

  getchar();
  tuple<int, int, int> *portal = new tuple<int, int, int>[M];
  int t1, t2, w;
  for(i=0; i<M; i++){
    scanf("%d %d %d", &t1, &t2, &w);
  //  cout<<"t2 = "<<t2<<endl;
    portal[i] = make_tuple(t1, t2, w);
    G[i].push_back(make_pair(0,0));
    getchar();
  //  cout<<t1<<" is connected with "<<t2<<" with a portal of length "<<w<<endl;
  }

  //..............................................

  if(N<=10000)
      tuple_quicksort(portal, 0, M-1);       //sort portals by length
  int u, v, c = 1;
  for(i = M-1; i >= 0; i--){             // examine portals in descending order
    u = get<0>(portal[i]);
    v = get<1>(portal[i]);
    int r = get<2>(portal[i]);
    //cout<<u<<" and "<<v<<" are connected with portal of length "<<get<2>(portal[i])<<endl;
    if(find(uf, u) != find(uf, v)){
          Union(uf, u, v, get<2>(portal[i]));
          vector<pair<int,int>> e{make_pair(0,0)};
          G[get<2>(portal[i])].push_back(make_pair(u,v));
    }
  }

    // Computing LCA
    compute_lca(dp, mx, level, G);

    //.............................................
    //.............READ QUERIES....................//
    scanf("%d",&Q);
    vector<vector<int>> queries[Q][2];
    for(i=0; i<M; i++){
      scanf("%d %d", &t1, &t2);
      queries[i][0] = t1;
      queries[i][1] = t2;
    }
    for (int i = 0; i < Q; i++) {
          int max_edge = getMax(queries[i][0],
                                queries[i][1]);
          cout << max_edge << endl;
      }

    return 0;
}
