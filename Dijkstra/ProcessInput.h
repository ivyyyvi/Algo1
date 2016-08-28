

#define ASCII_CODE_TAB 0x09
#define ASCII_CODE_SPACE 0x20
#define ASCII_CODE_COMMA 0x2C
#define ASCII_CODE_NEWLINE 0x0A
#define ASCII_CODE_CARRIAGE_RETURN 0x0D


enum readingFlag {
  isCleared = 0,
  isAfterNewLineCR = 1, // right after new line
  isAfterComma = 2, // right after comma
  isAfterTab = 3 // right after tab
};

#define ASK_FOR_INPUT 0
// input_Z.txt is the input from PA
#ifdef DEFAULT_INPUT_FILENAME_Z
  #define DEFAULT_INPUT_FILENAME "input_Z.txt"
#endif
#ifdef DEFAULT_INPUT_FILENAME_A
  #define DEFAULT_INPUT_FILENAME "input_A.txt"
#endif
#ifdef DEFAULT_INPUT_FILENAME_B
  #define DEFAULT_INPUT_FILENAME "input_B.txt"
#endif
#ifdef DEFAULT_INPUT_FILENAME_C
  #define DEFAULT_INPUT_FILENAME "input_C.txt"
#endif
#ifdef DEFAULT_INPUT_FILENAME_D
  #define DEFAULT_INPUT_FILENAME "input_D.txt"
#endif
#ifdef DEFAULT_INPUT_FILENAME_E
  #define DEFAULT_INPUT_FILENAME "input_E.txt"
#endif
#ifdef DEFAULT_INPUT_FILENAME_F
  #define DEFAULT_INPUT_FILENAME "input_F.txt"
#endif
#ifdef DEFAULT_INPUT_FILENAME_G
  #define DEFAULT_INPUT_FILENAME "input_G.txt"
#endif
#ifdef DEFAULT_INPUT_FILENAME_H
  #define DEFAULT_INPUT_FILENAME "input_H.txt"
#endif

//
// Function prototypes.
//
