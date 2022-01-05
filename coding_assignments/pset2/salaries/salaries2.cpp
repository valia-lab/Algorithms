#include <vector>
#include <iostream>
#include <tuple>
#include <stdio.h>
using namespace std;
#include <bits/stdc++.h>
time_t start, tend;

int find_lis(long long int a[], long int n, long long int *l){

    l[1] = 1;
    vector<int> subseqs;    // holds the last added element to each increasing subseq
    subseqs.push_back(-1); //dummy push to have vector index base = 1
    subseqs.push_back(a[1]);
    int back, mini, dual, repeat = 0;
    int pivot, from, to, j;
    for(int i = 2; i<n+1; i++){
      //scan subseqs by decreasing size
      back = subseqs.size()-1 ;
      mini = 1;
      dual = 0;
      repeat = 0;
      //printf("%d %lld\n", subseqs[back], a[i]);
      if(subseqs[back]<a[i]){     //if a[i] etxends longest subseq
        subseqs.push_back(a[i]);
        l[i] = back+1;
      }
      else{
        from = 1;
        to = back-1;
        while(from <= to && repeat<2){           //binary search to find the lis a[i] extends
          pivot = from  + (to-from)/2;
          if(subseqs[pivot]==a[i]){ //a[i] won't extend any lis
            dual = 1;
            break;
          }
          if(subseqs[pivot] > a[i])
              to = pivot-1;

          else{
            from = pivot +1;
            mini = 0;                     //a[i] is not the smallest
          }
          if(from == to)
              repeat++;

        }
        /*............end of bsearch .....................................*/
        if(dual)
            l[i] = l[pivot];
        else if(!mini ){
            j = to;
            subseqs[j+1] = a[i];
            l[i] = j+1;    //length of lds ending in a[i]
        }
        else {             //in case a[i] is minimum in a[1..i]
          subseqs[1]=a[i];
          l[i] = 1;
        }
      }
/*printf("after inserting a[ %d %s\n", i, "]");
    for(const auto& x: subseqs)
         printf("%d ", x);
      putchar('\n');*/


    }
    return subseqs.size()-1;

}

int find_lds(long long int a[], long int n, long long int *r, int K){


      vector<int> subseqs;    // holds the last added element to each increasing subseq
      subseqs.push_back(-1); //dummy push to have vector index base = 1
      subseqs.push_back(a[n]);
      int back, maxi, dual, repeat = 0, flag;
      int pivot, from, to, j;
      if(a[n-1]<a[n])
          r[n]=1;
      else r[n] = 0;
      for(int i =n-1; i>1; i--){
        //scan subseqs by decreasing size
        back = subseqs.size()-1 ;
        maxi = 1;
        dual = 0;
        repeat = 0;
        flag = 0;
        //printf("%d %lld\n", subseqs[back], a[i]);
        if(subseqs[back] > a[i]){    //if a[i] etxends longest subseq
          subseqs.push_back(a[i]);
          flag = 1;
        }

        else{
          from = 1;
          to = back-1;

          while(from <= to && repeat<2){           //binary search to find the lis a[i] extends
            pivot = from  + (to-from)/2;
            if(subseqs[pivot]==a[i]){ //a[i] won't extend any lis
              dual = 1;
              break;
            }
            if(subseqs[pivot] < a[i])
                to = pivot-1;

            else{
              from = pivot +1;
              maxi = 0;                     //a[i] is not the largest so far
            }
            if(from == to)
                repeat++;

           }
          /*............end of bsearch .....................................*/
          if(!dual && !maxi ){
              j = to;
              subseqs[j+1] = a[i];
          }
          else if(!dual)
            subseqs[1] = a[i];
         }

         ///......binary search in subseqs[] to find right most element
         //.......that is compatible with a[i-1]......................
         from = 1;
         to = subseqs.size()-1;
        //if(i == n-2)
    /*    for(const auto& x: subseqs)
              printf("%d ", x);
           putchar('\n');*/

         if(a[i]== subseqs[to] &&(a[i-1] <subseqs[to] || a[i-1]-subseqs[to]<K))
             r[i] = to;
         else{
         repeat = 0;
             while(from<=to && repeat<2){
                //  printf("%d\n",i);
                  pivot = from + (to-from)/2;
                  //printf("%d\n",pivot);
                  if((a[i-1]<subseqs[pivot]) || (a[i-1] - subseqs[pivot]<K)) {//compatible
                      from = pivot+1;
                      r[i] = pivot;
                  }
                  else to = pivot-1;
                  if(from ==to)
                     repeat++;
            }
          }


  /*  printf("after inserting a[ %d %s\n", i, "]");
      for(const auto& x: subseqs)
           printf("%d ", x);
        putchar('\n');*/

       }
    return subseqs.size()-1;
  }

  int main(){
    /*.............reading ...................*/
    time(&start);
    long int N, K;
    scanf("%ld %ld", &N, &K);

    long long int *a = new long long int[N+1];
    long long int *l = new long long int[N+1];
    long long int *r = new long long int[N+1];
    a[0] = -1;
    l[0] = -1;
    r[0] = -1;

    for(long int i=1; i<N+1; i++){
        scanf("%lld ", &a[i]);
    }
    getchar();

  //
  /*for(int i=1; i<N+1; i++){
      printf("%lld ", a[i]);
  }
  putchar('\n');*/
  //

    int res ,len;
    res = find_lis(a, N, l);
    if(K>0 && res!=N){
       find_lds(a, N, r, K);
        //
  /*    for(int i=1; i<N+1; i++)
           cout<<" "<<l[i];
        putchar('\n');
        for(int i=1; i<N+1; i++)
           cout<<" "<<r[i];
        putchar('\n');*/
        //
        //check for compatible
        int pivot , from, to, repeat;
        for(int y = 1; y<N; y++){
                len = l[y] + r[y+1];
                if(len > res)
                    res =len;
        }
    }
    /*  printf("after inserting a[ %d %s\n", i, "]");
        for(const auto& x: subseqs)
        }
     }
    printf("%d\n", res);
    //cout<<l[9]<<endl;
  //  cout<<r[10]<<endl;*/
    printf("%d\n", res);
    time(&tend);
    double time_taken = double(tend - start);
    if(time_taken>1)
        cout << "Time taken by program is : " << fixed  << time_taken << setprecision(5)<<endl;

    delete [] l;
    delete [] r;
    return 0;
  }
