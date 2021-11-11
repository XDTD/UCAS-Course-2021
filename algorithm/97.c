#include <stdio.h>

#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b


int main(void) {
    char c;
   int minCnt = 0, maxCnt = 0;
    while(scanf("%c", &c) != EOF) {
        if (c == '(') {
            maxCnt++;
            minCnt++;
        }
        else if (c == ')')  {
            minCnt = max(minCnt - 1, 0);
            maxCnt--;
            if(maxCnt < 0){
                minCnt = -1;
                break;
            }
        }
        else if (c == '*') {
            minCnt = max(minCnt - 1, 0);
            maxCnt++;
        }
        
       
    }
    if(minCnt == 0){
        printf("True\n");
    }else {
        printf("NO\n");
    }
	return 0;
}