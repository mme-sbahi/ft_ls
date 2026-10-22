#include "ft_ls.h"

char *get_time_string(time_t mtime)
{
    char *str;
    time_t now;
    struct tm *tm_info;

    time(&now);
    tm_info = localtime(&mtime);
    str = malloc(20);
    if (!str)
        return (NULL);
    if (now - mtime > 15778800 || mtime > now)
        strftime(str, 20, "%b %d  %Y", tm_info);
    else
        strftime(str, 20, "%b %d %H:%M", tm_info);
    return (str);
}