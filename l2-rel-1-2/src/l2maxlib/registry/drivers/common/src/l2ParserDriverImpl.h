/*
 * l2ParserDriverImpl.h
 *
 *  Created on: 29.03.2009
 *      Author: L2-Max
 */

#ifndef L2PARSERDRIVERIMPL_H_
#define L2PARSERDRIVERIMPL_H_

namespace l2max
{
   namespace Registry
   {
      namespace Driver
      {
         void CParserDriver::error( const location& aLocation, const std::string& aMessage )const
         {
            std::ostringstream str;
            str << aLocation << ": " << aMessage;

            RegisterError( str.str() );
         }

         void l2max::parser::error( const location_type& aLocation, const std::string& aMessage )
         {
            aDriver.error( aLocation, aMessage );
         }
      }
   }
}

#endif /* L2PARSERDRIVERIMPL_H_ */
