/*
 * ECUDiagnosticUnitsMonitorModel.h
 *
 *  Created on: 13.10.2008
 *      Author: L2-Max
 */

#ifndef ECUDIAGNOSTICUNITSMONITORMODEL_H_
#define ECUDIAGNOSTICUNITSMONITORMODEL_H_

#include "ECUDiagnosticUnitModelUtils.h"

#include <DiagnosticTable.h>
#include <ECUDataUnit.h>

#include <QAbstractItemModel>
#include <QFont>

#include <vector>
#include <string>

class CECUDiagnosticUnitsMonitorModelNode;
class CECUDiagnosticUnitsMonitorModelRootNode;

namespace ECU
{
   class CUnitsTable;
}

class CECUDiagnosticUnitsMonitorModel : public QAbstractItemModel
{
   Q_OBJECT;

   CECUDiagnosticUnitsMonitorModelRootNode* _rootNode;

   QFont _font;

   virtual bool dropMimeData( const QMimeData* aData, Qt::DropAction anAction, int aRow, int aColumn, const QModelIndex& aParent );
   virtual bool removeRows( int aRow, int aCount, const QModelIndex& aParent );
   virtual bool setData( const QModelIndex& anIndex, const QVariant& aValue, int aRole );
   virtual Qt::ItemFlags flags( const QModelIndex& anIndex )const;
   virtual QMimeData* mimeData( const QModelIndexList& anIndexes )const;
   virtual QStringList mimeTypes()const;
   virtual QVariant headerData( int section, Qt::Orientation orientation, int role )const;

   void MergeNode( const QModelIndex& aWhereParentIndex, int aWhereRow,
                   CECUDiagnosticUnitsMonitorModelNode& aWhichNode,
                   CECUDiagnosticUnitsMonitorModelNode* aWithNode = 0,
                   const QModelIndex& aParentIndex = QModelIndex() );
   
   Qt::DropActions supportedDropActions()const;

   CECUDiagnosticUnitsMonitorModelNode& Index2Node( const QModelIndex& anIndex )const;
   
   QModelIndex Key2Index( const Diagnostic::CUnitKey& aKey, const QModelIndex& anIndex )const;

protected:
   void BuildRootNode( bool anIncludeMonitored );

public:
   enum EColumnId
   {
      ciName,
      ciValue,
      ciDescription
   };

private:
   typedef std::map< EColumnId, std::string > TColumnNames;
   TColumnNames _columnNames;

   ECU::CUnitsTable& _dataUnitsTable;

public:
   CECUDiagnosticUnitsMonitorModel( const QFont& aFont, ECU::CUnitsTable& aTable );
   virtual ~CECUDiagnosticUnitsMonitorModel();

   virtual ECUDiagnosticModelUtils::TSynonyms Indexes2Synonyms( const QModelIndexList& anIndexes )const;
   virtual bool InsertSynonyms( const ECUDiagnosticModelUtils::TSynonyms& aSynonyms, const QModelIndex& aParent, int aRow );

   virtual QVariant data( const QModelIndex& index, int role )const;

   virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() )const;
   virtual QModelIndex parent( const QModelIndex & index )const;

   virtual int columnCount( const QModelIndex& parent = QModelIndex() )const;
   virtual int rowCount( const QModelIndex& parent = QModelIndex() )const;

   Diagnostic::TUnitKeys keys()const;
   Diagnostic::CUnitKey Index2Key( const QModelIndex& anIndex )const;
   QModelIndex Key2Index( const Diagnostic::CUnitKey& aKey )const;

   ECU::HDataUnit dataUnit( const Diagnostic::CUnitKey& aKey )const;

signals:
   void controlChangedEvent();
};

#endif /* ECUDIAGNOSTICUNITSMONITORMODEL_H_ */
