#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct forFlag {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int countFiles;
  int numberLine;
  int countMatches;
  int errF;
} forFlag;

char *increase_str(void *str, int size);
void add_temp(char **str, char *optarg);
void flag_o(char *line, regex_t *regex);
void print_line(char *line, forFlag *flags, char *template, char *name);
void print_instead_of_line(forFlag flags, char *name);
void flag_e(forFlag *flags, char **template, char *optarg);
int scan(int argc, char **argv, forFlag *flags, char **template);
int create_temp(char **str, char *optarg);
void greping(char *path, forFlag flags, char *template);
void proxy_grep(int argc, char **argv);
void flag_e(forFlag *flags, char **template, char *optarg);
int flag_f(forFlag *flags, char **template, char *optarg);

#endif  // SRC_GREP_S21_GREP_H_
