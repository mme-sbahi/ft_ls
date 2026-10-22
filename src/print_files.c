#include "ft_ls.h"
#include <sys/ioctl.h>
#include <sys/xattr.h>
#include <sys/acl.h>

#if !defined(ACL_TYPE_EXTENDED)
# define ACL_TYPE_EXTENDED 0
#endif

#if !defined(XATTR_NOFOLLOW)
# define XATTR_NOFOLLOW 0
#endif

void print_files(t_file *head, int flags, const char *path)
{
    (void)path;
    t_file *current;

    if (flags & OPT_L)
    {
        if (!(flags & OPT_D))
            print_total_blocks(head);
        current = head;
        while (current)
        {
            print_long_format(current, flags);
            if (flags & OPT_ACL)
                print_acl(current);
            current = current->next;
        }
    }
    else
    {
        print_files_columns(head, flags);
    }
}

int get_terminal_width(void)
{
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0)
        return ws.ws_col;
    return 80;
}

static int count_files(t_file *head)
{
    int count = 0;
    while (head)
    {
        count++;
        head = head->next;
    }
    return count;
}

static int get_max_name_len(t_file *head)
{
    int max = 0;
    int len;
    while (head)
    {
        len = ft_strlen(head->name);
        if (len > max)
            max = len;
        head = head->next;
    }
    return max;
}

void print_files_columns(t_file *head, int flags)
{
    int term_width = get_terminal_width();
    int file_count = count_files(head);
    int max_name_len = get_max_name_len(head);
    int col_width = max_name_len + 2;
    int cols = term_width / col_width;
    if (cols == 0)
        cols = 1;
    int rows = (file_count + cols - 1) / cols;

    t_file **files = malloc(sizeof(t_file *) * file_count);
    if (!files)
        return;
    t_file *current = head;
    int i = 0;
    while (current)
    {
        files[i++] = current;
        current = current->next;
    }

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            int idx = row * cols + col;
            if (idx < file_count)
            {
                char *color = "";
                char *reset = "";
                if (flags & OPT_COLOR)
                {
                    color = get_color(files[idx]->stat.st_mode);
                    reset = "\033[0m";
                }
                ft_printf("%s%-*s%s", color, col_width, files[idx]->name, reset);
            }
        }
        ft_printf("\n");
    }
    free(files);
}

char *get_color(mode_t mode)
{
    if (S_ISDIR(mode))
        return "\033[1;36m";
    if (S_ISLNK(mode))
        return "\033[36m";
    if (S_ISSOCK(mode))
        return "\033[1;35m";
    if (S_ISFIFO(mode))
        return "\033[33m";
    if (S_ISBLK(mode))
        return "\033[1;33m";
    if (S_ISCHR(mode))
        return "\033[1;33m";
    if (mode & (S_IXUSR | S_IXGRP | S_IXOTH))
        return "\033[31m";
    return "\033[0m";
}

void reset_color(void)
{
    ft_printf("\033[0m");
}

void print_acl(t_file *file)
{
    acl_t acl = acl_get_file(file->path, ACL_TYPE_EXTENDED);
    if (acl)
    {
        char *acl_text = acl_to_text(acl, NULL);
        if (acl_text)
        {
            ft_printf(" %s", acl_text);
            acl_free(acl_text);
        }
        acl_free(acl);
    }
    ssize_t xattr_size = listxattr(file->path, NULL, 0, XATTR_NOFOLLOW);
    if (xattr_size > 0)
    {
        char *xattr_list = malloc(xattr_size + 1);
        if (xattr_list)
        {
            listxattr(file->path, xattr_list, xattr_size, XATTR_NOFOLLOW);
            xattr_list[xattr_size] = '\0';
            ft_printf(" %s", xattr_list);
            free(xattr_list);
        }
    }
}

static int has_xattr(const char *path)
{
    ssize_t size = listxattr(path, NULL, 0, XATTR_NOFOLLOW);
    return (size > 0);
}

static void print_permissions(mode_t mode, const char *path)
{
    char perms[12];

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
    perms[10] = has_xattr(path) ? '@' : ' ';
    perms[11] = '\0';
    ft_printf("%s", perms);
}

void print_long_format(t_file *file, int flags)
{
    char *time_str;
    char link_target[1024];
    ssize_t link_len;
    char *color = "";
    char *reset = "";

    if (flags & OPT_COLOR)
    {
        color = get_color(file->stat.st_mode);
        reset = "\033[0m";
    }
    ft_printf("%s", color);
    print_permissions(file->stat.st_mode, file->path);
    ft_printf(" %lu", file->stat.st_nlink);
    if (!(flags & OPT_G))
    {
        ft_printf(" %s", get_cached_user(file->stat.st_uid));
    }
    ft_printf(" %s", get_cached_group(file->stat.st_gid));
    ft_printf(" %5lu", file->stat.st_size);
    if (flags & OPT_U)
        time_str = get_time_string(file->stat.st_atime);
    else
        time_str = get_time_string(file->stat.st_mtime);
    ft_printf(" %s", time_str);
    free(time_str);
    ft_printf(" %s%s", file->name, reset);
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