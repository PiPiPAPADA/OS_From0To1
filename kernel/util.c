void memory_copy(char * source, char* dest, int nbyte){
    int i;
    for(int i=0; i<nbyte; i++){
        *(dest+i) = *(source+i);
    }
}

void int_to_ascii(int n, char str[]){
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';
}