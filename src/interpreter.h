// interpreter.h
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include "packet.h" // Use the real Packet struct

// The new eval function signature
void eval(ASTNode *node, Packet* packet);

#endif
