/*
 * l2ExceptionDefs.h
 *
 *  Created on: 02.02.2009
 *      Author: mars
 */

#ifndef L2EXCEPTIONDEFS_H_
#define L2EXCEPTIONDEFS_H_

#include <typeinfo>

#define CHECK_STATIC_STATE( aCondition, aDescription, anObject ) \
   if( !( aCondition ) ) l2max::CExceptionThrower:: \
      ThrowObjectState( anObject, __FUNCTION__, #aCondition, aDescription, __FILE__, __LINE__ );

#define CHECK_OBJECT_STATE( aCondition, aDescription ) CHECK_STATIC_STATE( aCondition, aDescription, typeid( *this ).name() )

#endif /* L2EXCEPTIONDEFS_H_ */
