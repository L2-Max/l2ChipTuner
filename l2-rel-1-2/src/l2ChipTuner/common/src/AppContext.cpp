/*
 * AppContext.cpp
 *
 *  Created on: 03.10.2008
 *      Author: mars
 */

#include "AppContext.h"
#include "AppDefs.h"

#include <AbstractAdapter.h>

#include <l2Trace.h>
#include <l2TraceStream.h>
#include <l2Exception.h>

#include <sstream>

CAppContext::CAppContext( int anArgc, char* anArgv[] ) : l2max::CContext( anArgc, anArgv )
{
   TRACE_FUN( Routine, "CAppContext::CAppContext" );

   CAppContext::InitRegistry();
}

CAppContext::~CAppContext()throw()
{
   TRACE_FUN( Routine, "CAppContext::~CAppContext" );

   try
   {
      CAppContext::UinitRegistry();
   }
   catch( l2max::CException& anException ){}
}

void CAppContext::InitRegistry()
{
   TRACE_FUN( Routine, "CMainAppContext::InitRegistry" );

   l2max::SRegistryValue regValues[] =
      {
         { V_MAINWND_X,                         -1 },
         { V_MAINWND_Y,                         -1 },
         { V_MAINWND_WIDTH,                     -1 },
         { V_MAINWND_HEIGHT,                    -1 },
         { V_MAINWND_ISMAXI,                 false },
         { V_MAINWND_ISMINI,                 false },

         { V_PREFERENCESWND_X,                  -1 },
         { V_PREFERENCESWND_Y,                  -1 },
         { V_PREFERENCESWND_WIDTH,              -1 },
         { V_PREFERENCESWND_HEIGHT,             -1 },
         { V_PREFERENCESWND_PREFTREE_SELPATH,  "0" },

         { V_DDV_SELECTEVIEW,                    0 },
         { V_DDV_OPENEDVIEWS,                   "" },

         { V_DUMV_COLSIZE_NAME,                 -1 },
         { V_DUMV_COLSIZE_VALUE,                -1 },
         { V_DUMV_COLSIZE_DESCR,                -1 },

         { V_ECU_DIAG_SESS_REQAUTO,          false },
         { V_ECU_DIAG_SESS_REQQUEUE,             2 },
         { V_ECU_DIAG_SESS_REQTIMEO,           250 },
         { V_ECU_DIAG_SESS_DATAFILER,           "" },
         { V_ECU_DIAG_SESS_DATAFILEW,           "" },
         { V_ECU_DIAG_SESS_DATAFILER_E,      false },
         { V_ECU_DIAG_SESS_DATAFILEW_E,      false },
         { V_ECU_DIAG_SESS_DATADIR,           "" },
         { V_ECU_DIAG_SESS_PERIODICTRANSM,    true },

         { V_ECU_ADAPT_ADAPTYPE,                CAbstractAdapter::atKLine },
         { V_ECU_ADAPT_COMMDEVTYP,              CAbstractAdapter::dtSerialLine },
         { V_ECU_ADAPT_COMMDEVTYP_SL_NUM,       1 },
         { V_ECU_ADAPT_COMMDEVTYP_SL_BPS,       8192 },
         { V_ECU_ADAPT_COMMDEVTYP_SL_AUTOSCAN,  false },
         { V_ECU_ADAPT_COMMDEVTYP_SL_RDTIMEO,   35 },
         { V_ECU_ADAPT_COMMDEVTYP_SL_WRTIMEO,   50 },
         { V_ECU_ADAPT_COMMDEVTYP_SL_BUFSZ,     512 },
         { V_ECU_PROTO_TYPE,                    0 },
         { V_ECU_PROTO_ALDL_MODEL,              0 },
         { V_ECU_PROTO_ALDL_ENGINE,             0 },
         { V_ECU_PROTO_ALDL_ECUID,              0 },

         { V_ECU_FLOWMETER_LASTDIST,           .0 },
         { V_ECU_FLOWMETER_LASTFLOW,           .0 },
         { V_ECU_FLOWMETER_LASTTIME,            0 },
         { V_ECU_FLOWMETER_FUELTYPE,            0 },
         { V_ECU_FLOWMETER_INJCAL,          191.8 },
         { V_ECU_FLOWMETER_GASLAMBDA,       15.76 },
         { V_ECU_FLOWMETER_GASLITERWEIGHT,   .524 },

         { V_FILE_PROTO_PACKSEL_DLG_ISVISIBLE, true },
         
         { V_STARTUP_STARTDIAGNOSTICSESSION, false },
      };

   _mainRegistry.Open( "mainapp.rg" );
   _mainRegistry.Initialize( regValues, sizeof( regValues ) / sizeof( *regValues ) );
}

void CAppContext::UinitRegistry()
{
   _mainRegistry.Close();
}

l2max::CTextTreeRegistry& CAppContext::MainRegistry()
{
   if( _mainRegistry.hasError() )
   {
      std::ostringstream what;

      what << "The main registry not opened because of error: " << std::endl
           << _mainRegistry.errorString() << std::endl;

      throw l2max::CException( what.str() );
   }

   return _mainRegistry;
}

CAppContext* CAppContext::GetInstance()
{
   return static_cast< CAppContext* >( l2max::CContext::GetInstance() );
}
