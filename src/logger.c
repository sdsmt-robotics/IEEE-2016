//logger.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>

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

void log_print(char* filename, int line, const char* function, char *fmt,...)
{
    va_list list;
    char *p, *r;
    int e;
    float f;
    int thing = mkdir("logs/", ACCESSPERMS);
    static char logfile[37] = "logs/";

    if (thing == 0)
    {
        printf("logs/ created successfully\n");
    }

    if ( SESSION_TRACKER == 0 )
    {
        //Get the filename the first time log_print() is called
        //logfile = "logs/";
        strcat(logfile, print_time());
        strcat(logfile, ".log");
        printf("logfile: %s\n", logfile);
    }

    if(SESSION_TRACKER > 0)
      fp = fopen (logfile,"a+");
    else
      fp = fopen (logfile,"w");

    fprintf(fp,"[%s]",print_time());
    fprintf(fp, "[function %s() in %s][line: %d]: ", function, filename, line);
    va_start( list, fmt );

    for ( p = fmt ; *p ; ++p )
    {
        if ( *p != '%' )//If simple string
        {
            fputc( *p, fp );
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
            /* float precision setting specifiers */
            case '.':
            {
                f = va_arg( list, double );
                switch ( *++p )
                {
                    case '1':
                    {
                        fprintf(fp, "%.1f", f );
                        printf( "%.1f", f );
                        p++;
                        break;
                    }
                    case '2':
                    {
                        fprintf(fp, "%.2f", f );
                        printf( "%.2f", f );
                        p++;
                        break;
                    }
                    case '3':
                    {
                        fprintf(fp, "%.3f", f );
                        printf( "%.3f", f );
                        p++;
                        break;
                    }
                    default:
                    {
                        fprintf(fp, "%f", f );
                        printf( "%f", f );
                    }
                }
                continue;
            }

            default:
                fprintf( fp, "\n\tLogger doesn't support the %%%c format specifier yet.\n", *p);
                printf("\n\t\tLogger doesn't support the %%%c format specifier yet\n", *p);
                //fputc( *p, fp );
                //printf( "%c", *p );
            }
        }
    }
    va_end( list );
    //fprintf( fp, "\n" );
    SESSION_TRACKER++;
    fclose(fp);
}
