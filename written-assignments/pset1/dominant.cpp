#include <iostream>
#include <stack>
using namespace std;

void dominant(int a[], int d[], int n){
    d[0] = -1; //infty
    stack <int> s;
    int c;
    d[1] = 1;  //dominant of itself
    s.push(d[1]);
    for(int i=2; i<n; i++){
            c=i-1;
        while(!s.empty() && s.top()<a[i]){
            c--;
            s.pop();
        }
        if(!s.empty())
            d[i] = c;
        else d[i] = -1;    //no solution found 
        s.push(a[i]);     // self-push to be examined by i-1
    }
    for(int i=0; i<8; i++)
        cout<<d[i]<<" ";
    cout<<endl;
    return;
}

int main()
{
    int a[8] = {8,7,6,5,4,3,2,1};
    int d[8];
    dominant(a, d, 8);
    for(int i=0; i<8; i++)
        cout<<d[i]<<" ";
    cout<<endl;
    return 0;
}
