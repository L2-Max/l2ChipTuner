#ifndef __L2REGISTRY_PARSER_API_H__
#define __L2REGISTRY_PARSER_API_H__

#include "l2registrydefs.h"

struct SNode* add_identifier(const char* id);
struct SNode* add_integer(int i);
struct SNode* add_hex(int h,struct SNode* p);
struct SNode* add_real(double d);
struct SNode* add_char(char c);
struct SNode* add_string(const char* s,struct SNode* p);
struct SNode* add_value(struct SNode* lv,struct SNode* rv);
struct SNode* add_scope(const char* n,struct SNode* n1,struct SNode* n2);
void save_scopes(struct SNode* n);

#endif
