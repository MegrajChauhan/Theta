#include "includes/preprocessor.h"

int main()
{
    TPreprocessor *p = theta_init_preprocessor("main.th");
    if (theta_process_file(p) == false)
    {
        printf("Shit!\n");
        theta_destroy_preprocessor(p);
        return 0;
    }
    printf("%s\n", p->reader.file_contents);
    theta_destroy_preprocessor(p);
}