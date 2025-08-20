// implements lexer, tokens
#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef enum {
    // Single-character tokens.
    TKN_LEFT_PAREN, TKN_RIGHT_PAREN, TKN_LEFT_BRACE, TKN_RIGHT_BRACE,
    TKN_COMMA, TKN_DOT, TKN_MINUS, TKN_PLUS, TKN_SEMICOLON, TKN_SLASH, TKN_STAR,

    // One or two character tokens.
    TKN_BANG, TKN_BANG_EQUAL,
    TKN_EQUAL, TKN_EQUAL_EQUAL,
    TKN_GREATER, TKN_GREATER_EQUAL,
    TKN_LESS, TKN_LESS_EQUAL,

    // Literals.
    TKN_IDENTIFIER, TKN_STRING, TKN_NUMBER,

    // Keywords.
    TKN_AND, TKN_CLASS, TKN_ELSE, TKN_FALSE, TKN_FUN, TKN_FOR, TKN_IF, TKN_NIL, TKN_OR,
    TKN_PRINT, TKN_RETURN, TKN_SUPER, TKN_THIS, TKN_TRUE, TKN_VAR, TKN_WHILE,

    TKN_EOF // EOF is #define'd already
} TokenType;

typedef enum {TYPE_NUM, TYPE_STR, TYPE_NULL} TypeTag;

typedef union {
    double val_double;
    char *val_string;
    void *null;
} LiteralVal;

typedef struct {
    TypeTag type;
    LiteralVal value;
    int len; // for str
} Literal;
// tagged union to store literals
// only 'number' and 'string' literals possible for now
// number = double

typedef struct {
    TokenType type;
    char *lexeme;
    int line;
    Literal literal;
} Token;

typedef struct {
    Token *data;
    size_t i; // currently present
    size_t len; // max capacity
} TokenList;

typedef struct {
    TokenList *list;
    char *source;
    int cur; // size_t better?
    int beg;
    int line;
} Scanner;

void tokenListInit(TokenList *list);
void tokenListPush(TokenList *list, Token t);
void tokenListFree(TokenList *list);

void scanTokens(Scanner *scn, char *src);
void scannerFree(Scanner *scn);