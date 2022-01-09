#include <iostream>
#include <stdio.h>
#include <tuple>
#include <bits/stdc++.h>
using namespace std;
time_t start, tend;

int solve(vector<vector<long int>> &T, vector<tuple<long int, long int>> offers, long int maxi, long int N){

    tuple<long int, long int> temp;
    long int k, r, temp2, temp3;
    /*............. ......supplier 1.......................................*/
    long int n1 = offers.size()-1;      //amount of offers of type 1A
    tuple<long int, long int> t1 = offers[1];       //cheapest 1A
    long int n1A = get<1>(t1);    //amount of cheapest 1A
    cout<<n1A<<endl;

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
        T[1].push_back(maxi + 1);                       //practically infty
    }
    for(int i=2; i<n1+1; i++){
        T[i].push_back(T[1][1]);                      //T(i, 1) = infty if i>1
    }
    /*..............................................................*/
    int amount = n1A;
    for(int i = 2; i<n1+1; i++){
        temp = offers[i];
        k=2;
        r = get<1>(temp);                  //i-th offer amount
        amount+=r;
        while(k<=N){
            if(k > amount){
                T[i].push_back(maxi+1);      //we can't buy that much we that set of offers
                k++;
            }
            else if(k-r>0){
                T[i].push_back(min(T[i-1][k], T[i-1][k-get<1>(temp)] + get<0>(temp)));
                k++;

            }
            else{
                                  //if r>k then the i-th itself is enough for k
                T[i].push_back(min(T[i-1][k], get<0>(temp)));
                if(T[i][k]!=T[i-1][k]){             //fill T(i,j) for j = k+1, .., r
                    temp2 = k++;
                    temp3 = T[i][k];
                    while(temp2++ < r){
                      T[i].push_back(temp3);
                    }
                    k = temp2;
                }
                else k++;
            }
        }

    }

    return amount;
}

void supplier(vector<long int> &T, int supplier, vector<vector<tuple<long int, long int>>> offers, long int maxi, long int N){
      int amount;
      vector<vector<long int>> TA;
      vector<vector<long int>> TB;
      vector<vector<long int>> TC;
      amount = solve(TA, offers[0], maxi, N);
      amount = min(amount, solve(TB, offers[1], maxi, N));
      amount = min(amount, solve(TC, offers[2], maxi, N));
    //  T.push_back(0);
      for(int i = 1; i < amount + 1; i++){
          T.push_back(TA[offers[0].size()-1][i] + TB[offers[1].size()-1][i] + TC[offers[2].size()-1][i]);
          cout<<T[i]<<endl;
      }
}

int main(){

    //time(&start);
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
    /* 'A' ~ 0 , 'B' ~ 1, 'C' ~2 ....................................*/
    for(i=0; i<M; i++){
      scanf("%s %ld %ld", m, &n, &cost);
      if(m[0]=='1'){                                        //supplier 1
          offers1[m[1]-65].push_back(make_tuple(cost, n));  //turn ascii to number
          m1+=cost;
      }
      else if(m[0]=='2'){                                //supplier 2tuple<long int, long int>
          offers2[m[1]-65].push_back(make_tuple(cost, n));
          m2 +=cost;
      }
      else{
          offers3[m[1]-65].push_back(make_tuple(cost, n));
          m3+=cost;
      }
    }

    /* sort by cost */
    for(int i =0; i<3; i++){
      sort(offers1[i].begin(), offers1[i].end());
      sort(offers2[i].begin(), offers2[i].end());
      sort(offers3[i].begin(), offers3[i].end());
    }

    vector<vector<long int> > T(4);
    vector<long int> v{0};
    T.push_back(v);
    for(int i=1; i<4;)
        T[i].push_back(0);
    supplier(T[1], 1, offers1, m1, N);
    supplier(T[2], 2, offers2, m2, N);
    supplier(T[3], 3, offers3, m3, N);



}
