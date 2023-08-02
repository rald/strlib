#ifndef ESCAPE_H
#define ESCAPE_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>



char *escape(char *s);
char *unescape(char *s);



#ifdef ESCAPE_IMPLEMENTATION



char *escape(char *s) {
	char *r=malloc(sizeof(*r)*strlen(s)*2);
	char *p=s;
	r[0]='\0';
	while(*p) {
		switch(*p) {
			case '\"': strcat(r,"\\\""); break;
			case '\'': strcat(r,"\\\'"); break;
			case '\a': strcat(r,"\\a"); break;
			case '\b': strcat(r,"\\b"); break;
			case '\f': strcat(r,"\\f"); break;
			case '\n': strcat(r,"\\n"); break;
			case '\r': strcat(r,"\\r"); break;
			case '\t': strcat(r,"\\t"); break;
			case '\v': strcat(r,"\\v"); break;
			case '\\': strcat(r,"\\\\"); break;
			default: strcat(r,(char[2]){*p,'\0'});
		}
		p++;
	}
	return r;
}



char *unescape(char *s) {
	char *r=malloc(sizeof(*r)*strlen(s));
	char *p=s;
	r[0]='\0';
	while(*p) {
		if(*p=='\\') {
			p++;
			switch(*p) {
				case '\"': strcat(r,"\""); break;
				case '\'': strcat(r,"\'"); break;
				case 'a': strcat(r,"\a"); break;
				case 'b': strcat(r,"\b"); break;
				case 'f': strcat(r,"\f"); break;
				case 'n': strcat(r,"\n"); break;
				case 'r': strcat(r,"\r"); break;
				case 't': strcat(r,"\t"); break;
				case 'v': strcat(r,"\v"); break;
				case '\\': strcat(r,"\\"); break;
				default: strcat(r,(char[2]){*p,'\0'});
			}
		} else strcat(r,(char[2]){*p,'\0'});
		p++;
	}
	return r;
}



#endif /* ESCAPE_IMPLEMENTATION */



#endif /* ESCAPE_H */


