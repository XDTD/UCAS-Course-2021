#include <stdio.h>

#define min(a,b) a<b?a:b



int main(void) { 
    int pre[201], cur[201],triangle[201], m = 1;
    int n;
    scanf("%d",&n);
    scanf("%d",&cur[0]);
    pre[0] = cur[0];
    for(int row = 2; row <= n; row++) {
        for(int col = 0; col < row; col++){
            scanf("%d",&triangle[col]);
        }
        for(int j = 0; j < row; j++) {
            cur[j] = 0x7fffffff;
        }
        for(int j = 0; j < m; j++) {
            cur[j] = min(cur[j], pre[j]+triangle[j]);
            cur[j+1] = min(cur[j+1], pre[j]+triangle[j+1]);
        }
        m = row;
        for(int j = 0; j < m; j++) {
            pre[j] = cur[j];
        }
    }
    
    
    int ans = cur[0];
    for(int i = 0; i < m; i++) {
        ans = min(ans, cur[i]);
    }
    printf("%d\n",ans);
	return 0;
}