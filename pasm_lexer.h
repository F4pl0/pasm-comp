//
// Created by strak on 10/11/2020.
//

#include <ctype.h>
#include "utils.h"

#ifndef PASM_COMP_PASM_LEXER_H
#define PASM_COMP_PASM_LEXER_H

enum TYPES {
    TOKEN_TYPE_IDENTIFIER,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_SEPARATOR,
    TOKEN_TYPE_OPERATOR,
    TOKEN_TYPE_LITERAL,
    TOKEN_TYPE_COMMENT
};

typedef struct Token_s {
    enum TYPES type;
    char *value;
} Token;

struct lexer_res {
    Token *tseq;
    int tcount;
};

const char *separators = "; ()[]{}\n\t";
const char operators[23][3] = {
        "++",
        "--",
        "+=",
        "-=",
        "*=",
        "/=",
        "<<",
        ">>",
        "+",
        "-",
        "/",
        "*",
        "%",
        "=",
        "<",
        ">",
        "!",
        "&",
        "|",
        "^",
        "~",
        "?",
        "."
};;
const char keywords[32][10] = {
        "auto",
        "break",
        "case",
        "char",
        "const",
        "continue",
        "default",
        "do",
        "double",
        "else",
        "enum",
        "extern",
        "float",
        "for",
        "goto",
        "if",
        "int",
        "long",
        "register",
        "return",
        "short",
        "signed",
        "sizeof",
        "static",
        "struct",
        "switch",
        "typedef",
        "union",
        "unsigned",
        "void",
        "volatile",
        "while"
};

// Function to determine whether a character is a separator
bool issep(int c) {
    for (int i = 0; i < strlen(separators); ++i) {
        if (c == separators[i])
            return true;
    }
    return false;
}

// Function to determine if a character is a operator
bool isopc(int c) {
    for (int i = 0; i < sizeof(operators)/3; ++i) {
        if (strlen(operators[i]) == 1 && c == operators[i][0])
            return true;
    }
    return false;
}

// Function to determine whether certain combination of
// operators is indeed a valid operator such as +=, <<, *= etc..
bool isop(char* c) {
    for (int i = 0; i < sizeof(operators)/3; ++i) {
        bool match = true;
        for (int j = 0; j < strlen(operators[i]); ++j) {
            if (operators[i][j] != c[j]) {
                match = false;
                break;
            }
        }
        if (match)
            return true;
    }
    return false;
}

// Function to determine whether a certain combination of
// characters are a valid keyword
bool iskw(char *w) {

    for (int i = 0; i < 32; ++i) {
        if (strlen(keywords[i]) != strlen(w))
            continue;
        bool pass = true;
        for (int j = 0; j < sizeof(w); ++j) {
            if (w[j] != keywords[i][j]) {
                pass = false;
                break;
            }
        }
        if (pass)
            return true;
    }
    return false;
}

// Function to determine whether a certain combination of
// characters are a valid identifier
bool isiden(char *w) {
    if (strlen(w) < 1)
        return false;
    if (!(isalpha(w[0]) || w[0] == '_'))
        return false;
    for (int i = 1; i < strlen(w); ++i) {
        if (!(isalpha(w[i]) || w[i] == '_'))
            return false;
    }
    return true;
}


// Function to determine whether a certain combination of
// characters are a valid literal
bool islit(char *w) {
    // Check if its a dec digit
    bool isDecDig = true;
    for (int i = 0; i < strlen(w); ++i) {
        if (!isdigit(w[i])) {
            isDecDig = false;
            break;
        }
    }
    if (isDecDig)
        return true;
    // TODO Check for Octal
    // TODO Check for Hexadecimal
    // TODO Check for Float
    // TODO Check for Char
    // TODO Check for String
    return false;
}

struct lexer_res pasm_tokenize(char *buffer, int buf_len) {
    buf_len--;
    int current_len = 0;
    int t_count = 0;
    int current_line = 1;
    int current_char = 1;

    Token *tseq = (Token *) malloc(sizeof(Token));

    while (current_len < buf_len) {

        tseq = realloc(tseq, (t_count + 1) * sizeof(Token));
        char *value = NULL;

        if (isalnum(buffer[current_len]) || buffer[current_len] == '_') {
            // is Text
            int i = 0;
            while (isalnum(buffer[current_len + i]) || buffer[current_len + i] == '_') {
                i++;
                if ((current_len + i + 1) >= buf_len) {
                    break;
                }
            }

            // Assign the text to the value
            value = (char *) malloc((i + 1) * sizeof(char));
            memset(value, '\0', i + 1);
            for (int j = 0; j < i; ++j) {
                value[j] = buffer[current_len + j];
            }

            // Update the current_len
            current_len += i;

            if (iskw(value)) {
                // is Keyword
                tseq[t_count].type = TOKEN_TYPE_KEYWORD;
            } else if (isiden(value)) {
                // is Identifier
                tseq[t_count].type = TOKEN_TYPE_IDENTIFIER;
            } else if (islit(value)) {
                // is Literal
                tseq[t_count].type = TOKEN_TYPE_LITERAL;
            }

        }
            //#####################
            //     //COMMENT
            //#####################
        else if ((current_len + 1) < buf_len &&
                 (buffer[current_len] == '/' && buffer[current_len + 1] == '/')) {
            // is Comment
            int i = 1;
            bool isNewLine = true;

            while (buffer[current_len + i] != '\n') {
                i++;
                if ((current_len + i + 1) >= buf_len) {
                    isNewLine = false;
                    break;
                }
            }

            if (isNewLine)
                current_line++;

            // Assign the text to the value
            value = (char *) malloc((i + 1) * sizeof(char));
            memset(value, '\0', i + 1);
            for (int j = 0; j < i; ++j) {
                value[j] = buffer[current_len + j];
            }

            // Update the current_len
            current_len += i;
            tseq[t_count].type = TOKEN_TYPE_COMMENT;
        }
            //#####################
            //     /*COMMENT*/
            //#####################
        else if ((current_len + 1) < buf_len &&
                 (buffer[current_len] == '/' && buffer[current_len + 1] == '*')) {
            // is the /*  */ variant of comment

            int i = 2;
            while (true){
                while (buffer[current_len + i] != '*') {
                    if (buffer[current_len + i] == '\n')
                        current_line++;
                    i++;
                    if ((current_len + i + 1) >= buf_len) {
                        break;
                    }
                }
                if(current_len + i + 1 >= buf_len)
                    break;
                if(buffer[current_len + i + 1] == '/') {
                    i += 2;
                    break;
                }
                i++;
            }

            // Assign the text to the value
            value = (char *) malloc((i + 1) * sizeof(char));
            memset(value, '\0', i + 1);
            for (int j = 0; j < i; ++j) {
                value[j] = buffer[current_len + j];
            }
            printf("%s",value);
            // Update the current_len
            current_len += i;
            tseq[t_count].type = TOKEN_TYPE_COMMENT;
        }
            //###################
            //     OPERATOR
            //###################
        else if (isopc(buffer[current_len])) {
            // is Operator
            int i = 0;
            while (isopc(buffer[current_len + i])) {
                i++;

                if (i == 2) {
                    char *temp_op = (char *) malloc(3);
                    memset(temp_op, '\0', i + 1);
                    temp_op[0] = buffer[current_len];
                    temp_op[1] = buffer[current_len + 1];
                    if (isop(temp_op)) {
                        break;
                    } else {
                        i = 1;
                        break;
                    }
                }
                if ((current_len + i) >= buf_len) {
                    break;
                }
            }

            // Assign the text to the value
            value = (char *) malloc((i + 1) * sizeof(char));
            memset(value, '\0', i + 1);

            for (int j = 0; j < i; ++j) {
                value[j] = buffer[current_len + j];
            }

            // Update the current_len
            current_len += i;
            tseq[t_count].type = TOKEN_TYPE_OPERATOR;
        }
            //#####################
            //       SEPARATOR
            //#####################
        else if (issep(buffer[current_len])) {
            // is Separator
            int i = 0;
            char sep = buffer[current_len];
            while (buffer[current_len + i] == sep) {
                i++;

                if (sep == '\n')
                    current_line++;

                if ((current_len + i) >= buf_len) {
                    break;
                }
            }

            // Assign the text to the value
            value = (char *) malloc(2 * sizeof(char));
            memset(value, '\0', 2);
            value[0] = sep;

            // Update the current_len
            current_len += i;
            tseq[t_count].type = TOKEN_TYPE_SEPARATOR;
        } else {
            // Unknown character, throw error!
            printe("Unknown token at line %d: %c\n",current_line, buffer[current_len]);
            exit(1);
        }
        tseq[t_count].value = value;
        t_count++;
    }
    struct lexer_res lres;
    lres.tseq = tseq;
    lres.tcount = t_count;
    return lres;
}

#endif //PASM_COMP_PASM_LEXER_H