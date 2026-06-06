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

int find(report *first, int entries, int *result, int (*predicate)(report *, void **argument_data), void **argument_data, char type) // find num of matches with argument or print every match
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

	while (tmp->next != first)
	{
		if (predicate(tmp, argument_data))
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

						return SUCCESS;
					}
				}
			}
		}
		tmp = tmp->next;
	}

	if (predicate(tmp, argument_data))
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
	return SUCCESS;
}

int print_reverse(report *first, int entries, int count) // bonus function
{

	char c;
	int flag;

	if (entries == 0)
	{
		return RESULT_NO_ENTRIES;
	}

	report *tmp = first;
	if (count > 0)
	{
		flag = print_reverse(tmp->next, entries, count - 1);

		if (flag != SUCCESS)
		{
			print_report(tmp);

			if (count < entries)
				printf("\n[%d/%d]\nPress ENTER to view the next report or q to stop\n", count, entries);
			else
				printf("\n[%d/%d]\nPress ENTER to finish viewing reports\n", count, entries);

			while ((c = getchar()) != '\n')
			{
				if (c == 'q')
				{
					while (getchar() != '\n')
						;

					return SUCCESS;
				}
			}

			if (count == entries)
				return SUCCESS;
		}
		else
			return SUCCESS;
	}
	else
	{
		printf("[%d entries]\nPress ENTER to cycle through the reports\n", entries);
		while (getchar() != '\n')
			;
	}
	return 420;
}

int reverse_print_command(report *first, int entries, char *command_str)
{

	if (strcmp(command_str, "reversePrint") != 0)
	{
		return ERR_INVALID_COMMAND;
	}

	int err_code = print_reverse(first, entries, entries);

	if (err_code != SUCCESS)
	{
		return err_code;
	}

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
	void *argument_data[2];
	if (strcmp(find_type, "findS") == 0) // find surname
	{

		argument_data[0] = get_surname;
		argument_data[1] = to_lower(argument);
	}
	else if (strcmp(find_type, "findN") == 0) // find name
	{

		argument_data[0] = get_name;
		argument_data[1] = to_lower(argument);
	}
	else if (strcmp(find_type, "findR") == 0) // find race
	{
		argument_data[0] = get_race;
		argument_data[1] = to_lower(argument);
	}
	else if (strcmp(find_type, "findF") == 0) // find final words
	{

		argument_data[0] = get_final;
		argument_data[1] = to_lower(argument);
	}
	else
	{
		return ERR_INVALID_ARGS;
	}

	err_code = find(first, entries, &result, find_predicate, argument_data, 'c');

	if (err_code != SUCCESS)
	{
		return err_code;
	}
	else if (result > 0)
	{
		printf("\n[%d matches found]\nPress ENTER to cycle through results\n", result);
		while (getchar() != '\n')
			;
		err_code = find(first, entries, &result, find_predicate, argument_data, 'p');

		if (err_code != SUCCESS)
		{
			return err_code;
		}
	}
	else
	{
		return ERR_MATCHES_NOT_FOUND;
	}

	return SUCCESS;
}

int print_command(report *first, int entries, char *command_str)
{
	char *args[2] = {0};

	args[0] = strtok(command_str, " ");
	args[1] = strtok(NULL, " ");

	if (args[0] == NULL || strstr(args[0], "print") == NULL)
	{
		return ERR_INVALID_COMMAND;
	}

	if (args[1] == NULL)
	{
		return ERR_INVALID_ARGS;
	}

	int err_code = 0;
	char *print_type = args[0];
	int num_argument = (int)strtol(args[1], NULL, 10);
	int result = 0;

	if (num_argument == 0)
	{
		return ERR_INVALID_ARGS;
	}
	void *argument_data[2];

	if (strcmp(print_type, "printG") == 0)
	{
		argument_data[0] = compare_greater;
		argument_data[1] = &num_argument;
	}
	else if (strcmp(print_type, "printL") == 0)
	{
		argument_data[0] = compare_lower;
		argument_data[1] = &num_argument;
	}
	else if (strcmp(print_type, "printE") == 0)
	{
		argument_data[0] = compare_equal;
		argument_data[1] = &num_argument;
	}
	else
	{
		return ERR_INVALID_ARGS;
	}

	err_code = find(first, entries, &result, print_predicate, argument_data, 'c');

	if (err_code != SUCCESS)
	{
		return err_code;
	}
	else if (result > 0)
	{
		printf("\n[%d matches found]\nPress ENTER to cycle through results\n", result);
		while (getchar() != '\n')
			;
		err_code = find(first, entries, &result, print_predicate, argument_data, 'p');
		if (err_code != SUCCESS)
		{
			return err_code;
		}
	}
	else
	{
		return ERR_MATCHES_NOT_FOUND;
	}

	return SUCCESS;
}

int load_command(report **first, report **last, int *entries, char *command_str)
{
	char *args[2] = {0};

	args[0] = strtok(command_str, " ");
	args[1] = strtok(NULL, " ");

	if (args[0] == NULL || strcmp(args[0], "load") != 0)
	{
		return ERR_INVALID_COMMAND;
	}

	if (args[1] == NULL)
	{
		return ERR_INVALID_ARGS;
	}

	int err_code = 0;
	char *argument = args[1];

	err_code = parse_file(argument, first, last, entries);

	if (err_code == SUCCESS)
	{
		printf("Loaded %d entries\n", *entries);
	}
	else
	{
		return err_code;
	}

	return SUCCESS;
}

int save_command(report *first, int entries, char *command_str)
{
	char *args[2] = {0};

	args[0] = strtok(command_str, " ");
	args[1] = strtok(NULL, " ");

	if (args[0] == NULL || strcmp(args[0], "save") != 0)
	{
		return ERR_INVALID_COMMAND;
	}

	if (args[1] == NULL)
	{
		return ERR_INVALID_ARGS;
	}

	int err_code = 0;
	char *argument = args[1];

	err_code = save_to_file(argument, first, entries);

	if (err_code == SUCCESS)
	{
		printf("Successfully saved file\n");
	}
	else
	{
		return err_code;
	}

	return SUCCESS;
}

int new_report_command(report **first, report **last, int *entries, char *command_str)
{
	char *args[2] = {0};

	args[0] = strtok(command_str, " ");

	if (args[0] == NULL || strcmp(args[0], "newReport") != 0)
	{
		return ERR_INVALID_COMMAND;
	}

	int err_code = 0;
	char *argument = command_str + strlen(args[0]) + 1;

	report *new_report = (report *)malloc(sizeof(report));
	if (new_report == NULL)
	{
		printf("Could not allocate memory exiting program\n");
		exit(0);
	}

	err_code = parse_report(new_report, argument, "keyb");

	if (err_code != SUCCESS)
	{
		free(new_report);
		return err_code;
	}
	else
	{
		add_to_list(new_report, first, last, entries);
		printf("Successfully added report\n");
	}

	return SUCCESS;
}
int delete_command(report **first, report **last, int *entries, char *command_str)
{

	if (strstr(command_str, "delete") == NULL)
	{
		return ERR_INVALID_COMMAND;
	}

	int err_code = 0;

	if (strcmp("delete", command_str) == 0)
	{
		char *args[2] = {0};

		args[0] = strtok(command_str, " ");

		args[1] = strtok(NULL, " ");

		if (args[1] == NULL)
		{
			return ERR_INVALID_ARGS;
		}
		int argument = (int)strtol(args[1], NULL, 10);

		err_code = delete(first, last, argument, entries);
		if (err_code != SUCCESS)
		{
			return err_code;
		}
		else
		{
			printf("Successfully deleted report at index %d\n", argument);
		}
	}
	else if (strcmp("deleteO", command_str) == 0)
	{

		err_code = delete(first, last, *entries, entries);
		if (err_code != SUCCESS)
		{
			return err_code;
		}
		else
		{
			printf("Successfully deleted oldest report\n");
		}
	}
	else if (strcmp("deleteR", command_str) == 0)
	{

		err_code = delete(first, last, 1, entries);
		if (err_code != SUCCESS)
		{
			return err_code;
		}
		else
		{
			printf("Successfully deleted most recent report\n");
		}
	}

	return SUCCESS;
}

int help_command(char *command_str)
{

	int flag = 0, found = 0;

	char *args[2] = {0};

	args[0] = strtok(command_str, " ");
	args[1] = strtok(NULL, " ");

	if (args[0] == NULL || strcmp(args[0], "help") != 0)
	{
		return ERR_INVALID_COMMAND;
	}

	if (args[1] == NULL)
	{
		printf("Flag list\n");
		printf("--help shows this help message\n");
		printf("--load {file name} loads the selected file in the list\n");
		printf("Command list\n");
		printf("avg\ncount\nfind\nprint\nload\nsave\nnewReport\ndelete\nreversePrint\n");
		printf("Type: help {command} to see the details of the command or help all to see all the command details\n");
		return SUCCESS;
	}

	if (strcmp(args[1], "all") == 0)
	{
		flag = 1;
	}

	if (strcmp(args[1], "avg") == 0 || flag)
	{
		printf("Average command details\nusage: avg {age/received/education}\nexplanation: Prints the average age/age at the time of the crime/education level\n");
		found = 1;
	}
	if (strcmp(args[1], "count") == 0 || flag)
	{
		printf("Count command details\nusage: count {men/women/victims}\nexplanation: Prints the number of male/female/all victims killed\n");
		found = 1;
	}
	if (strcmp(args[1], "find") == 0 || flag)
	{
		printf("Find command details\nusage: findS {str} explanation: prints reports that have {str} inside their surname\nusage: findN {str} explanation: prints reports that have {str} inside their name\nusage: findR {str} explanation: prints reports that have {str} inside their race\nusage: findF {str} explanation: prints reports that have {str} inside their final words\nif {str} is * then all find commands will print all reports loaded\n");
		found = 1;
	}
	if (strcmp(args[1], "print") == 0 || flag)
	{
		printf("Print command details\nusage: printG {age} explanation: finds and prints reports that have either age or age of the crime greater than {age}\nusage: printL {age} explanation: finds and prints reports that have either age or age of the crime lower than {age}\nusage: printE {age} explanation: finds and prints reports that have either age or age of the crime equal to {age}\n");
		found = 1;
	}
	if (strcmp(args[1], "load") == 0 || flag)
	{
		printf("Load command details\nusage: load {filePath}\nexplanation: Loads reports from a file with the correct format from {filePath}\n");
		found = 1;
	}
	if (strcmp(args[1], "save") == 0 || flag)
	{
		printf("Save command details\nusage: save {filePath}\nexplanation: Save all loaded reports at {filePath}\n");
		found = 1;
	}
	if (strcmp(args[1], "newReport") == 0 || flag)
	{
		printf("New report command details\nusage: newReport {report}\nexplanation: Add a new report to the list, {report} must be a valid report string\n");
		found = 1;
	}
	if (strcmp(args[1], "delete") == 0 || flag)
	{
		printf("Delete command details\nusage: delete {index} explanation: deletes report at {index}\nusage: deleteO explanation: Deletes oldest report\nusage: deleteR explanation: deletes the most recent report\n");
		found = 1;
	}
	if (strcmp(args[1], "help") == 0 || flag)
	{
		printf("Help command details\nusage: help explanation: prints commands and flags of the program\nusage: help {command} explanation: Prints details of the {command}, if {command} is all then it prints the details of all commands\n");
		found = 1;
	}
	if (strcmp(args[1], "reversePrint") == 0 || flag)
	{
		printf("Reverse print command details\nusage: reversePrint\nexplanation: prints the loaded reports in the opposite orders inputted\n");
		found = 1;
	}

	if (!found)
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

	if (r->age != REPORT_NA)
		printf("Age: %d\n", r->age);
	else
		printf("Age: NA\n");
	printf("Race: %s\n", r->race ? r->race : "(null)");
	printf("City: %s\n", r->city ? r->city : "(null)");

	if (r->felony_age != REPORT_NA)
		printf("Felony age: %d\n", r->felony_age);
	else
		printf("Felony age: NA\n");

	if (r->education != REPORT_NA)
		printf("Education: %d\n", r->education);
	else
		printf("Education: NA\n");

	if (r->num_victims != REPORT_NA)
		printf("Victims (total/male/female): %d / ", r->num_victims);
	else
		printf("Victims (total/male/female): NA / ");

	if (r->male_victims != REPORT_NA)
		printf("%d / ", r->male_victims);
	else
		printf("NA / ");

	if (r->fem_victims != REPORT_NA)
		printf("%d\n", r->fem_victims);
	else
		printf("NA\n");

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

int compare_greater(int age, int argument)
{
	return age > argument;
}
int compare_lower(int age, int argument)
{
	return age < argument;
}
int compare_equal(int age, int argument)
{
	return age == argument;
}

int find_predicate(report *this_report, void **argument_data)
{
	// get_fied is at 0

	char *(*get_field)(report *r) = (char *(*)(report * r)) argument_data[0];

	// search str is at 1

	char *argument = (char *)argument_data[1];

	char *field_str = to_lower(strdup(get_field(this_report)));

	int result = argument[0] == '*' || strstr(field_str, argument) != NULL;

	free(field_str);
	return result;
}

int print_predicate(report *this_report, void **argument_data)
{
	// compare function is at 0
	int (*compare_function)(int age, int argument) = (int (*)(int age, int argument))argument_data[0];

	int argument = *((int *)argument_data[1]);

	int check1 = 0;
	int check2 = 0;

	if (this_report->age != REPORT_NA)
	{
		check1 = compare_function(this_report->age, argument);
	}

	if (this_report->felony_age != REPORT_NA)
	{
		check2 = compare_function(this_report->felony_age, argument);
	}

	return check1 || check2;
}
