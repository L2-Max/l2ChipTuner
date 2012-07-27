/*
 * DescriptionTableBuilder.hxx
 *
 *  Created on: 24.03.2009
 *      Author: L2-Max
 */

#ifndef DESCRIPTIONTABLEBUILDER_HXX_
#define DESCRIPTIONTABLEBUILDER_HXX_

namespace
{
   template< class TDescriptionMap >
      const TDescriptionMap& buildDescriptionTable( const typename TDescriptionMap::mapped_type* aDescriptors,
                                                    unsigned int aCount )
      {
         static TDescriptionMap descriptionMapInstance;

         if( !descriptionMapInstance.size() )
         {
            for( int i( 0 ); i < aCount; ++i )
            {
               descriptionMapInstance[ aDescriptors[ i ]._id ] = aDescriptors[ i ];
            }
         }

         return descriptionMapInstance;
      }
}

#endif /* DESCRIPTIONTABLEBUILDER_HXX_ */
