// flat.h - Directory traversal utility functions

#ifndef FLAT_H

#include <stdint.h>
#include <sys/stat.h>

char *flat_path(int, ...);
int64_t flat_directory_entry_count(char const *);
char **flat_directory_entries(char const *);

#define MKDIR(x) (mkdir(x, 0777))

#endif // FLAT_H

#ifdef FLAT_IMPLEMENTATION

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>

char *flat_path(int s_n, ...) {
  va_list list;
  size_t c_n = 0;

  if (s_n < 1)
    return NULL;

  // First, measure the total length required.
  va_start(list, s_n);
  for (int i = 0; i < s_n; i++) {
    const char *s = va_arg(list, char *);
    c_n += strlen(s);
  }

  va_end(list);

  // Allocate return buffer.
  char *r = malloc(c_n + 1);
  if (r == NULL)
    return NULL;

  // Concatenate all the strings into the return buffer.
  char *d = r;
  va_start(list, s_n);

  for (int i = 0; i < s_n; i++) {
    const char *s = va_arg(list, char *);

    // This loop is a strcpy.
    while ((*d++ = *s++))
      ;
    d--;
  }

  va_end(list);
  return r;
}

int64_t flat_directory_entry_count(char const *path) {
  DIR *dir;
  struct dirent *ent;

  if (!(dir = opendir(path))) {
    return 0;
  }
  int64_t n = 0;

  while ((ent = readdir(dir))) {
    if (!strcmp(ent->d_name, "."))
      continue;

    if (!strcmp(ent->d_name, ".."))
      continue;

    n++;
  }

  closedir(dir);

  return n;
}

char **flat_directory_entries(char const *path) {
  int64_t n = flat_directory_entry_count(path);
  char **entries = calloc(n + 1, sizeof(char **));

  DIR *dir;
  struct dirent *ent;

  int64_t i = 0;
  int64_t entry_n = 0;

  if (!(dir = opendir(path))) {
    return NULL;
  }
  while (i < n) {
    ent = readdir(dir);

    if (!strcmp(ent->d_name, "."))
      continue;

    if (!strcmp(ent->d_name, ".."))
      continue;

    entry_n = strlen(ent->d_name);
    entries[i] = malloc(entry_n + 1);
    strcpy(entries[i], ent->d_name);

    i++;
  }

  closedir(dir);

  return entries;
}

#endif // FLAT_IMPELEMTATION
