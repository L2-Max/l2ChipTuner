/*
 * AppDefs.h
 *
 *  Created on: 03.10.2008
 *      Author: mars
 */

#ifndef MAINAPPDEFS_H_
#define MAINAPPDEFS_H_

/*
 * Common keys definitions
 * */
#define V_CMN_X                                 "::x"
#define V_CMN_Y                                 "::y"
#define V_CMN_WIDTH                             "::width"
#define V_CMN_HEIGHT                            "::height"
#define V_CMN_ISVISIBLE                         "::isVisible"

/*
 * Main Registry keys definitions
 * */
#define S_MAINWND                               "::MainWindow"
#define V_MAINWND_X                             S_MAINWND V_CMN_X
#define V_MAINWND_Y                             S_MAINWND V_CMN_Y
#define V_MAINWND_WIDTH                         S_MAINWND V_CMN_WIDTH
#define V_MAINWND_HEIGHT                        S_MAINWND V_CMN_HEIGHT
#define V_MAINWND_ISMAXI                        S_MAINWND"::isMaximized"
#define V_MAINWND_ISMINI                        S_MAINWND"::isMinimized"

#define S_PREFERENCESWND                        "::PreferencesWindow"
#define V_PREFERENCESWND_X                      S_PREFERENCESWND V_CMN_X
#define V_PREFERENCESWND_Y                      S_PREFERENCESWND V_CMN_Y
#define V_PREFERENCESWND_WIDTH                  S_PREFERENCESWND V_CMN_WIDTH
#define V_PREFERENCESWND_HEIGHT                 S_PREFERENCESWND V_CMN_HEIGHT

#define S_PREFERENCESWND_PREFTREE               S_PREFERENCESWND"::PreferencesTree"
#define V_PREFERENCESWND_PREFTREE_SELPATH       S_PREFERENCESWND_PREFTREE"::lastSelectedPath"

#define S_DDV                                   "::DiagnosticDeviceView"
#define V_DDV_SELECTEVIEW                       S_DDV"::selectedView"
#define V_DDV_OPENEDVIEWS                       S_DDV"::openedViews"

#define S_DUMV                                  "::DiagnosticUnitsMonitorView"
#define S_DUMV_COLSIZE                          S_DUMV"::ColumnsSizes"
#define V_DUMV_COLSIZE_NAME                     S_DUMV_COLSIZE"::name"
#define V_DUMV_COLSIZE_VALUE                    S_DUMV_COLSIZE"::value"
#define V_DUMV_COLSIZE_DESCR                    S_DUMV_COLSIZE"::description"

#define S_ECU                                   "::ECU"
#define S_ECU_DIAG                              S_ECU"::Diagnostic"
#define S_ECU_DIAG_SESS                         S_ECU_DIAG"::Session"
#define V_ECU_DIAG_SESS_REQAUTO                 S_ECU_DIAG_SESS"::requestAuto"
#define V_ECU_DIAG_SESS_REQQUEUE                S_ECU_DIAG_SESS"::requestQueueSize"
#define V_ECU_DIAG_SESS_REQTIMEO                S_ECU_DIAG_SESS"::requestTimeout"
#define V_ECU_DIAG_SESS_DATAFILER               S_ECU_DIAG_SESS"::dataFileReadFrom"
#define V_ECU_DIAG_SESS_DATAFILEW               S_ECU_DIAG_SESS"::dataFileWriteTo"
#define V_ECU_DIAG_SESS_DATAFILER_E             S_ECU_DIAG_SESS"::dataFileReadFromEnabled"
#define V_ECU_DIAG_SESS_DATAFILEW_E             S_ECU_DIAG_SESS"::dataFileWriteToEnabled"
#define V_ECU_DIAG_SESS_DATADIR                 S_ECU_DIAG_SESS"::dataDirectory"
#define V_ECU_DIAG_SESS_PERIODICTRANSM          S_ECU_DIAG_SESS"::periodicTransmission"

#define S_ECU_ADAPT                             S_ECU"::Adapter"
#define V_ECU_ADAPT_ADAPTYPE                    S_ECU_ADAPT"::adapterType"
#define V_ECU_ADAPT_COMMDEVTYP                  S_ECU_ADAPT"::communicationDeviceType"

#define S_ECU_UNIT                              S_ECU"::units"

#define S_ECU_ADAPT_COMMDEVTYP_SL               S_ECU_ADAPT"::SerialLine"
#define V_ECU_ADAPT_COMMDEVTYP_SL_NUM           S_ECU_ADAPT_COMMDEVTYP_SL"::number"
#define V_ECU_ADAPT_COMMDEVTYP_SL_BPS           S_ECU_ADAPT_COMMDEVTYP_SL"::bitsPerSecond"
#define V_ECU_ADAPT_COMMDEVTYP_SL_AUTOSCAN      S_ECU_ADAPT_COMMDEVTYP_SL"::autoScan"
#define V_ECU_ADAPT_COMMDEVTYP_SL_RDTIMEO       S_ECU_ADAPT_COMMDEVTYP_SL"::readTimeout"
#define V_ECU_ADAPT_COMMDEVTYP_SL_WRTIMEO       S_ECU_ADAPT_COMMDEVTYP_SL"::writeTimeout"
#define V_ECU_ADAPT_COMMDEVTYP_SL_BUFSZ         S_ECU_ADAPT_COMMDEVTYP_SL"::bufferSize"

#define S_ECU_PROTO                             S_ECU"::Proto"
#define V_ECU_PROTO_TYPE                        S_ECU_PROTO"::type"

#define S_ECU_PROTO_ALDL                        S_ECU_PROTO"::ALDL"
#define V_ECU_PROTO_ALDL_MODEL                  S_ECU_PROTO_ALDL"::model"
#define V_ECU_PROTO_ALDL_ENGINE                 S_ECU_PROTO_ALDL"::engine"
#define V_ECU_PROTO_ALDL_ECUID                  S_ECU_PROTO_ALDL"::ecuId"

#define S_ECU_FLOWMETER                         S_ECU"::FlowMeter"
#define V_ECU_FLOWMETER_LASTDIST                S_ECU_FLOWMETER"::lastDistance"
#define V_ECU_FLOWMETER_LASTFLOW                S_ECU_FLOWMETER"::lastFlow"
#define V_ECU_FLOWMETER_LASTTIME                S_ECU_FLOWMETER"::lastTime"
#define V_ECU_FLOWMETER_FUELTYPE                S_ECU_FLOWMETER"::fuelType"
#define V_ECU_FLOWMETER_INJCAL                  S_ECU_FLOWMETER"::injectorCalibration"
#define V_ECU_FLOWMETER_GASLAMBDA               S_ECU_FLOWMETER"::gasLambda"
#define V_ECU_FLOWMETER_GASLITERWEIGHT          S_ECU_FLOWMETER"::gasLiterWeight"

#define S_METER                                 "::Meter"
#define S_METER_GRAPH                           S_METER"::Graph"
#define V_METER_GRAPH_OPENEDGRAPHS              "::openedGraphs"
#define V_METER_GRAPH_OPENINWINDOW              "::openInWindow"
#define V_METER_GRAPH_TIME_FRAME                "::timeFrame"
#define V_METER_GRAPH_TIME_RESOLUTION           "::timeResolution"
#define V_METER_GRAPH_VALUE_RESOLUTION          "::valueResolution"

#define S_FILE_PROTO_PACKSEL_DLG                "::FileProtocolPacketSelectionView"
#define V_FILE_PROTO_PACKSEL_DLG_ISVISIBLE      S_FILE_PROTO_PACKSEL_DLG V_CMN_ISVISIBLE

#define S_STARTUP                               "::StartUp"
#define V_STARTUP_STARTDIAGNOSTICSESSION        S_STARTUP"::startDiagnosticSession"

/*
 * Diagnostic session data files registry definitions
 * */
#define S_DIAGF_CONF                            "::Configuration"
#define V_DIAGF_CONF_PROTOTYPE                  S_DIAGF_CONF"::protocolType"
#define V_DIAGF_CONF_ALDLECUID                  S_DIAGF_CONF"::aldlECUId"

#define S_DIAGF_DIAGNOSTICDATA                  "::DiagnosticData"
#define S_DIAGF_DIAGNOSTICDATA_PACKET           S_DIAGF_DIAGNOSTICDATA"::packet"
#define V_DIAGF_DIAGNOSTICDATA_TIMEO            "::timeo"
#define V_DIAGF_DIAGNOSTICDATA_DATA             "::data"

#endif /* MAINAPPDEFS_H_ */
