#include <stdio.h>
#include <string.h>


int main(void)
{
    char option[10] = {};
    char calc_option[5] = {};
    int digit = -5;
    //scanf("%s %d", option, &digit);
    fgets(option, 10, stdin);
    printf("option %s\n\n", option);

    sscanf(option, "%s %d", calc_option, &digit);
    printf("%s\n", calc_option);
    printf("%d", digit);

    return 0;
}