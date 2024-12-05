#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define BUFFER_SIZE 256000
#define MAX_ROWS 6

int main() {
    FILE *file = fopen("day3_input.txt", "r");
    const char *pattern_m = "mul\\((0|[1-9][0-9]{0,2}),(0|[1-9][0-9]{0,2})\\)";
    const char *pattern_do = "do\\(\\)";
    const char *pattern_dont = "don't\\(\\)";
    int output = 0;

    if (file == NULL) {
        printf("Error: Unable to open file\n");
        return 1;
    }

    char line[BUFFER_SIZE];
    regex_t mul, dos, donts;
    regmatch_t match[3]; // Store the match
    regmatch_t match_do; // Store the match
    regmatch_t match_dont; // Store the match

    if (regcomp(&mul, pattern_m, REG_EXTENDED) != 0 ||
        regcomp(&dos, pattern_do, REG_EXTENDED) != 0 ||
        regcomp(&donts, pattern_dont, REG_EXTENDED) != 0) {
        fprintf(stderr, "Error compiling regex\n");
        fclose(file);
        return 1;
    }

    int line_number = 1;
    int enable = 1;

    while (fgets(line, sizeof(line), file)) {
        const char *cursor = line;

        while (*cursor){
            if (enable && (regexec(&mul, cursor, 3, match, 0) == 0)&&(match[0].rm_so == 0)) {
                char num1[16], num2[16];
                // Extract and print the matched numbers
                int len1 = match[1].rm_eo - match[1].rm_so;
                int len2 = match[2].rm_eo - match[2].rm_so;

                snprintf(num1, len1 + 1, "%.*s", len1, cursor + match[1].rm_so);
                snprintf(num2, len2 + 1, "%.*s", len2, cursor + match[2].rm_so);

                output = output + atoi(num2)*atoi(num1);

                cursor += match[0].rm_eo;

            }else if((regexec(&dos, cursor, 1, &match_do, 0) == 0)&& (match_do.rm_so == 0)) {
                enable = 1;
                cursor += strlen("do()");
            }else if((regexec(&donts, cursor, 1, &match_dont, 0) == 0)&& (match_dont.rm_so == 0)) {
                enable = 0;
                cursor += strlen("don't()");
            }else{
                cursor++;
            }
        }
        line_number++;
    }
    printf("%d\n", output);
    fclose(file);


    regfree(&mul);
    regfree(&dos);
    regfree(&donts);

    return 0;
}