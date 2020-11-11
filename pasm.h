//
// Created by strak on 10/11/2020.
//

#include "pasm_lexer.h"

#ifndef PASM_COMP_PASM_H
#define PASM_COMP_PASM_H

#endif //PASM_COMP_PASM_H

void pasm_compile(char *buffer, int buf_len) {

    struct lexer_res lres = pasm_tokenize(buffer, buf_len);
}