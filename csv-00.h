#ifndef CSV_H
#define CSV_H



#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define STRING_MAX 1024



typedef enum ParserState {
  PARSERSTATE_DEFAULT=0,
  PARSERSTATE_STRING,
  PARSERSTATE_MAX  
} ParserState;



void CSV_AddLine(char ***lines,size_t *nlines,char *line);

void CSV_Parse(char ***lines,size_t *nlines,char *line);

void CSV_Print(char **lines,size_t nlines);

void CSV_Free(char ***lines,size_t *nlines);



#ifdef CSV_IMPLEMENTATION



void CSV_AddLine(char ***lines,size_t *nlines,char *line) {
  (*lines)=realloc(*lines,sizeof(**lines)*(*nlines+1));  
  (*lines)[(*nlines)++]=strdup(line);
}
 


void CSV_Parse(char ***lines,size_t *nlines,char *line) {
  ParserState parserState=PARSERSTATE_DEFAULT;
  char *p=line;
  char text[STRING_MAX];
  text[0]='\0';
  while(*p) {
    switch(parserState) {
      case PARSERSTATE_DEFAULT: 
        if(*p=='\"') {
          parserState=PARSERSTATE_STRING;
        } 
      break;
      case PARSERSTATE_STRING: 
        while(*p!='\"') {
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
          p++;
        }
        CSV_AddLine(lines,nlines,text);
        text[0]='\0';
        parserState=PARSERSTATE_DEFAULT;
      break;
      default: break;
    }
    p++;
  }
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


