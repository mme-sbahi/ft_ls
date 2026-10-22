#include "ft_ls.h"

void print_files(t_file *head, int flags, const char *path)
{
    (void)path;
    t_file *current;

    if (flags & OPT_L)
        print_total_blocks(head);
    current = head;
    while (current)
    {
        if (flags & OPT_L)
            print_long_format(current);
        else
        {
            ft_printf("%s     ", current->name);
        }
        current = current->next;
    }
            ft_printf("\n");
}

static void print_permissions(mode_t mode)
{
    char perms[11];

    perms[0] = S_ISDIR(mode) ? 'd' : (S_ISLNK(mode) ? 'l' : (S_ISCHR(mode) ? 'c' : (S_ISBLK(mode) ? 'b' : (S_ISFIFO(mode) ? 'p' : (S_ISSOCK(mode) ? 's' : '-')))));
    perms[1] = (mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (mode & S_IXUSR) ? 'x' : '-';
    perms[4] = (mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (mode & S_IXGRP) ? 'x' : '-';
    perms[7] = (mode & S_IROTH) ? 'r' : '-';
    perms[8] = (mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (mode & S_IXOTH) ? 'x' : '-';
    perms[10] = '\0';
    ft_printf("%s", perms);
}

void print_long_format(t_file *file)
{
    struct passwd *pwd;
    struct group *grp;
    char *time_str;
    char link_target[1024];
    ssize_t link_len;

    print_permissions(file->stat.st_mode);
    ft_printf(" %lu", file->stat.st_nlink);
    pwd = getpwuid(file->stat.st_uid);
    grp = getgrgid(file->stat.st_gid);
    ft_printf(" %s %s", pwd ? pwd->pw_name : "unknown", grp ? grp->gr_name : "unknown");
    ft_printf(" %5lu", file->stat.st_size);
    time_str = get_time_string(file->stat.st_mtime);
    ft_printf(" %s", time_str);
    free(time_str);
    ft_printf(" %s", file->name);
    if (S_ISLNK(file->stat.st_mode))
    {
        link_len = readlink(file->path, link_target, sizeof(link_target) - 1);
        if (link_len != -1)
        {
            link_target[link_len] = '\0';
            ft_printf(" -> %s", link_target);
        }
    }
    ft_printf("\n");
}

void print_total_blocks(t_file *head)
{
    t_file *current;
    unsigned long total;

    total = 0;
    current = head;
    while (current)
    {
        total += current->stat.st_blocks;
        current = current->next;
    }
    ft_printf("total %lu\n", total);
}