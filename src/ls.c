#include "../inc/uls.h"

void ls(const char *dir) {

  struct dirent *d;
  DIR *dh = opendir(dir);
  int file_count = 0;
  // op_l++;
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
    // If hidden files are found we continue
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

  if (info->is_terminal == false)
    mx_print_strarr(info->list_of_filenames, "\n");
  else
    print_child_folder();

  closedir(dh);
  free(d);
  freeing_memory();
}

int is_dir(const char *name) {

  DIR *dh = opendir(name);

  if (!dh || errno == ENOENT)
    return TYPE_FILE;
  else if (!dh && errno == ENOENT)
    return NOT_EXIST;

  return TYPE_DIR;
}
