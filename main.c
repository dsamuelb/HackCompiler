#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "string.h"

typedef struct {
    char* cmd;
    int line;
}Tokens;

enum BinTypes {
    M = 001,
    D = 010,
    A = 100
};

Tokens cmds[10000];

int puthash(char* s, int strc, int linec);
void printhash(int linec);
void assemble(int linec);

int main(void)
{

    char c;
    int linec = 0;
    int strc = 0;

    FILE* fpr;

    char str[1000];

    fpr = fopen("Prog.asm", "r");

    if (fpr == NULL)
    {
        fprintf(stderr, "Failed to open file\n");
        exit(1);
    }

    while ((c = fgetc(fpr)) != EOF)
    {
        if (c != '\n')
        {
            *(str + strc) = c;
            strc++;
        } else {
            *(str + strc) = '\0';
            linec = puthash(str, strc, linec);
            linec++;
            strc = 0;
        }
    }
    linec = puthash(str, strc, linec);
    linec++;

    fclose(fpr);
    printhash(linec);
    assemble(linec);

    return 0;
}

int puthash(char* s, int strc, int linec)
{
    if (*s == 'M' || *s == 'D' || *s == 'A' || *s == '@' || *s == 'J' || *s == '0')
    {
        int i;
        cmds[linec].cmd = malloc(strc * sizeof(char));

        for (i = 0; i < strc; i++)
        {
            cmds[linec].cmd[i] = *(s+i);
        }

        cmds[linec].cmd[i] = '\0';
        cmds[linec].line = linec;
    }  else {
        return --linec;
    }

    return linec;
}

void assemble(int linec)
{
    FILE* fpr;

    fpr = fopen("Prog.hack", "w");

    if (fpr == NULL)
    {
        fprintf(stderr, "Failed to create file\n");
        exit(1);
    }
    for (int cnt = 0; cnt < linec; cnt++)
    {
    if (cmds[0].cmd[0] == '@')
    {
        int bin = 0b0000000000000000;
        int i = 0;
        int num = 0;

        while(!isdigit(cmds[cnt].cmd[i++]))
        ;

        i--;
        
        for (int j=i; j < strlen(cmds[cnt].cmd); j++)
        {   
            num = num * 10 + ((cmds[cnt].cmd[j] - '0'));
        }

        for (int j = 14; j >= 0; j--)
        {
            bin = bin << 1;
            bin = bin | ((num >> j) & 1);
        }

        bin = bin | 0b1000000000000000;

        fprintf(fpr, "%b\n", bin);

    } else
    {

    }
    }
    fclose(fpr);
}

void printhash(int linec)
{
    for (int i = 0; i < linec; i++)
        printf("%s, %d\n", cmds[i].cmd, cmds[i].line);
}