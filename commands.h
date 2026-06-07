#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

#include "report.h"
#include "errors.h"

int average(report *first, int entries, float *result, int (*get_field)(report *));
int count_fields(report *first, int entries, int *result, int (*get_field)(report *));
void print_report(report *report);
int average_command(report *first, int entries, char *command_str);
int count_command(report *first, int entries, char *command_str);
int find(report *first, int entries, int *result, int (*predicate)(report *, void **argument_data), void **argument_data, char type);
int find_command(report *first, int entries, char *command_str);
int print_command(report *first, int entries, char *command_str);
int load_command(report **first, report **last, int *entries, char *command_str);
int save_command(report *first,int entries, char *command_str);
int new_report_command(report**first ,report **last,int* entries,char* command_str);
int delete_command(report**first,report**last,int*entries,char* command_str);
int reverse_print_command(report* first,int entries,char *command_str);
int help_command(char *command_str);


int find_predicate(report *this_report, void **argument_data);
int print_predicate(report *this_report, void **argument_data);
int print_reverse(report* first, int entries, int count);

int get_age(report *r);
int get_felony_age(report *r);
int get_education(report *r);
int get_num_victims(report *r);
int get_male_victims(report *r);
int get_fem_victims(report *r);
char *get_final(report *r);
char *get_race(report *r);
char *get_name(report *r);
char *get_surname(report *r);
char *get_city(report *r);

int compare_greater(int age, int argument);
int compare_lower(int age, int argument);
int compare_equal(int age, int argument);

#endif
