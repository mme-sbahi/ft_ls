#include "ft_ls.h"
#include <pwd.h>
#include <grp.h>

static struct {
    uid_t uid;
    char *name;
} user_cache[128];

static struct {
    gid_t gid;
    char *name;
} group_cache[128];

void handle_error(const char *path)
{
    ft_printf("ft_ls: %s: %s\n", path, strerror(errno));
}

char *get_cached_user(uid_t uid)
{
    for (int i = 0; i < 128; i++)
    {
        if (user_cache[i].name && user_cache[i].uid == uid)
            return user_cache[i].name;
    }
    struct passwd *pwd = getpwuid(uid);
    char *name = pwd ? ft_strdup(pwd->pw_name) : ft_strdup("unknown");
    for (int i = 0; i < 128; i++)
    {
        if (!user_cache[i].name)
        {
            user_cache[i].uid = uid;
            user_cache[i].name = name;
            break;
        }
    }
    return name;
}

char *get_cached_group(gid_t gid)
{
    for (int i = 0; i < 128; i++)
    {
        if (group_cache[i].name && group_cache[i].gid == gid)
            return group_cache[i].name;
    }
    struct group *grp = getgrgid(gid);
    char *name = grp ? ft_strdup(grp->gr_name) : ft_strdup("unknown");
    for (int i = 0; i < 128; i++)
    {
        if (!group_cache[i].name)
        {
            group_cache[i].gid = gid;
            group_cache[i].name = name;
            break;
        }
    }
    return name;
}

void free_cache(void)
{
    for (int i = 0; i < 128; i++)
    {
        free(user_cache[i].name);
        user_cache[i].name = NULL;
        free(group_cache[i].name);
        group_cache[i].name = NULL;
    }
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