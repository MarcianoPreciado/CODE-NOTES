#include <stdio.h>
#include <wchar.h>

int main(void)
{
    int err = fwide(stdin, 0);
    printf("%d\n",err);
    return 0;
}
