/* 

 Skernti Ntisani
 AM: 2022202500108
 dit25108@go.uop.gr
 
 Kostantinos Doukas
 AM: 2022202500036
 dit25036@go.uop.gr

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "report.h"
#include "commands.h"
#include "str_utils.h"

int main(int argc, char *argv[])
{

	report *first = NULL;
	report *last = NULL;
	int entries = 0;
	int err_code = 0;

	int i;
	for (i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "--help") == 0)
		{
			if (i + 1 < argc)
			{
				if (!starts_with(argv[i + 1], "-"))
				{
					int len = snprintf(NULL, 0, "help %s", argv[i + 1]);

					char *command_str = (char *)malloc(len + 1);
					if (command_str == NULL)
					{
						printf("Memory allocation failed\n");
						exit(0);
					}

					snprintf(command_str, len + 1, "help %s", argv[i + 1]);

					int err_code = help_command(command_str);
					if (err_code != SUCCESS)
					{
						printf("Error: %s\n", err_to_str(err_code));
					}

					i++;
					free(command_str);
					return 0;
				}
			}
			else
			{
				char *tmp = strdup("help");
				help_command(tmp);
				free(tmp);
				return 0;
			}
		}
		else if (strcmp(argv[i], "--load") == 0)
		{

			if (i + 1 < argc)
			{
				if (!starts_with(argv[i + 1], "-"))
				{
					int len = snprintf(NULL, 0, "load %s", argv[i + 1]);

					char *command_str = (char *)malloc(len + 1);
					if (command_str == NULL)
					{
						printf("Memory allocation failed\n");
						exit(0);
					}

					snprintf(command_str, len + 1, "load %s", argv[i + 1]);

					int err_code = load_command(&first, &last, &entries, command_str);
					if (err_code != SUCCESS)
					{
						printf("Error: %s\n", err_to_str(err_code));
					}

					free(command_str);
					i++;
				}
				else
				{
					printf("File name not specified for --load\n");
				}
			}
			else
			{
				printf("File name not specified for --load\n");
			}
		}
		else if (i != 0)
		{
			printf("Invalid flag: %s\n",argv[i]);
		}
	}

	char buffer[5000] = {0};
	while (1)
	{
		printf("$> ");

		fgets(buffer, 5000, stdin);
		buffer[strcspn(buffer, "\n")] = '\0';

		if (strlen(buffer) <= 0)
			;
		else if (starts_with(buffer, "avg"))
		{
			err_code = average_command(first, entries, buffer);

			if (err_code != SUCCESS)
			{
				printf("Error: %s\n", err_to_str(err_code));
			}
		}
		else if (starts_with(buffer, "count"))
		{
			err_code = count_command(first, entries, buffer);
			if (err_code != SUCCESS)
			{
				printf("Error: %s\n", err_to_str(err_code));
			}
		}
		else if (starts_with(buffer, "find"))
		{
			err_code = find_command(first, entries, buffer);
			if (err_code != SUCCESS)
			{
				printf("Error: %s\n", err_to_str(err_code));
			}
		}
		else if (starts_with(buffer, "print"))
		{
			err_code = print_command(first, entries, buffer);
			if (err_code != SUCCESS)
			{
				printf("Error: %s\n", err_to_str(err_code));
			}
		}
		else if (starts_with(buffer, "load"))
		{
			err_code = load_command(&first, &last, &entries, buffer);
			if (err_code != SUCCESS)
			{
				printf("Error: %s\n", err_to_str(err_code));
			}
		}

		else if (starts_with(buffer, "save"))
		{
			err_code = save_command(first, entries, buffer);
			if (err_code != SUCCESS)
			{
				printf("Error: %s\n", err_to_str(err_code));
			}
		}

		else if (starts_with(buffer, "newReport"))
		{
			err_code = new_report_command(&first, &last, &entries, buffer);
			if (err_code != SUCCESS)
			{
				printf("Error: %s\n", err_to_str(err_code));
			}
		}
		else if (starts_with(buffer, "delete"))
		{
			err_code = delete_command(&first, &last, &entries, buffer);

			if (err_code != SUCCESS)
			{
				printf("Error: %s\n", err_to_str(err_code));
			}
		}
		else if (starts_with(buffer, "reversePrint"))
		{
			err_code = reverse_print_command(first, entries, buffer);
			if (err_code != SUCCESS)
			{
				printf("Error: %s\n", err_to_str(err_code));
			}
		}

		else if (starts_with(buffer, "clear"))
		{
			printf("\033[2J\033[H");
		}
		else if (starts_with(buffer, "help"))
		{
			err_code = help_command(buffer);
			if (err_code != SUCCESS)
			{
				printf("Error: %s\n", err_to_str(err_code));
			}
		}

		else if (starts_with(buffer, "exit"))
		{
			if (entries > 0)
			{
				report *tmp = first;

				while (tmp->next != first)
				{
					report *tmp_cpy = tmp;
					tmp = tmp->next;
					clear_report(tmp_cpy);
				}

				clear_report(tmp);
			}
			return 0;
		}
		else
		{
			printf("%s: %s\n", strtok(buffer, " "), err_to_str(ERR_COMMAND_NOT_FOUND));
		}
	}

	return 0;
}
