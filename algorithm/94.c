#include <stdio.h>   


using namespace std;

int lower_bound(int *nums, int n, int targets) {
    int ans = -1;
    int l = 0,mid = 0;
    int r = n - 1;
    while(l <= r) {
        mid = l + (r - l) / 2;
        if (nums[mid] == targets && mid > 0 && nums[mid - 1] < targets ){
           break;
        }else if(nums[mid] < targets) {
            l = mid + 1;
        }else {
            r = mid - 1;
        }
    }
    if (nums[mid] == targets) {
        ans = mid;
    }
    return ans;
}

int upper_bound(int *nums, int n, int targets) {
    int ans = -1;
    int l = 0,mid = 0;
    int r = n - 1;
    while(l <= r) {
        mid = l + (r - l) / 2;
        if (nums[mid] == targets && mid < n - 1 && nums[mid + 1] > targets ){
           break;
        }else if(nums[mid] > targets) {
            r = mid - 1;
        }else {
             l = mid + 1;
        }
    }
    if (nums[mid] == targets) {
        ans = mid;
    }
    return ans;
}


int main() {
    
    int n,m;
    scanf("%d %d", &n, &m);
    int nums[100001];
    int targets[100001];
    for(int i = 0; i < n; i++) {
        int temp;
        scanf("%d",&temp);
        nums[i] = temp;
    }
    for(int i = 0; i < m; i++) {
        int temp;
        scanf("%d",&temp);
        targets[i] = temp; 
    } 
    for(int i = 0; i < m; i++) {
       int idx1 = lower_bound(nums, n, targets[i]);
       int idx2 = upper_bound(nums, n, targets[i]);
       printf("%d %d\n", idx1, idx2);

    }
    

    return 0;
}