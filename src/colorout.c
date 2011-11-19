
#include <R.h>  /* to include Rconfig.h */

#ifdef ENABLE_NLS
#include <libintl.h>
#define _(String) dgettext ("colorout", String)
#else
#define _(String) (String)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Rinterface.h>

#define R_INTERFACE_PTRS 1

extern void *ptr_R_WriteConsole;
extern void *ptr_R_WriteConsoleEx;

static void *save_R_Outputfile;
static void *save_R_Consolefile;

static char crnormal[16], crnumber[16], crstring[16],
     crconst[16], crstderr[16], crwarn[16], crerror[16];
static int normalsize, numbersize, stringsize, constsize;

void colorout_initColors()
{
    strcpy(crnormal, "\033[32m");
    strcpy(crnumber, "\033[33m");
    strcpy(crstring, "\033[36m");
    strcpy(crconst,  "\033[35m");
    strcpy(crstderr, "\033[34m");
    strcpy(crwarn,  "\033[1;31m");
    strcpy(crerror,  "\033[41;37m");
    normalsize = 5;
    numbersize = 5;
    stringsize = 5;
    constsize  = 5;
}

void colorout_SetColors(int *normal, int *number, int *string, int *constant, int *stderror, int *warn, int *error)
{
    /* We use a tmpbuffer because the value of 'normal', 'number', 'string',
     * and 'constant' are set up in two steps. */
    char tmpnormal[16];
    char tmpnumber[16];
    char tmpstring[16];
    char tmpconst[16];

    /* There is no need of adding the first element if its value is 0 */

    if(error[0] == 0)
        snprintf(crerror, 12, "\033[%dm", error[1]);
    else
        snprintf(crerror, 12, "\033[%d;%dm", error[0], error[1]);

    if(warn[0] == 0)
        snprintf(crwarn, 12, "\033[%dm", warn[1]);
    else
        snprintf(crwarn, 12, "\033[%d;%dm", warn[0], warn[1]);

    if(stderror[0] == 0)
        snprintf(crstderr, 12, "\033[%dm", stderror[1]);
    else
        snprintf(crstderr, 12, "\033[%d;%dm", stderror[0], stderror[1]);

    if(normal[0] == 0)
        snprintf(tmpnormal, 12, "\033[%dm", normal[1]);
    else
        snprintf(tmpnormal, 12, "\033[%d;%dm", normal[0], normal[1]);

    if(number[0] == 0)
        snprintf(tmpnumber, 12, "\033[%dm", number[1]);
    else
        snprintf(tmpnumber, 12, "\033[%d;%dm", number[0], number[1]);

    if(string[0] == 0)
        snprintf(tmpstring, 12, "\033[%dm", string[1]);
    else
        snprintf(tmpstring, 12, "\033[%d;%dm", string[0], string[1]);

    if(constant[0] == 0)
        snprintf(tmpconst, 12, "\033[%dm", constant[1]);
    else
        snprintf(tmpconst, 12, "\033[%d;%dm", constant[0], constant[1]);

    /* The format of 'normal', 'string', 'number', and 'constant' must be
     * stopped before starting a new format. This would not be necessary if we
     * have the '0' in the code above. */
    if(normal[0] > 0 || string[0] > 0 || constant[0] > 0)
        snprintf(crnumber, 15, "\033[0m%s", tmpnumber);
    else
        strcpy(crnumber, tmpnumber);

    if(number[0] > 0 || string[0] > 0 || constant[0] > 0)
        snprintf(crnormal, 15, "\033[0m%s", tmpnormal);
    else
        strcpy(crnormal, tmpnormal);
    
    if(number[0] > 0 || normal[0] > 0 || constant[0] > 0)
        snprintf(crstring, 15, "\033[0m%s", tmpstring);
    else
        strcpy(crstring, tmpstring);
    
    if(number[0] > 0 || normal[0] > 0 || string[0] > 0)
        snprintf(crconst, 15, "\033[0m%s", tmpconst);
    else
        strcpy(crconst, tmpconst);
    
    normalsize = strlen(crnormal);
    numbersize = strlen(crnumber);
    stringsize = strlen(crstring);
    constsize = strlen(crconst);
}

char *colorout_make_bigger(char *ptr, int *len)
{
    char *nnbuf;
    *len = *len + 1024;
    nnbuf = (char*)calloc(1, *len);
    strcpy(nnbuf, ptr);
    free(ptr);
    *len = *len - 64;
    return(nnbuf);
}

void colorout_R_WriteConsoleEx (const char *buf, int len, int otype)
{
    char *newbuf, *bbuf;
    char piece[64];
    int neednl, i, j, l;

    /* gnome-terminal extends the background color for the other line
     * if the "\033[0m" is after the newline*/
    neednl = 0;
    bbuf = (char*)malloc((len+1) * sizeof(char));
    strcpy(bbuf, buf);
    if(buf[len - 1] == '\n'){
        neednl = 1;
        bbuf[len - 1] = 0;
        len -= 1;
    }

    /* It would be better if 'otype' was passed with more than two possible
     * values (0 and 1). It would be nice if it had at least four values:
     * 0 = Normal output (should be sent to stdout)
     * 1 = Information
     * 2 = Warning
     * 3 = Error
     * */
    if(otype){
        /* The value of otype does not tell whether the message is
         * Information, Warning or Error. So, we have to guess: */
        char *hasWarn = strstr(buf, _("Warning"));
        if(hasWarn == NULL)
            hasWarn = strstr(buf, _("WARNING"));
        if(hasWarn == NULL)
            hasWarn = strstr(buf, "Warning");
        if(hasWarn == NULL)
            hasWarn = strstr(buf, "WARNING");

        char *hasError = strstr(buf, _("Error"));
        if(hasError == NULL)
            hasError = strstr(buf, _("ERROR"));
        if(hasError == NULL)
            hasError = strstr(buf, "Error");
        if(hasError == NULL)
            hasError = strstr(buf, "ERROR");

        /* Print the message */
        if(hasWarn == buf)
            fprintf(stderr, "%s%s\033[0m", crwarn, bbuf);
        else if(hasError == buf)
            fprintf(stderr, "%s%s\033[0m", crerror, bbuf);
        else
            fprintf(stderr, "%s%s\033[0m", crstderr, bbuf);

        /* Put the newline back */
        if(neednl)
            fprintf(stderr, "\n");

        fflush(stderr);
    } else {
        l = len + 1024;
        newbuf = (char*)calloc(sizeof(char), l);
        l -= 16;
        strcpy(newbuf, crnormal);
        i = 0;
        j = normalsize;
        while(i < len){
            if(j >= l)
                newbuf = colorout_make_bigger(newbuf, &l);
            if(bbuf[i] == '"'){
                strcat(newbuf, crstring);
                j += stringsize;
                newbuf[j] = bbuf[i];
                i++;
                j++;
                while(i < len && bbuf[i] != '\n'){
                    if(j >= l)
                        newbuf = colorout_make_bigger(newbuf, &l);
                    newbuf[j] = bbuf[i];
                    i++;
                    j++;
                    if(i > 2 && bbuf[i-1] == '"' && bbuf[i-2] != '\\')
                        break;
                }
                strcat(newbuf, crnormal);
                j += normalsize;
            } else if(bbuf[i] == 'N' && bbuf[i+1] == 'U' && bbuf[i+2] == 'L' && bbuf[i+3] == 'L'
                    && (bbuf[i+4] < 'A' || (bbuf[i+4] > 'Z' && bbuf[i+4] < 'a') || bbuf[i+4] > 'z')){
                sprintf(piece, "%sNULL%s", crconst, crnormal);
                strcat(newbuf, piece);
                i += 4;
                j += 4 + normalsize + constsize;
            } else if(bbuf[i] == 'T' && bbuf[i+1] == 'R' && bbuf[i+2] == 'U' && bbuf[i+3] == 'E'
                    && (bbuf[i+4] < 'A' || (bbuf[i+4] > 'Z' && bbuf[i+4] < 'a') || bbuf[i+4] > 'z')){
                sprintf(piece, "%sTRUE%s", crconst, crnormal);
                strcat(newbuf, piece);
                i += 4;
                j += 4 + normalsize + constsize;
            } else if(bbuf[i] == 'F' && bbuf[i+1] == 'A' && bbuf[i+2] == 'L' && bbuf[i+3] == 'S' && bbuf[i+4] == 'E'
                    && (bbuf[i+5] < 'A' || (bbuf[i+5] > 'Z' && bbuf[i+5] < 'a') || bbuf[i+5] > 'z')){
                sprintf(piece, "%sFALSE%s", crconst, crnormal);
                strcat(newbuf, piece);
                i += 5;
                j += 5 + normalsize + constsize;
            } else if(bbuf[i] == 'N' && bbuf[i+1] == 'A'
                    && (bbuf[i+2] < 'A' || (bbuf[i+2] > 'Z' && bbuf[i+2] < 'a') || bbuf[i+2] > 'z')){
                sprintf(piece, "%sNA%s", crconst, crnormal);
                strcat(newbuf, piece);
                i += 2;
                j += 2 + normalsize + constsize;
            } else if(bbuf[i] == 'I' && bbuf[i+1] == 'n' && bbuf[i+2] == 'f'
                    && (bbuf[i+3] < 'A' || (bbuf[i+3] > 'Z' && bbuf[i+3] < 'a') || bbuf[i+3] > 'z')){
                if(i > 0 && bbuf[i-1] == '-'){
                    newbuf[j-1] = 0;
                    sprintf(piece, "%s-Inf%s", crconst, crnormal);
                    strcat(newbuf, piece);
                } else {
                    sprintf(piece, "%sInf%s", crconst, crnormal);
                    strcat(newbuf, piece);
                }
                i += 3;
                j += 3 + normalsize + constsize;
            } else if(bbuf[i] == 'N' && bbuf[i+1] == 'a' && bbuf[i+2] == 'N'
                    && (bbuf[i+3] < 'A' || (bbuf[i+3] > 'Z' && bbuf[i+3] < 'a') || bbuf[i+3] > 'z')){
                sprintf(piece, "%sNaN%s", crconst, crnormal);
                strcat(newbuf, piece);
                i += 3;
                j += 3 + normalsize + constsize;
            } else if(bbuf[i] >= '0' && bbuf[i] <= '9' || (bbuf[i] == '-' && bbuf[i+1] >= '0' && bbuf[i+1] <= '9')){
                strcat(newbuf, crnumber);
                j += numbersize;
                while(i < len && ((bbuf[i] >= '0' && bbuf[i] <= '9') || bbuf[i] == '.' || bbuf[i] == '-')){
                    if(j >= l)
                        newbuf = colorout_make_bigger(newbuf, &l);
                    newbuf[j] = bbuf[i];
                    i++;
                    j++;
                }
                strcat(newbuf, crnormal);
                j += normalsize;
            } else {
                newbuf[j] = bbuf[i];
                i++;
                j++;
            }
        }

        
        if(neednl)
            printf("%s\033[0m\n", newbuf);
        else
            printf("%s\033[0m", newbuf);

        free(newbuf);
        fflush(stdout);
    }
    free(bbuf);
}

void colorout_ColorOutput()
{
    colorout_initColors();
    save_R_Outputfile = R_Outputfile;
    save_R_Consolefile = R_Consolefile;
    R_Outputfile = NULL;
    R_Consolefile = NULL;
    ptr_R_WriteConsoleEx = colorout_R_WriteConsoleEx;
    ptr_R_WriteConsole = NULL;
}

void colorout_noColorOutput()
{
    R_Outputfile = save_R_Outputfile;
    R_Consolefile = save_R_Consolefile;
}
