#ifndef _LOG_H
#define _LOG_H

#define LOG_FILE "test_dll.log"

FILE * logFile;

void GetLogFilePath(char * path, const int len);

void Log(const char * text);

void LogError(const int id);

#endif /* _LOG_H */