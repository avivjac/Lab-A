#include <stdio.h>
#include <string.h>
#include <ctype.h>

unsigned char password[] = "my_password1";
int debug_mode = 1;      // on by default
FILE *infile = NULL, *outfile = NULL;

char encode_key[] = "0";
int encode_add = 1; // 1: add, 0: subtract

int encode_pos = 0; // position in the key

int encode(int c) {
    int key_digit = encode_key[encode_pos % strlen(encode_key)] - '0';
    int result = c;
    
    if ('A' <= c && c <= 'Z') {
        if (encode_add)
            result = ((c - 'A' + key_digit) % 26) + 'A';
        else
            result = ((c - 'A' - key_digit + 26) % 26) + 'A';
    } else if ('0' <= c && c <= '9') {
        if (encode_add)
            result = ((c - '0' + key_digit) % 10) + '0';
        else
            result = ((c - '0' - key_digit + 10) % 10) + '0';
    }

    encode_pos++;
    return result;
}


int main(int argc, char *argv[]) {
    infile = stdin;
    outfile = stdout;
    debug_mode = 1;         // Default debug ON
    strcpy(encode_key, "0"); // Default encoding key "0"
    encode_add = 1;          // Default to addition (not used for "0")

    // Pass through all arguments to set up variables before first debug print
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-D") == 0) {
            debug_mode = 0;
        } else if (strncmp(argv[i], "+D", 2) == 0 &&
                   strcmp(argv[i] + 2, (const char *)password) == 0) {
            debug_mode = 1;
        } else if (strncmp(argv[i], "-i", 2) == 0) {
            infile = fopen(argv[i]+2, "r");
            if (!infile) {
                fprintf(stderr, "Error opening input file\n");
                exit(1);
            }
        } else if (strncmp(argv[i], "-o", 2) == 0) {
            outfile = fopen(argv[i]+2, "w");
            if (!outfile) {
                fprintf(stderr, "Error opening output file\n");
                exit(1);
            }
        } else if (strncmp(argv[i], "+E", 2) == 0) {
            strcpy(encode_key, argv[i]+2);
            encode_add = 1;
        } else if (strncmp(argv[i], "-E", 2) == 0) {
            strcpy(encode_key, argv[i]+2);
            encode_add = 0;
        }
    }

    // Second loop: debug print arguments, after config is set
    for (int i = 1; i < argc; i++) {
        if (debug_mode) {
            fprintf(stderr, "%s\n", argv[i]);
        }
        // If debug flag disabled here, next prints will be skipped
        if (strcmp(argv[i], "-D") == 0) {
            debug_mode = 0;
        } else if (strncmp(argv[i], "+D", 2) == 0 && strcmp(argv[i] + 2, (const char *)password) == 0) {
            debug_mode = 1;
        }
    }

    encode_pos = 0; // reset key cycling
    int c;
    while ((c = fgetc(infile)) != EOF) {
        c = encode(c);
        fputc(c, outfile);
    }
    if (infile != stdin) fclose(infile);
    if (outfile != stdout) fclose(outfile);
    return 0;
}
