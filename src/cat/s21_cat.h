#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct forFlag {
  int b;
  int e;
  int v;
  int n;
  int s;
  int t;
  int empty_flag;
  int counter_for_b;
  int counter_for_s;
  int number_files_name;
  int new_line;
} forFlag;

void proxy_cat(int argc, char **argv);
int flag_parsing(int argc, char **argv, forFlag *flags);
FILE *outputting(FILE *file, forFlag *flags);
void buffering(forFlag *a, forFlag *b);

#endif  //  SRC_CAT_S21_CAT_H_
