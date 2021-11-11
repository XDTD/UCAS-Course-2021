#include <stdio.h>
#include <string.h>

#define max(a,b) a>b?a:b
#define min(a,b) a<b?a:b


int main(void) {
    int letters[26], cnt = 0;
    char s[501];
    scanf("%s",s);
    cnt = strlen(s);
    for(int i = 0; i < cnt; i++) {
        letters[s[i] - 'a'] = i;
    }
    int last = 0,next = letters[s[0] - 'a'];
    do{
        for(int i = last; i <= next; i++) {
            next = max(next, letters[s[i] - 'a']);
        }
        if(next != cnt - 1) {
            printf("%d ",next - last + 1);
        }else{
            printf("%d",next - last + 1);
        }
        next++;
        last = next;
    }while(next < cnt );
	return 0;
}