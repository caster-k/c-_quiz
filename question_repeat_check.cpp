#include<iostream>
#include<time.h>
#include<cstdlib>
using namespace std;

int main() {
    int arr[]= {1,5,2,3,4};
    int* rand = new int[5];
    

    for(int i=0; i<sizeof(arr) / sizeof(arr[0]); i++){
        // ran = &ran;
        // srand(time(0));
        // int ran = rand()%5;
        int ran;
        cout<<"Enter a number: ";
        cin>>ran;
        *(rand+i) = ran;


        for(int j = 0; j < sizeof(arr)/ sizeof(arr[0]); j++){
            if( *(rand+j) != ran ){
                cout<<"You have entered a unique number\n"; 
                break;
            }
            else{
                cout<<"You have entered repeated values\n";
                break;
            }
        }
    }

}