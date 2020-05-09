#include "command.h"
#include "coder.h"
#include <inttypes.h>
int encode_file(const char *in_file_name, const char *out_file_name) {
  FILE *in = fopen(in_file_name, "r");
  FILE *out = fopen(out_file_name, "wb");
  if ((in == NULL) || (out == NULL)) {
    printf("Error: invalid files\n");
    return -1;
  }
  int code_point;
  while (!feof(in)) {
    int y = fscanf(in, "%" SCNx32, &code_point);
    if (y == EOF) {
      return 0;
    }
    if (!y) {
      printf("Error: empty file\n");
      return -1;
    }
    CodeUnits codeunits;
    if (encode(code_point, &codeunits)) {
      continue;
    }
    if (!fwrite(codeunits.code, sizeof(uint8_t), codeunits.length, out)) {
      printf("Error: invalid writing");
      return -1;
    }
  }
  fclose(in);
  fclose(out);
  return 0;
}

int decode_file(const char *in_file_name, const char *out_file_name) {
  FILE *in = fopen(in_file_name, "rb");
  FILE *out = fopen(out_file_name, "w");
  if ((in == NULL) || (out == NULL)) {
    printf("Error: invalid files\n");
    return -1;
  }
  while (!feof(in)) {
    CodeUnits codeunits;
    if (read_next_code_unit(in, &codeunits)) {
      printf("Error: invalid input file\n");
      return -1;
    }
    if (codeunits.code[0] != 0 && codeunits.length != 0) {
      if (!fprintf(out, "%" PRIx32 "\n", decode(&codeunits))) {
        printf("Error: invalid writing\n");
        return -1;
      }
    }
  }
  fclose(in);
  fclose(out);
  return 0;
}
