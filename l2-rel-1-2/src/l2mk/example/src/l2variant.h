#ifndef __L2VARIANT_H__
#define __L2VARIANT_H__

#include "l2bytearray.h"

namespace l2{

	class CVariant{
		CByteArray data;
		mutable CByteArray dataCopy;

	public:
		enum EType{
			tInteger,
			tReal,
			tString,
			tBinary,
			tUnknown=-1
		};

	private:
		EType type;
	
	public:
		CVariant();
		CVariant(const CVariant& v);

                CVariant(const int& i);
                CVariant(const double& d);
                CVariant(const char* const& s);
                CVariant(const void* const& d,const int& s);

		~CVariant();

	public:
		void setInteger(const int& i);
		void setReal(const double& d);
		void setString(const char* const& s);
		void setBinary(const CByteArray& b);
                void setBinary(const void* const& d,const int& s);

		const int& getInteger()const;
		const double& getReal()const;
		const char* const& getString()const;
		const CByteArray& getBinary()const;

		bool changeType(EType t);

		const CByteArray& getByteArray()const;
	};
}

#endif