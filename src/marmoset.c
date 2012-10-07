#ifndef __MARMOSET_C__
#define __MARMOSET_C__

/* Marmoset test tool. Details: https://github.com/andryunin/marmoset */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


enum m_state {
    M_SUCCESS, M_FAILURE, M_PENDING
};

unsigned int _m_succeed_count = 0;
unsigned int _m_failed_count  = 0;
unsigned int _m_pending_count = 0;


void message_success(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    printf("\033[32m");         /* Set green text color */
    vprintf(format, ap);        /* Print message */
    printf("\033[39m\033[49m"); /* Reset color */
    va_end(ap);
}

void message_failure(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    printf("\033[31m");         /* Set red text color */
    vprintf(format, ap);        /* Print message */
    printf("\033[39m\033[49m"); /* Reset color */
    va_end(ap);
}

void message_pending(const char *format, ...)
{
    va_list ap;

    va_start(ap, format);
    printf("\033[33m");         /* Set yellow text color */
    vprintf(format, ap);        /* Print message */
    printf("\033[39m\033[49m"); /* Reset color */
    va_end(ap);
}


#define suite(suite_name)                  \
    int main(int argc, char *argv[]) {     \
        printf("start: %s\n", suite_name);

#define endsuite                                       \
        if (_m_succeed_count > 0)                      \
            printf("Succeed: %d\n", _m_succeed_count); \
        if (_m_failed_count > 0)                       \
            printf("Failed: %d\n", _m_failed_count);   \
        if (_m_pending_count > 0)                      \
            printf("Pending: %d\n", _m_pending_count); \
        return _m_failed_count;                        \
    }

#define assert0(value, comment, ...)                      \
    do {                                                  \
        if ((value) == 0) {                               \
            message_success("- success: ");               \
            message_success(comment, ##__VA_ARGS__);      \
            message_success("\n");                        \
            ++_m_succeed_count;                           \
        } else {                                          \
            message_failure("- failure: ");               \
            message_failure(comment, ##__VA_ARGS__);      \
            message_failure("\n     code: %s\n", #value); \
            ++_m_failed_count;                            \
        }                                                 \
    } while(0)

#define pending(comment, ...)                    \
    do {                                         \
        message_pending("- pending: ");          \
        message_pending(comment, ##__VA_ARGS__); \
        message_pending("\n");                   \
        ++_m_pending_count;                      \
    } while(0)


#endif