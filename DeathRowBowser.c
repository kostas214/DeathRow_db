#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "report.h"
#include "commands.h"


int main(void)
{

	report *first = NULL;
	report *last = NULL;
	int entries = 0;
	int err_code = 0;


	err_code = parse_file("texas_new.csv", &first, &last, &entries);
	printf("parse done entries %d error code %d\n", entries, err_code);
	save_to_file("test.txt", first, entries);


	err_code =  average_command(first,entries,strdup("avg age"));
	printf("avg command result %d %s\n",err_code,err_to_str(err_code));

	err_code = count_command(first,entries,strdup("count men"));
	printf("count result %d %s\n",err_code,err_to_str(err_code));


	print_report(first);


	return 0;
}