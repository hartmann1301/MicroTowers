#ifndef MyFont_h
#define MyFont_h

// 'textSet', 54x32px
const unsigned char textSet [] PROGMEM = {
  0x1e, 0x1f, 0x09, 0x09, 0x1f, 0x1e, 0x1f, 0x1f, 0x15, 0x15, 0x1f, 0x0e, 0x0e, 0x1f, 0x11, 0x11,
  0x1b, 0x0a, 0x1f, 0x1f, 0x11, 0x11, 0x0f, 0x0e, 0x1f, 0x1f, 0x15, 0x15, 0x11, 0x11, 0x1f, 0x1f,
  0x05, 0x05, 0x01, 0x01, 0x0e, 0x1f, 0x11, 0x15, 0x0d, 0x1c, 0x1f, 0x1f, 0x04, 0x04, 0x1f, 0x1f,
  0x11, 0x1f, 0x1f, 0x11, 0x00, 0x00, 0x08, 0x11, 0x11, 0x1f, 0x0f, 0x01, 0x1f, 0x1f, 0x04, 0x0e,
  0x1f, 0x19, 0x1f, 0x1f, 0x10, 0x10, 0x10, 0x10, 0x1f, 0x17, 0x0e, 0x07, 0x1f, 0x1f, 0x1f, 0x1f,
  0x06, 0x0c, 0x1f, 0x1f, 0x0e, 0x1f, 0x11, 0x11, 0x1f, 0x0e, 0x1f, 0x1f, 0x09, 0x09, 0x07, 0x06,
  0x0e, 0x1f, 0x11, 0x09, 0x1f, 0x16, 0x1f, 0x1f, 0x09, 0x09, 0x1f, 0x16, 0x16, 0x17, 0x15, 0x15,
  0x1d, 0x0d, 0x01, 0x01, 0x1f, 0x1f, 0x01, 0x01, 0x0f, 0x1f, 0x10, 0x10, 0x1f, 0x0f, 0x07, 0x0f,
  0x18, 0x18, 0x0f, 0x07, 0x1f, 0x19, 0x0c, 0x18, 0x1f, 0x1f, 0x11, 0x1b, 0x0e, 0x0e, 0x1b, 0x11,
  0x03, 0x07, 0x04, 0x1c, 0x1f, 0x03, 0x19, 0x1d, 0x1d, 0x17, 0x17, 0x13, 0x00, 0x00, 0x17, 0x17,
  0x00, 0x00, 0x02, 0x03, 0x01, 0x15, 0x17, 0x06, 0x04, 0x0e, 0x1f, 0x1b, 0x11, 0x00, 0x00, 0x11,
  0x1b, 0x1f, 0x0e, 0x04, 0x00, 0x00, 0x1b, 0x1b, 0x00, 0x00, 0x12, 0x1f, 0x1f, 0x10, 0x00, 0x00,
  0x12, 0x1b, 0x19, 0x1d, 0x17, 0x13, 0x11, 0x11, 0x15, 0x1f, 0x1f, 0x0a, 0x0f, 0x0f, 0x08, 0x1e,
  0x1e, 0x08, 0x17, 0x17, 0x15, 0x1d, 0x1d, 0x09, 0x0e, 0x1f, 0x15, 0x15, 0x1d, 0x0c, 0x01, 0x01,
  0x19, 0x1d, 0x07, 0x03, 0x0a, 0x1f, 0x15, 0x15, 0x1f, 0x0a, 0x06, 0x17, 0x15, 0x15, 0x1f, 0x0e
};

struct myFont {

  // variables to store the current cursor position
  int8_t x;
  int8_t y;

  void setCursor(int8_t n_x, int8_t n_y) {

    x = n_x;
    y = n_y;
  }

  void drawLetter(char c) {

    // could also replace small letters with big ones to aviod user mistakes

    // replace the 0 with O because it looks equal
    if (c == '0')
      c = 'O';

    // calculate the offset depending on the order placed in the textSet
    uint8_t offset;
    if (c >= 'A' && c <= 'Z') {
      offset =  c - 'A';

    } else if (c >= '1' && c <= '9') {
      offset =  c - '1' + 31;

    } else if (c == '!') {
      offset = 26;

    } else if (c == '?') {
      offset = 27;

    } else if (c == '<') {
      offset = 28;

    } else if (c == '>') {
      offset = 29;

    } else if (c == ':') {
      offset = 30;

    } else {
      offset = 0xff;
    }

    // draw if it is a known char
    if (offset != 0xff)
      drawBitmapFast(x, y, textSet, 6, offset, false);

    // there are two smaller chars
    if (c != 'I' && c != '1') {
      x += 7;
    } else {
      x += 5;
    }
  }

  /*
  void print(String s) {
    for (uint8_t i = 0; i < s.length(); i++) {
      drawLetter(s.charAt(i));
    }
  }
  */

  void print(uint8_t v) {

    // maybe take care of minus values
    
    uint8_t stellen;
    if (v < 10) {
      stellen = 1;
    } else if (v < 100) {
      stellen = 2;
    } else {
      stellen = 3;
    }

    uint16_t dividor = 1;
    for (uint8_t i = 0; i < stellen - 1; i++) {
      dividor *= 10; 
    }

    for (uint8_t i = 0; i < stellen; i++) {

      uint8_t currentValue = v / dividor;

      char currentNumber = currentValue % 10 + '0';

      drawLetter(currentNumber);
      
      dividor /= 10;
    }
  }

  void print(const __FlashStringHelper *ifsh) {

    // PGM_P is just a const char*
    // reinterpret_cast< > to cast from __FlashStringHelper type
    // this line is from Print.h, it gets me something I can work with
    PGM_P p = reinterpret_cast<PGM_P>(ifsh);

    while (1) {
      uint8_t c = pgm_read_byte(p++);

      drawLetter(c);
      if (c == 0)
        break;
    }
  }

};

#endif
