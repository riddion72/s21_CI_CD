#include "s21_cat.h"

int main(int argc, char** argv) {
  fprintf(stderr, "Hello from montasco");
  proxy_cat(argc, argv);
  return 0;
}

void proxy_cat(int argc, char** argv) {
  if (argc > 1) {
    forFlag flags = {0};
    if (flag_parsing(argc, argv, &flags)) {
      for (int a = (argc - flags.number_files_name); a < argc; a++) {
        FILE* file = fopen(argv[a], "r");
        if (file != NULL) {
          fclose(outputting(file, &flags));
        } else {
          fprintf(stderr, "cat: %s: No such file or directory\n", argv[a]);
        }
      }
    }
  }
}

int flag_parsing(int argc, char** argv, forFlag* flags) {
  int flagsCounter = 1;
  int err = 1;

  for (int a = 1; (a < argc && argv[a][0] == '-'); flagsCounter = (a += 1)) {
    if (!strcmp(argv[a], "-b") || !strcmp(argv[a], "--number-nonblank")) {
      if (flags->n) flags->n = 0;
      flags->b = 1;
    } else if (!strcmp(argv[a], "-s") || !strcmp(argv[a], "--squeeze-blank")) {
      flags->s = 1;
    } else if (!strcmp(argv[a], "-n") || !strcmp(argv[a], "--number")) {
      flags->n = flags->b ? 0 : 1;
    } else if (!strcmp(argv[a], "-T")) {
      flags->t = 1;
    } else if (!strcmp(argv[a], "-t")) {
      flags->t = 1;
      flags->v = 1;
    } else if (!strcmp(argv[a], "-v")) {
      flags->v = 1;
    } else if (!strcmp(argv[a], "-E")) {
      flags->e = 1;
    } else if (!strcmp(argv[a], "-e")) {
      flags->e = 1;
      flags->v = 1;
    } else {
      fprintf(stderr, "illegal option: %s", argv[a]);
      err = 0;
    }
  }

  flags->number_files_name = argc - flagsCounter;
  return err;
}

FILE* outputting(FILE* file, forFlag* flags) {
  forFlag buffer = {0};
  buffering(&buffer, flags);
  if (flags->s) {
    flags->counter_for_s = 1;  //  если файл начинается с пустых строк
  } else {
    flags->counter_for_s = 0;
  }

  for (char curentSymbol = '0'; (curentSymbol = getc(file)) != EOF;) {
    flags->empty_flag = 0;

    if (flags->s && flags->counter_for_s == 0 &&
        curentSymbol == '\n') {  //  сжимаем строки
      flags->counter_for_s += 1;
    } else if (flags->counter_for_s != 0 && curentSymbol == '\n') {
      flags->counter_for_s += 1;
      flags->empty_flag = 1;
    } else if (flags->counter_for_s > 1 && curentSymbol != '\n') {
      flags->counter_for_s = 0;
      flags->e ? printf("$\n") : printf("\n");
      if (flags->n != 0) printf("%6d\t", flags->n++);
    } else {
      flags->counter_for_s = 0;
    }

    if (flags->n != 0 || flags->b != 0) {  // нумеруем строки
      if (flags->new_line == 1) {
        flags->new_line = 0;
        printf("%6d\t", flags->n++);
      }
      if (curentSymbol == '\n' && flags->b) flags->counter_for_b += 1;
      if (flags->n == 1) printf("%6d\t", flags->n++);
      if (flags->b == 1 && curentSymbol != '\n' && flags->counter_for_b == 0)
        printf("%6d\t", flags->b++);
      if (curentSymbol == '\n' && flags->n != 0 && flags->empty_flag == 0)
        flags->new_line = 1;
      if (curentSymbol != '\n' && flags->counter_for_b != 0 &&
          flags->counter_for_s == 0) {
        flags->counter_for_b = 0;
        printf("%6d\t", flags->b++);
      }
    }

    if (curentSymbol == '\n' && flags->e &&
        flags->empty_flag == 0)  //  ставит '$' перед '\n'
      printf("$");

    if (flags->v) {
      if (curentSymbol < 32 && curentSymbol != 9 &&
          curentSymbol != 10) {  //  отображает символы
        curentSymbol += 64;
        printf("^");
      }
      if (curentSymbol == 127) {
        curentSymbol = '?';
        printf("^");
      }
    }

    if (flags->t && (curentSymbol == '\t')) {  //  отображает табуляцию
      curentSymbol = 'I';
      printf("^");
    }

    if (flags->empty_flag == 0)
      printf("%c", curentSymbol);  //  печатаем текущий символ
  }
  if (flags->counter_for_s > 1) {
    flags->e ? printf("$\n")
             : printf("\n");  //  если файл заканчивается пустыми строками
  }

  buffering(flags, &buffer);

  return file;
}

void buffering(forFlag* a, forFlag* b) {
  a->b = b->b;
  a->e = b->e;
  a->v = b->v;
  a->n = b->n;
  a->s = b->s;
  a->t = b->t;
  a->empty_flag = b->empty_flag;
  a->counter_for_b = b->counter_for_b;
  a->counter_for_s = b->counter_for_s;
  a->new_line = b->new_line;
}
