#include "../inc/uls.h"

blkcnt_t set_columns_width(const char *dir) {
  struct passwd *pass;
  struct group *grp;
  char *pass_str = NULL;
  char *grp_str = NULL;
  char *links = NULL;
  char *wsize = NULL;
  blkcnt_t blocks_sum = 0;

  struct stat st;
  char *path = NULL;

  for (int i = 0; i < info->file_count; i++) {

    int size_len = 5;

    path = mx_strnew(mx_strlen(dir) + mx_strlen(info->list_of_filenames[i]) + 1);
    mx_strcpy(path, dir);
    mx_strcat(path, "/");
    mx_strcat(path, info->list_of_filenames[i]);
    lstat(path, &st);

    blocks_sum +=st.st_blocks;

    links = mx_ltoa(st.st_nlink);
    set_bigger(&info->widht->links, mx_strlen(links));

    pass = getpwuid(st.st_uid);
    pass_str = pass ? mx_strdup(pass->pw_name) : mx_ltoa(st.st_uid);
    set_bigger(&info->widht->user, mx_strlen(pass_str));

    grp = getgrgid(st.st_gid);
    grp_str = grp ? mx_strdup(grp->gr_name) : mx_ltoa(st.st_gid);
    set_bigger(&info->widht->group, mx_strlen(grp_str));

    if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode)) {
      wsize = mx_nbr_to_hex(st.st_rdev);
      size_len = mx_strlen(wsize) + 2;
    } else {
      wsize = mx_ltoa(st.st_size);
      size_len = mx_strlen(wsize);
    }
    set_bigger(&info->widht->size, size_len);

    mx_strdel(&links);
    mx_strdel(&pass_str);
    mx_strdel(&grp_str);
    mx_strdel(&wsize);
    mx_strdel(&path);
  }

  return blocks_sum;
}
