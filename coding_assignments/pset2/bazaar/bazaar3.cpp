#include <iostream>
#include <stdio.h>
#include <tuple>
#include <bits/stdc++.h>
using namespace std;
time_t start, tend;
long int MAXCOST=1;

int solve(vector<vector<long int>> &T, vector<tuple<long int, long int>> offers, long int maxi, long int N){

    tuple<long int, long int> temp;
    long int k, r, temp2, temp3;
    long int n1 = offers.size()-1;      //amount of offers of this type
    tuple<long int, long int> t1 = offers[1];       //cheapest
    long int n1A = get<1>(t1);    //amount of cheapest

    for(int i= 0; i<n1+1; i++){
        vector<long int> vec;
        vec.push_back(0);
        T.push_back(vec);      //dummy initilisation to T[i][0]
    }

    T[1].push_back(get<0>(t1)); //  initialise T[1][1]
    /* .......................base of recursion.......................*/

    for(int j=2; j<n1A+1; j++){
        T[1].push_back(T[1][1]);
    }
    for(int j=n1A+1; j<N+1; j++){
        T[1].push_back(maxi);                       //practically infty
    }
    for(int i=2; i<n1+1; i++){
        T[i].push_back(T[1][1]);                      //T(i, 1) = infty if i>1
    }
    /*..............................................................*/
    int amount = n1A;
    for(int i = 2; i<n1+1; i++){
        temp = offers[i];
        r = get<1>(temp);                  //i-th offer amount
        amount += r;
        for(int k = 2; k < N +1; k++){
            if(k > amount)
                T[i].push_back(maxi);      //we can't buy that much we that set of offers
            else if(k - r > 0)
                T[i].push_back(min(T[i-1][k], T[i-1][k-get<1>(temp)] + get<0>(temp)));
            else                //if r>k then the i-th itself is enough for k
                T[i].push_back(min(T[i-1][k], get<0>(temp)));
        }
    }
  return amount;
}

long int supplier(vector<long int> &T, int supplier, vector<vector<tuple<long int, long int>>> offers, long int maxi, long int N){
      int amount;
      vector<vector<long int>> TA;
      vector<vector<long int>> TB;
      vector<vector<long int>> TC;
      amount = solve(TA, offers[0], maxi, N);
      amount = min(amount, solve(TB, offers[1], maxi, N));
      amount = min(amount, solve(TC, offers[2], maxi, N));

      for(int i = 1; i < amount + 1; i++){
          T.push_back(TA[offers[0].size()-1][i] + TB[offers[1].size()-1][i] + TC[offers[2].size()-1][i]);
      }
      for(int i = amount+1; i < N +1; i++){
          T.push_back(MAXCOST);         //infty
      }
      return amount;
}

int main(){

    int N, M, i;

    scanf("%d %d", &N, &M);
    getchar();

    vector< vector<tuple<long int, long int>> > offers1(3);
    vector< vector<tuple<long int, long int>> > offers2(3);
    vector< vector<tuple<long int, long int>> > offers3(3);
    for(int i=0; i<3; i++){
      offers1[i].push_back(make_tuple(-1, -1));
      offers2[i].push_back(make_tuple(-1, -1));
      offers3[i].push_back(make_tuple(-1, -1));
    }

    long int n, cost, n1 = 0, m1 = 0, m2=0, m3 = 0;
    char m[3];

    /*.....................'A' ~ 0 , 'B' ~ 1, 'C' ~2 ....................*/
    for(i=0; i<M; i++){
      scanf("%s %ld %ld", m, &n, &cost);
      MAXCOST+=cost;
      if(m[0]=='1')                                          //supplier 1
          offers1[m[1]-65].push_back(make_tuple(cost, n));  //turn ascii to number
      else if(m[0]=='2')                                   //supplier 2tuple<long int, long int>
          offers2[m[1]-65].push_back(make_tuple(cost, n));
      else
          offers3[m[1]-65].push_back(make_tuple(cost, n));

    }

    /* sort by cost */
    int s[4]={1,1,1,1};
    for(int i = 0; i<3; i++){
      sort(offers1[i].begin(), offers1[i].end());
      if(offers1[i].size()==1)
          s[1] = 0;
      sort(offers2[i].begin(), offers2[i].end());
      if(offers2[i].size()==1)
          s[2] = 0;
      sort(offers3[i].begin(), offers3[i].end());
      if(offers3[i].size()==1)
          s[3] = 0;
    }

    vector<long int> t1;
    vector<long int> t2;
    vector<long int> t3;
    t1.push_back(0);
    t2.push_back(0);
    t3.push_back(0);

    long int amount1 = 0, amount2 = 0, amount3 = 0;
    if(s[1]>0){
        amount1 = supplier(t1, 1, offers1, MAXCOST, N);
    }
    if(s[2]>0){
        amount2 = supplier(t2, 2, offers2, MAXCOST, N);
    }
    if(s[3]>0){
        amount3 = supplier(t3, 3, offers3, MAXCOST, N);
    }
    if(amount1 + amount2 + amount3 < N){        //we can't make N sets at any cost!
      cout<<-1<<endl;
      return 0;
    }


    //.............solve..............................//
    vector<vector<long int> > T(4);
    vector<long int> v{0};
    T.push_back(v);
    for(int i=0; i<4; i++)
        T.push_back(v);

    for(int k=0; k<N+1; k++)
        T[1].push_back(t1[k]);

   //cout<<t1[0]<<" "<<t2[0]<<" "<<t3[0]<<endl;

    long int mini;
    for(int i = 2; i < 4; i++){
        if(s[i] == 0){                  //can't produce any set from supplier i
            for(int k = 0; k < N+1; k++)
                T[i].push_back(T[i-1][k]);
        }
        else{
            T[i].push_back(0);        //T[i][0] = 0 for any set of suppliers
            for(int k = 1; k < N+1; k++){
                  mini = T[i-1][k];
                  for(int j = 1; j < k+1; j++){
                      if(i == 2)
                              mini = min(mini, t2[j] + T[i-1][k-j]);
                      else
                              mini = min(mini, t3[j] + T[i-1][k-j]);
                   }
                   T[i].push_back(mini);
            }
        }
    }

    cout << T[3][N] << flush << endl;
    return 0;
}
