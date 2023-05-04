#include "../inc/uls.h"

void print_str_with_spaces(char *str, int count_of_spaces) {

  mx_printstr(str);

  for (int i = 0; i < count_of_spaces; i++)
    mx_printchar(' ');
}

void print_str_with_spaces_from_right(char *str, int count_of_spaces) {

  for (int i = 0; i < count_of_spaces; i++)
    mx_printchar(' ');

  mx_printstr(str);
}

int find_last_slash(const char *str) {
  int index = -1;
  for (int i = 0; i < mx_strlen(str); i++)
    if (str[i] == '/' && i > index)
      index = i;

  return index;
}

void print_filetype(mode_t perms) {
  switch (perms & S_IFMT) {
  case S_IFDIR:
    mx_printchar('d');
    break;
  case S_IFLNK:
    mx_printchar('l');
    break;
  case S_IFIFO:
    mx_printchar('p');
    break;
  case S_IFSOCK:
    mx_printchar('s');
    break;
  case S_IFBLK:
    mx_printchar('b');
    break;
  case S_IFCHR:
    mx_printchar('c');
    break;
  case S_IFREG:
    mx_printchar('-');
    break;
  default:
    mx_printchar('?');
    break;
  }
}

void print_based_permissions(mode_t mode) {
  char perms[] = "---------";
  perms[0] = (mode & S_IRUSR) ? 'r' : '-';
  perms[1] = (mode & S_IWUSR) ? 'w' : '-';
  if (mode & S_IXUSR) {
    if (mode & S_ISUID)
      perms[2] = 's';
    else
      perms[2] = 'x';
  } else if (mode & S_ISUID)
    perms[2] = 'S';
  perms[3] = (mode & S_IRGRP) ? 'r' : '-';
  perms[4] = (mode & S_IWGRP) ? 'w' : '-';
  if (mode & S_IXGRP) {
    if (mode & S_ISGID)
      perms[5] = 's';
    else
      perms[5] = 'x';
  } else if (mode & S_ISGID)
    perms[5] = 'S';
  perms[6] = (mode & S_IROTH) ? 'r' : '-';
  perms[7] = (mode & S_IWOTH) ? 'w' : '-';
  if (mode & S_IXOTH) {
    if (mode & S_ISVTX)
      perms[8] = 't';
    else
      perms[8] = 'x';
  } else if (mode & S_ISVTX)
    perms[8] = 'T';

  mx_printstr(perms);
}

void print_acl_permission(const char *path) {
  char *print_str = NULL;
  char add_perm = ' ';
  char attribute_name[10000];
  ssize_t buffer_len = listxattr(path, attribute_name, 10000,
  XATTR_NOFOLLOW); acl_t acl = acl_get_file(path, ACL_TYPE_EXTENDED);

  if (buffer_len > 0)
    add_perm = '@';
  else if (acl != NULL) {
    add_perm = '+';
    acl_free(acl);
  }

  print_str = mx_strjoin(&add_perm, " ");
  mx_printstr(print_str);
}

void print_passwd(struct stat st) {
  struct passwd *pwd = getpwuid(st.st_uid);
  char *str = pwd ? pwd->pw_name : mx_itoa(st.st_uid);

  print_str_with_spaces(str, info->widht->user - mx_strlen(str));
  mx_printchar(' ');
  mx_printchar(' ');
}

void print_group(struct stat st) {
  struct group *grp = getgrgid(st.st_gid);
  char *str = grp ? grp->gr_name : mx_itoa(st.st_gid);

  print_str_with_spaces(str, info->widht->group - mx_strlen(str));
  mx_printchar(' ');
  mx_printchar(' ');
}

void print_size(struct stat st, int size_width) {
  if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode)) {
    char *hex = mx_nbr_to_hex(st.st_rdev);
    char *hex_str = NULL;
    if (st.st_rdev == 0)
      hex_str = mx_strdup("0");
    else
      hex_str = mx_strjoin("0x", hex);
    print_str_with_spaces_from_right(hex_str, size_width - mx_strlen(hex_str));
    // print_aligned_str(hex_str, size_width, true);
    mx_strdel(&hex_str);
    mx_strdel(&hex);
  } else {
    char *size_str = mx_ltoa(st.st_size);
    print_str_with_spaces_from_right(size_str,
                                     size_width - mx_strlen(size_str));
    // print_aligned_str(size_str, size_width, true);
    mx_strdel(&size_str);
  }
  mx_printchar(' ');
}

void print_time(struct stat st) {
  time_t current_time = time(NULL);
  time_t file_time = st.st_mtime;
  char *month = NULL;
  char *day = NULL;
  char *time = NULL;

  char *time_str = ctime(&file_time);
  char **date = mx_strsplit(time_str, ' ');
  month = date[1];
  day = date[2];

  print_str_with_spaces(month, 1);
  print_str_with_spaces_from_right(day, 2 - mx_strlen(day));
  mx_printchar(' ');

  if ((current_time - file_time) > 15552000 ||
      ((current_time - file_time) < 0 &&
       (current_time - file_time) > -15552000)) {
    mx_printchar(' ');
    time = mx_strndup(date[4], 4);
  } else
    time = mx_strndup(date[3], 5);
  print_str_with_spaces(time, 1);

  mx_strdel(&time);
  mx_del_strarr(&date);
}

void print_file_with_l_flag(struct stat st, const char *file_name, const char *dir) {
  print_filetype(st.st_mode);
  print_based_permissions(st.st_mode);
  mx_printchar(' ');
  print_acl_permission(dir);
  char *links_str = mx_ltoa(st.st_nlink);
  print_str_with_spaces_from_right(links_str,
                                   info->widht->links - mx_strlen(links_str));
  mx_printchar(' ');
  print_passwd(st);
  print_group(st);
  print_size(st, info->widht->size);
  print_time(st);
  mx_printstr(file_name);
  mx_printchar('\n');

  mx_strdel(&links_str);
}

void freeing_memory() {
  for (int i = 0; i < info->file_count; i++)
    mx_strdel(&info->list_of_filenames[i]);

  mx_strdel(info->list_of_filenames);
  info->file_count = 0;
  info->widht->group = 0;
  info->widht->links = 0;
  info->widht->size = 0;
  info->widht->user = 0;
}

void ls_dir_flag_l(const char *dir) {

  struct dirent *d;
  DIR *dh = opendir(dir);
  int file_count = 0;
  int i = 0;

  if (!dh) {
    if (errno == ENOENT) {
      // If the directory is not found
      perror("Directory doesn't exist");
    } else {
      // If the directory is not readable then throw error and exit
      perror("Unable to read directory");
    }
    return;
  }

  while ((d = readdir(dh)) != NULL) {
    // If hidden files are found we continue
    if (d->d_name[0] == '.')
      continue;

    // if (mx_strcmp(d->d_name, "obj") == 0 || mx_strcmp(d->d_name, "src") == 0
    // ||
    //     mx_strcmp(d->d_name, "Makefile") == 0 ||
    //     mx_strcmp(d->d_name, "libmx") == 0 ||
    //     mx_strcmp(d->d_name, "uls") == 0 || mx_strcmp(d->d_name, "inc") == 0)
    //   continue;

    file_count++;
  }

  info->file_count = file_count;

  info->list_of_filenames = (char **)malloc(sizeof(char *) * file_count);

  dh = opendir(dir);

  while ((d = readdir(dh)) != NULL) {
    if (d->d_name[0] == '.')
      continue;

    // if (mx_strcmp(d->d_name, "obj") == 0 || mx_strcmp(d->d_name, "src") == 0
    // ||
    //     mx_strcmp(d->d_name, "Makefile") == 0 ||
    //     mx_strcmp(d->d_name, "libmx") == 0 ||
    //     mx_strcmp(d->d_name, "uls") == 0 || mx_strcmp(d->d_name, "inc") == 0)
    //   continue;

    info->list_of_filenames[i] = mx_strdup(d->d_name);
    i++;
  }

  // mx_bubble_sort(info->list_of_filenames, file_count);
  mx_lower_bubble_sort(info->list_of_filenames, file_count);

  struct stat st;
  char *path = NULL;
  blkcnt_t blocks_sum = set_columns_width(dir) / 2;
  char *blocks_sum_str = mx_ltoa(blocks_sum);

  mx_printstr("total ");
  mx_printstr(blocks_sum_str);
  mx_printchar('\n');

  for (int i = 0; i < file_count; i++) {

    path =
        mx_strnew(mx_strlen(dir) + mx_strlen(info->list_of_filenames[i]) + 1);
    mx_strcpy(path, dir);
    mx_strcat(path, "/");
    mx_strcat(path, info->list_of_filenames[i]);
    lstat(path, &st);

    // print_filetype(st.st_mode);
    // print_based_permissions(st.st_mode);
    // mx_printchar(' ');
    // // print_acl_permission(dir);
    // char *links_str = mx_ltoa(st.st_nlink);
    // print_str_with_spaces_from_right(links_str,
    //                                  info->widht->links -
    //                                  mx_strlen(links_str));
    // mx_printchar(' ');
    // print_passwd(st);
    // print_group(st);
    // print_size(st, info->widht->size);
    // print_time(st);
    // mx_printstr(info->list_of_filenames[i]);

    // mx_printchar('\n');

    print_file_with_l_flag(st, info->list_of_filenames[i], dir);

    mx_strdel(&path);
  }

  closedir(dh);
  free(d);
  mx_strdel(&blocks_sum_str);
  freeing_memory();
}

void ls_file_flag_l(const char *dir) {
  // int index_of_last_slash = find_last_slash(dir) + 1;
  // char *path = mx_strndup(dir, index_of_last_slash);
  // char *file_name = mx_strdup(&dir[index_of_last_slash]);

  struct stat st;
  lstat(dir, &st);
  set_columns_width(dir);

  print_file_with_l_flag(st, dir, dir);

  // mx_strdel(&path);
  // mx_strdel(&file_name);
}
