#ifndef EXTENSO_CLI_H
#define EXTENSO_CLI_H

/*
**  Command-line interface: menu, keyboard/file modes, display.
*/
#include "extenso/config.h"
#include "extenso/context.h"

/* Menu that starts the analysis. */
int runMenu (Context *ctx);

/* Prints the usage manual. */
void printHelp (void);

/* Triggers the analysis reading from inPath and writing to outPath. */
int runFile (Context *ctx, const char *inPath, const char *outPath);

/* Shows the result file. */
void printResults (void);

/* Length of the longest line in the stream. */
size_t longestLine (FILE *stream);

/* Portable method to clear the screen. */
void clearScreen (void);

#endif
