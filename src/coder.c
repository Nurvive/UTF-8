#include "coder.h"
#include <stdio.h>
int encode(uint32_t value, CodeUnits *codeunits) {
  if (value < 128) { //проверка интервала кодируемого значения
    codeunits->length = 1; //присвоение соответствующего значения полю length
    uint8_t bi_value1 =
        value; //присвоение значения переменной, которую кодируем
    codeunits->code[0] =
        bi_value1 | 0x00; //запись значения в поле code и подгонка под шаблон
  } else if (value < 2048) { //проверка интервала кодируемого значения
    codeunits->length = 2; //присвоение соответствующего значения полю length
    uint8_t bi_value1 = value % 64; //вычисление последнего байта
    uint8_t bi_value2 = (value - bi_value1) / 64;
    codeunits->code[0] = bi_value2 | 0xC0;
    codeunits->code[1] =
        bi_value1 | 0x80; //запись значения в поле code и подгонка под шаблон
  } else if (value < 65536) { //проверка интервала кодируемого значения
    codeunits->length = 3; //присвоение соответствующего значения полю length
    uint8_t bi_value1 = value % 64;
    uint8_t bi_value2 = ((value - bi_value1) / 64) % 64;
    uint8_t bi_value3 = (value - bi_value1 - (bi_value2 * 64)) / (64 * 64);
    codeunits->code[0] = bi_value3 | 0xE0;
    codeunits->code[1] = bi_value2 | 0x80;
    codeunits->code[2] =
        bi_value1 | 0x80; //запись значения в поле code и подгонка под шаблон
  } else if (value < 2097151) { //проверка интервала кодируемого значения
    codeunits->length = 4; //присвоение соответствующего значения полю length
    uint8_t bi_value1 = value % 64;
    uint8_t bi_value2 = ((value - bi_value1) / 64) % 64;
    uint8_t bi_value3 =
        ((value - bi_value1 - (bi_value2 * 64)) / (64 * 64)) % 64;
    uint8_t bi_value4 =
        (value - bi_value1 - (bi_value2 * 64) - (bi_value3 * 64 * 64)) /
        (64 * 64 * 64);
    codeunits->code[0] = bi_value4 | 0xF0;
    codeunits->code[1] = bi_value3 | 0x80;
    codeunits->code[2] = bi_value2 | 0x80;
    codeunits->code[3] =
        bi_value1 | 0x80; //запись значения в поле code и подгонка под шаблон
  }
  return 0;
}

uint32_t decode(const CodeUnits *codeunits) {
  uint32_t result = 0;
  if (codeunits->length == 1) { //проверка поля length
    result = codeunits->code[0]; //присвоение переменной result значения
  } else if (codeunits->length == 2) { //проверка поля length
    result =
        codeunits->code[1] &
        0x3F; //отделение служебных бит и присвоение переменной result значения
    result += (codeunits->code[0] & 0x1F) * 64;
  } else if (codeunits->length == 3) { //проверка поля length
    result =
        codeunits->code[2] &
        0x3F; //отделение служебных бит и присвоение переменной result значения
    result += (codeunits->code[1] & 0x3F) * 64;
    result += (codeunits->code[0] & 0x0F) * 64 * 64;
  } else if (codeunits->length == 4) { //проверка поля length
    result =
        codeunits->code[3] &
        0x3F; //отделение служебных бит и присвоение переменной result значения
    result += (codeunits->code[2] & 0x3F) * 64;
    result += (codeunits->code[1] & 0x3F) * 64 * 64;
    result += (codeunits->code[0] & 0x07) * 64 * 64 * 64;
  }
  return result;
}

int read_next_code_unit(FILE *in, CodeUnits *codeunits) {
  uint8_t byte;

  fread(&byte, sizeof(uint8_t), 1, in);
  if ((byte & 0xF0) ==
      0xF0) { //проверка первого байта, для определения значения
    codeunits->length = 4; //поля length
  } else if ((byte & 0xE0) == 0xE0) {
    codeunits->length = 3;
  } else if ((byte & 0xC0) == 0xC0) {
    codeunits->length = 2;
  } else if ((byte | 0x80) != byte) {
    codeunits->length = 1;
  } else {
    codeunits->length = 0;
    return 0;
  }

  codeunits->code[0] = byte; //присвоение первому байту поля code значения
                             //первого считанного байта
  if (codeunits->length != 1) { //проверка значения поля length
    if (!fread(codeunits->code + 1, sizeof(uint8_t), codeunits->length - 1,
               in)) {
      return -1;
    } //считывание оставшихся байтов, если длина не равна 1
    for (size_t i = 1; i < codeunits->length; i++) {
      if (((codeunits->code[i] | 0x40) == codeunits->code[i]) ||
          (codeunits->code[i] | 0x80) != codeunits->code[i]) {
        codeunits->length = 0;
        return -1;
      } //проверка остальных байтов на верность по шаблону
    }
  }
  return 0;
}
