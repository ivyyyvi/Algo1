#include "Dijkstra.h"

int
ReadInput (edge *E, int *numEdge, int *numVertices)
{
  char *InputFileName = DEFAULT_INPUT_FILENAME;
  char ch;
  char collectFlag;
  char ReturnFlag = 0;
  enum readingFlag flag;
  int currentReadNum;
  int vertexIndexMax = 0;
  int verticesCount = 0;
  int edgesCount = 0;
  edge* currentEdge = NULL;

  char currentChar;
  FILE *f = NULL;
  f = fopen(InputFileName, "r");
  if (f == NULL) {
    DEBUG_PROCESS_INPUT ("Error opening file.\n");
    return -1;
  }

  fseek(f, 0, SEEK_END);
  long pos = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *bytes = malloc(pos);
  fread(bytes, pos, 1, f);
  fclose(f);

  ch = 0;
  currentReadNum = 0;

  DEBUG_PROCESS_INPUT ("Start processing bytes\n");

  flag = isAfterNewLineCR;
  for (int tracker = 0; tracker < pos; tracker++) {
    DEBUG_PROCESS_INPUT ("%dth byte = 0x%x = %c \n",\
                          tracker,\
                          bytes[tracker],\
                          bytes[tracker]);

    currentChar = bytes[tracker];

    //
    // accumulatedly collect digits of number in currentReadNum
    //
    if (currentChar >= 0x30 && currentChar <= 0x39) {
      DEBUG_PROCESS_INPUT ("_%d_ \n", (currentChar - 0x30));
      currentReadNum = currentReadNum * 10 + (currentChar - 0x30);
      collectFlag = 1;
    } else {

      //
      // The character reading here is not a number.
      // so first see what we have collected bfr this non-number char.
      //
      DEBUG_PROCESS_INPUT ("collectFlag = %d\n", collectFlag);
      if (collectFlag == 1) {

        //
        // if a number has been collected.
        //
        collectFlag = 0;

        switch (flag) {
          case isAfterNewLineCR:

            //
            // Before collecting this number, 
            // there was immediately a newline/CR
            //

            //
            // use max of num saw at the beginnging of lines
            // to be the vertices count
            // this assumes the input file arranges data so that 
            // the leading number of the lines 
            // are always (tail) vertex index
            //
            if (currentReadNum > vertexIndexMax) {
              vertexIndexMax = currentReadNum;
            }
            verticesCount++;//1-base
            DEBUG_PROCESS_INPUT ("verticesCount++ to (%d)\n", verticesCount);
            currentEdge = &E[verticesCount];
            if (currentEdge) {
              DEBUG_PROCESS_INPUT ("1 Read (%d) as tailIndex\n", currentReadNum);
              currentEdge->tailIndex = currentReadNum;
            }
            break;
          case isAfterComma:
            if (currentEdge) {
              DEBUG_PROCESS_INPUT ("3 Read (%d) as weight\n", currentReadNum);
              currentEdge->weight = currentReadNum;
              edgesCount++;
              DEBUG_PROCESS_INPUT ("-----So this is %dth edge = "
                                   " (%d, %d; %d)\n",\
                                    edgesCount,\
                                    currentEdge->tailIndex,\
                                    currentEdge->headIndex,\
                                    currentEdge->weight);
            }
            break;
          case isAfterTab:
            if (currentEdge) {
              DEBUG_PROCESS_INPUT ("2 Read (%d) as headIndex\n", currentReadNum);
              currentEdge->headIndex = currentReadNum;
            }
            break;
          default:
            DEBUG_PROCESS_INPUT ("------ Unexpected input.\n!!!!!");
            ReturnFlag = 1;
            break;

        } // switch flag to deal with number just collected

        if (ReturnFlag) break;

        currentReadNum = 0;
        flag = isCleared;

      } // if collectFlag is 1

      //
      // Now let's look at the current non-number char
      // so that we can know what the number after this char 
      // is e.g. head, tail, weight, etc
      //
      switch (currentChar) {

        case ASCII_CODE_CARRIAGE_RETURN:
        case ASCII_CODE_NEWLINE:
          DEBUG_PROCESS_INPUT ("The next number is leading number of a line => (tail) vertex index\n");
          flag = isAfterNewLineCR;
          break;

        case ASCII_CODE_COMMA:
          DEBUG_PROCESS_INPUT ("The next number is the number after comma => weight\n");
          flag = isAfterComma;
          break;

        case ASCII_CODE_TAB:
          DEBUG_PROCESS_INPUT ("The next reading could be"
                               "(head) vertex index"
                               "or a newline then a new vertex\n");
          flag = isAfterTab;
          break;
        default:
          DEBUG_PROCESS_INPUT ("------ Unexpected input.\n!!!!!");
          ReturnFlag = 1;
          break;

      } // switch case to raise the isAfterxxx flag
      if (ReturnFlag) break;

    } // if-else reading number or non-number
  } // for loop traverse bytes

  DEBUG_PROCESS_INPUT ("End processing bytes\n");
  free(bytes); // free allocated memory

  *numEdge = edgesCount;
  *numVertices = vertexIndexMax;
  return 0;
} // ReadInput
