#include "ft_ls.h"
#include <string.h>

static int compare_name(t_file *a, t_file *b)
{
    return (strcmp(a->name, b->name));
}

static int compare_time(t_file *a, t_file *b)
{
    if (a->stat.st_mtime > b->stat.st_mtime)
        return (-1);
    else if (a->stat.st_mtime < b->stat.st_mtime)
        return (1);
#if defined(__APPLE__) || defined(__MACH__)
    if (a->stat.st_mtimespec.tv_nsec > b->stat.st_mtimespec.tv_nsec)
        return (-1);
    else if (a->stat.st_mtimespec.tv_nsec < b->stat.st_mtimespec.tv_nsec)
        return (1);
#endif
    return (compare_name(a, b));
}

static int compare_access_time(t_file *a, t_file *b)
{
    if (a->stat.st_atime > b->stat.st_atime)
        return (-1);
    else if (a->stat.st_atime < b->stat.st_atime)
        return (1);
#if defined(__APPLE__) || defined(__MACH__)
    if (a->stat.st_atimespec.tv_nsec > b->stat.st_atimespec.tv_nsec)
        return (-1);
    else if (a->stat.st_atimespec.tv_nsec < b->stat.st_atimespec.tv_nsec)
        return (1);
#endif
    return (compare_name(a, b));
}

int compare_files(t_file *a, t_file *b, int flags)
{
    int result;

    if (flags & OPT_T)
    {
        if (flags & OPT_U)
            result = compare_access_time(a, b);
        else
            result = compare_time(a, b);
    }
    else
        result = compare_name(a, b);
    if (flags & OPT_R_REV)
        return (-result);
    return (result);
}

static t_file *merge_sorted(t_file *a, t_file *b, int flags)
{
    t_file *result;

    if (!a)
        return (b);
    if (!b)
        return (a);
    if (compare_files(a, b, flags) <= 0)
    {
        result = a;
        result->next = merge_sorted(a->next, b, flags);
    }
    else
    {
        result = b;
        result->next = merge_sorted(a, b->next, flags);
    }
    return (result);
}

static void split_list(t_file *head, t_file **front, t_file **back)
{
    t_file *slow;
    t_file *fast;

    if (!head || !head->next)
    {
        *front = head;
        *back = NULL;
        return;
    }
    slow = head;
    fast = head->next;
    while (fast)
    {
        fast = fast->next;
        if (fast)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *front = head;
    *back = slow->next;
    slow->next = NULL;
}

static void merge_sort(t_file **head, int flags)
{
    t_file *a;
    t_file *b;

    if (!*head || !(*head)->next)
        return;
    if (flags & OPT_F)
        return;
    split_list(*head, &a, &b);
    merge_sort(&a, flags);
    merge_sort(&b, flags);
    *head = merge_sorted(a, b, flags);
}

void sort_files(t_file **head, int flags)
{
    if (flags & OPT_F)
        return;
    merge_sort(head, flags);
}