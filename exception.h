#ifndef CEASYEXCEPTION_EXCEPTION_H
#define CEASYEXCEPTION_EXCEPTION_H

#include <stdio.h>
#include <string.h>

/*!
 * Max size of exception message
 */
#define EXCEPTION_MSG_LEN 512

/*!
 * @struct exception
 * @brief Type that defines exception
 *
 * @var int error
 * @brief Thrown error code, may contain an errno value or a user defined error code
 *
 * @var char msg[EXCEPTION_MSG_LEN]
 * @brief buffer that contains exception what() message
 */
typedef struct
{
    int error;
    char msg[EXCEPTION_MSG_LEN];
} exception;

extern exception current_exception;

/*!
 * Sets current exception to desired state
 * It's user's responsibility to return (or stay) from current method after throwing the exception
 * @param err Error code
 */
#define EXCEPTION_THROW_NOMSG(err)                                          \
current_exception.error = (err);                                            \
current_exception.msg[0] = '\0';                                            \
strerror_r(err, current_exception.msg, EXCEPTION_MSG_LEN)

/*!
 * Sets current exception to desired state
 * It's user's responsibility to return (or stay) from current method after throwing the exception
 * @param err Error code
 * @param format message Format, uses the same notation as printf
 * @param ... Parameters that were specified in format string
 */
#define EXCEPTION_THROW(err, format, ...)                                       \
EXCEPTION_THROW_NOMSG(err);                                                     \
{                                                                               \
    char* ptr = current_exception.msg;                                          \
    size_t len = EXCEPTION_MSG_LEN;                                             \
    size_t size = strlen(ptr);                                                  \
    if(size > 0)                                                                \
    {                                                                           \
        ptr += size;                                                            \
        len -= size;                                                            \
        size = snprintf(ptr, len, " : ");                                       \
        ptr += size;                                                            \
        len -= size;                                                            \
    }                                                                           \
    snprintf(ptr, len, (format), __VA_ARGS__);                                  \
}((void)(0))

/*!
 * Check if exception has been thrown
 */
#define EXCEPTION_IS_THROWN (current_exception.error != 0)

/*!
 * Clear last thrown exception
 */
#define EXCEPTION_CLEAR() (current_exception.error = 0)

/*!
 * Get error code of last thrown exception
 */
#define EXCEPTION_ERROR (const int)(current_exception.error)

/*!
 * Get message of last thrown exception
 */
#define EXCEPTION_MSG (const char*)(current_exception.msg)

#endif //CEASYEXCEPTION_EXCEPTION_H
