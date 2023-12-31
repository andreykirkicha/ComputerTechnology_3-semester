#include <dirent.h>

int getNumFiles(char *path) 
{
  int num_files = 0;
  DIR *dir;
  struct dirent *entry;
  dir = opendir(path);
  while ((entry = readdir(dir)) != NULL) 
  {
    if (???->d_type == DT_REG)// что означает этот флаг
      num_files++;
  }
  closedir(dir);
  return num_files;
}

