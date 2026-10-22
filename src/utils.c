#include "ft_ls.h"

void handle_error(const char *path)
{
    ft_printf("ft_ls: %s: %s\n", path, strerror(errno));
}

char *ft_strdup(const char *s)
{
    char *dup;
    size_t len;

    len = ft_strlen(s) + 1;
    dup = malloc(len);
    if (!dup)
        return (NULL);
    ft_memcpy(dup, s, len);
    return (dup);
}