#include <stdio.h> /* printf */
#include <dirent.h> /* DIR, dirent, opendir, readdir */

int main(int argc, char const *argv[])
{
  DIR* dp;
  struct dirent* dirp;
  if (argc != 2) {
    printf("Usage: %s directory\n", argv[0]);
    return 1;
  }
  dp = opendir(argv[1]);
  if (dp == NULL) {
    printf("Can not to open %s\n", argv[1]);
    return 2;
  }
  while ((dirp = readdir(dp)) != NULL) {
    if (dirp->d_name[0] == '.') continue;
    printf("%s  ", dirp->d_name);
  }
  printf("\n");
  closedir(dp);
  return 0;
}
