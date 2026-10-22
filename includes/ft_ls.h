#ifndef FT_LS_H
#define FT_LS_H

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "libft.h"

#define OPT_L 1
#define OPT_R 2
#define OPT_A 4
#define OPT_R_REV 8
#define OPT_T 16

typedef struct s_file {
    char            *name; // Filename only (e.g., "Makefile")
    char            *path; // Full path (e.g., "./Makefile")
    struct stat     stat;  // All file metadata from lstat()
    struct s_file   *next; // Linked list pointer
} t_file;


typedef struct s_options {
    int flags;      // Bitmask: OPT_L=1, OPT_R=2, OPT_A=4, OPT_R_REV=8, OPT_T=16
    char **paths;   // Array of directory paths from CLI
    int path_count; // Number of paths
} t_options;

int             main(int argc, char **argv);
t_options       *parse_args(int argc, char **argv);
t_file          *read_directory(const char *path, int flags);
t_file          *create_file_node(const char *name, const char *path);
void            free_file_list(t_file *head);
void            sort_files(t_file **head, int flags);
int             compare_files(t_file *a, t_file *b, int flags);
void            print_files(t_file *head, int flags, const char *path);
void            print_long_format(t_file *file);
char            *get_permissions(mode_t mode);
char            *get_time_string(time_t mtime);
void            print_recursive(t_file *head, int flags, const char *path);
int             is_hidden(const char *name);
void            handle_error(const char *path);
void            print_total_blocks(t_file *head);
int             ft_printf(const char *format, ...);

#endif