/*
 * l2RefHandle.hxx
 *
 *  Created on: 05.01.2009
 *      Author: mars
 */

#ifndef L2REFHANDLE_HXX_
#define L2REFHANDLE_HXX_

namespace l2max
{
   /////////////////////////////////////////////////////////////////////////////
   template< class TRefObject >
      class CRefHandle
      {
         TRefObject* _refObject;

      public:
         inline CRefHandle();
         inline CRefHandle( const CRefHandle& aRefHandle );
         inline CRefHandle( TRefObject& aRefObject );

         inline virtual ~CRefHandle();

         inline CRefHandle& operator = ( const CRefHandle& aRefObject );
         inline CRefHandle& operator = ( TRefObject& aRefObject );

         inline TRefObject* operator -> ()const;
         inline TRefObject& operator * ()const;

         inline bool IsNull()const;
         
         inline bool operator == ( const CRefHandle< TRefObject >& aHandle )const;
         inline bool operator != ( const CRefHandle< TRefObject >& aHandle )const;
      };

   /////////////////////////////////////////////////////////////////////////////
   template< class TRefObject >
      inline CRefHandle< TRefObject >::CRefHandle()
      {
         _refObject = 0;
      }

   template< class TRefObject >
      inline CRefHandle< TRefObject >::CRefHandle( const CRefHandle& aRefObject ) :
         _refObject( aRefObject._refObject )
      {
         if( _refObject )
         {
            _refObject->AddRef();
         }
      }
   
   template< class TRefObject >
      inline CRefHandle< TRefObject >::CRefHandle( TRefObject& aRefObject ) :
         _refObject( &aRefObject )
      {
         if( _refObject )
         {
            _refObject->AddRef();
         }
      }

   template< class TRefObject >
      inline CRefHandle< TRefObject >::~CRefHandle()
      {
         if( _refObject )
         {
            _refObject->Release();
         }
      }

   template< class TRefObject >
      inline CRefHandle< TRefObject >& CRefHandle< TRefObject >::operator = ( const CRefHandle& aRefObject )
      {
         return operator = ( *aRefObject );
      }
   
   template< class TRefObject >
      inline CRefHandle< TRefObject >& CRefHandle< TRefObject >::operator = ( TRefObject& aRefObject )
      {
         if( &aRefObject )
         {
            aRefObject.AddRef();
         }

         if( _refObject )
         {
            _refObject->Release();
         }

         _refObject = &aRefObject;

         return *this;
      }

   template< class TRefObject >
      inline TRefObject* CRefHandle< TRefObject >::operator -> ()const
      {
         return _refObject;
      }

   template< class TRefObject >
      inline TRefObject& CRefHandle< TRefObject >::operator * ()const
      {
         return *CRefHandle::operator -> ();
      }
   
   template< class TRefObject >
      inline bool CRefHandle< TRefObject >::IsNull()const
      {
         return !_refObject;
      }
   
   template< class TRefObject >
      inline bool CRefHandle< TRefObject >::operator == ( const CRefHandle< TRefObject >& aHandle )const
      {
         return _refObject == aHandle._refObject;
      }
   
   template< class TRefObject >
      inline bool CRefHandle< TRefObject >::operator != ( const CRefHandle< TRefObject >& aHandle )const
      {
         return !operator == ( aHandle );
      }
}

#endif /* L2REFHANDLE_HXX_ */
