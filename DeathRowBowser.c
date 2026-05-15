#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define ERR_INVALID_FORMAT -1
#define ERR_SURNAME_NOT_SPECIFIED -2
#define ERR_NAME_NOT_SPECIFIED -3
#define ERR_AGE_NOT_SPECIFIED -4
#define ERR_RACE_NOT_SPECIFIED -5
#define ERR_CITY_NOT_SPECIFIED -6
#define ERR_FELONY_AGE_NOT_SPECIFIED -7
#define ERR_INVALID_AGE -8
#define ERR_INVALID_FELONY_AGE -9
#define ERR_INVALID_EDUCATION -10
#define ERR_INVALID_M_VICTIMS -11
#define ERR_INVALID_F_VICTIMS -12
#define ERR_INVALID_NUM_VICTIMS -13
#define ERR_INVALID_TOTAL_VICTIMS -14
#define SUCCESS 1

#define REPORT_NA -1

typedef struct
{

	int year;
	int month;
	int day;

} dateinfo;

typedef struct
{

	int hour;
	int minutes;
	int seconds;

} timeinfo;

typedef struct reportList
{
	dateinfo date_added;
	timeinfo time_added;
	char insert_type[5];

	char *surname;
	char *name;
	int age;
	char *race;
	char *city;
	int felony_age;
	int education;
	int male_victims;
	int fem_victims;
	int num_victims;
	char *final_words;

	struct reportList *next;

} report;

int parse_report(report *Report, char buffer[]); // Check info of user input and decide whether a report should be made
void addReport(report *input);					 // Insert the report onto the list
int is_digits(char buffer[]);
const char *err_to_str(int err);

int main(void)
{

	report *first = NULL;
	report *last = NULL;

	char buffer[] = "Miller;John;50;White;Cansas;40;12;1;2;3;It wasn't me";

	report new_report;

	int err_code = parse_report(&new_report, buffer);

	if (err_code != SUCCESS)
	{
		printf("%s\n",err_to_str(err_code));
	}
	
	
	return 0;
}

int parse_report(report *Report, char buffer[])
{

	int i, j;
	char *report_elements[11];

	// extract values from buffer
	report_elements[0] = strtok(buffer, ";");
	if (report_elements[0] == NULL)
	{
		return ERR_INVALID_FORMAT;
	}

	for (i = 1; i < 11; i++)
	{
		report_elements[i] = strtok(NULL, ";");
		if (report_elements[i] == NULL)
		{
			return ERR_INVALID_FORMAT;
		}
	}

	// Parse values

	char *surname = NULL;
	char *name = NULL;
	int age = -1;
	char *race = NULL;
	char *city = NULL;
	int felony_age = -1;
	int education = -1;
	int male_victims = -1;
	int fem_victims = -1;
	int num_victims = -1;
	char *final_words = NULL;

	if (strlen(report_elements[0]) == 0)
		return ERR_SURNAME_NOT_SPECIFIED;
	else
	{
		surname = report_elements[0];
	}

	if (strlen(report_elements[1]) == 0)
		return ERR_NAME_NOT_SPECIFIED;
	else
	{
		name = report_elements[1];
	}

	// age checks
	if (strlen(report_elements[2]) == 0 || !is_digits(report_elements[2]))
	{
		return ERR_AGE_NOT_SPECIFIED;
	}
	else
	{
		int age_parsed = (int)strtol(report_elements[2], NULL, 10);

		if (age_parsed > 120)
		{
			return ERR_INVALID_AGE;
		}
		else
			age = age_parsed;
	}

	if (strlen(report_elements[3]) == 0)
	{
		return ERR_RACE_NOT_SPECIFIED;
	}
	else
	{
		race = report_elements[3];
	}

	if (strlen(report_elements[4]) == 0)
	{
		return ERR_CITY_NOT_SPECIFIED;
	}
	else
	{
		city = report_elements[4];
	}

	// felony age checks
	if (strlen(report_elements[5]) == 0 || !is_digits(report_elements[5]))
	{
		felony_age = REPORT_NA;
	}
	else
	{

		int parsed_felony_age = (int)(strtol(report_elements[5], NULL, 10));

		if (parsed_felony_age > age)
		{
			return ERR_INVALID_FELONY_AGE;
		}

		felony_age = parsed_felony_age;
	}

	// education checks
	if (strlen(report_elements[6]) == 0 || !is_digits(report_elements[6]))
	{
		education = REPORT_NA;
	}
	else
	{

		int parsed_education = (int)strtol(report_elements[6], NULL, 10);

		if (parsed_education > age - 5)
		{
			return ERR_INVALID_EDUCATION;
		}

		education = parsed_education;
	}

	// Male victims checks
	if (strlen(report_elements[7]) == 0 || !is_digits(report_elements[7]))
	{
		male_victims = REPORT_NA;
	}
	else
	{
		int parsed_male_victims = (int)strtol(report_elements[7], NULL, 10);

		if (parsed_male_victims < 0)
		{
			return ERR_INVALID_M_VICTIMS;
		}

		male_victims = parsed_male_victims;
	}

	// Female victims checks
	if (strlen(report_elements[8]) == 0 || !is_digits(report_elements[8]))
	{
		fem_victims = REPORT_NA;
	}
	else
	{
		int parsed_female_victims = (int)strtol(report_elements[8], NULL, 10);

		if (parsed_female_victims < 0)
		{
			return ERR_INVALID_F_VICTIMS;
		}

		fem_victims = parsed_female_victims;
	}

	if (strlen(report_elements[9]) == 0 || !is_digits(report_elements[9]))
	{
		num_victims = REPORT_NA;
	}
	else
	{
		int parsed_num_victims = (int)strtol(report_elements[9], NULL, 10);

		if (parsed_num_victims < 0)
		{
			return ERR_INVALID_NUM_VICTIMS;
		}

		num_victims = parsed_num_victims;
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

	if (num_victims != total_victims)
	{
		return ERR_INVALID_TOTAL_VICTIMS;
	}

	final_words = report_elements[10];

	// Fill report
	Report->surname = strdup(surname);
	Report->name = strdup(name);
	Report->age = age;
	Report->race = strdup(race);
	Report->city = strdup(city);
	Report->felony_age = felony_age;
	Report->education = education;
	Report->male_victims = male_victims;
	Report->fem_victims = fem_victims;
	Report->num_victims = num_victims;
	Report->final_words = strdup(final_words);

	printf("%s, %s, %d, %s, %s, %d, %d, %d, %d, %d, %s\n",
		   Report->surname,
		   Report->name,
		   Report->age,
		   Report->race,
		   Report->city,
		   Report->felony_age,
		   Report->education,
		   Report->male_victims,
		   Report->fem_victims,
		   Report->num_victims,
		   Report->final_words);

	return SUCCESS;
}

int is_digits(char buffer[])
{
	int i;
	for (i = 0; i < strlen(buffer); i++)
		if (!isdigit(buffer[i]))
			return 0;
	return 1;
}


const char *err_to_str(int err)
{
    switch (err)
    {
    case ERR_INVALID_FORMAT:
        return "The format of the report is invalid.";

    case ERR_SURNAME_NOT_SPECIFIED:
        return "Surname not specified.";

    case ERR_NAME_NOT_SPECIFIED:
        return "Name not specified.";

    case ERR_AGE_NOT_SPECIFIED:
        return "Age not specified.";

    case ERR_RACE_NOT_SPECIFIED:
        return "Race not specified.";

    case ERR_CITY_NOT_SPECIFIED:
        return "City not specified.";

    case ERR_FELONY_AGE_NOT_SPECIFIED:
        return "Felony age not specified.";

    case ERR_INVALID_AGE:
        return "Invalid age.";

    case ERR_INVALID_FELONY_AGE:
        return "Invalid felony age.";

    case ERR_INVALID_EDUCATION:
        return "Invalid education value.";

    case ERR_INVALID_M_VICTIMS:
        return "Invalid male victims value.";

    case ERR_INVALID_F_VICTIMS:
        return "Invalid female victims value.";

    case ERR_INVALID_NUM_VICTIMS:
        return "Invalid number of victims value.";

    case ERR_INVALID_TOTAL_VICTIMS:
        return "Mismatch in total victims.";

    default:
        return "Unknown error.";
    }
}
