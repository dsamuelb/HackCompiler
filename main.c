#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "string.h"

typedef struct {
    char* cmd;
    int line;
}Tokens;

enum BinTypes {
    M = 0b001,
    D = 0b010,
    A = 0b100
};

typedef struct {
    char* str;
    int corr;
}CmdStruct;

CmdStruct strarr[] = {
    {"0", 0b0101010},
    {"1", 0b0111111},
    {"-1", 0b0111010},
    {"D", 0b0001100},
    {"A", 0b0110000},
    {"!D", 0b0001101},
    {"!A", 0b0110001},
    {"-D", 0b0001111},
    {"-A", 0b0110011},
    {"D+1", 0b0011111},
    {"A+1", 0b0110111},
    {"D-1", 0b0001110},
    {"A-1", 0b0110010},
    {"D+A", 0b0000010},
    {"D-A", 0b0010011},
    {"A-D", 0b0000111},
    {"D&A", 0b0000000},
    {"D|A", 0b0010101},
    {"M", 0b1110000},
    {"!M", 0b1110001},
    {"-M", 0b1110011},
    {"M+1", 0b1110111},
    {"M-1", 0b1110010},
    {"D+M", 0b1000010},
    {"D-M", 0b1010011},
    {"M-D", 0b1000111},
    {"D&M", 0b1000000},
    {"D|M", 0b1010101}
};

Tokens cmds[10000];

int puthash(char* s, int strc, int linec);
void printhash(int linec);
void assemble(int linec);
void printbin(FILE* file, int num);

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
            strc++;
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
    if (cmds[cnt].cmd[0] == '@')
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

        num = num | 0b0000000000000000;

        printbin(fpr, num);

    } else
    {

        int num = 0;

        int dest = 0;
        int comp = 0;
        int jmp = 0;
        int i = -1;
        int isjmp = 0;
        int j;
        for (j = 0; j < strlen(cmds[cnt].cmd); j++)
        {
            if (cmds[cnt].cmd[j] == ';')
            {
                isjmp = 1;
                break; 
            }
        }
        if (!isjmp)
        {
        while (cmds[cnt].cmd[i++] != '=')
        {
            if (cmds[cnt].cmd[i] == 'M')
            {
                dest = dest | M;
            } else if (cmds[cnt].cmd[i] == 'A')
            {
                dest = dest | A;
            } else if (cmds[cnt].cmd[i] == 'D')
            {
                dest = dest | D;
            }
        }

        char *s = malloc(sizeof(char) * (strlen(cmds[cnt].cmd)));

        for (i = i, j = 0; i < strlen(cmds[cnt].cmd); i++, j++)
        {
            s[j] = cmds[cnt].cmd[i];
        }

        s[i] = '\0';

        int length = sizeof(strarr) / sizeof(CmdStruct);

        for (int c = 0; c < length; c++)
        {
            if (!strcmp(s, strarr[c].str))
            {
                comp = strarr[c].corr;
                break;
            }
        }

        num = 0b1110000000000000;

        num = num | ((dest << 3) | (comp << 6));

        printbin(fpr, num);
        }   
    }
    }
    fclose(fpr);
}

void printhash(int linec)
{
    for (int i = 0; i < linec; i++)
        printf("%s, %d\n", cmds[i].cmd, cmds[i].line);
}

void printbin(FILE* file, int num)
{
    for (int i = 15; i >= 0; i--)
    {
        char conv = (((num >> i) & 1) + '0');
        fputc(conv, file);
    }
    fputc('\n', file);
}
