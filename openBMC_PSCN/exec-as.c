#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>

#define USAGE "usage: exec-as uid gid cmd..."

int main(int argc, char *argv[])
{
  if (argc < 4)
    errx(EXIT_FAILURE, "exec-as: not enough arguments\n" USAGE);

  char *last;
  uid_t uid = strtol(argv[1], &last, 10);
  if (*last != '\0')
    err(EXIT_FAILURE, "uid should be numeric, '%s' given\n" USAGE, argv[1]);

  uid_t gid = strtol(argv[2], &last, 10);
  if (*last != '\0')
    err(EXIT_FAILURE, "gid should be numeric, '%s' given\n" USAGE, argv[2]);

  if (setgid(atoi(argv[2])) == -1)
    err(EXIT_FAILURE, "setgid(%d)", atoi(argv[2]));

  if (setuid(atoi(argv[1])) == -1)
    err(EXIT_FAILURE, "setuid(%d)", atoi(argv[1]));

  char **p = &argv[3];
  printf("exec-as: uid: %d gid: %d cmd:", uid, gid);
  while(*p) {
    printf(" '%s'", *p);
    p++;
  }
  printf("\n");
  execvp(argv[3], argv+3);
  err(EXIT_FAILURE, "execvp('%s', ...)", argv[3]);
}
