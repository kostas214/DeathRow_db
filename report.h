#ifndef REPORT_H
#define REPORT_H

#include <stdio.h>

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
	int num_victims;
	int male_victims;
	int fem_victims;
	char *final_words;

	struct reportList *next;

} report;


int parse_report(report *Report, char *buffer, char *insert_type);			  // Check info of user input and decide whether a report should be made

void add_to_list(report *input, report **first, report **last, int *entries); // Insert the report onto the list
int delete(report **first, report **last, int index, int *entries); // fully customizable delete function


int save_to_file(char *filename, report *first, int entries);
int parse_file(char *filename, report **first, report **last, int *entries);

void clear_report(report *input); // free up a report from memory

int get_len_line(FILE *fp);

#endif