#include <iostream>
#include <vector>
#include <algorithm>
#include <array>
//#include "solve.h"
using namespace std;

int solve(int N, int K, vector<vector<long long int>> S, int *nums){
    int m, temp, mini = K+1;

    for(int y = N-1; y>0; y--){               //search s(x,y) with fixed y for all y

        //.......create array with nums = {1,..,K-1}
        for(int j = 1; j< S[y+1].size(); j++){ //O(n)
            m = S[y+1][j];
            if(m < K )
                if(j < nums[m])
                    nums[m] = j;
        }
        /*........................................*/

        //............check all s(i,y) with y fixed ....................
       // the relative position of s(i,y) in S[i] is y -i +1............
        for(int i = 1; i < y+1; i++){
            if(S[i].size() >= y - i + 2){ //if S[i] contains s(i,y)
                temp = K - S[i][y - i + 1];
                if(temp < K && nums[temp] < K+1){       //if K-s[i][y] exists right of y
                    mini = min(nums[temp] + y - i + 1, mini);
                }
            }
        }
    }
    if(mini<K+1)
        return mini;
    else return -1;
}

int main(int argc, char **argv){

    long int N, K;
    cin>> N >> K;
    long long int *pref = new long long int[N + 1];
    pref[0] = 0; //dummy
    int temp, mini = K+1;
    cin>>pref[1];
    vector<long long int> q;
    q.push_back(-1);
    q.push_back(pref[1]);

    int flag= 0;

    for(int i=2; i<N+1; i++){
        cin>>temp;
      /*  if(temp == 600 && flag==0){
          cout<<"i found 600 in "<<i<<endl;
          flag=1;
        }*/
        if(temp ==K){
           cout<<1<<endl;
           return 0;
        }
        pref[i] = pref[i-1]+temp;
        if(pref[i]<K)
            q.push_back(pref[i]);
        if(pref[i]==K && i < mini){
            mini = i;
            //cout<<i<<endl;
        }
    }

    vector<vector<long long int>> S;
    S.reserve(N+1);
    S.push_back({});
    S.push_back(q);     //contains s(1,j) for all j
  //s  cout<<"S size is " <<S.size()<<endl;

    //produce s(i,j)'s
    for(int i=1; i<N; i++){
        vector<long long int> s;
        s.push_back(-1);
        for(int j=i+1; j<N+1; j++){
            temp = pref[j]-pref[i];
            if(temp < K)
              s.push_back(temp);
            else if(temp == K){
              mini = min(mini, j - i);
          //    cout<<j-i<<endl;
              break;                        //s(i,j+1)>s(i,j) >K
            }
           else break;                      //s(i,j) already larger than K
      }
        S.push_back(s);
     }

     int *nums = new int[K+1];
     nums[0] = 0;             //dummy initialisation```````````````````````````````````````````````````````````
    //.....initialise array for y fixed at N-1...............
    for(int i = 1; i < K; i++)
      nums[i] = K+1;               //initialise in infty
    if(pref[N]-pref[N-1] < K)
      nums[pref[N]-pref[N-1]] = 1;

    delete [] pref;
    int m;
/*.................................................................................*/
   cout<<solve(N, K, S, nums)<<flush<<endl;
   return 0;
}
