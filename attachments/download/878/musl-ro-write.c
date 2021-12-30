#include <stdio.h>

int main(void)
{
    FILE *fp = fopen("ro.txt","a");
    if (fp) {
        printf("Error, should not be open\n");
        fprintf(fp, "How can this be written to a read only file?\n");
        fclose(fp);
    }
    return 0;
}
