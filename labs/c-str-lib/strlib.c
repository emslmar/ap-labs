int mystrlen(char *str){
    int n = 0;
    while (str[n] != '\0')
    {
        n++;
    }
    return n;
}

char *mystradd(char *origin, char *addition){
    int i = mystrlen(origin);
    int j = 0;

    while (addition[j] != '\0')
    {
        origin[i] = addition[j];
        i++;
        j++;
    }
    origin[i] = '\0';
    return origin;
}

int mystrfind(char *origin, char *substr){
    int i = 0;
    int j = 0;

    while (origin[i] != '\0')
    {
        if(origin[i] == substr[j]){
            if(j == mystrlen(substr) - 1){
                return i - (mystrlen(substr) - 1);
            }
            i++;
            j++;
        }else{
            i++;
            j = 0;
        }
    }
    return -1;
}