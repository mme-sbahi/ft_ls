#include "ft_ls.h"

static void process_path(t_options *opts, const char *path)
{
    t_file *files;

    files = read_directory(path, opts->flags);
    if (!files)
        return;
    sort_files(&files, opts->flags);
    if (opts->path_count > 1)
        ft_printf("%s:\n", path);
    print_files(files, opts->flags, path);
    if (opts->flags & OPT_R)
        print_recursive(files, opts->flags, path);
    free_file_list(files);
}

int main(int argc, char **argv)
{
    t_options *opts;
    int i;

    opts = parse_args(argc, argv);
    if (!opts)
        return (1);
    if (opts->path_count == 0)
        process_path(opts, ".");
    else
    {
        i = 0;
        while (i < opts->path_count)
        {
            process_path(opts, opts->paths[i]);
            if (i < opts->path_count - 1)
                ft_printf("\n");
            i++;
        }
    }
    free(opts->paths);
    free(opts);
    return (0);
}