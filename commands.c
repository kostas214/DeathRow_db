#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "commands.h"
#include "str_utils.h"

int count_fields(report *first, int entries, int *result, int (*get_field)(report *))
{
	/* types:
	m = male
	f = female
	a = all
	*/

	report *tmp = first;
	int count = 0;

	if (entries == 0)
	{
		return RESULT_NO_ENTRIES;
	}

	while (tmp->next != first)
	{

		int field = get_field(tmp);

		if (field != REPORT_NA)
		{
			count += field;
		}
		tmp = tmp->next;
	}
	int field = get_field(tmp);
	if (field != REPORT_NA)
	{
		count += field;
	}

	*result = count;
	return SUCCESS;
}

int average(report *first, int entries, float *result, int (*get_field)(report *))
{
	/* types:
	a = age
	f = felony age
	e = education
	*/

	report *tmp = first;
	int count = 0;

	if (entries == 0)
	{
		return RESULT_NO_ENTRIES;
	}

	int err = count_fields(first, entries, &count, get_field);

	if (err != SUCCESS)
	{
		return err;
	}

	*result = count / (float)entries;
	return SUCCESS;
}

int find(report *first, int entries, int *result, char *(*get_field)(report *), char *argument, char type) // find num of matches with argument or print every match
{
	/* types:
	c = count matches
	p = print matches
	*/

	report *tmp = first;
	int count = 0, c = 0;

	if (entries == 0)
	{
		*result = 0;
		return RESULT_NO_ENTRIES;
	}

	to_lower(argument);

	while (tmp->next != first)
	{
		char* field_str = to_lower(strdup(get_field(tmp)));
		if (strstr(field_str, argument) != NULL)
		{
			count++;
			if (type == 'c')
				(*result)++;
			else if (type == 'p')
			{
				print_report(tmp);

				if (count != *result)
					printf("\n[%d/%d]\nPress ENTER to view the next report or q to stop\n", count, *result);
				else
					printf("\n[%d/%d]\nPress ENTER to finish viewing reports\n", count, *result);

				while ((c = getchar()) != '\n')
				{
					if (c == 'q')
					{
						while (getchar() != '\n')
							;

						free(field_str);
						return SUCCESS;
					}
				}
			}
		}
		free(field_str);
		tmp = tmp->next;
	}

	char* field_str = to_lower(strdup(get_field(tmp)));

	if (strstr(field_str, argument) != NULL)
	{
		count++;
		if (type == 'c')
			(*result)++;
		else if (type == 'p')
		{
			print_report(tmp);
			printf("\n[%d/%d]\nPress ENTER to finish viewing reports\n", count, *result);
			while (getchar() != '\n')
				;
		}
	}
	free(field_str);

	return SUCCESS;
}

int average_command(report *first, int entries, char *command_str)
{

	char *args[2] = {0};

	args[0] = strtok(command_str, " ");
	args[1] = strtok(NULL, " ");

	if (args[0] == NULL || strcmp(args[0], "avg") != 0)
	{
		return ERR_INVALID_COMMAND;
	}

	if (args[1] == NULL)
	{
		return ERR_INVALID_ARGS;
	}

	int err_code = 0;
	char *argument = args[1];
	float result = 0;

	if (strcmp(argument, "age") == 0)
	{
		err_code = average(first, entries, &result, get_age);
		if (err_code == SUCCESS)
		{
			printf("The average age of documented felons is %.2f years\n", result);
		}
		else
		{
			return err_code;
		}
	}
	else if (strcmp(argument, "received") == 0)
	{
		err_code = average(first, entries, &result, get_felony_age);
		if (err_code == SUCCESS)
		{
			printf("The average felony age of documented felons is %.2f years\n", result);
		}
		else
		{
			return err_code;
		}
	}
	else if (strcmp(argument, "education") == 0)
	{
		err_code = average(first, entries, &result, get_education);

		if (err_code == SUCCESS)
		{
			printf("The average education level of documented felons is %.2f\n", result);
		}
		else
		{
			return err_code;
		}
	}
	else
	{
		return ERR_INVALID_ARGS;
	}

	return SUCCESS;
}

int count_command(report *first, int entries, char *command_str)
{

	char *args[2] = {0};

	args[0] = strtok(command_str, " ");
	args[1] = strtok(NULL, " ");

	if (args[0] == NULL || strcmp(args[0], "count") != 0)
	{
		return ERR_INVALID_COMMAND;
	}
	if (args[1] == NULL)
	{
		return ERR_INVALID_ARGS;
	}
	int err_code = 0;
	char *argument = args[1];
	int result = 0;

	if (strcmp(argument, "victims") == 0)
	{
		err_code = count_fields(first, entries, &result, get_num_victims);
		if (err_code == SUCCESS)
		{
			printf("Number of total victims from documented felons: %d\n", result);
		}
		else
		{
			return err_code;
		}
	}
	else if (strcmp(argument, "men") == 0)
	{
		err_code = count_fields(first, entries, &result, get_male_victims);
		if (err_code == SUCCESS)
		{
			printf("Number of male victims from documented felons: %d\n", result);
		}
		else
		{
			return err_code;
		}
	}
	else if (strcmp(argument, "women") == 0)
	{
		err_code = count_fields(first, entries, &result, get_fem_victims);

		if (err_code == SUCCESS)
		{
			printf("Number of female victims from documented felons: %d\n", result);
		}
		else
		{
			return err_code;
		}
	}
	else
	{
		return ERR_INVALID_ARGS;
	}

	return SUCCESS;
}

int find_command(report *first, int entries, char *command_str)
{

	char *args[2] = {0};

	args[0] = strtok(command_str, " ");
	args[1] = strtok(NULL, " ");

	if (args[0] == NULL || strstr(args[0], "find") == NULL)
	{
		return ERR_INVALID_COMMAND;
	}

	if (args[1] == NULL)
	{
		return ERR_INVALID_ARGS;
	}

	int err_code = 0;
	char *find_type = args[0];
	char *argument = args[1];
	int result = 0;

	// printf("%s\n", find_type);
	// printf("%s\n", argument);

	if (strcmp(find_type, "findS") == 0) // find surname
	{
		err_code = find(first, entries, &result, get_surname, argument, 'c');

		if (err_code != SUCCESS)
		{
			return err_code;
		}
		else if (result > 0)
		{
			printf("\n[%d matches found]\nPress ENTER to cycle through results\n", result);
			while (getchar() != '\n')
				;
			err_code = find(first, entries, &result, get_surname, argument, 'p');

			if (err_code != SUCCESS)
			{
				return err_code;
			}
		}
		else
		{
			return ERR_NO_MATCHES_FOUND;
		}
	}
	else if (strcmp(find_type, "findN") == 0) // find name
	{
		err_code = find(first, entries, &result, get_name, argument, 'c');

		if (err_code != SUCCESS)
		{
			return err_code;
		}
		else if (result > 0)
		{
			printf("\n[%d matches found]\nPress ENTER to cycle through results\n", result);
			while (getchar() != '\n')
				;
			err_code = find(first, entries, &result, get_name, argument, 'p');
			if (err_code != SUCCESS)
			{
				return err_code;
			}
		}
		else
		{
			return ERR_NO_MATCHES_FOUND;
		}
	}
	else if (strcmp(find_type, "findR") == 0) // find race
	{
		err_code = find(first, entries, &result, get_race, argument, 'c');

		if (err_code != SUCCESS)
		{
			return err_code;
		}
		else if (result > 0)
		{
			printf("\n[%d matches found]\nPress ENTER to cycle through results\n", result);
			while (getchar() != '\n')
				;
			err_code = find(first, entries, &result, get_race, argument, 'p');
			if (err_code != SUCCESS)
			{
				return err_code;
			}
		}
		else
		{
			return ERR_NO_MATCHES_FOUND;
		}
	}
	else if (strcmp(find_type, "findF") == 0) // find final words
	{
		err_code = find(first, entries, &result, get_final, argument, 'c');

		if (err_code != SUCCESS)
		{
			return err_code;
		}
		else if (result > 0)
		{
			printf("\n[%d matches found]\nPress ENTER to cycle through results\n", result);
			while (getchar() != '\n')
				;
			err_code = find(first, entries, &result, get_final, argument, 'p');
			if (err_code != SUCCESS)
			{
				return err_code;
			}
		}
		else
		{
			return ERR_NO_MATCHES_FOUND;
		}
	}
	else
	{
		return ERR_INVALID_ARGS;
	}

	return SUCCESS;
}

void print_report(report *r)
{

	printf("============================\n");
	printf("Date added: %04d-%02d-%02d\n",
		   r->date_added.year,
		   r->date_added.month,
		   r->date_added.day);

	printf("Time added: %02d:%02d:%02d\n",
		   r->time_added.hour,
		   r->time_added.minutes,
		   r->time_added.seconds);

	printf("Insert type: %s\n",
		   r->insert_type);

	printf("Name: %s %s\n",
		   r->name ? r->name : "(null)",
		   r->surname ? r->surname : "(null)");

	printf("Age: %d\n", r->age);
	printf("Race: %s\n", r->race ? r->race : "(null)");
	printf("City: %s\n", r->city ? r->city : "(null)");

	printf("Felony age: %d\n", r->felony_age);
	printf("Education: %d\n", r->education);

	printf("Victims (total/male/female): %d / %d / %d\n",
		   r->num_victims,
		   r->male_victims,
		   r->fem_victims);

	printf("Final words: %s\n",
		   r->final_words ? r->final_words : "(null)");
	printf("============================\n");
}

int get_age(report *r)
{
	return r->age;
}

int get_felony_age(report *r)
{
	return r->felony_age;
}

int get_education(report *r)
{
	return r->education;
}

int get_num_victims(report *r)
{
	return r->num_victims;
}

int get_male_victims(report *r)
{
	return r->male_victims;
}

int get_fem_victims(report *r)
{
	return r->fem_victims;
}

char *get_final(report *r)
{
	return r->final_words;
}

char *get_race(report *r)
{
	return r->race;
}

char *get_name(report *r)
{
	return r->name;
}

char *get_surname(report *r)
{
	return r->surname;
}