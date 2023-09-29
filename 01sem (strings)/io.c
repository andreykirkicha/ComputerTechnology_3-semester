#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH 30
#define NUMBER_OF_STRINGS 30

int main()
{
    char **strings;
    strings = (char **)calloc(LENGTH * NUMBER_OF_STRINGS, sizeof(char *));
    *strings = (char *)calloc(LENGTH, sizeof(char));

    char sym = 0;
    int i = 0, j = 0, k = 0, cur_strings_count = 0;

    char **tmp_ptr = strings;

    while ((sym = getchar()) != '\n' && sym != EOF)
    {
        if (i == LENGTH || sym == ' ')
        {
            (*strings)[i] = '\0';
            ++strings;
            *strings = (char *)calloc(LENGTH, sizeof(char));
            i = 0;
            ++cur_strings_count;

            if (sym == ' ')
                continue;
        }
        
        (*strings)[i++] = sym;
    }

    strings = tmp_ptr;

    if (cur_strings_count > 1)
    {
        for (j = 0; j <= cur_strings_count / 2; ++j)
        {
            char *tmp_string = strings[j];
            strings[j] = strings[cur_strings_count - j];
            strings[cur_strings_count - j] = tmp_string;
        }
    }

    if (cur_strings_count > 2)
    {
        int str_len = strlen(strings[2]);

        for (k = 0; k < str_len / 2; ++k)
        {
            char tmp_sym = strings[2][k];
            strings[2][k] = strings[2][str_len - k - 1];
            strings[2][str_len - k - 1] = tmp_sym;
        }
    }

    printf("\n");

    for (j = 0; j <= cur_strings_count; ++j)
    {
        printf("%d: %s\n", j + 1, strings[j]);
    }

    return 0;
}