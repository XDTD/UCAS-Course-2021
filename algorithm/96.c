#include <stdio.h>

#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b
long long max4(long long a, long long b, long long c, long long d) {
    long long temp1 = max(a,b);
    long long temp2 = max(c,d);
    return max(temp1, temp2);
}

long long maxAlternatingSum(int* nums, int numsSize){
    int lastVal =  -1,  minVal = 100001;
    long long ans = -1;
    for(int i = 1; i < numsSize; i++) {
        long long temp = ans;
        ans = max4(ans, nums[i], ans+nums[i]-lastVal, ans + nums[i]-minVal);
        if (ans == temp) {
            minVal = min(nums[i], minVal);
        }else {
            lastVal = nums[i];
            minVal = 100001;
        } 
    }
    return ans;
}


int main(void) { 
    long long n;
    long long lastVal =  -1,  minVal = 100001;
    long long ans = -1;
    while(scanf("%lld",&n) != EOF) {
        long long temp = ans;
        ans = max4(ans, n, ans+n-lastVal, ans + n-minVal);
        if (ans == temp) {
            minVal = min(n, minVal);
        }else {
            lastVal = n;
            minVal = 100001;
        } 
    }
    printf("%lld\n",ans);
	return 0;
}