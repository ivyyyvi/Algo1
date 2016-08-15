// This is faster than reading one char at a time using fgetc
// http://stackoverflow.com/questions/410943/reading-a-text-file-into-an-array-in-c

FILE *f = fopen("text.txt", "rb");
fseek(f, 0, SEEK_END);
long pos = ftell(f);
fseek(f, 0, SEEK_SET);

char *bytes = malloc(pos);
fread(bytes, pos, 1, f);
fclose(f);

hexdump(bytes); // do some stuff with it
free(bytes); // free allocated memory

