/*
 * IProtocolDiagnosticDataStorageEventSupplier.h
 *
 *  Created on: 07.09.2009
 *      Author: mars
 */

#ifndef IPROTOCOLDIAGNOSTICDATASTORAGEEVENTSUPPLIER_H_
#define IPROTOCOLDIAGNOSTICDATASTORAGEEVENTSUPPLIER_H_

struct IProtocolDiagnosticDataStorageEventSupplier
{
   virtual void packetRead() = 0;
};

#endif /* IPROTOCOLDIAGNOSTICDATASTORAGEEVENTSUPPLIER_H_ */
