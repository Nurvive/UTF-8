#include "coder.h"
#include "command.h"
#include <stdio.h>
#include <string.h>
int error() {
  printf("Usage:\n");
  printf("coder encode <in-file-name> <out-file-name>\n");
  printf("coder decode <in-file-name> <out-file-name>\n");
  return -1;
} //ошибка, которую выкидывает программа, если не хватает аргументов,
  //неправильные аргументы и т.п.

int main(int argc, char const *argv[]) {
  if (argc != 4) { //проверка на количество аргументов
    error();
    return -1;
  }
  if (!strcmp(argv[1], "encode")) {
    if (encode_file(argv[2], argv[3])) { //запускает кодированрие
      error();
      return -1;
    }
  } else if (!strcmp(argv[1], "decode")) {
    if (decode_file(argv[2], argv[3])) { //запускает декодирование
      error();
      return -1;
    }
  } else { //проверка на непредвиденную ошибку
    error();
    return -1;
  }
  return 0;
}
