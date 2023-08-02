#include <stdio.h>
#include <ctype.h>

#define CSV_IMPLEMENTATION
#include "csv.h"

int main(void) {

	char **lines=NULL;
	size_t nlines=0;

	char *line;
	size_t llen=0;
	ssize_t rlen=0;

	FILE *fp=fopen("data.csv","r");

	while((rlen=getline(&line,&llen,fp))!=-1) {

		char *p=strchr(line,'\n'); if(p) *p='\0'; 
	
		if(CSV_Parse(&lines,&nlines,line)) exit(1);

		CSV_Print(lines,nlines);

		CSV_Free(&lines,&nlines);
	}

	return 0;
}
