/*
 * AbstractProtoFactory.h
 *
 *  Created on: 25.03.2009
 *      Author: L2-Max
 */

#ifndef ABSTRACTPROTOFACTORY_H_
#define ABSTRACTPROTOFACTORY_H_

class CAbstractProto;
class CAbstractAdapter;

class CAbstractProtoFactory
{
   CAbstractProtoFactory( const CAbstractProtoFactory& );
   CAbstractProtoFactory& operator = ( const CAbstractProtoFactory& );

   CAbstractProtoFactory();
   virtual ~CAbstractProtoFactory();

public:
   static CAbstractProto* CreateProtocol( CAbstractAdapter& anAdapter );
};

#endif /* ABSTRACTPROTOFACTORY_H_ */
