#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define check printf("Line number %d", __LINE__);

int regs[4] = {0, 0, 0, 0};
int number = 0;

int first_reg(char x)
{
    switch (x) {
        case 'a':
        {
            return 0;
            break;
        }
        case 'b':
        {
            return 1;
            break;
        }
        case 'c':
        {
            return 2;
            break;
        }
        case 'd':
        {
            return 3;
            break;
        }
        default :
        {
            return -1;
        }
    }
}

int second_reg(char y[4], int i)
{
    int j = 0, senior = 0;
    number = 0;
    if(y[0] <= '9' && y[0] >= '0' || y[0] == '-')
    {
        if(y[0] == '-')
        {
            senior = 1;
        }
        for (i; i >= senior; i--)
        {
            number += (y[i] - '0') * pow(10, j);
            j++;
        }
        if(senior == 1)
        {
            number *= -1;
        }
        return 0;
    }
    else
    {
        switch (y[1]) {
            case 'a':
            {
                number =  regs[0];
                return 0;
            }
            case 'b':
            {
                number =  regs[1];
                return 0;
            }
            case 'c':
            {
                number =  regs[2];
                return 0;
            }
            case 'd':
            {
                number =  regs[3];
                return 0;
            }
            default :
            {
                return -1;
            }
        }
    }
}

int the_command(int c)
{
    switch (c) {
        case 'm':
        {
            return 1;
            break;
        }
        case 'a':
        {
            return 2;
            break;
        }
        case 's':
        {
            return 3;
            break;
        }
        case 'r':
        {
            return 0;
            break;
        }
        default :
        {
            return -1;
            break;
        }
    }
}

int function(int first, int second, int func)
{
    switch (func) {
        case 1:
        {
            regs[first] = second;
            return 0;
            break;
        }
        case 2:
        {
            regs[first] += second;
            return 0;
            break;
        }
        case 3:
        {
            regs[first] -= second;
            return 0;
            break;
        }
        default :
        {
            return -1;
            break;
        }
    }
}

int main()
{
    
    int com, reg1, reg2, i, quit = 1, error;
    char command[4] = "", register1[4] = "", register2[4] = "";
    char * p;
    printf("Type 'ret' to quit\n");
    while (1)
    {
        com = 0;
        reg1 = 0;
        reg2 = 0;
        i = 0;
        for (com = getchar(); com != ' ' && com != '\n'; )
	    {
		    command[i] = com;
		    i++;
		    com = getchar();
	    }
        if(command[0] == 'r')
        {
            break;
        }
        i = 0;
        for (reg1 = getchar(); reg1 != ','; )
	    {
	    	register1[i] = reg1;
	    	i++;
	    	reg1 = getchar();
	    }
        i = 0;
        for (reg2 = getchar(); reg2 != '\n'; )
	    {
	    	register2[i] = reg2;
	    	i++;
	    	reg2 = getchar();
	    }
        reg1 = first_reg(register1[1]);
        reg2 = second_reg(register2, i - 1);
        com = the_command(command[0]);
        if(com == -1)
        {
            printf("Unknown command");
        }
        else if(reg1 == -1)
        {
            printf("Unknown first register\n");
        }
        else if(reg2 == -1)
        {
            printf("Unknown second register\n");
        }
        else
        {
            error = function(reg1, number, com);
        }
        
        if(error == 0)
        {
            printf("%d %d %d %d\n", regs[0], regs[1], regs[2], regs[3]);
        }
        else
        {
            printf("Something went wrong :/\n");
        }
    }
    return 0;
}
