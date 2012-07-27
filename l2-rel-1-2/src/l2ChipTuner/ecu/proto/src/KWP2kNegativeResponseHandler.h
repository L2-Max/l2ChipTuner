/*
 * KWP2kNegativeResponseHandler.h
 *
 *  Created on: 09.11.2008
 *      Author: L2-Max
 */

#ifndef KWP2KNEGATIVERESPONSEHANDLER_H_
#define KWP2KNEGATIVERESPONSEHANDLER_H_

namespace l2max
{
   class CByteArray;
}

namespace KWP2k
{

   class CNegativeResponseHandler
   {
   protected:
      enum ENegativeResponseHandlerServiceId
      {
         nrhsiNegativeResp = 0x7f
      };

      enum ENegativeResponseHandlerResponseCode
      {
         nrhrcNoError,
         nrhrcGeneralReject = 0x10,
         nrhrcServiceNotSupported = 0x11,
         nrhrcSubFunctionNotSupported = 0x12,
         nrhrcBusyRepeatRequest = 0x21,
         nrhrcConditionsNotCorrectOrRequestSequenceError = 0x22,
         nrhrcRoutineNotCompleteOrServiceInProgress = 0x23,
         nrhrcRequestOutOfRange = 0x31,
         nrhrcSecurityAccessDeniedSecurityAccessRequested = 0x33,
         nrhrcInvalidKey = 0x35,
         nrhrcExceedNumberOfAttempts = 0x36,
         nrhrcRequiredTimeDelayNotExpired = 0x37,
         nrhrcDownloadNotAccepted = 0x40,
         nrhrcImproperDownloadType = 0x41,
         nrhrcCanNotDownloadToSpecifiedAddress = 0x42,
         nrhrcCanNotDownloadNumberOfBytesRequested = 0x43,
         nrhrcUploadNotAccepted = 0x50,
         nrhrcImproperUploadType = 0x51,
         nrhrcCanNotUploadFromSpecifiedAddress = 0x52,
         nrhrcCanNotUploadNumberOfBytesRequested = 0x53,
         nrhrcTransferSuspended = 0x71,
         nrhrcTransferAborted = 0x72,
         nrhrcIllegalAddressInBlockTransfer = 0x74,
         nrhrcIllegalByteCountInBlockTransfer = 0x75,
         nrhrcIllegalBlockTransferType = 0x76,
         nrhrcBlockTransferDataChecksumError = 0x77,
         nrhrcRequestCorrectlyReceivedResponsePending = 0x78,
         nrhrcIncorrectByteCountDuringBlockTransfer = 0x79,
         nrhrcServiceNotSupportedInActiveDiagnosticSession = 0x80
      };

      unsigned char _negativeResponseHandlerResponseCode;

      bool ProcessNegativeResponse( unsigned char aServiceId, l2max::CByteArray& aResp );

   public:
      CNegativeResponseHandler();
      virtual ~CNegativeResponseHandler();
   };

}

#endif /* KWP2KNEGATIVERESPONSEHANDLER_H_ */
