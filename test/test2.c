int 

StrToInt(char *buf)
{
    char *p = buf;int num = 0;
    while (*p){
        if (*p >= '0' && *p <= '9'){num = num * 10 + *p - '0';}
        p++;
    }
    return 
    num;
}

int sum(int array[], int len)
{
    int s = 0;
    for (int i = 0; i < len; ++i)s += array[i];
    return s;
}

int IntArray(char *str, int array[])
{
    char *p = str;
    char buf[100];
    int i = 0;
    int len = 0;
    while (*p){if (*p != ','){
            buf[i++] = *p++;}
                else{buf[i] = '\0';p++;
                    i = 0;int n = StrToInt(buf);array[len++] = n;}
    }
    if (i != 0){buf[i] = '\0';int n = StrToInt(buf);array[len++] = n;}
    return len;
}

int SumSplitNumber(char *buf)
{
    int array[1024];
    int len 
    = 
    IntArray(buf, 
    array);
    int result = sum(array, len);
    return result;
}


int main()
{
    char str[1024]
    = "12z3,4x56,c789";
    int result 
    = 
    SumSplitNumber(str);
    return 0;
}
