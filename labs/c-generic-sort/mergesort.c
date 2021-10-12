#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void final_merge(void *lineptr[], int left, int right, int (*comp)(void *, void *)){
    int mid=((left+right)/2)+1;      
    for (int i=mid;i<=right;i++)
    {
        for(int j=0;j<mid;j++)
        {
            if ((*comp)(lineptr[i],lineptr[j])<0) {
                void* tmp=lineptr[i];
                lineptr[i]=lineptr[j]; 
                lineptr[j]=tmp;
                lineptr[j]=lineptr[j]; 
            }
        }
    }
}

void merge_sort(void *lineptr[], int left, int right, int(*comp)(void *, void *)){
    if(left < right){
        int middle = (left + right) / 2;
        merge_sort(lineptr, left, middle, (*comp));
        merge_sort(lineptr, middle + 1, right, (*comp));
        final_merge(lineptr, left, right, (*comp));
    }
}

