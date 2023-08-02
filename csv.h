#ifndef CSV_H
#define CSV_H



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


#define STRLIB_IMPLEMENTATION
#include "strlib.h"


#define STRING_MAX 1024



typedef enum ParserState {
	PARSERSTATE_DEFAULT=0,
	PARSERSTATE_QUOTED_STRING,
	PARSERSTATE_STRING,
	PARSERSTATE_COMMA,
	PARSERSTATE_ERROR,
	PARSERSTATE_MAX  
} ParserState;



void CSV_AddLine(char ***lines,size_t *nlines,char *line);

int CSV_Parse(char ***lines,size_t *nlines,char *line);

void CSV_Print(char **lines,size_t nlines);

void CSV_Free(char ***lines,size_t *nlines);



#ifdef CSV_IMPLEMENTATION



void CSV_AddLine(char ***lines,size_t *nlines,char *line) {
	(*lines)=realloc(*lines,sizeof(**lines)*(*nlines+1));  
	(*lines)[(*nlines)++]=strdup(line);
}
 


int CSV_Parse(char ***lines,size_t *nlines,char *line) {
	ParserState parserState=PARSERSTATE_DEFAULT;
	char *p=line;
	char text[STRING_MAX];
	bool quit=false;
	int retval=0;
	text[0]='\0';
	while(!quit) {
		switch(parserState) {
			case PARSERSTATE_DEFAULT: 
				if(!*p) {
					quit=true;
				} else if(*p=='\"') {
					parserState=PARSERSTATE_QUOTED_STRING;
				} else if(*p==',') {
					p--;
					parserState=PARSERSTATE_COMMA;
				} else {
					strcat(text,(char[2]){*p,'\0'});
					parserState=PARSERSTATE_STRING;
				}
			break;
			case PARSERSTATE_QUOTED_STRING: 
				while(*p && *p!='\"') {
					if(*p=='\\') {
						p++;
						switch(*p) {
							case 'a': strcat(text,"\a"); break;
							case 'b': strcat(text,"\b"); break;
							case 'f': strcat(text,"\f"); break;
							case 'n': strcat(text,"\n"); break;
							case 'r': strcat(text,"\r"); break;
							case 't': strcat(text,"\t"); break;
							case 'v': strcat(text,"\v"); break;
							case '\'': strcat(text,"\'"); break;
							case '\"': strcat(text,"\""); break;
							case '\\': strcat(text,"\\"); break;
							default: strcat(text,(char[2]){*p,'\0'});
						}
					} else strcat(text,(char[2]){*p,'\0'});
					if(*p) p++; 
				}

				if(*p) {
					if(*p=='\"') {
						trim(text);
						CSV_AddLine(lines,nlines,text);
						text[0]='\0';
						parserState=PARSERSTATE_COMMA;
					} else {
						p--;
						parserState=PARSERSTATE_ERROR;
					}
				} else {
					p--;
					parserState=PARSERSTATE_ERROR;
				}
							 
				break;
			case PARSERSTATE_STRING:
				while(*p && *p!=',') {
					strcat(text,(char[2]){*p,'\0'});
					p++;
				}
				trim(text);
				CSV_AddLine(lines,nlines,text);
				text[0]='\0';
				p--;
				parserState=PARSERSTATE_COMMA;
			break;
			case PARSERSTATE_COMMA:
				while(*p && *p!=',' && isspace(*p)) {
					p++;
				}
				if(*p) {
					if(*p==',')
						parserState=PARSERSTATE_DEFAULT;
					else if(!isspace(*p))
						parserState=PARSERSTATE_ERROR;
				}
			break;
			case PARSERSTATE_ERROR:
				printf("Error parsing column %ld:\n%s\n",p-line+1,line);
				for(int i=0;i<p-line;i++) printf(" ");
				printf("^\n");
				retval=1;
				quit=true;
			break;				
			default: break;
		}
		if(*p) p++;
	}
	return retval;
}



void CSV_Print(char **lines,size_t nlines) {
	for(size_t i=0;i<nlines;i++) {
		printf("%s\n",lines[i]);
	}
	printf("\n");
}



void CSV_Free(char ***lines,size_t *nlines) {
	for(size_t i=0;i<(*nlines);i++) {
		free((*lines)[i]);
		(*lines)[i]=NULL;
	}
	free(*lines);
	(*lines)=NULL;
	(*nlines)=0;
}



#endif /* CSV_IMPLEMENTATION */



#endif /* CSV_H */


