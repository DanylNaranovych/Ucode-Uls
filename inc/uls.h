#pragma once

#include "../libmx/inc/libmx.h"
#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
// #include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/acl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <unistd.h>
#include <time.h>

#define TAB_SIZE 8

typedef enum e_file_type {
  TYPE_FILE,
  TYPE_DIR,
  NOT_EXIST,
} t_file_type;

typedef struct s_col_width_for_l {
  int links;
  int user;
  int group;
  int size;
} t_col_width_for_l;

typedef struct s_info {
  char **list_of_filenames;
  int file_count;
  int win_col_count;
  bool is_terminal;
  bool is_flag_l;
  t_col_width_for_l *widht;
} t_info;

extern t_info *info;

void ls(const char *dir);
void ls_dir_flag_l(const char *dir);
void ls_file_flag_l(const char *dir);

int max_length_of_filenames();
blkcnt_t set_columns_width(const char *dir);
int is_dir(const char *name);
void freeing_memory();
void set_bigger(int *dest, int src);
void print_child_folder();
// void print_size(struct stat st, int size_width);
void print_str_with_spaces(char *str, int count_of_spaces);
void print_str_with_spaces_from_right(char *str, int count_of_spaces);
