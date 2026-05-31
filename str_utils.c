#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "str_utils.h"

int is_digits(const char *buffer)
{
    int i;
    for (i = 0; i < strlen(buffer); i++)
        if (!isdigit(buffer[i]))
            return 0;
    return 1;
}

int count_char(const char *buffer, char target_char)
{

    int count = 0, i;
    for (i = 0; i < strlen(buffer); i++)
    {
        if (buffer[i] == target_char)
        {
            count++;
        }
    }
    return count;
}

char **split_str(char *buffer, char split_char, int *split_len)
{

    char *buffer_end = buffer + strlen(buffer) + 1;
    int field_count = count_char(buffer, split_char) + 1;

    *split_len = field_count;

    char **split_arr = calloc(field_count, sizeof(char *));

    if (split_arr == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    char delimiter[2];
    delimiter[0] = split_char;
    delimiter[1] = '\0';

    char *split_str = strtok(buffer, delimiter);

    if (split_str != NULL)
    {
        split_arr[0] = strdup(split_str);

        if (split_arr[0] == NULL)
        {
            printf("Memory allocation failed\n");
            free_str_arr(split_arr, field_count);
            exit(0);
        }
    }
    else
    {
        split_arr[0] = NULL;
    }

    int i;
    for (i = 1; i < field_count; i++)
    {
        split_str = strtok(NULL, delimiter);

        if (split_str == NULL)
        {
            split_arr[i] = NULL;
        }
        else if (
            //Check if it starts with " means it can have ; inside
            split_str[0] == '\"'
            && 
            //Check if its the last words field
            i == 10
            && 
            //Check if it actually has more semicolons
            field_count > 11
        )
        {
            //undo strtok
            split_str[strlen(split_str)] = ';';
            split_arr[i] = strdup(split_str);
            break;
        }
        else if (
            //check if there is a next string in the buffer
            buffer_end > split_str + strlen(split_str) + 1
            &&
            //check if the next string is empty (;;)
            *(split_str + strlen(split_str) + 1) == split_char)
        {
            printf("%d %d %d\n", i, i + 1, field_count);
            split_arr[i] = strdup(split_str);
            split_arr[++i] = NULL;
            continue;
        }
        else if (split_str != NULL)
        {
            split_arr[i] = strdup(split_str);
            if (split_arr[i] == NULL)
            {
                printf("Memory allocation failed\n");
                free_str_arr(split_arr, field_count);
                exit(0);
            }
        }
    }

    return split_arr;
}

void free_str_arr(char **array, int length)
{

    int i;
    for (i = 0; i < length; i++)
    {
        if (array[i] != NULL)
        {
            free(array[i]);
        }
    }

    free(array);
}
