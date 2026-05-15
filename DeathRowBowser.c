#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


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
	
	char* surname;
	char* name;
	int age;
	char* race;
	char* city;
	int felony_age;
	int education;
	int male_victims;
	int fem_victims;
	int num_victims;
	char* final_words;
	
	struct reportList* next;
	
} report;


void newReport(char buffer[]); //Check info of user input and decide whether a report should be made
void addReport(report* input); //Insert the report onto the list

int main(void)
{

	report* first = NULL;
	report* last = NULL;
	
	char buffer[] = "Miller;John;50;White;Cansas;40;12;1;2;3;It wasn't me";
	
	
	newReport(buffer);
	

	return 0;
}

void newReport(char buffer[])
{

	report Report; //putting this here for now
	
	int i, j, failed = 0, males, females;
	char *report_elements[11], temp_buffer[101] = {0};
	
	//extract values from buffer
	report_elements[0] = strtok(buffer,";");
	if (report_elements[0] == NULL)
	{
		printf("Invalid format\n");
		failed = 1;
	}
		
	for (i=1 ; i<11; i++) {
		report_elements[i] = strtok(NULL,";");
		if (report_elements[i] == NULL) {
			printf("Invalid Format\n");
			failed = 1;
			break;
		}
	
	}
	
	//Assign values to struct
	if (failed == 0) {
	
		Report.surname = strdup(report_elements[0]);
		Report.name = strdup(report_elements[1]);


		//age checks
		if (strcmp(report_elements[2], "") == 0) {
			Report.age = -1; //NA age
		}
		else if (strtol(report_elements[2], NULL, 10) > 120) {
			printf("Felon age too high\n");
			failed = 1;
		}
		else
			Report.age = strtol(report_elements[2], NULL, 10);


		Report.race = strdup(report_elements[3]);
		Report.city = strdup(report_elements[4]);


		//felony age checks
		if (strcmp(report_elements[5], "") == 0) {
			Report.felony_age = -1; //NA felon age
		}
		else if (strtol(report_elements[5], NULL, 10) > Report.age) {
			printf("Felon age during felony too high\n");
			failed = 1;
		}
		else
			Report.felony_age = strtol(report_elements[5], NULL, 10);


		//education checks
		if (strcmp(report_elements[6], "") == 0) {
			Report.education = -1; //NA education
		}
		else if (strtol(report_elements[6], NULL, 10) > Report.age - 5) {
			printf("Felon education too high or unrealistic\n");
			failed = 1;
		}
		else
			Report.education = strtol(report_elements[6], NULL, 10);


		//Male victims checks
		if (strcmp(report_elements[7], "") == 0) {
			Report.male_victims = -1; //NA male victims
			males = Report.male_victims;
		}
		else if (strtol(report_elements[7], NULL, 10) < 0) {
				printf("Invalid male victims\n");
				failed = 1;
		}
		else {
			Report.male_victims = strtol(report_elements[7], NULL, 10);
			males = Report.male_victims;
		}


		//Female victims checks
		if (strcmp(report_elements[8], "") == 0) {
			Report.fem_victims = -1; //NA female victims
			females = Report.fem_victims;
		}
		else if (strtol(report_elements[8], NULL, 10) < 0) {
				printf("Invalid female victims\n");
				failed = 1;
		}
		else {
			Report.fem_victims = strtol(report_elements[8], NULL, 10);
			females = Report.fem_victims;
		}


		//Total victims checks
		if (strcmp(report_elements[9], "") == 0 || males == -1 || females == -1) {
			Report.num_victims = -1; //NA total victims
		}
		else if (males + females != strtol(report_elements[9], NULL, 10) ) {
			printf("Total victim count doesn't match\n"); //Maybe ask the user if they wanna allow males + females to become num_victims
		}
		else
			Report.num_victims = strtol(report_elements[9], NULL, 10);
			
			
		Report.final_words = strdup(report_elements[10]);
		
		printf("%s, %s, %d, %s, %s, %d, %d, %d, %d, %d, %s\n", Report.surname, Report.name, Report.age, Report.race, Report.city, Report.felony_age, Report.education, Report.male_victims, Report.fem_victims, Report.num_victims, Report.final_words);
	}
	
	
		
		

}
