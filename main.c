/*
 * main.c
 *
 *  Created on: Nov 3, 2014
 *      Author: s1310307036
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdarg.h>

#include "typedefs.h"

/*
 * define
 */
#define MAX_ARG 3
#define PRINT "-print"
#define SEARCH "-grep"
/*
 * typedefs
 */
typedef void (*Visitor)(char *pathname, struct stat *stat, ...);


/*
 *
 */
static void printFileAttribute(char *pathname, struct stat *stat, ...) {
  va_list arg;

  va_start(arg, stat);

  printf("printFileAttribute: %s\n", pathname);

  va_end(arg);
}
/*
 *
 */
static void searchFileFor(char *pathname, struct stat *stat, ...) {
  va_list arg;
  char ch = 0;
  char *str = NULL;

  va_start(arg, stat);

  printf("searchFileFor: %s\n", pathname);
  str = (stat + 1);
  while(str++) {
    ch = va_arg(arg, char);
  }

  va_end(arg);
}

static void printName(char *pathname, struct stat *stat, ...) {
  va_list arg;

  va_start(arg, stat);

  printf("File: %s\n", pathname);

  va_end(arg);
}
/*
 *
 */
static void walkDir(char *dirName, Visitor visitor) {
  struct dirent *pDirent;
  DIR *pDir = NULL;
  int len = strlen(dirName);

  printf("directory %s\n", dirName);
  pDir = opendir(dirName);

  if (pDir == NULL) {
    printf("Cannot open directory %s\n", dirName);
    closedir(pDir);
    return;
  }

  pDirent = readdir(pDir);

  while(pDirent != NULL) {
    sprintf(&dirName[len], "/%s", pDirent->d_name);

    if (pDirent->d_type == DT_DIR) {
      if (strncmp(pDirent->d_name, ".", 1) != 0 &&
          strncmp(pDirent->d_name, "..", 2) != 0) {
        walkDir(dirName, visitor);
      }
    } else if (pDirent->d_type == DT_REG) {
      if (visitor != NULL) {
        visitor(dirName, NULL);
      }
    }
    pDirent = readdir(pDir);
  }

  closedir(pDir);
}
/*
 *
 */
int main(int argc, char **argv) {
  char *name = NULL;

  /*
   * check for valid argument number
   * don't forget: first argument is name of the file,
   * reason for the + 1 in the if
   */
  if (argc != (MAX_ARG + 1)) {
    printf ("Usage: program_name + <dirname> + <type>\n");
    return EXIT_FAILURE;
  }

  name = argv[1];

  if (argc == 2 && strncmp(argv[2], PRINT, strlen(PRINT)) == 0) {
    walkDir(name, printFileAttribute);
  } else if (argc == 3 && strncmp(argv[2], SEARCH, strlen(SEARCH)) == 0) {
    walkDir(name, printFileAttribute, argv[3]);
  } else {
    walkDir(name, printName);
  }

  return EXIT_SUCCESS;
}
