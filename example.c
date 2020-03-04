#include "exception.h"
#include <errno.h>
#include <zconf.h>

int factorial(int num)
{
    if(num < 0)
    {
        //Set exception
        EXCEPTION_THROW(EINVAL, "%s", "Number must be larger than zero");
        //Return whatever
        return -1;
    }
    if(num == 0)
    {
        return 1;
    }
    int result = 1;
    for(int i = 2; i <= num; ++i)
    {
        //Check for overflow
        if(INT_MAX / i < result)
        {
            //Set exception, assuming that error code will be enough to understand the cause
            EXCEPTION_THROW_NOMSG(EOVERFLOW);
            //Return whatever
            return -1;
        }
        result *= i;
    }
    return result;
}

int main()
{
    int result;
    //Initialize exception to zero at start
    EXCEPTION_CLEAR();
    result = factorial( -1);
    //Check if exception is thrown
    if(EXCEPTION_IS_THROWN)
    {
        //Print the error : "Invalid argument : Number must be larger than zero"
        printf("%s\n", EXCEPTION_MSG);
        //Clear last thrown exception
        EXCEPTION_CLEAR();
    }
    result = factorial( 100);
    //Check if exception is thrown
    if(EXCEPTION_IS_THROWN)
    {
        //Print the error : "Value too large for defined data type"
        printf("%s\n", EXCEPTION_MSG);
        //Clear last thrown exception
        EXCEPTION_CLEAR();
    }
    result = factorial(10);
    //Check if exception is thrown (this time it won't be)
    if(EXCEPTION_IS_THROWN)
    {
        //Print the error (nothing will be printed)
        printf("%s\n", EXCEPTION_MSG);
        //Clear last thrown exception
        EXCEPTION_CLEAR();
    }
    //Print the result
    printf("factorial = %d\n", result);
    return 0;
}
