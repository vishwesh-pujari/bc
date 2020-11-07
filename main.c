#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "infix.h"

int readLine(char**); 

int main() {
	char *str = NULL;
	int length, scale = 0, i, flag = 0;

	do {
		length = readLine(&str);

		if (strstr(str, "scale")) { // strstr() returns NULL if substring is not found
			if (str[5] == '\0') { // if user has just entered scale and nothing else
				printf("%d\n", scale); // print the existing scale
				continue;
			}

			for (i = 5; str[i]; i++) {
				if (str[i] == ' ' || str[i] == '\t') {

				} else if (str[i] == '=') {
					flag = 1;
				} else if (str[i] >= '0' && str[i] <= '9') {
					if (flag) { // if '=' has been encountered
						flag = 0;
						scale = str[i] - '0';
						if (str[i + 1] >= '0' && str[i + 1] <= '9') { // if number is multidigit
							do {
								i++;
								scale = scale * 10 + (str[i] - '0');
							} while (str[i + 1] >= '0' && str[i + 1] <= '9');
						}
					} else { // if number is encountered first than =
						printf("Invalid Scale\n");
						break;
					}
				} else {
					printf("Invalid Scale\n");
					break;
				}
			}
	
		} else if (!strcmp(str, "quit")) {
			break;

		} else if (!strcmp(str, "clear")) {
			system("clear");

		} else if (infixEval(str, length, scale) == INT_MIN) {
			printf("Invalid Expression\n");
		}

	} while (1);

	free(str);

	return 0;
}

/*
 * readLine():
 * Pre-condition:
 * 	Takes a pointer to character pointer as input
 * Post-condition:
 * 	Takes user input in the character pointer which is passed to the function
 * RETURN VALUES:
 * 	On Success it returns the number of characters read
 * 	On Failure it returns INT_MIN 
 */

int readLine(char **ptr) {
	int i;
	*ptr = (char*) malloc(sizeof(char));
	if (*ptr == NULL)
		return INT_MIN;

	for (i = 0; ((*ptr)[i] = getchar()) != '\n'; i++)
		(*ptr) = (char*) realloc(*ptr, (i + 2) * sizeof(char));	

	(*ptr)[i] = '\0';
	return i;
}