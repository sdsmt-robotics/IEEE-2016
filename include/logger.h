#ifndef LOGGER_H
#define LOGGER_H

//logger.h
#define LOG(...) log_print(__FILE__, __LINE__, __VA_ARGS__ )


char* print_time();
void log_print(char* filename, int line, char *fmt,...);

#endif
