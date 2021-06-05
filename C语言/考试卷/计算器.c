#include<stdio.h>
#include<stdlib.h>
double operate_if(double a, double b, char operation);
double operate_switch(double a, double b, char operation);

int main()
{
	char operation;
	int t;
	double a, b;
	while (scanf("%d %lf %lf %c", &t, &a, &b, &operation) != EOF)
	{
	    if (a==0||b==0)
	    continue;
		if (t == 0)
			printf("After if-else processing, the result is : %.2lf\n", operate_if(a, b, operation));
		else if (t == 1)
			printf("After switch processing, the result is : %.2lf\n", operate_switch(a, b, operation));
		else if (t == 2) {
			printf("After if-else processing, the result is : %.2lf\n", operate_if(a, b, operation));
			printf("After switch processing, the result is : %.2lf\n", operate_switch(a, b, operation));
		}
		printf("\n");
	}
	system("pause");
	return 0;
}

double operate_if(double a, double b, char operation)
{
	if (operation == '+')
		return a + b;
	else if (operation == '-')
		return a - b;
	else if (operation == '/')
		return a / b;
	else if (operation == '*')
		return a * b;
}

double operate_switch(double a, double b, char operation)
{
	switch (operation)
	{
	case '+':return a + b;
	case '-':return a - b;
	case '/':return a / b;
	case '*':return a * b;
	}
}