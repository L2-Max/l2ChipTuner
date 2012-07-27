/*
 * ECURequest.h
 *
 *  Created on: 25.01.2009
 *      Author: mars
 */

#ifndef ECUREQUEST_H_
#define ECUREQUEST_H_

namespace ECU
{
   class CRequest
   {
   public:
      enum EId
      {
         iIdle,
         iStartDiagnosticSession,
         iStopDiagnosticSession,
         iReadDataByLocalIdentifier,
         iWriteDataByLocalIdentifier,
         iUnknown = -1
      };
      
   private:
      EId _id;
      
   public:
      CRequest();
      CRequest( EId anId );
      virtual ~CRequest();
      
      EId id()const;
   };
}

#endif /* ECUREQUEST_H_ */
