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
    //printf("TA[1][1] = %ld\n", TA[1][1]);
    /* .......................base of recursion.......................*/

    for(int j=2; j<n1A+1; j++){
        T[1].push_back(T[1][1]);     // fill T[1][j] for j up to n1A
        //printf("TA[1][%d] = %ld\n" ,j, TA[1][1]);
    }
    for(int j=n1A+1; j<N+1; j++){
        T[1].push_back(maxi + 1);                       //practically infty
        //printf("TA[1][%d] = %ld\n" ,j, maxi+1);
    }
    for(int i=2; i<n1+1; i++){
        T[i].push_back(T[1][1]);                      //T(i, 1) = infty if i>1
        //printf("TA[%d][1]= %ld\n" ,i, TA[1][1]);
    }
    //printf("hello there \n");

    /*..............................................................*/
    int amount = n1A;
    for(int i = 2; i<n1+1; i++){
        temp = offers[i];
        k=2;
        r = get<1>(temp);                  //i-th offer amount
        amount+=r;
      //  cout<<"ith offer is "<<r<<endl;
      //  cout<<"desired k is "<<k<<" and we have an amount of "<<amount<<" goods" <<endl;
        while(k<=N){
            if(k > amount){
                T[i].push_back(maxi+1);      //we can't buy that much we that set of offers
                k++;
            }
            else if(k-r>0){
                T[i].push_back(min(T[i-1][k], T[i-1][k-get<1>(temp)] + get<0>(temp)));
              //  printf("k is %d\n and k-r>0", k);
                k++;

            }
            else{
              //  printf("k is %ld and r is %ld \n",k, r);                                   //if r>k then the i-th itself is enough for k
                T[i].push_back(min(T[i-1][k], get<0>(temp)));
              //  printf("TA[%d][%ld] = %ld\n", i,k,  get<0>(temp));
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
      else if(m[0]=='2'){                                //supplier 2
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

    int amount1;
    /* solve T(S1A, i) ............................*/
    n1 = offers1[0].size()-1;             //offers is 1 bases so its size is = #real_offers + 1
    vector<vector<long int>> TA;
    amount1 = solve(TA, offers1[0], m1, N);

    for(int i=1; i<n1+1; i++){
       printf("T[%d]={ ", i);
        for(int j = 1; j<=N; j++)
            printf("%ld ", TA[i][j]);
        printf(" }");
        putchar('\n');
    }

    //.......................
   /* solve T(S1B, i) ............................*/
   n1 = offers1[1].size()-1;             //offers is 1 bases so its size is = #real_offers + 1
   vector<vector<long int>> TB;
   amount1 = min(amount1, solve(TB, offers1[1], m1, N));

   printf("1B is \n");
   for(int i=1; i<n1+1; i++){
      printf("T[%d]={ ", i);
       for(int j = 1; j<=N; j++)
           printf("%ld ", TB[i][j]);
       printf(" }");
       putchar('\n');
   }
       //.......................
       /* solve T(S1B, i) ............................*/
       n1 = offers1[2].size()-1;             //offers is 1 bases so its size is = #real_offers + 1
       vector<vector<long int>> TC;
       amount1 = min(amount1, solve(TC, offers1[2], m1, N));

       printf("1C is \n");
       for(int i=1; i<n1+1; i++){
          printf("T[%d]={ ", i);
           for(int j = 1; j<=N; j++)
               printf("%ld ", TC[i][j]);
           printf(" }");
           putchar('\n');
       }
           //.......................
      cout<<"we can make at most "<<amount1<<" goods of type 1"<<endl;
      vector<vector<long int> > T(4);
      T[1].push_back(0);
      for(int i = 1; i<amount1 + 1; i++){
          T[1][i] = TA[offers1[0].size()-1][i] + TB[offers1[1].size()-1][i] + TC[offers1[2].size()-1][i];
          cout<<T[1][i]<<endl;
      }
      if(!TA.empty())
          TA.clear();
      if(!TB.empty());
          TB.clear();
      if(!TC.empty());
          TC.clear();
      /*.............supplier 2....................................*/
      int amount2;
      /* solve T(S2A, i) ............................*/
      n1 = offers2[0].size()-1;             //offers is 1 bases so its size is = #real_offers + 1
      amount2 = solve(TA, offers2[0], m2, N);

      for(int i=1; i<n1+1; i++){
         printf("T[%d]={ ", i);
          for(int j = 1; j<=N; j++)
              printf("%ld ", TA[i][j]);
          printf(" }");
          putchar('\n');
      }

      //.......................
     /* solve T(S2B, i) ............................*/
     n1 = offers2[1].size()-1;             //offers is 1 bases so its size is = #real_offers + 1
     amount2 = min(amount2, solve(TB, offers2[1], m2, N));

     printf("2B is \n");
     for(int i=1; i<n1+1; i++){
        printf("T[%d]={ ", i);
         for(int j = 1; j<=N; j++)
             printf("%ld ", TB[i][j]);
         printf(" }");
         putchar('\n');
     }
         //.......................
         /* solve T(S1B, i) ............................*/
         n1 = offers2[2].size()-1;             //offers is 1 bases so its size is = #real_offers + 1
         amount2 = min(amount2, solve(TC, offers2[2], m2, N));

         printf("2C is \n");
         for(int i=1; i<n1+1; i++){
            printf("T[%d]={ ", i);
             for(int j = 1; j<=N; j++)
                 printf("%ld ", TC[i][j]);
             printf(" }");
             putchar('\n');
         }
             //.......................
        cout<<"we can make at most "<<amount2<<" goods of type 2"<<endl;
        vector<long int> T2(amount2+1);
        T[2].push_back(0);
        for(int i = 1; i<amount2 + 1; i++){
            T[2].push_back(TA[offers2[0].size()-1][i] + TB[offers2[1].size()-1][i] + TC[offers2[2].size()-1][i]);
            cout<<T[2][i]<<endl;
        }

        if(!TA.empty())
            TA.clear();
        if(!TB.empty());
            TB.clear();
        if(!TC.empty());
            TC.clear();
        /* supplier3.....................................*/
        /*.............supplier 2....................................*/
        int amount3;
        /* solve T(S1A, i) ............................*/
        n1 = offers3[0].size()-1;             //offers is 1 bases so its size is = #real_offers + 1
        amount3 = solve(TA, offers3[0], m3, N);

        for(int i=1; i<n1+1; i++){
           printf("T[%d]={ ", i);
            for(int j = 1; j<=N; j++)
                printf("%ld ", TA[i][j]);
            printf(" }");
            putchar('\n');
        }

        //.......................
       /* solve T(S1B, i) ............................*/
       n1 = offers3[1].size()-1;             //offers is 1 bases so its size is = #real_offers + 1
       amount3 = min(amount3, solve(TB, offers3[1], m3, N));

       printf("1B is \n");
       for(int i=1; i<n1+1; i++){
          printf("T[%d]={ ", i);
           for(int j = 1; j<=N; j++)
               printf("%ld ", TB[i][j]);
           printf(" }");
           putchar('\n');
       }
           //.......................
           /* solve T(S3C, i) ............................*/
           n1 = offers3[2].size()-1;             //offers is 1 bases so its size is = #real_offers + 1
           amount3 = min(amount3, solve(TC, offers3[2], m3, N));

           printf("3C is \n");
           for(int i=1; i<n1+1; i++){
              printf("T[%d]={ ", i);
               for(int j = 1; j<=N; j++)
                   printf("%ld ", TC[i][j]);
               printf(" }");
               putchar('\n');
           }
               //.......................
          cout<<"we can make at most "<<amount3<<" goods of type 3"<<endl;
          T[3].push_back(0);
          for(int i = 1; i< amount3 + 1; i++){
              T[3].push_back(TA[offers3[0].size()-1][i] + TB[offers3[1].size()-1][i] + TC[offers3[2].size()-1][i]);
              cout<<T[3][i]<<endl;
          }
          if(!TA.empty())
              TA.clear();
          if(!TB.empty());
              TB.clear();
          if(!TC.empty());
              TC.clear();
              
          //..........SOLVE ..................................................//



}
