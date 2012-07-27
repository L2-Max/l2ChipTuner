#ifndef __L2REGISTRYDEFS_H__
#define __L2REGISTRYDEFS_H__

#define MAX_ID_NAME_LEN		256
#define MAX_SCOPE_NAME_LEN	256
#define MAX_STRING_BUF_LEN	8192

enum ENodeType{
	tIdentifier,
	tInteger,
	tReal,
	tBinary,
	tChar,
	tString,
	tValue,
	tScope
};

struct SNode{
	enum ENodeType type;
	void* data;	
};

#endif