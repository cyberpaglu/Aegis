// interpreter.h
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "packet.h"

void evaluate_rule(AST *rule, Packet *pkt);

#endif

