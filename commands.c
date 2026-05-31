#include <string.h>

#include "commands.h"

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

int average_command(report *first, int entries, char *command_str)
{

	char *args[2] = {0};

	args[0];

	args[0] = strtok(command_str, " ");
	args[1] = strtok(NULL, " ");

	if (strcmp(args[0], "avg") != 0)
	{
		return ERR_INVALID_COMMAND;
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
			printf("%s\n", err_to_str(err_code));
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
			printf("%s\n", err_to_str(err_code));
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
			printf("%s\n", err_to_str(err_code));
		}
	}
	else
	{
		return ERR_INVALID_COMMAND;
	}

	return SUCCESS;
}

int count_command(report *first, int entries, char *command_str)
{

	char *args[2] = {0};

	args[0];

	args[0] = strtok(command_str, " ");
	args[1] = strtok(NULL, " ");

	if (strcmp(args[0], "count") != 0)
	{
		return ERR_INVALID_COMMAND;
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
		return ERR_INVALID_COMMAND;
	}

	return SUCCESS;
}

void print_report(report *r)
{

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
