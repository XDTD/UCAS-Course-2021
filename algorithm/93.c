#include <stdio.h>

int fastPow(int x, int y) {
    int mod = 1337, ans = 1, base = x % mod;
    while(y != 0 ) {
        if(y % 2 == 1) {
            ans = ans * base;
        }
        base = base * base % mod;
        y /= 2;
    }
    return ans % mod;
}


int main(void) { 
	int x;
	char ch;
	scanf("%d",&x);
	int ans = 1, mod = 1337;
	while((ch=getchar()) !=']'){
	    if(ch < '0' || ch > '9') 
	        continue;
        int curr = ch - '0';
	    ans = fastPow(ans,10) * fastPow(x,curr) % mod;     
	
	}
	printf("%d\n",ans);
	return 0;
}