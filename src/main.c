#include "coder.h"
#include "command.h"
#include <stdio.h>
#include <string.h>
int error() {
  printf("Usage:\n");
  printf("coder encode <in-file-name> <out-file-name>\n");
  printf("coder decode <in-file-name> <out-file-name>\n");
  return -1;
}

int main(int argc, char const *argv[]) {
  encode_file("hex.txt", "uhex.bin");
  decode_file("uhex.bin", "h.txt");
  if (argc != 4) {
    error();
    return -1;
  }
  if (!strcmp(argv[1], "encode")) {
    if (encode_file(argv[2], argv[3])) {
      error();
      return -1;
    }
  } else if (!strcmp(argv[1], "decode")) {
    if (decode_file(argv[2], argv[3])) {
      error();
      return -1;
    }
  } else {
    error();
    return -1;
  }
  return 0;
}
