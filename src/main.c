#include "../inc/uls.h"

void set_bigger(int *dest, int src) {
  if (*dest < src)
    *dest = src;
}

int max_length_of_filenames() {

  int max_name_len = 0;
  int name_len;

  for (int i = 0; i < info->file_count; i++) {
    name_len = mx_strlen(info->list_of_filenames[i]);

    if (name_len > max_name_len)
      max_name_len = name_len;
  }

  return max_name_len;
}

static void print_tabs(int col_width, int name_len) {
  int difference = col_width - name_len;

  if (difference % TAB_SIZE != 0) {
    difference = difference / TAB_SIZE + 1;
  } else {
    difference = difference / TAB_SIZE;
  }

  for (int i = 0; i < difference; i++)
    mx_printchar('\t');
}

void print_child_folder() {

  int col_width = max_length_of_filenames();
  int max_name_len = (col_width % TAB_SIZE == 0)
                         ? col_width + TAB_SIZE
                         : col_width + TAB_SIZE - (col_width % TAB_SIZE);

  int line_count = (max_name_len * info->file_count / info->win_col_count);

  if ((max_name_len * info->file_count % info->win_col_count) > 0)
    line_count++;

  for (int i = 0; i < line_count; i++) {
    for (int j = i; j < info->file_count; j += line_count) {
      mx_printstr(info->list_of_filenames[j]);

      if (info->list_of_filenames[j + 1] && (j + line_count < info->file_count))
        print_tabs(max_name_len, mx_strlen(info->list_of_filenames[j]));
    }
    mx_printchar('\n');
  }
}

t_info *info;

int main(int argc, char const *argv[]) {

  info = malloc(sizeof(*info));
  info->widht = malloc(sizeof(*info->widht));
  info->widht->group = 0;
  info->widht->links = 0;
  info->widht->size = 0;
  info->widht->user = 0;
  info->is_flag_l = false;
  int file_count = 0;
  int dir_count = 0;
  int file_counter = 0;
  int dir_counter = 0;
  int file_type = -1;

  if (isatty(1) == 0)
    info->is_terminal = false;
  else
    info->is_terminal = true;

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  info->win_col_count = w.ws_col;

  if (argc == 1)
    ls(".");

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
        if (!mx_strcmp(argv[1], "-l"))
          info->is_flag_l = true;
        else {
          mx_printerr("uls: illegal option -- ");
          mx_printerr(&argv[i][1]);
          mx_printerr("\nusage: uls [-l] [file ...]\n");
        }

        continue;
      } else {
        file_type = is_dir(argv[i]);
        if (is_dir(argv[i]) == TYPE_DIR)
          dir_count++;
        else if (file_type == TYPE_FILE)
          file_count++;
        else {
          mx_printerr("uls: ");
          mx_printerr(argv[i]);
          mx_printerr(" No such file or directory\n");
        }
      }
    }

    char **file_list = (char **)malloc(sizeof(char *) * file_count);
    char **dir_list = (char **)malloc(sizeof(char *) * dir_count);

    for (int i = 1; i < argc; i++) {
      if (argv[i][0] == '-') {
        continue;
      } else {
        file_type = is_dir(argv[i]);
        if (file_type == TYPE_DIR) {
          dir_list[dir_counter] = mx_strdup(argv[i]);
          dir_counter++;
        } else {
          file_list[file_counter] = mx_strdup(argv[i]);
          file_counter++;
        }
      }
    }

    if (!info->is_flag_l) {
      if (file_count > 0) {
        mx_print_strarr(file_list, " ");
        if (dir_count == 0) {
          mx_printchar('\n');
        }
      }

      if (dir_count == 1) {
        ls(dir_list[0]);
      } else if (dir_count > 1) {
        for (int i = 0; i < dir_count; i++) {
          mx_printstr(dir_list[i]);
          mx_printchar(':');
          mx_printchar('\n');
          ls(dir_list[i]);
          if (i != dir_count)
            mx_printchar('\n');
        }
      }
    } else {
      if (file_count > 0) {
        for (int i = 0; i < file_count; i++) {
          ls_file_flag_l(file_list[i]);
          if (i != file_count)
            mx_printchar('\n');
        }
      }
      if (dir_count == 1) {
        ls_dir_flag_l(dir_list[0]);
      } else if (dir_count > 1) {
        for (int i = 0; i < dir_count; i++) {
          mx_printstr(dir_list[i]);
          mx_printchar(':');
          mx_printchar('\n');
          ls_dir_flag_l(dir_list[i]);
          if (i != dir_count)
            mx_printchar('\n');
        }
      }
    }
    if (info->is_flag_l && dir_count == 0 && file_count == 0)
      ls_dir_flag_l(".");
  }

  free(info->widht);
  free(info);

  return 0;
}
