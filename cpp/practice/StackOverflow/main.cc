#include <stdio.h>

void quicksort(float x[],int first,int last)
{
    float temp;int  j, pivot, i;

    if(first<last){
         pivot=first;
         i=first;
         j=last;
         while(i<j){
             while(x[i]<=x[pivot]&&i<last)
                 i++;
             while(x[j]>x[pivot])
                 j--;
             if(i<j){
                 temp=x[i];
                  x[i]=x[j];
                  x[j]=temp;
             }
         }

         temp=x[pivot];
         x[pivot]=x[j];
         x[j]=temp;
         quicksort(x,first,j-1);
         quicksort(x,j+1,last);
    }
}

int main()
{
    float a[]={4.0,6.0,8.5,9.1,6,11.1,1.1};
    int len=6;
    //int len=7;
    quicksort(a,0,len);
    int i=0;
    for(i=0;i<len;i++)
        printf("%f\n",a[i]);
}