#ifndef __L2EXCEPTION_H__
#define __L2EXCEPTION_H__

#include <exception>
#include <string>

namespace l2max
{
	class CException : public std::exception
	{
	   std::string _what;
	   
	public:
		CException( const std::string& aWhat );
		virtual ~CException()throw();
		
		virtual const char* what()const throw();
	};
}

#endif
