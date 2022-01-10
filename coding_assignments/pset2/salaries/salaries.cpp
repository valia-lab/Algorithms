#include <vector>
#include <iostream>
#include <stdio.h>
using namespace std;

int find_lis(long long int a[], long int n, long long int *l){

    l[1] = 1;
    vector<long int> subseqs;    // holds the last added element to each increasing subseq
    subseqs.push_back(-1); //dummy push to have vector index base = 1
    subseqs.push_back(a[1]);
    long int back, mini, dual, repeat = 0;
    int pivot, from, to, j;
    for(int i = 2; i<n+1; i++){
      //scan subseqs by decreasing size
      back = subseqs.size()-1 ;
      mini = 1;
      dual = 0;
      repeat = 0;

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

    }
    return subseqs.size()-1;

}

int find_lds(long long int a[], long int n, long long int *r, int K){


      vector<long int> subseqs;    // holds the last added element to each increasing subseq
      subseqs.push_back(-1); //dummy push to have vector index base = 1
      subseqs.push_back(a[n]);
      long int back, maxi, dual, repeat = 0, flag;
      long int pivot, from, to, j;
      if(a[n-1] < a[n] || a[n-1]-a[n]< K)
          r[n] = 1;
      else r[n] = 0;
      for(int i =n-1; i>1; i--){
        //scan subseqs by decreasing size
        back = subseqs.size()-1 ;
        maxi = 1;
        dual = 0;
        repeat = 0;
        flag = 0;

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

         if(a[i]== subseqs[to] &&(a[i-1] <subseqs[to] || a[i-1]-subseqs[to]< K))
             r[i] = to;
         else{
         repeat = 0;
             while(from<=to && repeat<2){
                  pivot = from + (to-from)/2;
                  if((a[i-1]< subseqs[pivot]) || (a[i-1] - subseqs[pivot]< K)) {//compatible
                      from = pivot+1;          //try to see if there is an even bigger compatible subseq
                      r[i] = pivot;           //the size of decreasing subseq ending in subseqs[i] is i
                  }
                  else to = pivot-1;
                  if(from ==to)
                     repeat++;
            }
          }


       }
    return subseqs.size()-1;
  }

  int main(){
    /*.............reading ...................*/
    long int N, K;
    scanf("%ld %ld", &N, &K);
    if(N==1){
      printf("%d\n", 1);
      return 0;
    }

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

    long int res ,len;
    res = find_lis(a, N, l);
    if(K>0){
       find_lds(a, N, r, K);

        //check for compatible
        long int pivot , from, to, repeat;
        for(int y = 1; y < N; y++){
                len = l[y] + r[y+1];
                if(len > res)
                    res =len;
        }
    }

    printf("%ld\n", res);

    delete [] l;
    delete [] r;
    return 0;
  }
