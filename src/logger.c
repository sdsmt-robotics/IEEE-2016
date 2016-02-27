//logger.c
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "../include/logger.h"
 
FILE *fp ;
static int SESSION_TRACKER; //Keeps track of session
 
char* print_time()
{
    time_t t;
    char *buf;
     
    time(&t);
    buf = (char*) malloc( strlen( ctime(&t) ) + 1 );
     
    snprintf(buf, strlen(ctime(&t)), "%s ", ctime(&t));
    
    return buf;
}

void log_print(char* filename, int line, char *fmt,...)
{
    va_list list;
    char *p, *r;
    int e;
    float f;
 
    if(SESSION_TRACKER > 0)
      fp = fopen ("log.txt","a+");
    else
      fp = fopen ("log.txt","w");
     
    fprintf(fp,"%s ",print_time());
    va_start( list, fmt );
 
    for ( p = fmt ; *p ; ++p )
    {
        if ( *p != '%' )//If simple string
        {
            fputc( *p,fp );
            printf( "%c", *p );
        }
        else
        {
            switch ( *++p )
            {
                /* string */
            case 's':
            {
                r = va_arg( list, char * );
 
                fprintf(fp,"%s", r);
                printf( "%s", r );
                continue;
            }
 
            /* integer */
            case 'd':
            {
                e = va_arg( list, int );
 
                fprintf(fp,"%d", e);
                printf( "%d", e );
                continue;
            }
            /* float */
            case 'f':
            {
                f = va_arg( list, double );
                fprintf(fp, "%f", f);
                printf( "%f", f );
                continue;
            }
 
            default:
                fprintf( fp, "\n\tLogger doesn't support the \%%c format specifier yet.", *p);
                printf("\n\t\tLogger doesn't support the \%%c format specifier yet", *p);
                //fputc( *p, fp );
                //printf( "%c", *p );
            }
        }
    }
    va_end( list );
    fprintf(fp, "\t[%s][line: %d] ", filename, line);
    fputc( '\n', fp );
    SESSION_TRACKER++;
    fclose(fp);
}