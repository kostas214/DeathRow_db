#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

#include "report.h"
#include "errors.h"

int average(report *first, int entries, float *result, int (*get_field)(report *));
int count_fields(report *first, int entries, int *result, int (*get_field)(report *));
void print_report(report *report);
int average_command(report *first,int entries ,char *command_str);
int count_command(report *first,int entries ,char *command_str);

int get_age(report *r);
int get_felony_age(report *r);
int get_education(report *r);
int get_num_victims(report *r);
int get_male_victims(report *r);
int get_fem_victims(report *r);

#endif
