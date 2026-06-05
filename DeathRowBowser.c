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

	err_code = load_command(&first, &last, &entries, strdup("load texas_new.csv"));
	printf("load command result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = new_report_command(&first, &last, &entries, strdup("newReport Fuller;Barney;58;White;Houston;45;9;2;1;1;I don't have anything to say, you can proceed Warden Jones."));
	printf("new_report command result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = delete_command(&first, &last, &entries, "deleteO");
	printf("delete command result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = delete_command(&first, &last, &entries, "deleteR");
	printf("delete command result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = delete_command(&first, &last, &entries, "delete 3");
	printf("delete command result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = save_command(first, entries, strdup("save test.txt"));
	printf("save command result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = average_command(first, entries, strdup("avg age"));
	printf("avg command result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = count_command(first, entries, strdup("count men"));
	printf("count result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = find_command(first, entries, strdup("findF pain"));
	printf("find result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = find_command(first, entries, strdup("findR black"));
	printf("find result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = find_command(first, entries, strdup("findN John"));
	printf("find result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = find_command(first, entries, strdup("findS Smith"));
	printf("find result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = find_command(first, entries, strdup("findS *"));
	printf("find result %d %s\n\n", err_code, err_to_str(err_code));

	err_code = print_command(first, entries, strdup("printE 51"));
	printf("print result %d %s\n\n", err_code, err_to_str(err_code));

	/* TODO print_report() doesnt display NA */

	return 0;
}
