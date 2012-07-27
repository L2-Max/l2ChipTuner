/*
 * ECUDiagnosticUnitsMonitorModel.cpp
 *
 *  Created on: 13.10.2008
 *      Author: L2-Max
 */

#include "ECUDiagnosticUnitsMonitorModel.h"
#include "ECUDiagnosticUnitsMonitorModelNodes.h"
#include "ECUDiagnosticUnitsDecorationStorage.h"

#include <AppDefs.h>
#include <AppContext.h>

#include <ECUUnitsTable.h>
#include <ECUDataUnit.h>
#include <ECUControlUnit.h>

#include <l2Trace.h>
#include <l2ExceptionThrower.h>
#include <l2Profiler.h>

#include <QMimeData>
#include <QStringList>

CECUDiagnosticUnitsMonitorModel::CECUDiagnosticUnitsMonitorModel( const QFont& aFont, ECU::CUnitsTable& aTable ) :
   _dataUnitsTable( aTable )
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorModel::CECUDiagnosticUnitsMonitorModel" );

   _font = aFont;
   _font.setBold( true );
   _font.setItalic( true );

   _rootNode = new CECUDiagnosticUnitsMonitorModelRootNode( ECU::TDataUnitContainer() );

   _columnNames[ CECUDiagnosticUnitsMonitorModel::ciName ] = "Name";
   _columnNames[ CECUDiagnosticUnitsMonitorModel::ciValue ] = "Value";
   _columnNames[ CECUDiagnosticUnitsMonitorModel::ciDescription ] = "Description";

   ECUDiagnosticModelUtils::TSynonyms synonyms;

   Diagnostic::TUnitKeys keys( aTable.dataKeys() );

   for( Diagnostic::TUnitKeys::const_iterator I_key( keys.begin() ); I_key != keys.end(); ++I_key )
   {
      synonyms.push_back( aTable.data( *I_key )->synonym() );
   }

   InsertSynonyms( synonyms, index( -1, -1, QModelIndex() ), -1 );
}

CECUDiagnosticUnitsMonitorModel::~CECUDiagnosticUnitsMonitorModel()
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorModel::~CECUDiagnosticUnitsMonitorModel" );

   delete _rootNode;
}

int CECUDiagnosticUnitsMonitorModel::columnCount( const QModelIndex& parent )const
{
   return _columnNames.size();
}

ECUDiagnosticModelUtils::TSynonyms CECUDiagnosticUnitsMonitorModel::Indexes2Synonyms( const QModelIndexList& anIndexes )const
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModel::Indexes2Synonyms" );

   ECUDiagnosticModelUtils::TSynonyms ret;

   for( QModelIndexList::const_iterator i( anIndexes.begin() ); i != anIndexes.end(); ++i )
   {
      if( i->column() == ciName )
      {
         CECUDiagnosticUnitsMonitorModelUnitNode* node( reinterpret_cast< CECUDiagnosticUnitsMonitorModelUnitNode* >( i->internalPointer() ) );

         ret.push_back( node->synonym() );
      }
   }

   return ret;
}

bool CECUDiagnosticUnitsMonitorModel::InsertSynonyms( const ECUDiagnosticModelUtils::TSynonyms& aSynonyms, const QModelIndex& aParent, int aRow )
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorModel::InsertSynonyms" );

   bool ret( true );

   ECU::TDataUnitContainer dataUnitContainer;

   for( ECUDiagnosticModelUtils::TSynonyms::const_iterator I_synonym( aSynonyms.begin() );
         I_synonym != aSynonyms.end(); ++I_synonym )
   {
      Diagnostic::CUnitKey key( _dataUnitsTable.Synonym2Key( *I_synonym ) );
      
      if( key.isValid() )
      {
         dataUnitContainer.push_back( _dataUnitsTable.data( key ) );
      }
   }
   
   CECUDiagnosticUnitsMonitorModelRootNode whichNode( dataUnitContainer );
   
   MergeNode( aParent, aRow, whichNode );

   return ret;
}

void CECUDiagnosticUnitsMonitorModel::MergeNode( const QModelIndex& aWhereParentIndex, int aWhereRow,
                                                 CECUDiagnosticUnitsMonitorModelNode& aWhichNode,
                                                 CECUDiagnosticUnitsMonitorModelNode* aWithNode,
                                                 const QModelIndex& aParentIndex )
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorModel::MergeNode" );
   
   if( !aWithNode )
   {
      aWithNode = &Index2Node( QModelIndex() );
   }
   
   CHECK_OBJECT_STATE( aWhichNode == *aWithNode, "Nodes aren't qeual" );
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   typedef std::list< std::pair< CECUDiagnosticUnitsMonitorModelNode*, CECUDiagnosticUnitsMonitorModelNode* > > TNodes;
   TNodes toMerge;

   for( int i( 0 ); i < aWhichNode.childCount(); ++i )
   {
      for( int j( 0 ); j < aWithNode->childCount(); ++j )
      {
         if( *aWhichNode.child( i ) == *aWithNode->child( j ) )
         {
            toMerge.push_back( TNodes::value_type( aWhichNode.child( i ), aWithNode->child( j ) ) );
         }
      }
   }
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   QModelIndex thisIndex;

   if( aWithNode->parent() )
   {
      thisIndex = index( aWithNode->index(), CECUDiagnosticUnitsMonitorModel::ciName, aParentIndex );
   }
   
   for( TNodes::const_iterator I_node( toMerge.begin() ); I_node != toMerge.end(); ++I_node )
   {
      MergeNode( aWhereParentIndex, aWhereRow, *I_node->first, I_node->second, thisIndex );
   }
   
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   if( aWhichNode.childCount() )
   {
      int first( aWithNode->childCount() );
      
      if( first )
      {
         --first;
      }
      
      if( aWhereRow != -1 && *aWithNode == Index2Node( aWhereParentIndex ) )
      {
         first = aWhereRow;
      }
      
      beginInsertRows( thisIndex, first, first + aWhichNode.childCount() - 1 );

      while( aWhichNode.childCount() )
      {
         aWithNode->Insert( aWhichNode.child( 0 ), first++ );
      }

      endInsertRows();
   }
   
   if( toMerge.size() )
   {
      if( aWhichNode.parent() )
      {
         aWhichNode.parent()->Remove( aWhichNode.index() );
      }
   }
}

CECUDiagnosticUnitsMonitorModelNode& CECUDiagnosticUnitsMonitorModel::Index2Node( const QModelIndex& anIndex )const
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModel::Index2Node" );

   CECUDiagnosticUnitsMonitorModelNode* ret( _rootNode );

   if( anIndex.isValid() )
   {
      ret = reinterpret_cast< CECUDiagnosticUnitsMonitorModelNode* >( anIndex.internalPointer() );
   }

   return *ret;
}

QVariant CECUDiagnosticUnitsMonitorModel::data( const QModelIndex& index, int role )const
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModel::data" );

   QVariant ret;

   if( index.isValid() )
   {
      CECUDiagnosticUnitsMonitorModelNode* node( reinterpret_cast< CECUDiagnosticUnitsMonitorModelNode* >( index.internalPointer() ) );

      if( role == Qt::DisplayRole )
      {
         if( index.column() == ciName )
         {
            ret = node->name().c_str();
         }
         else if( index.column() == ciValue )
         {
            ret = QString::fromUtf8( node->text().c_str() );
         }
         else if( index.column() == ciDescription )
         {
            ret = node->description().c_str();
         }
      }
      else if( role == Qt::FontRole )
      {
         if( node->type() == CECUDiagnosticUnitsMonitorModelNode::tGroupUnit )
         {
            ret = _font;
         }
      }
      else if( role == Qt::DecorationRole )
      {
         if( index.column() == ciName )
         {
            if( node->type() == CECUDiagnosticUnitsMonitorModelNode::tUnit )
            {
               CECUDiagnosticUnitsMonitorModelUnitNode* unitNode( static_cast< CECUDiagnosticUnitsMonitorModelUnitNode* >( node ) );

               CECUDiagnosticUnitsDecorationStorage::EUnitType type( CECUDiagnosticUnitsDecorationStorage::utReadOnly );

               if( _dataUnitsTable.hasControl( unitNode->key().type() ) )
               {
                  type = CECUDiagnosticUnitsDecorationStorage::utReadWrite;
               }

               ret = CECUDiagnosticUnitsDecorationStorage::GetInstance()->unitIcon( type );
            }
         }
      }
      else if( role == Qt::EditRole )
      {
         if( index.column() == ciValue )
         {
            CHECK_OBJECT_STATE( node->type() == CECUDiagnosticUnitsMonitorModelNode::tUnit, "Edit role should be applied only for unit" );

            CECUDiagnosticUnitsMonitorModelUnitNode* unitNode( static_cast< CECUDiagnosticUnitsMonitorModelUnitNode* >( node ) );

            CHECK_OBJECT_STATE( _dataUnitsTable.hasControl( unitNode->key() ), "Edit role should be applied only for editable unit" );

            l2max::CVariant dataSnapshot( _dataUnitsTable.data( unitNode->key() )->value() );

            if( dataSnapshot.type() == l2max::CVariant::tBool )
            {
               ret = dataSnapshot.toBool();
            }
            else if( dataSnapshot.type() == l2max::CVariant::tDouble )
            {
               ret = dataSnapshot.toDouble();
            }
            else if( dataSnapshot.type() == l2max::CVariant::tInteger )
            {
               ret = dataSnapshot.toInt();
            }
         }
      }
   }

   return ret;
}

QModelIndex CECUDiagnosticUnitsMonitorModel::index( int row, int column, const QModelIndex& parent )const
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModel::index" );

   QModelIndex ret;
   
   CECUDiagnosticUnitsMonitorModelNode* node( &Index2Node( parent ) );

   if( row != -1 && row < node->childCount() )
   {
      node = node->child( row );
      
      ret = createIndex( row, column, node );
   }

   return ret;
}

QModelIndex CECUDiagnosticUnitsMonitorModel::parent( const QModelIndex& index )const
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModel::parent" );

   QModelIndex ret;

   CECUDiagnosticUnitsMonitorModelNode* node( Index2Node( index ).parent() );

   if( node && node->parent() )
   {
      ret = createIndex( node->index(), CECUDiagnosticUnitsMonitorModel::ciName, node );
   }

   return ret;
}

int CECUDiagnosticUnitsMonitorModel::rowCount( const QModelIndex& parent )const
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModel::rowCount" );

   return Index2Node( parent ).childCount();
}

Qt::ItemFlags CECUDiagnosticUnitsMonitorModel::flags( const QModelIndex& index )const
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModel::flags" );

   Qt::ItemFlags f( Qt::NoItemFlags );

   CECUDiagnosticUnitsMonitorModelNode* node( 0 );

   if( index.isValid() )
   {
      node = reinterpret_cast< CECUDiagnosticUnitsMonitorModelNode* >( index.internalPointer() );

      if( node->type() == CECUDiagnosticUnitsMonitorModelNode::tUnit )
      {
         f |= Qt::ItemIsEnabled | Qt::ItemIsDragEnabled | Qt::ItemIsSelectable;

         CECUDiagnosticUnitsMonitorModelUnitNode* unitNode( static_cast< CECUDiagnosticUnitsMonitorModelUnitNode* >( node ) );

         if( index.column() == ciValue && _dataUnitsTable.hasControl( unitNode->key() ) )
         {
            f |= Qt::ItemIsEditable;
         }
      }
   }

   if( !node ||
       node->type() == CECUDiagnosticUnitsMonitorModelNode::tGroupUnit ||
       node->type() == CECUDiagnosticUnitsMonitorModelNode::tGroupUnitState )
   {
      f |= Qt::ItemIsDropEnabled;
   }

   return f;
}

Qt::DropActions CECUDiagnosticUnitsMonitorModel::supportedDropActions()const
{
   return Qt::MoveAction;
}

QMimeData* CECUDiagnosticUnitsMonitorModel::mimeData( const QModelIndexList& anIndexes )const
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModel::mimeData" );

   QMimeData* mimeData( new QMimeData() );

   QByteArray units;
   QDataStream unitsStream( &units, QIODevice::WriteOnly );

   unitsStream << ECUDiagnosticModelUtils::Synonyms2Str( Indexes2Synonyms( anIndexes ) ).c_str();

   mimeData->setData( ECUDiagnosticModelUtils::mimeTypes().c_str(), units );

   return mimeData;
}


QStringList CECUDiagnosticUnitsMonitorModel::mimeTypes()const
{
   QStringList types;

   types << ECUDiagnosticModelUtils::mimeTypes().c_str();

   return types;
}

bool CECUDiagnosticUnitsMonitorModel::dropMimeData( const QMimeData* aData, Qt::DropAction anAction, int aRow, int aColumn, const QModelIndex& aParent )
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModel::dropMimeData" );

   bool ret( false );

   if( anAction == Qt::MoveAction )
   {
      if( aData->hasFormat( mimeTypes().front() ) )
      {
         QByteArray units( aData->data( mimeTypes().front() ) );
         QDataStream unitsStream( &units, QIODevice::ReadOnly );

         char* dataBuf( new char[ units.size() ] );
         unitsStream >> dataBuf;

         ECUDiagnosticModelUtils::TSynonyms synonyms( ECUDiagnosticModelUtils::Str2Synonyms( dataBuf ) );

         delete[] dataBuf;

         ret = InsertSynonyms( synonyms, aParent, aRow );
      }
   }

   return ret;
}

bool CECUDiagnosticUnitsMonitorModel::removeRows( int aRow, int aCount, const QModelIndex& aParent )
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModel::removeRows" );

   bool ret( true );

   CECUDiagnosticUnitsMonitorModelNode& node( Index2Node( aParent ) );

   beginRemoveRows( aParent, aRow, aRow + aCount - 1 );

   node.Remove( aRow, aCount );

   endRemoveRows();

   if( !node.childCount() && aParent.isValid() )
   {
      ret = removeRows( node.index(), node.index(), aParent.parent() );
   }

   return ret;
}

QVariant CECUDiagnosticUnitsMonitorModel::headerData( int section, Qt::Orientation orientation, int role )const
{
   QVariant ret;

   if( orientation == Qt::Horizontal )
   {
      if( role == Qt::DisplayRole )
      {
         TColumnNames::const_iterator i( _columnNames.find( static_cast< EColumnId >( section ) ) );

         if( i != _columnNames.end() )
         {
            ret = i->second.c_str();
         }
         else
         {
            ret = "No header data";
         }
      }
   }

   return ret;
}

Diagnostic::TUnitKeys CECUDiagnosticUnitsMonitorModel::keys()const
{
   TRACE_FUN( Frequently, "CECUDiagnosticUnitsMonitorModel::keys" );

   Diagnostic::TUnitKeys ret;

   for( int i( 0 ); i < _rootNode->childCount(); ++i )
   {
      Diagnostic::TUnitKeys groupKeys( static_cast< CECUDiagnosticUnitsMonitorModelGroupUnitNode* >( _rootNode->child( i ) )->keys() );

      ret.insert( groupKeys.begin(), groupKeys.end() );
   }

   return ret;
}

bool CECUDiagnosticUnitsMonitorModel::setData( const QModelIndex& anIndex, const QVariant& aValue, int aRole )
{
   TRACE_FUN( Routine, "CECUDiagnosticUnitsMonitorModel::setData" );

   bool ret( false );

   if( aRole == Qt::EditRole )
   {
      CHECK_OBJECT_STATE( anIndex.isValid() && anIndex.column() == ciValue, "Data modification have to be applied to valid value index" );

      CECUDiagnosticUnitsMonitorModelNode* node( reinterpret_cast< CECUDiagnosticUnitsMonitorModelNode* >( anIndex.internalPointer() ) );
      CHECK_OBJECT_STATE( node->type() == CECUDiagnosticUnitsMonitorModelNode::tUnit, "Data modification have to be applied to unit" );

      CECUDiagnosticUnitsMonitorModelUnitNode* unitNode( static_cast< CECUDiagnosticUnitsMonitorModelUnitNode* >( node ) );
      CHECK_OBJECT_STATE( _dataUnitsTable.hasControl( unitNode->key() ), "Data modification have to be applied to control" );

      l2max::CVariant value;

      if( aValue.type() == QVariant::Bool )
      {
         value.SetBool( aValue.toBool() );
      }
      else if( aValue.type() == QVariant::Double )
      {
         value.SetDouble( aValue.toDouble() );
      }
      else if( aValue.type() == QVariant::Int )
      {
         value.SetInt( aValue.toInt() );
      }

      if( ret = value.isValid() )
      {
         ECU::CControlUnit& controlUnit( _dataUnitsTable.control( unitNode->key() ) );
         controlUnit.SetValue( value, _dataUnitsTable.data( unitNode->key() )->measureUnit() );

         controlChangedEvent();
      }
   }

   return ret;
}

Diagnostic::CUnitKey CECUDiagnosticUnitsMonitorModel::Index2Key( const QModelIndex& anIndex )const
{
   Diagnostic::CUnitKey ret;

   if( anIndex.isValid() )
   {
      CECUDiagnosticUnitsMonitorModelNode* node( reinterpret_cast< CECUDiagnosticUnitsMonitorModelNode* >( anIndex.internalPointer() ) );

      if( node->type() == CECUDiagnosticUnitsMonitorModelNode::tUnit )
      {
         CECUDiagnosticUnitsMonitorModelUnitNode* unitNode( static_cast< CECUDiagnosticUnitsMonitorModelUnitNode* >( node ) );

         ret = unitNode->key();
      }
   }

   return ret;
}

ECU::HDataUnit CECUDiagnosticUnitsMonitorModel::dataUnit( const Diagnostic::CUnitKey& aKey )const
{
   CHECK_OBJECT_STATE( aKey.isValid(), "Unit key is not valid" );

   return _dataUnitsTable.data( aKey );
}

QModelIndex CECUDiagnosticUnitsMonitorModel::Key2Index( const Diagnostic::CUnitKey& aKey )const
{
   return Key2Index( aKey, QModelIndex() );
}

QModelIndex CECUDiagnosticUnitsMonitorModel::Key2Index( const Diagnostic::CUnitKey& aKey, const QModelIndex& anIndex )const
{
   QModelIndex ret;
   
   for( int i( 0 ); i < rowCount( anIndex ); ++i )
   {
      QModelIndex theIndex( index( i, 0, anIndex ) );
      CECUDiagnosticUnitsMonitorModelNode& node( Index2Node( theIndex ) );
      
      if( node.type() == CECUDiagnosticUnitsMonitorModelNode::tUnit )
      {
         CECUDiagnosticUnitsMonitorModelUnitNode& unitNode( static_cast< CECUDiagnosticUnitsMonitorModelUnitNode& >( node ) );
         
         if( unitNode.key() == aKey )
         {
            ret = theIndex;
            break;
         }
      }
      else
      {
         ret = Key2Index( aKey, theIndex );
      }
   }
   
   return ret;
}
