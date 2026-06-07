/* 

 Skernti Ntisani
 AM: 2022202500108
 dit25108@go.uop.gr
 
 Kostantinos Doukas
 AM: 2022202500036
 dit25036@go.uop.gr

*/
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "report.h"
#include "errors.h"
#include "str_utils.h"

int parse_report(report *Report, char *buffer, char *insert_type)
{

	int err = SUCCESS;

	// extract values from buffer

	int split_len = 0;
	char **report_elements = split_str(buffer, ';', &split_len);

	if (split_len < 11)
	{
		err = ERR_INVALID_FORMAT;
		goto cleanup;
	}

	// Parse values
	char *surname = NULL;
	char *name = NULL;
	int age = REPORT_NA;
	char *race = NULL;
	char *city = NULL;
	int felony_age = REPORT_NA;
	int education = REPORT_NA;
	int male_victims = REPORT_NA;
	int fem_victims = REPORT_NA;
	int num_victims = REPORT_NA;
	char *final_words = NULL;

	if (!report_elements[0] || strlen(report_elements[0]) == 0)
	{
		err = ERR_SURNAME_NOT_SPECIFIED;
		goto cleanup;
	}
	else
	{
		surname = report_elements[0];
	}

	if (!report_elements[1] || strlen(report_elements[1]) == 0)
	{
		err = ERR_NAME_NOT_SPECIFIED;
		goto cleanup;
	}
	else
	{
		name = report_elements[1];
	}

	// age checks
	if (!report_elements[2] || strlen(report_elements[2]) == 0 || !is_digits(report_elements[2]))
	{
		err = ERR_AGE_NOT_SPECIFIED;
		goto cleanup;
	}
	else
	{
		int age_parsed = (int)strtol(report_elements[2], NULL, 10);

		if (age_parsed > 120)
		{
			err = ERR_INVALID_AGE;
			goto cleanup;
		}
		else
			age = age_parsed;
	}

	if (!report_elements[3] || strlen(report_elements[3]) == 0)
	{
		err = ERR_RACE_NOT_SPECIFIED;
		goto cleanup;
	}
	else
	{
		race = report_elements[3];
	}

	if (!report_elements[4] || strlen(report_elements[4]) == 0)
	{
		err = ERR_CITY_NOT_SPECIFIED;
		goto cleanup;
	}
	else
	{
		city = report_elements[4];
	}

	// felony age checks
	if (!report_elements[5] || strlen(report_elements[5]) == 0 || !is_digits(report_elements[5]))
	{
		felony_age = REPORT_NA;
	}
	else
	{

		int parsed_felony_age = (int)(strtol(report_elements[5], NULL, 10));

		if (parsed_felony_age > age)
		{
			err = ERR_INVALID_FELONY_AGE;
			goto cleanup;
		}

		felony_age = parsed_felony_age;
	}

	// education checks
	if (!report_elements[6] || strlen(report_elements[6]) == 0 || !is_digits(report_elements[6]))
	{
		education = REPORT_NA;
	}
	else
	{

		int parsed_education = (int)strtol(report_elements[6], NULL, 10);

		if (parsed_education > age - 5)
		{
			err = ERR_INVALID_EDUCATION;
			goto cleanup;
		}

		education = parsed_education;
	}

	if (!report_elements[7] || strlen(report_elements[7]) == 0 || !is_digits(report_elements[7]))
	{
		num_victims = REPORT_NA;
	}
	else
	{
		int parsed_num_victims = (int)strtol(report_elements[7], NULL, 10);

		if (parsed_num_victims < 0)
		{
			err = ERR_INVALID_NUM_VICTIMS;
			goto cleanup;
		}

		num_victims = parsed_num_victims;
	}

	// Male victims checks
	if (!report_elements[8] || strlen(report_elements[8]) == 0 || !is_digits(report_elements[8]))
	{
		male_victims = REPORT_NA;
	}
	else
	{
		int parsed_male_victims = (int)strtol(report_elements[8], NULL, 10);

		if (parsed_male_victims < 0)
		{
			err = ERR_INVALID_M_VICTIMS;
			goto cleanup;
		}

		male_victims = parsed_male_victims;
	}

	// Female victims checks
	if (!report_elements[9] || strlen(report_elements[9]) == 0 || !is_digits(report_elements[9]))
	{
		fem_victims = REPORT_NA;
	}
	else
	{
		int parsed_female_victims = (int)strtol(report_elements[9], NULL, 10);

		if (parsed_female_victims < 0)
		{
			err = ERR_INVALID_F_VICTIMS;
			goto cleanup;
		}

		fem_victims = parsed_female_victims;
	}

	// Check victims
	int total_victims = 0;

	if (male_victims != REPORT_NA)
	{
		total_victims += male_victims;
	}

	if (fem_victims != REPORT_NA)
	{
		total_victims += fem_victims;
	}

	if (num_victims == REPORT_NA)
	{
		if (total_victims != 0)
		{
			err = ERR_INVALID_TOTAL_VICTIMS;
			goto cleanup;
		}
	}
	else if (num_victims != total_victims)
	{
		err = ERR_INVALID_TOTAL_VICTIMS;
		goto cleanup;
	}

	if (report_elements[10] == NULL)
	{
		final_words = "";
	}
	else
	{
		final_words = report_elements[10];
	}

	// Fill report
	Report->surname = strdup(surname);
	if (Report->surname == NULL)
	{
		printf("Memory allocation failed\n");
		exit(0);
	}

	Report->name = strdup(name);
	if (Report->name == NULL)
	{
		printf("Memory allocation failed\n");
		exit(0);
	}
	Report->age = age;
	Report->race = strdup(race);
	if (Report->race == NULL)
	{
		printf("Memory allocation failed\n");
		exit(0);
	}
	Report->city = strdup(city);
	if (Report->city == NULL)
	{
		printf("Memory allocation failed\n");
		exit(0);
	}
	Report->felony_age = felony_age;
	Report->education = education;
	Report->male_victims = male_victims;
	Report->fem_victims = fem_victims;
	Report->num_victims = num_victims;
	Report->final_words = strdup(final_words);
	if (Report->final_words == NULL)
	{
		printf("Memory allocation failed\n");
		exit(0);
	}

	strcpy(Report->insert_type, insert_type);

	// add timestamps
	time_t curr_time;
	struct tm *User_time;

	time(&curr_time);
	User_time = localtime(&curr_time);

	Report->date_added.year = User_time->tm_year + 1900;
	Report->date_added.month = User_time->tm_mon + 1;
	Report->date_added.day = User_time->tm_mday;

	Report->time_added.hour = User_time->tm_hour;
	Report->time_added.minutes = User_time->tm_min;
	Report->time_added.seconds = User_time->tm_sec;

	err = SUCCESS;

cleanup:
	free_str_arr(report_elements, split_len);
	return err;
}

void add_to_list(report *input, report **first, report **last, int *entries)
{
	if (*last == NULL) // First element
	{
		*first = input;
		*last = input;
		input->next = input;
	}

	else if (*last != NULL)
	{
		(*last)->next = input;
		*last = input;
		input->next = *first;
	}
	(*entries)++;
}

int delete(report **first, report **last, int index, int *entries)
{
	if (*entries == 0)
	{
		return RESULT_NO_ENTRIES;
	}

	else if (index == 1) // delete first element (deleteR)
	{
		report *tmp = *first;
		if (*entries == 1)
		{
			clear_report(*first);
			*first = NULL;
			*last = NULL;
			(*entries)--;
		}
		else
		{
			*first = (*first)->next;
			(*last)->next = *first;
			clear_report(tmp);
			(*entries)--;
		}
	}
	else if (index == *entries) // delete last element (deleteO)
	{
		report *tmp = *first;
		if (*entries == 1)
		{
			clear_report(*last);
			*first = NULL;
			*last = NULL;
			(*entries)--;
		}
		else
		{
			while (tmp->next->next != *first) // find second to last element of list
			{
				tmp = tmp->next;
			}
			clear_report(tmp->next);
			*last = tmp;
			tmp->next = *first;
			(*entries)--;
		}
	}
	else if (0 < index && index < *entries) // delete anything thats not the first or last element
	{
		report *tmp = *first;
		report *tmp2;
		for (int i = 1; i < index - 1; i++) // stop at the previous element of the one we wanna delete
			tmp = tmp->next;

		tmp2 = tmp->next->next;
		clear_report(tmp->next);
		tmp->next = tmp2;
		(*entries)--;
	}
	else
		// printf("We couldn't find what you're looking for\n");
		return ERR_INDEX_NOT_FOUND;

	return SUCCESS;
}

int save_to_file(char *filename, report *first, int entries)
{

	if (entries == 0)
	{
		return RESULT_NOTHING_TO_SAVE;
	}

	FILE *file = fopen(filename, "w");

	if (file == NULL)
	{
		return ERR_FILE_NOT_FOUND;
	}

	report *tmp = first;

	while (tmp->next != first)
	{
		fprintf(file, "[%d/%d/%d @ ", tmp->date_added.day, tmp->date_added.month, tmp->date_added.year);
		fprintf(file, "%02d:%02d:%02d (", tmp->time_added.hour, tmp->time_added.minutes, tmp->time_added.seconds);
		fprintf(file, "%s)] ", tmp->insert_type);

		fprintf(file, "%s;", tmp->surname);
		fprintf(file, "%s;", tmp->name);
		fprintf(file, "%d;", tmp->age);
		fprintf(file, "%s;", tmp->race);
		fprintf(file, "%s;", tmp->city);
		if (tmp->felony_age == REPORT_NA)
		{
			fprintf(file, "NA;");
		}
		else
		{
			fprintf(file, "%d;", tmp->felony_age);
		}
		if (tmp->education == REPORT_NA)
		{
			fprintf(file, "NA;");
		}
		else
		{
			fprintf(file, "%d;", tmp->education);
		}

		if (tmp->num_victims == REPORT_NA)
		{
			fprintf(file, "NA;");
		}
		else
		{
			fprintf(file, "%d;", tmp->num_victims);
		}
		if (tmp->male_victims == REPORT_NA)
		{
			fprintf(file, "NA;");
		}
		else
		{
			fprintf(file, "%d;", tmp->male_victims);
		}
		if (tmp->fem_victims == REPORT_NA)
		{
			fprintf(file, "NA;");
		}
		else
		{
			fprintf(file, "%d;", tmp->fem_victims);
		}
		fprintf(file, "%s\n", tmp->final_words);

		tmp = tmp->next;
	}

	fprintf(file, "[%d/%d/%d @ ", tmp->date_added.day, tmp->date_added.month, tmp->date_added.year);
	fprintf(file, "%02d:%02d:%02d (", tmp->time_added.hour, tmp->time_added.minutes, tmp->time_added.seconds);
	fprintf(file, "%s)] ", tmp->insert_type);

	fprintf(file, "%s;", tmp->surname);
	fprintf(file, "%s;", tmp->name);
	fprintf(file, "%d;", tmp->age);
	fprintf(file, "%s;", tmp->race);
	fprintf(file, "%s;", tmp->city);

	if (tmp->felony_age == REPORT_NA)
	{
		fprintf(file, "NA;");
	}
	else
	{
		fprintf(file, "%d;", tmp->felony_age);
	}

	if (tmp->education == REPORT_NA)
	{
		fprintf(file, "NA;");
	}
	else
	{
		fprintf(file, "%d;", tmp->education);
	}

	if (tmp->num_victims == REPORT_NA)
	{
		fprintf(file, "NA;");
	}
	else
	{
		fprintf(file, "%d;", tmp->num_victims);
	}
	if (tmp->male_victims == REPORT_NA)
	{
		fprintf(file, "NA;");
	}
	else
	{
		fprintf(file, "%d;", tmp->male_victims);
	}
	if (tmp->fem_victims == REPORT_NA)
	{
		fprintf(file, "NA;");
	}
	else
	{
		fprintf(file, "%d;", tmp->fem_victims);
	}

	fprintf(file, "%s", tmp->final_words);

	fclose(file);

	return SUCCESS;
}

int parse_file(char *filename, report **first, report **last, int *entries)
{

	int err = 0;

	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		return ERR_FILE_NOT_FOUND;
	}

	int len_line = get_len_line(fp);

	while (len_line > 1)
	{

		char *buffer = (char *)malloc(len_line + 1);
		if (buffer == NULL)
		{
			printf("Failed to allocate memory, exiting program\n");
			exit(0);
		}

		fgets(buffer, len_line + 1, fp);

		// printf("%s", buffer);

		report *new_report = (report *)malloc(sizeof(report));
		if (new_report == NULL)
		{
			printf("Failed to allocate memory, exiting program\n");
			exit(0);
		}
		// printf("%s\n", buffer);

		int parse_error = parse_report(new_report, buffer, "file");
		free(buffer);
		if (parse_error == SUCCESS)
		{
			add_to_list(new_report, first, last, entries);
		}
		else
		{
			printf("Failed to parse report %s\n", err_to_str(parse_error));
			free(new_report);
		}
		fgetc(fp); // read \n
		len_line = get_len_line(fp);
	}

	err = SUCCESS;

	fclose(fp);
	return err;
}

void clear_report(report *input)
{
	free(input->surname);
	free(input->name);
	free(input->race);
	free(input->city);
	free(input->final_words);
	free(input);
}

int get_len_line(FILE *fp)
{

	int prev_idx = ftell(fp);

	int count = 0;

	int character;
	while ((character = fgetc(fp)) != EOF && character != '\n')
	{
		count++;
	}

	fseek(fp, prev_idx, SEEK_SET);

	return count;
}
