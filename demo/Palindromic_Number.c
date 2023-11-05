int main(){
    int num;
    scanf("%d", &num);

    if(num > 0){

        printf("Properties of %d\n", num);
        if(num % 2 == 0)printf("\teven:true\n\t odd:false\n");
        
        else printf("\teven:false\n\t odd:true\n");
        if(num % 7 == 0 || num % 10 == 7)printf("\tbuzz:true\n");
        else printf("\tbuzz:false\n");

        char ch[1000000+10];				
        sprintf(ch, "%d", num);
        if(strchr(ch, '0') == NULL)printf("\tduck:false\n");
        else printf("\tduck:true\n");
        
        char ch[1000000+10];				
        sprintf(ch, "%d", num);
        if(strchr(ch, '0') == NULL)printf("\tduck:false\n");
        else printf("\tduck:true\n");

        int r = strlen(ch) - 1, l = 0;          
        int isPalindromic = 1;
        while(l <= r){
            if(ch[r] != ch[l]){
                isPalindromic = 0;
                break;
            }
            r--;
            l++;
        }
		
        if(isPalindromic == 1)printf(" palindromic:true\n\n");
        else printf(" palindromic:false\n\n");

    }else{
        printf("This parameter should be a natural number.\n\n");
    }
    return 0;
}
