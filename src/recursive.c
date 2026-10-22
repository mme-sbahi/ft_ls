#include "ft_ls.h"
#include <string.h>

static int is_dir(t_file *file)
{
    return (S_ISDIR(file->stat.st_mode) && strcmp(file->name, ".") != 0 && strcmp(file->name, "..") != 0);
}

void print_recursive(t_file *head, int flags, const char *path)
{
    t_file *current;
    t_file *sub_head;

    (void)path;
    current = head;
    while (current)
    {
        if (is_dir(current))
        {
            ft_printf("\n%s:\n", current->path);
            sub_head = read_directory(current->path, flags);
            if (sub_head)
            {
                sort_files(&sub_head, flags);
                print_files(sub_head, flags, current->path);
                print_recursive(sub_head, flags, current->path);
                free_file_list(sub_head);
            }
        }
        current = current->next;
    }
}