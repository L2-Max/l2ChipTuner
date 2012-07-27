/*
 * FileProtocolPacketSelectionView.h
 *
 *  Created on: 31.08.2009
 *      Author: L2-Max
 */

#ifndef FILEPROTOCOLPACKETSELECTIONVIEW_H_
#define FILEPROTOCOLPACKETSELECTIONVIEW_H_

#include <ui_FileProtocolPacketSelection.h>

#include <QWidget>

class CProtocolDiagnosticDataStorage;
class CProtocolDiagnosticDataStorageEventChannel;

class CFileProtocolPacketSelectionView : public QWidget,
                                         public Ui_FileProtocolPacketSelection
{
   Q_OBJECT;

   CProtocolDiagnosticDataStorage& _storage;
   CProtocolDiagnosticDataStorageEventChannel* _channel;

public:
   CFileProtocolPacketSelectionView( QWidget* aParent, CProtocolDiagnosticDataStorage& aStorage );
   virtual ~CFileProtocolPacketSelectionView();
   
private slots:
   void OnPacketRead();
   void OnPacketChangedBySlider();
   void OnPacketChangedBySpinBox();
};

#endif /* FILEPROTOCOLPACKETSELECTIONVIEW_H_ */
