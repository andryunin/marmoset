#ifndef __MARMOSET_C__
#define __MARMOSET_C__

/* Marmoset test tool. Details: https://github.com/andryunin/marmoset */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#define _M_COMMENT_SIZE 4096
#define _M_INDENT_SIZE  4

enum m_state {
    M_SUCCESS, M_FAILURE, M_PENDING
};

unsigned int _m_success_count = 0;
unsigned int _m_failure_count = 0;
unsigned int _m_pending_count = 0;


struct _m_nesting_level {
    struct _m_nesting_level *prev;
    char *comment;
} *_m_nesting_head = NULL;

unsigned int _m_nesting_count = 0;


#define _M_RED    31
#define _M_GREEN  32
#define _M_YELLOW 33
#define _M_BLUE   34


#define _m_msg(color, format, ...)                    \
    do {                                              \
        printf("\033[%dm", color);  /* set color */   \
        printf(format, ##__VA_ARGS__);                \
        printf("\033[39m\033[49m"); /* reset color */ \
    } while(0)


void _m_msg_indent()
{
    int i;

    for (i = 0; i < _M_INDENT_SIZE * _m_nesting_count; ++i)
        putchar(' ');
}


#define _m_report_success(value, assert_mark, comment, ...) \
    do {                                                    \
        ++_m_success_count;                                 \
        _m_msg_indent();                                    \
        _m_msg(_M_GREEN, "- success: ");                    \
        _m_msg(_M_GREEN, comment, ##__VA_ARGS__);           \
        _m_msg(_M_GREEN, "\n");                             \
    } while(0)


#define _m_report_failure(value, assert_mark, comment, ...)          \
    do {                                                             \
        ++_m_failure_count;                                          \
        _m_msg_indent();                                             \
        _m_msg(_M_RED, "- failure: ");                               \
        _m_msg(_M_RED, comment, ##__VA_ARGS__);                      \
        _m_msg(_M_RED, "\n");                                        \
        _m_msg_indent();                                             \
        _m_msg(_M_RED, "     code: (%s) %s", #value, assert_mark);   \
        _m_msg(_M_RED, "\n");                                        \
    } while(0)


#define _m_report_pending(comment, ...)     \
    do {                                           \
        ++_m_pending_count;                        \
        _m_msg_indent();                           \
        _m_msg(_M_YELLOW, "- pending: ");          \
        _m_msg(_M_YELLOW, comment, ##__VA_ARGS__); \
        _m_msg(_M_YELLOW, "\n");                   \
    } while(0)


void* _m_xalloc(size_t size)
{
    void *memory = malloc(size);
    if (memory == NULL) {
        _m_msg(_M_RED, "out of memory, aborting...");
        abort();
    }
    return memory;
}


void _m_nesting_push(const char *comment, ...)
{
    struct _m_nesting_level *l;
    va_list ap;

    l          = _m_xalloc(sizeof(struct _m_nesting_level));
    l->comment = _m_xalloc(sizeof(char) * _M_COMMENT_SIZE);
    l->prev    = _m_nesting_head;

    _m_nesting_head = l;

    va_start(ap, comment);
    vsnprintf(l->comment, _M_COMMENT_SIZE, comment, ap);
    va_end(ap);
    
    ++_m_nesting_count;
}


void _m_nesting_pop(void)
{
    struct _m_nesting_level *l;

    if (_m_nesting_head == NULL)
        return;

    l = _m_nesting_head;
    _m_nesting_head = l->prev;
    
    free(l->comment);
    free(l);
    
    --_m_nesting_count;
}


#define suite(suite_name)                  \
    int main(int argc, char *argv[]) {     \
        printf("start: %s\n", suite_name);


#define endsuite                                       \
        if (_m_success_count > 0)                      \
            printf("Succeed: %d\n", _m_success_count); \
        if (_m_failure_count > 0)                      \
            printf("Failed: %d\n", _m_failure_count);  \
        if (_m_pending_count > 0)                      \
            printf("Pending: %d\n", _m_pending_count); \
        return _m_failure_count;                       \
    }


#define context(comment, ...)                    \
    do {                                         \
        _m_msg_indent();                         \
        _m_msg(_M_BLUE, "- context: ");          \
        _m_msg(_M_BLUE, comment, ##__VA_ARGS__); \
        _m_msg(_M_BLUE, "\n");                   \
        _m_nesting_push(comment, ##__VA_ARGS__); \
    } while(0);


#define endcontext   \
    _m_nesting_pop();


#define assert0(value, comment, ...)                                  \
    do {                                                              \
        if ((value) == 0) {                                           \
            _m_report_success(value, "== 0", comment, ##__VA_ARGS__); \
        } else {                                                      \
            _m_report_failure(value, "== 0", comment, ##__VA_ARGS__); \
        }                                                             \
    } while(0)


#define assertN0(value, comment, ...)                                 \
    do {                                                              \
        if ((value) != 0) {                                           \
            _m_report_success(value, "!= 0", comment, ##__VA_ARGS__); \
        } else {                                                      \
            _m_report_failure(value, "!= 0", comment, ##__VA_ARGS__); \
        }                                                             \
    } while(0)


#define pending(comment, ...)                      \
    do {                                           \
        _m_report_pending(comment, ##__VA_ARGS__); \
    } while(0)


#endif