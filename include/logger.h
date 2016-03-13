#ifndef LOGGER_H
#define LOGGER_H

//logger.h
#define LOG(...) log_print(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__ )

char* print_time();
void log_print(char* filename, int line, const char* function, char *fmt,...);

#endif
