#include "ft_ls.h"
#include <stdio.h>
#include <stdarg.h>

int ft_printf(const char *format, ...)
{
    va_list args;
    int ret;

    va_start(args, format);
    ret = vprintf(format, args);
    va_end(args);
    return (ret);
}