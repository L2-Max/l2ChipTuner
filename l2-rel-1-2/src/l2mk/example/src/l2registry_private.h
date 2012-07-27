#ifndef __L2_REGISTRY_PRIVATE_H__
#define __L2_REGISTRY_PRIVATE_H__

#include "l2registry_parser_api.h"
#include "l2variant.h"

#include <vector>

using namespace l2;

struct SValue{
	enum EType{
		tChar,
		tInteger,
		tReal,
		tString,
		tBinary,
	};
	
	EType type;

	std::string name;
	CVariant data;
};

struct SScope{
	std::string name;

	typedef std::vector<SScope*> TScopeCnt;
	typedef std::vector<SValue*> TValueCnt;

	TScopeCnt scopeCnt;
	TValueCnt valueCnt;

	SScope(const char* n="::"){
		name=n;
	}
};

#endif