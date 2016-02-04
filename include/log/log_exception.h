#ifndef _LOG_EXCEPTION_H
#define _LOG_EXCEPTION_H


#define LOG_EXCEPTION_FS "/dev/log/exception"

extern int log_to_exception(char* tag, char* msg);

#define log_exception		log_to_exception


#endif	//#ifndef _LOG_EXCEPTION_H
