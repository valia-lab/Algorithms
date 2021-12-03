#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <array>
using namespace std;
ifstream infile;

int main(int argc, char **argv){

    int N, K;
    infile.open(argv[1]);
    infile >> N >> K;
    int *pref = new int[N + 1];
    pref[0] = 0; //dummy
    int temp, mini = K+1;
    infile>>pref[1];
    vector<int> q;

    for(int i=2; i<N+1; i++){
        infile>>temp;
        if(temp ==K){
           cout<<1<<endl;
           return 0;
        }
        pref[i] = pref[i-1]+temp;
        if(pref[i]<K)
            q.push_back(pref[i]);
        if(pref[i]==K && i < mini)
            mini = i;
    }
    infile.close();
    return 0;
    vector<vector<int>> S;
    S.reserve(N+1);
    S.push_back({});
    S.push_back(q);     //contains s(1,j) for all j

    //produce s(i,j)'s
    for(int i=1; i<N; i++){
        vector<int> s;
        for(int j=i+1; j<N+1; j++){
          temp = pref[j]-pref[i]
          if(temp<K)
            s.push_back(temp);
          else if(temp==K){
            mini = min(mini, j - i + 1);
            break;                        //s(i,j+1)>s(i,j) >K
        }
      }
        S.push_back(s);
     }

     int *nums = new int[K+1];
     nums[0] = 0;             //dummy initialisation```````````````````````````````````````````````````````````
    //.....initialise array for y fixed at N-1...............
    for(int i = 1; i<N+1; i++)
      nums[i] = 0;
    if(pref[N]-pref[N-1] <K)
      nums[pref[N]-pref[N-1]] = 1;


    delete [] pref;
    int m;
/*.................................................................................*/
    for(int y = N-3; y>0; y--){               //search s(x,y) with fixed y for all y
        //.......create array with nums = {1,..,K}
        for(int j = y+1; j<N+1; j++){ //O(n)
            m = S[y+1][j];
            if(m<=K && (j-y <nums[m]))
                nums[m] = j - y + 1;

        }
        //............check all s(i,y) with y fixed ....................
        for(int i = 1; i<y; i++){
            temp = K-S[i][y];
            if(nums[temp] > 0)        //if K-s[i][y] exists right of y
                mini = min(nums[temp] + y - i + 1, mini);
        }
    }
    cout<<mini<<flush<<endl;
    delete [] nums;
    return 0;
}
/*.................................................................................*/
