#include<stdio.h>
#include<string.h>

int main(){
    char data[50], stuff[50],dstuff[50];
    int i, j, fsize, noofframes, tframe, dsize, s;

    printf("Enter data\n");
    scanf("%s",data);
    dsize = strlen(data);
    printf("%d\n",dsize);
    printf("Enter frame size :\n");
    scanf("%d",&fsize);
    printf("%d",fsize);
    tframe = fsize-1;

    noofframes = (dsize/tframe) + 1;
    j = 0;
    for(i = 0 ; i<(dsize + noofframes) ; i++){
        if(i%fsize == 0){
            if((dsize-j)<fsize){
                fsize = dsize-j+1;
            }
            stuff[i] = (char)(48+fsize);
            fsize = s;
        }
        else{
            stuff[i] = data[j];
            j++;
        }
    }
    stuff[dsize + noofframes] = '\0';
    printf("Hello");
    printf("stuffed data is : %s",stuff);
    return 0;
}