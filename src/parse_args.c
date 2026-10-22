#include "ft_ls.h"

static int is_option(const char *arg)
{
    return (arg[0] == '-' && arg[1] != '\0');
}

static void parse_flags(const char *arg, t_options *opts)
{
    int i = 1;
    while (arg[i])
    {
        if (arg[i] == 'l')
            opts->flags |= OPT_L;
        else if (arg[i] == 'R')
            opts->flags |= OPT_R;
        else if (arg[i] == 'a')
            opts->flags |= OPT_A;
        else if (arg[i] == 'r')
            opts->flags |= OPT_R_REV;
        else if (arg[i] == 't')
            opts->flags |= OPT_T;
        else if (arg[i] == 'u')
            opts->flags |= OPT_U;
        else if (arg[i] == 'f')
        {
            opts->flags |= OPT_F;
            opts->flags |= OPT_A;
        }
        else if (arg[i] == 'g')
        {
            opts->flags |= OPT_G;
            opts->flags |= OPT_L;
        }
        else if (arg[i] == 'd')
            opts->flags |= OPT_D;
        else if (arg[i] == 'G')
            opts->flags |= OPT_COLOR;
        else if (arg[i] == '@')
            opts->flags |= OPT_ACL;
        else
        {
            ft_printf("ft_ls: illegal option -- %c\n", arg[i]);
            ft_printf("usage: ft_ls [-RalrtufgdG@] [file ...]\n");
            exit(1);
        }
        i++;
    }
}

t_options *parse_args(int argc, char **argv)
{
    t_options *opts;
    int i;
    int path_idx;

    opts = malloc(sizeof(t_options));
    if (!opts)
        return (NULL);
    opts->flags = 0;
    opts->paths = malloc(sizeof(char *) * argc);
    if (!opts->paths)
        return (free(opts), NULL);
    opts->path_count = 0;
    i = 1;
    path_idx = 0;
    while (i < argc)
    {
        if (is_option(argv[i]))
            parse_flags(argv[i], opts);
        else
            opts->paths[path_idx++] = argv[i];
        i++;
    }
    opts->path_count = path_idx;
    return (opts);
}