#include "lexer.h"

#define CHAR_TOKEN(ch, tk) case ch: addToken(scn, tk); break;

void tokenListInit(TokenList *list)
{
    list->i = 0;
    list->len = 0;
    list->data = NULL;
}

void tokenListPush(TokenList *list, Token t)
{
    if (list->i == list->len)
    {
        // double
        size_t realloc_size = list->len ? 2*list->len : 8;
        list->data = realloc(list->data, realloc_size * sizeof(Token));
        list->len = realloc_size;
    }
    list->data[list->i++] = t;
}

void tokenListFree(TokenList *list)
{
    for (size_t i = 0; i < list->i; i++)
        free(list->data[i].lexeme);
    free(list->data);
    tokenListInit(list); // probably unnecessary?
    free(list);
}

void addToken(Scanner *scn, TokenType type)
{
    char *lexeme = malloc(scn->cur - scn->beg + 1);
    memcpy(lexeme, &(scn->source[scn->beg]), scn->cur - scn->beg);
    lexeme[scn->cur - scn->beg] = '\0';

    Token t = { /*.len = scn->cur - scn->beg,*/ .lexeme = lexeme, .line = scn->line, .type = type };
    tokenListPush(scn->list, t);
}

int matchNext(char expected, Scanner *scn)
{
    char cur_char = scn->source[scn->cur];
    if (cur_char == '\0' || cur_char != expected)
        return 0;
    scn->cur++;
    return 1;
}

void scanTokens(Scanner *scn, char *src)
{
    scn->list = malloc(sizeof(TokenList));
    if (scn->list == NULL)
        printf("TokenList malloc failed\n");
    tokenListInit(scn->list);

    scn->beg = 0;
    scn->cur = 0;
    scn->line = 1;
    scn->source = src;

    //while (scn->cur < strlen(scn->source)) // check null terminator?
    
    while (scn->source[scn->cur] != '\0')
    {
        scn->beg = scn->cur;
        char c = scn->source[scn->cur++];
        switch (c)
        {
            CHAR_TOKEN('(', TKN_LEFT_PAREN)
            CHAR_TOKEN(')', TKN_RIGHT_PAREN)
            CHAR_TOKEN('{', TKN_LEFT_BRACE)
            CHAR_TOKEN('}', TKN_RIGHT_BRACE)
            CHAR_TOKEN(',', TKN_COMMA)
            CHAR_TOKEN('.', TKN_DOT)
            CHAR_TOKEN('-', TKN_MINUS)
            CHAR_TOKEN('+', TKN_PLUS)
            CHAR_TOKEN(';', TKN_SEMICOLON)
            CHAR_TOKEN('*', TKN_STAR)
            CHAR_TOKEN('!', matchNext('=', scn) ? TKN_BANG_EQUAL : TKN_BANG)
            CHAR_TOKEN('=', matchNext('=', scn) ? TKN_BANG_EQUAL : TKN_EQUAL)
            CHAR_TOKEN('<', matchNext('=', scn) ? TKN_LESS_EQUAL : TKN_LESS)
            CHAR_TOKEN('>', matchNext('=', scn) ? TKN_GREATER_EQUAL : TKN_GREATER)
            
            case '/':
                if (matchNext('/', scn))
                {
                    while (c != '\n' && c != '\0') c = scn->source[++scn->cur];
                } else {
                    addToken(scn, TKN_SLASH);
                }
                break;
            
            case '"':
                c = scn->source[scn->cur];
                while (c != '"' && c != '\0')
                {
                    if (c == '\n') scn->line++;
                    c = scn->source[++scn->cur];
                }
                if (c == '\0')
                {
                    printf("[line %d] Unterminated string\n", scn->line);
                    return;
                }
                scn->cur++; // consume ending "
                char *str_val = malloc(scn->cur - scn->beg - 1); //-2 + 1
                memcpy(str_val, &(scn->source[scn->beg+1]), scn->cur - scn->beg - 2);
                str_val[scn->cur - scn->beg-2] = '\0';

                break;
            
            // whitespace
            case '\n': scn->line++; break;
            case '\r': break; // CR not recognised, only LF or CRLF
            case '\t': break;
            case ' ': break;
            default:
                printf("[line %d] Unexpected character: %c\n", scn->line, c);
                break;
        }
    }

    Token t = {.type = TKN_EOF, .lexeme = malloc(1), /*.len = 0,*/ .line = scn->line };
    t.lexeme[0] = '\0';

    tokenListPush(scn->list, t);
}

void scannerFree(Scanner *scn) {
    tokenListFree(scn->list);
    free(scn->list);
}