#include "l2Exception.h"
#include "l2Trace.h"

namespace l2max
{
	CException::CException( const std::string& aWhat ) : _what( aWhat ){}
	CException::~CException()throw(){}
	
	const char* CException::what()const throw()
	{
	   return _what.c_str();
	}
}
