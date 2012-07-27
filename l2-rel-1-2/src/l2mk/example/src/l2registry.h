#ifndef __L2REGISTRY_H__
#define __L2REGISTRY_H__

#include <map>
#include <string>

namespace l2{

	class CRegistry{

    	int fileDesc;

		std::string fileName;

	public:
    	CRegistry(const char* fn="test.rg");
    	~CRegistry();

	public:
    	int load();
		int save();
	};
}

#endif