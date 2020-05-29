#include "command.h"
#include "coder.h"
#include <inttypes.h>
int encode_file(const char *in_file_name, const char *out_file_name) {
  FILE *in = fopen(in_file_name, "r");
  FILE *out = fopen(out_file_name, "wb"); //открытие файлов
  if ((in == NULL) || (out == NULL)) { //проверка на корректность файлов
    printf("Error: invalid files\n");
    return -1;
  }
  int code_point;
  while (1) {
    int y = fscanf(in, "%" SCNx32,
                   &code_point); //считывание значения из входного файла
    if (y == EOF) { //проверка на выход за конец файла
      return 0;
    }
    if (!y) { //проверка, что файл не пустой
      printf("Error: empty file\n");
      return -1;
    }
    CodeUnits codeunits;
    encode(code_point, &codeunits); //кодирование значения
    if (!fwrite(codeunits.code, sizeof(uint8_t), codeunits.length,
                out)) { // запись закодированного значения
      printf("Error: invalid writing");
      return -1;
    }
  }
  fclose(in);
  fclose(out); //закрытие файлов
  return 0;
}

int decode_file(const char *in_file_name, const char *out_file_name) {
  FILE *in = fopen(in_file_name, "rb"); //открытие файлов
  FILE *out = fopen(out_file_name, "w");
  if ((in == NULL) || (out == NULL)) { //проверка на корректность файлов
    printf("Error: invalid files\n");
    return -1;
  }
  while (!feof(in)) {
    CodeUnits codeunits;
    if (read_next_code_unit(in,
                            &codeunits)) { //считывание закодированных значений
      printf("Error: invalid input file\n");
      return -1;
    }
    if (codeunits.code[0] != 0 &&
        codeunits.length != 0) { //проверка, что значение не 0
      if (!fprintf(
              out, "%" PRIx32 "\n",
              decode(&codeunits))) { //декодирование и запись в выходной файл
        printf("Error: invalid writing\n");
        return -1;
      }
    }
  }
  fclose(in);
  fclose(out); //закрытие файлов
  return 0;
}
