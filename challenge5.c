#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RULES 1200
#define MAX_ORDERS 500
#define MAX_ORDER_LENGTH 50
#define MAX_LINE_LENGTH 100


typedef struct {
    int part1;
    int part2;
} Rule;

Rule *global_rules;
int global_rule_count;


int compare_elements(const void *a, const void *b) {
    int element1 = *(const int *)a;
    int element2 = *(const int *)b;

    for (int i = 0; i < global_rule_count; i++) {
        if (global_rules[i].part1 == element1 && global_rules[i].part2 == element2) {
            return -1; // element1 should come before element2
        }
        if (global_rules[i].part1 == element2 && global_rules[i].part2 == element1) {
            return 1; // element2 should come before element1
        }
    }

    return 0; // No specific ordering in the rules
}

void read_parse(const char *filename, Rule rules[MAX_RULES], int *rule_count,
                int orders[MAX_ORDERS][MAX_ORDER_LENGTH], int order_sizes[MAX_ORDERS], int *order_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    *rule_count = 0;
    *order_count = 0;
    int parsing_rules = 1;

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0';

        if (strlen(line) == 0) {
            parsing_rules = 0;
            continue;
        }

        if (parsing_rules) {
            char *part1 = strtok(line, "|");
            char *part2 = strtok(NULL, "|");

            if (part1 && part2 && *rule_count < MAX_RULES) {
                rules[*rule_count].part1 = atoi(part1);
                rules[*rule_count].part2 = atoi(part2);
                (*rule_count)++;
            }
        } else {
            if (*order_count < MAX_ORDERS) {
                int order_index = *order_count;
                order_sizes[order_index] = 0;

                char *token = strtok(line, ",");
                while (token && order_sizes[order_index] < MAX_ORDER_LENGTH) {
                    orders[order_index][order_sizes[order_index]++] = atoi(token);
                    token = strtok(NULL, ",");
                }

                (*order_count)++;
            }
        }
    }

    fclose(file);
}

int reorder_order(Rule rules[MAX_RULES], int rule_count, int order[MAX_ORDER_LENGTH], int order_size, int order_index) {
    // Set global variables
    global_rules = rules;
    global_rule_count = rule_count;

    // Sort the order array
    qsort(order, order_size, sizeof(int), compare_elements);

    // Return the middle value of the reordered array
    int middle_index = (order_size - 1) / 2;
    return order[middle_index];
}


int check_order(Rule rules[MAX_RULES], int rule_count, int orders[MAX_ORDERS][MAX_ORDER_LENGTH], int order_sizes[MAX_ORDERS], int order_index) {
    for (int j = 0; j < order_sizes[order_index] - 1; j++) {
        for (int k = j + 1; k < order_sizes[order_index]; k++) {
            for (int r = 0; r < rule_count; r++) {
                if ((rules[r].part2 == orders[order_index][j]) &&
                    (rules[r].part1 == orders[order_index][k])) {
                    return reorder_order(rules, rule_count, orders[order_index], order_sizes[order_index], order_index);
                }
            }
        }
    }

    // Return the middle value of the valid order
    int middle_index = (order_sizes[order_index]-1) / 2;
    // return orders[order_index][middle_index];
    return 0;
}

int get_sum_of_middle_values(Rule rules[MAX_RULES], int rule_count, int orders[MAX_ORDERS][MAX_ORDER_LENGTH], int order_sizes[MAX_ORDERS], int order_count) {
    int sum = 0;

    for (int i = 0; i < order_count; i++) {
        int middle_value = check_order(rules, rule_count, orders, order_sizes, i);
        sum += middle_value;  // Only valid middle values are added
    }

    return sum;
}

int main() {
    const char *fname = "day5_input.txt";

    Rule rules[MAX_RULES];
    int orders[MAX_ORDERS][MAX_ORDER_LENGTH];
    int order_sizes[MAX_ORDERS];
    int rule_count = 0;
    int order_count = 0;

    read_parse(fname, rules, &rule_count, orders, order_sizes, &order_count);
    int sum_of_middle_values = get_sum_of_middle_values(rules, rule_count, orders, order_sizes, order_count);
    printf("Sum of middle values of valid orders: %d\n", sum_of_middle_values);

    return 0;
}
