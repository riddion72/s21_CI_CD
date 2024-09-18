#include "s21_grep.h"

int main(int argc, char **argv) {
  proxy_grep(argc, argv);
  return 0;
}

void proxy_grep(int argc, char **argv) {
  if (argc > 1) {
    char *temp = NULL;
    forFlag flags = {0};
    for (int i = scan(argc, argv, &flags, &temp); (i < argc) && !(flags.errF);
         i += 1) {
      if (optind < argc) {
        greping(argv[i], flags, temp);
      } else {
        fprintf(stderr, "invalid input\n");
      }
    }
    if (temp != NULL) free(temp);
  }
}

int scan(int argc, char **argv, forFlag *flags, char **template) {
  for (int sym = 0; (sym = getopt(argc, argv, "e:ivclnhsf:o")) != (-1);) {
    switch (sym) {
      case 'i':
        flags->i = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 'c':
        flags->c = 1;
        break;
      case 'l':
        flags->l = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 'h':
        flags->h = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 'o':
        flags->o = 1;
        break;
      case 'e':
        flag_e(flags, template, optarg);
        flags->e = 1;
        break;
      case 'f':
        if (flag_f(flags, template, optarg)) flags->errF = 1;
        flags->f = 1;
        break;
    }
  }
  if ((flags->e || flags->f) ==
      0) {  //  создаём шаблон, если он небыл создан ранее
    create_temp(template, argv[optind]);
    optind += 1;
  }
  if (flags->o && (flags->l || flags->v || flags->c))
    flags->o = 0;  //  выставляем приорететные флаги
  flags->countFiles = argc - optind;
  if (flags->l) flags->h = 0;
  // printf("- %s -", *template);
  return optind;
}

void print_instead_of_line(forFlag flags, char *name) {  //  флаги c l
  if (flags.c) {
    if (flags.l) {
      if (flags.countFiles > 1) {
        flags.countMatches ? printf("%s:1\n", name) : printf("%s:0\n", name);
      } else {
        flags.countMatches ? printf("1\n") : printf("0\n");
      }
    } else {
      if ((flags.countFiles > 1) && !(flags.h)) printf("%s:", name);
      printf("%i\n", flags.countMatches);
    }
  }
  if (flags.l && flags.countMatches) printf("%s\n", name);
}

void greping(char *name, forFlag flags, char *template) {
  FILE *file = fopen(name, "r");
  if (file != NULL) {
    for (char sym = '\0'; (sym = getc(file)) != EOF;) {
      char *line = calloc(256, sizeof(char));
      flags.numberLine += 1;
      int length = 0;

      for (line[length] = '\0'; sym != EOF && sym != '\n'; sym = getc(file)) {
        line[length] = sym;
        line[length += 1] = '\0';
        if (length % 255 == 0) line = increase_str(line, length + 256);
      }

      print_line(line, &flags, template, name);
      free(line);
    }
    print_instead_of_line(flags, name);
    fclose(file);
  } else if (flags.s == 0) {  //  флаг s
    fprintf(stderr, "No such file or directory\n");
  }
  flags.numberLine = 0;
}

int create_temp(char **str, char *optarg) {
  *str = calloc(strlen(optarg) + 1, sizeof(char));
  if (*str) strcpy(*str, optarg);
  return str ? 1 : 0;
}

void print_line(char *line, forFlag *flags, char *template, char *name) {
  regex_t regex;
  if (regcomp(&regex, template,
              flags->i ? (REG_EXTENDED + REG_ICASE) : REG_EXTENDED) ==
      0) {                                                //  флаг i
    if (regexec(&regex, line, 0, NULL, 0) == flags->v) {  //  флаг v
      flags->countMatches += 1;
      if (!flags->c && !flags->l) {  //  исли нет флагов c l
        if ((flags->countFiles > 1) && !(flags->h))
          printf("%s:", name);                           //  флаг h
        if (flags->n) printf("%d:", flags->numberLine);  //  флаг n
        if (!flags->o) {
          printf("%s\n", line);
        } else {
          flag_o(line, &regex);
        }
      }
    }
    regfree(&regex);
  }
}

void flag_o(char *line, regex_t *regex) {  //  флаг o
  regmatch_t matchSize = {0};
  char *buffer = line;
  // printf("-*-");
  while (!(regexec(regex, buffer, (size_t)1, &matchSize, 0))) {
    if (matchSize.rm_so < 0) matchSize.rm_so = 0;
    if (matchSize.rm_eo < 0) {
      matchSize.rm_eo = strlen(buffer);
      printf("%.*s\n", (int)(matchSize.rm_eo - matchSize.rm_so),
             &buffer[matchSize.rm_so]);
      break;
    } else {
      printf("%.*s\n", (int)(matchSize.rm_eo - matchSize.rm_so),
             &buffer[matchSize.rm_so]);
      buffer += matchSize.rm_eo;
    }
  }
}

int flag_f(forFlag *flags, char **template, char *optarg) {
  FILE *file = fopen(optarg, "r");
  int err = 0;
  if (file != NULL) {
    for (char sym = '0'; (sym = getc(file)) != EOF;) {
      int length = 0;
      char *line = calloc(256, 1);

      for (line[length] = '\0'; sym != EOF && sym != '\n'; sym = getc(file)) {
        line[length] = sym;
        line[length += 1] = '\0';
        if (length % 255 == 0) line = increase_str(line, length + 256);
      }

      if (!(flags->e || flags->f))
        flags->f = create_temp(template, line);
      else
        add_temp(template, line);
      free(line);
    }
    fclose(file);
  } else {
    err = 1;
    fprintf(stderr, "No such file or directory\n");
  }
  return err;
}

void flag_e(forFlag *flags, char **template, char *optarg) {  //  флаг e
  if (!(flags->e || flags->f))
    flags->e = create_temp(template, optarg);
  else
    add_temp(template, optarg);
}

char *increase_str(void *str, int size) {
  char *buffer = NULL;
  buffer = realloc(str, size);
  return buffer;
}

void add_temp(char **str, char *optarg) {
  *str = increase_str(*str, strlen(*str) + strlen(optarg) + 2);
  if (*str) strcat(strcat(*str, "|"), optarg);
}
