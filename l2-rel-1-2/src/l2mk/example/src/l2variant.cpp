#include "l2variant.h"
#include "l2exception.h"

using namespace l2;

CVariant::CVariant(){
	type=tUnknown;
}
CVariant::CVariant(const CVariant& v){
	type=tUnknown;

	*this=v;
}

CVariant::CVariant(const int& i){
    setInteger(i);
}
CVariant::CVariant(const double& d){
    setReal(d);
}
CVariant::CVariant(const char* const& s){
    setString(s);
}
CVariant::CVariant(const void* const& d,const int& s){
    setBinary(d,s);
}

CVariant::~CVariant(){
}

void CVariant::setInteger(const int& i){
	type=tInteger;
	data.setData(&i,sizeof(i));
}
void CVariant::setReal(const double& d){
	type=tReal;
	data.setData(&d,sizeof(d));
}
void CVariant::setString(const char* const& s){
	type=tString;
	data.setData(s,strlen(s)+1);
}
void CVariant::setBinary(const CByteArray& b){
	type=tBinary;
	data=b;
}
void CVariant::setBinary(const void* const& d,const int& s){
	type=tBinary;
        data.setData(d,s);
}
const int& CVariant::getInteger()const{
	int n;
	int res;

	switch(type){
		case tInteger:
			n=*reinterpret_cast<const int*>(data.getData());
			break;

		case tReal:
			n=*reinterpret_cast<const double*>(data.getData());
			break;

		case tString:
			res=sscanf(reinterpret_cast<const char*>(data.getData()),"%d",&n);

			if(!res||res==EOF)
				throw exception(emTypeConversion,exception::tError);

			break;

		case tBinary:
			n=data.getSize();
			memcpy(&n,data.getData(),n<sizeof(n)?n:sizeof(n));			
			break;
	}

	dataCopy.setData(&n,sizeof(n));
	return *reinterpret_cast<const int*>(dataCopy.getData());
}
const double& CVariant::getReal()const{
	double d;
	int res;

	switch(type){
		case tInteger:
			d=*reinterpret_cast<const int*>(data.getData());
			break;

		case tReal:
			d=*reinterpret_cast<const double*>(data.getData());
			break;

		case tString:
			res=sscanf(reinterpret_cast<const char*>(data.getData()),"%lf",&d);

			if(!res||res==EOF)
				throw exception(emTypeConversion,exception::tError);

			break;

		case tBinary:
			res=data.getSize();
			memcpy(&d,data.getData(),res<sizeof(d)?res:sizeof(d));			
			break;
	}

	dataCopy.setData(&d,sizeof(d));
	return *reinterpret_cast<const double*>(dataCopy.getData());
}
const char* const& CVariant::getString()const{

	dataCopy.resize(0x200);

	switch(type){
		case tInteger:
			sprintf(reinterpret_cast<char*>(dataCopy.getData()),
					"%d",
					*reinterpret_cast<const int*>(data.getData()));

			break;

		case tReal:
			sprintf(reinterpret_cast<char*>(dataCopy.getData()),
					"%lf",
					*reinterpret_cast<const double*>(data.getData()));

			break;

		case tString:
			dataCopy=data;
			break;

		case tBinary:
			dataCopy.resize(data.getSize()*2+1);

			for(int i=0;i<data.getSize();++i)
				sprintf(reinterpret_cast<char*>(dataCopy.getData())+i*2,"%2.2X",data[i]);
			
			break;
	}

	return reinterpret_cast<const char* const>(dataCopy.getData());
}
const CByteArray& CVariant::getBinary()const{
	unsigned int n;

	switch(type){
		case tString:
			n=data.getSize()-1;
			if(n<0)n=0;
			else{
				if(n%2)++n;
				n/=2;
			}

			dataCopy.resize(n);

			for(int i=0,b;i<n;++i){
				sscanf(reinterpret_cast<const char* const>(data.getData())+i*2,
					"%X",
					&b
					);
				dataCopy[i]=b;
			}
			break;

		default:
			dataCopy=data;
	}
	return dataCopy;
}

bool CVariant::changeType(EType t){
	try{
		switch(type){
			case tInteger:setInteger(getInteger());break;
			case tReal:setReal(getReal());break;
			case tString:setString(getString());break;
			case tBinary:setBinary(getBinary());break;
		}
	}catch(const exception& e){
		return false;
	}
	return true;
}

const CByteArray& CVariant::getByteArray()const{
	return data;
}
