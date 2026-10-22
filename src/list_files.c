#include "ft_ls.h"

static char *build_path(const char *dir, const char *name)
{
    char *path;
    size_t len;

    len = ft_strlen(dir) + ft_strlen(name) + 2;
    path = malloc(len);
    if (!path)
        return (NULL);
    ft_strlcpy(path, dir, len);
    if (path[ft_strlen(path) - 1] != '/')
        ft_strlcat(path, "/", len);
    ft_strlcat(path, name, len);
    return (path);
}

t_file *create_file_node(const char *name, const char *path)
{
    t_file *node;
    char *full_path;

    node = malloc(sizeof(t_file));
    if (!node)
        return (NULL);
    node->name = ft_strdup(name);
    full_path = build_path(path, name);
    node->path = full_path;
    if (lstat(full_path, &node->stat) == -1)
    {
        free(node->name);
        free(node->path);
        free(node);
        return (NULL);
    }
    node->next = NULL;
    return (node);
}

int is_hidden(const char *name)
{
    return (name[0] == '.');
}

t_file *read_directory(const char *path, int flags)
{
    DIR *dir;
    struct dirent *entry;
    t_file *head;
    t_file *current;
    t_file *new_node;

    dir = opendir(path);
    if (!dir)
    {
        handle_error(path);
        return (NULL);
    }
    head = NULL;
    current = NULL;
    while ((entry = readdir(dir)) != NULL)
    {
        if (!(flags & OPT_A) && is_hidden(entry->d_name))
            continue;
        new_node = create_file_node(entry->d_name, path);
        if (!new_node)
            continue;
        if (!head)
            head = new_node;
        else
            current->next = new_node;
        current = new_node;
    }
    closedir(dir);
    return (head);
}

void free_file_list(t_file *head)
{
    t_file *tmp;

    while (head)
    {
        tmp = head;
        head = head->next;
        free(tmp->name);
        free(tmp->path);
        free(tmp);
    }
}