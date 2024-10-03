#include <iostream>
using namespace std;

int main()
{
    srand(time(0));
    int unq[46]={0};
    int c = 0;
    while (c < 45)
    {
        int r = rand() % 45;
        int flag = 0;
        for (int j = 0; j < 45; j++)
        {
            // printf("%d ", unq[j]);
            if (unq[j] == r)
            {
                    flag = 1;
                break;
                
            }
           
        }
        //  printf("\n");
        // printf("\n");
        if(flag == 0){
        unq[c] = r;
        c++;
        }
    }
    for (int i = 0; i < 45; i++)
    {
        printf("%d ", unq[i]);
    }
}