#include "infix.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

int precedence(char operator) {
	if (operator == 's' || operator == 'q' || operator == 'c' || operator == 't' || operator == 'u')
		return 50;
	if (operator == '^')
		return 40;
	if (operator == '*' || operator == '/' || operator == '%')
		return 30;
	if (operator == '+' || operator == '-')
		return 20;
	if (operator == '(')
		return 10;
	return 0;
}

int unaryEvaluate(operandStack *s2, char ch, int scale) {
	if (s2->top >= 0) {
		Num nPush, n1;
		init(&nPush);
		double res;
		char str[20];
		int len, j;
		n1 = operandPop(s2);
		length(&n1);
		switch (ch) {
			case 's': // sine
				//print(&tp);i
				res = s(n1);
				sprintf(str, "%lf", res);
				len = strlen(str);

				if (str[0] == '-') {
					nPush.sign = 1;
					j = 1;
				} else {
					j = 0;
				}
				for (; j < len; j++)
					append(&nPush, str[j]);

				length(&nPush);
				/*if (flag3) {
					if (nPush.sign == 0)
						nPush.sign = 1;
					else
						nPush.sign = 0;
				}*/
				operandPush(s2, nPush);
				deleteNum(&n1);
				break;

			case 'c': // cosine
				//print(&tp);i
				res = c(n1);
				sprintf(str, "%lf", res);
				len = strlen(str);

				if (str[0] == '-') {
					nPush.sign = 1;
					j = 1;
				} else {
					j = 0;
				}									
				for (; j < len; j++)
					append(&nPush, str[j]);

				length(&nPush);
				/*if (flag3) {
					if (nPush.sign == 0)
						nPush.sign = 1;
					else
						nPush.sign = 0;
				}*/
				operandPush(s2, nPush);
				deleteNum(&n1);
				break;

			case 't': // cosine
				//print(&tp);i
				res = t(n1);
				sprintf(str, "%lf", res);
				if (strcmp(str, "nan") == 0) {
					deleteNum(&n1);
					printf("tan(90) or tan(270) is undefined. ");	
					return INT_MIN;
				}
				len = strlen(str);

				if (str[0] == '-') {
					nPush.sign = 1;
					j = 1;
				} else {
					j = 0;
				}
				for (; j < len; j++)
					append(&nPush, str[j]);

				length(&nPush);
				/*if (flag3) {
					if (nPush.sign == 0)
						nPush.sign = 1;
					else
						nPush.sign = 0;
				}*/
				operandPush(s2, nPush);
				deleteNum(&n1);
				break;

			case 'q': // sqrt
				if (n1.sign == 1) {
					deleteNum(&n1);
					printf("Square Root of negative Number doesn't exist. ");	
					return INT_MIN;
				}

				nPush = sqroot(&n1, scale);
				/*if (flag3) {
					if (nPush.sign == 0)
						nPush.sign = 1;
					else
						nPush.sign = 0;
				}*/
				operandPush(s2, nPush);
				deleteNum(&n1);
				break;

			case 'u':
				nPush = n1;
				if (n1.sign == 0)
					nPush.sign = 1;
				else if (n1.sign == 1)
					nPush.sign = 0;
				operandPush(s2, nPush);
				break;
		}
	} else {
		printf("Too many operators and less operands. ");
		return INT_MIN;
	}

	return 1;
}

int binaryEvaluate(operandStack *s2, char ch, int scale) {
	if (s2->top >= 1) {
		Num n1, n2;
		init(&n1);
		init(&n2);
		n2 = operandPop(s2);
		length(&n2);
		n1 = operandPop(s2);
		length(&n1);
		switch (ch) {
			case '+':
				operandPush(s2, add(&n1, &n2));
				deleteNum(&n1);
				deleteNum(&n2);
				break;
			case '-':
				operandPush(s2, sub(&n1, &n2));
				deleteNum(&n1);
				deleteNum(&n2);
				break;
			case '*':
				operandPush(s2, mul(&n1, &n2));
				deleteNum(&n1);
				deleteNum(&n2);
				break;
			case '/':
				if (n2.head == NULL) {
					deleteNum(&n1);
					deleteNum(&n2);
					printf("DivisionByZero error. ");
					return INT_MIN;
				}
				operandPush(s2, divi(&n1, &n2, scale));
				deleteNum(&n1);
				deleteNum(&n2);
				break;
			case '%':
				if (n2.head == NULL) {
					deleteNum(&n1);
					deleteNum(&n2);
					printf("DivisionByZero error. ");
					return INT_MIN;
				}								
				operandPush(s2, mod(&n1, &n2, scale));
				deleteNum(&n1);
				deleteNum(&n2);
				break;
			case '^':
				if (n1.head == 0 && (n2.head == NULL || n2.sign == 1)) {
					deleteNum(&n1);
					deleteNum(&n2);
					printf("Base and Exponent cannot simultaneously be 0 and less than or equal to zero respectively. ");
					return INT_MIN;
				}

				if (n2.intCount > 10) {
					deleteNum(&n1);
					deleteNum(&n2);
					printf("Exponent too large. ");
					return INT_MIN;
				}
				operandPush(s2, powe(n1, n2, scale));
				deleteNum(&n2);
				break;
			default: // When operatorPop() returns 0 then it means operator stack is empty
				deleteNum(&n1);
				deleteNum(&n2);
				printf("Too many operands and less operators. ");
				return INT_MIN;
		}
	} else { // there are less operands and more operators
		printf("Too many operators and less operands. ");
		return INT_MIN;
	}

	return 1;
}

/*
 * int infixToPostfix(char*, int);
 * Returns INT_MIN if expression is invalid
 */

int infixEval(char *str, int len, int scale) {
	if (str[0] < '0' || str[0] > '9') {
		if (str[0] == '(' || (str[0] == '-' && ((str[1] >= '0' && str[1] <= '9') || str[1] == '(' || str[1] == 's' || str[1] == 'c' || str[1] == 'q' || str[1] == 't'))) { // if unary minus is there in the beginning

		} else if (str[0] == 's' || str[0] == 'c' || str[0] == 't') {

		} else {		
			return INT_MIN;
		}
	}
	
	operatorStack s1;
	operandStack s2;
	operatorInit(&s1, len);
	operandInit(&s2, len);

	Num n, ndel;
	init(&n);
	
	int i, flag = 0, flag1 = 0; // flag1 is for finding occurence of multiple decimal points, flag is for handling unary minus 
	char ch;
	
	for (i = 0; str[i]; i++) {
		if (str[i] >= '0' && str[i] <= '9') { // if digit or space is there then simply append to str
			append(&n, str[i]);		
			//num = num * 10 + (str[i] - '0');
			if (str[i + 1] == '.' || (str[i + 1] >= '0' && str[i + 1] <= '9')) { // if next char is also a digit
				do {
					i++;
					if (flag1 == 0 && str[i] == '.') {
						flag1 = 1;
					} else if (flag1 == 1 && str[i] == '.') { // if more than one decimal point is there in one number
						//operandDeleteStack(&s2);
						operatorDeleteStack(&s1);
						while (!operandIsEmpty(&s2)) {
							ndel = operandPop(&s2);
							deleteNum(&ndel);
						}
						operandDeleteStack(&s2);
						printf("Too many decimal points in a single number. ");
						return INT_MIN;
					}
					append(&n, str[i]);
					//num = num * 10 + (str[i] - '0');
				} while (str[i + 1] == '.' || (str[i + 1] >= '0' && str[i + 1] <= '9'));
			} 

			if (flag) { // this flag was set during unary -
				n.sign = 1;
				//num = -num;
				flag = 0;
			}

			/*if (flag2) { // for unary minus cases like -(2 + -3) = -2 + 3
				if (n.sign == 0)
					n.sign = 1;
				else
					n.sign = 0;
			}*/
			
			//length(&n);
			deleteLeadingTrailingZeroes(&n);
			//print(&n);
			operandPush(&s2, n);
			
			if (str[i + 1] == ' ' || str[i + 1] == ')' || str[i + 1] == '\0' || str[i + 1] == '\t') { 
				flag1 = 0;
				//print(&n);
				n.head = n.tail = NULL;
				n.sign = n.intCount = n.decimalCount = 0;
				//num = 0;
			} else {
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2); 
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				printf("Space or tab is mandatory after every operand. ");
				return INT_MIN;
			}

		} else if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == '%') {
			if (str[i] == '-' && (str[i + 1] >= '0' && str[i + 1] <= '9')) { // if - is unary minus 
				flag = 1;
			} else if (str[i] == '-' && str[i + 1] == '(') { // minus followed by an opening bracket is also a case of unary minus
				operatorPush(&s1, 'u');
				//flag2 = 1;
			} else if (str[i] == '-' && (str[i + 1] == 's' || str[i + 1] == 'c' || str[i + 1] == 't')) {
				operatorPush(&s1, 'u');
				//flag3 = 1;	
			} else {
				if (str[i + 1] == ' ' || str[i + 1] == '\t') {
					while (!operatorIsEmpty(&s1) && precedence(str[i]) <= precedence(operatorPeek(&s1))) {
						ch = operatorPop(&s1);
						
						if (ch == 's' || ch == 'q' || ch == 'c' || ch == 't' || ch == 'u') { // for unary operator
							if (unaryEvaluate(&s2, ch, scale) == INT_MIN) {
								operatorDeleteStack(&s1);
								while (!operandIsEmpty(&s2)) {
									ndel = operandPop(&s2);
									deleteNum(&ndel);
								}
								operandDeleteStack(&s2);
								return INT_MIN;
							}
							//flag3 = 0;
						} else { // for binary operator
							if (binaryEvaluate(&s2, ch, scale) == INT_MIN) {
								operatorDeleteStack(&s1);
								while (!operandIsEmpty(&s2)) {
									ndel = operandPop(&s2);
									deleteNum(&ndel);
								}
								operandDeleteStack(&s2);
								return INT_MIN;
							}
						}	
					}
					operatorPush(&s1, str[i]);
				} else { // if there is no space or tab after operator
					operatorDeleteStack(&s1);
					while (!operandIsEmpty(&s2)) {
						ndel = operandPop(&s2);
						deleteNum(&ndel);
					}
					operandDeleteStack(&s2);
					printf("Every operator (except unary) must be followed by a space or tab. ");
					return INT_MIN;
				}
			}
	
		} else if (str[i] == '^') { // if we come across exponential operator then simply push without any condition
			if (str[i + 1] == ' ' || str[i + 1] == '\t') {
				
				while (!operatorIsEmpty(&s1) && precedence(str[i]) < precedence(operatorPeek(&s1))) {
					ch = operatorPop(&s1);
					
					if (ch == 's' || ch == 'q' || ch == 'c' || ch == 't' || ch == 'u') { // for unary operator
						if (unaryEvaluate(&s2, ch, scale) == INT_MIN) {
							operatorDeleteStack(&s1);
							while (!operandIsEmpty(&s2)) {
								ndel = operandPop(&s2);
								deleteNum(&ndel);
							}
							operandDeleteStack(&s2);
							return INT_MIN;
						}
						//flag3 = 0;
					}
				}
				operatorPush(&s1, '^');
			} else {
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2);
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				printf("Every operator (except unary) must be followed by a space or tab. ");
				return INT_MIN;
			} 
		} else if (str[i] == '(') {
			operatorPush(&s1, '(');
		} else if (str[i] == ')') {
			while (!operatorIsEmpty(&s1) && (ch = operatorPop(&s1)) != '(') {
				if (operatorIsEmpty(&s1) && ch != '(') { // if ')' is present but '(' is not present
					operatorDeleteStack(&s1);
					while (!operandIsEmpty(&s2)) {
						ndel = operandPop(&s2);
						deleteNum(&ndel);
					}
					operandDeleteStack(&s2);
					printf("( is missing. ");
					return INT_MIN;
				}

				//flag2 = 0; // for -(
				
				if (ch == 's' || ch == 'q' || ch == 'c' || ch == 't' || ch == 'u') {
					if (unaryEvaluate(&s2, ch, scale) == INT_MIN) {
						operatorDeleteStack(&s1);
						while (!operandIsEmpty(&s2)) {
							ndel = operandPop(&s2);
							deleteNum(&ndel);
						}
						operandDeleteStack(&s2);
						return INT_MIN;
					}
					//flag3 = 0;
				} else {
					if (binaryEvaluate(&s2, ch, scale) == INT_MIN) {
						operatorDeleteStack(&s1);
						while (!operandIsEmpty(&s2)) {
							ndel = operandPop(&s2);
							deleteNum(&ndel);
						}
						operandDeleteStack(&s2);
						return INT_MIN;
					}
				}
			}

		} else if (str[i] == ' ' || str[i] == '\t') {
			// do nothing
		} else if (str[i] == 's') {
			if (str[i + 1] == 'q' && str[i + 2] == 'r' && str[i + 3] == 't' && str[i + 4] == '(') {
				operatorPush(&s1, 'q');
				i = i + 3;
			} else if (str[i + 1] == '(') {
				operatorPush(&s1, 's');
			} else {
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2);
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				printf("Invalid Character. ");
				return INT_MIN;
			}
		} else if (str[i] == 'c') {
			if (str[i + 1] == '(')
				operatorPush(&s1, 'c');
			else {
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2);
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				printf("Invalid Character. ");
				return INT_MIN;
			}
		} else if (str[i] == 't') {
			if (str[i + 1] == '(')
				operatorPush(&s1, 't');	
			else {
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2);
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				printf("Invalid Character. ");
				return INT_MIN;
			}
		} else {
			operatorDeleteStack(&s1);
			while (!operandIsEmpty(&s2)) {
				ndel = operandPop(&s2);
				deleteNum(&ndel);
			}
			operandDeleteStack(&s2);
			printf("Invalid Character. ");
			return INT_MIN;
		}
	}

	do { // pop the remaining elements from the stack
		ch = operatorPop(&s1);
		if (ch == '(') { // if '(' is found here, this means that there was a missing ')'
			operatorDeleteStack(&s1);
			while (!operandIsEmpty(&s2)) {
				ndel = operandPop(&s2);
				deleteNum(&ndel);
			}
			operandDeleteStack(&s2);
			printf(") is missing. ");
			return INT_MIN;
		}

		if (ch == 's' || ch == 'q' || ch == 'c' || ch == 't' || ch == 'u') {
			if (unaryEvaluate(&s2, ch, scale) == INT_MIN) {
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2);
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				return INT_MIN;
			}
			//flag3 = 0;
		} else {
			if (s2.top == 0 && ch == '\0') {
				// do nothing
			} else if (binaryEvaluate(&s2, ch, scale) == INT_MIN) {
				operatorDeleteStack(&s1);
				while (!operandIsEmpty(&s2)) {
					ndel = operandPop(&s2);
					deleteNum(&ndel);
				}
				operandDeleteStack(&s2);
				return INT_MIN;
			}		
		}
	} while (ch != '\0'); // operatorPop() returns '\0' when stack becomes empty

	ndel = operandPop(&s2);
	print(&ndel);
	deleteNum(&ndel);

	operatorDeleteStack(&s1);
	while (!operandIsEmpty(&s2)) {
		ndel = operandPop(&s2);
		deleteNum(&ndel);
	}
	operandDeleteStack(&s2);
	return 1;
}