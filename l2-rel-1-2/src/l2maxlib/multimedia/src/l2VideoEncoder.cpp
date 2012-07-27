#include "l2VideoEncoder.h"

#include <l2Trace.h>
#include <l2Image.h>
#include <l2File.h>

extern "C"
{
   #include <mp_msg.h>
   #include <codec-cfg.h>
   #include <stream/stream.h>

   #include <libmpdemux/ms_hdr.h>
   #include <libmpdemux/aviheader.h>
   #include <libmpdemux/muxer.h>
   #include <libmpdemux/demuxer.h>
   #include <libmpdemux/stheader.h>

   #include <libmpcodecs/mp_image.h>
   #include <libmpcodecs/vf.h>
   #include <libmpcodecs/dec_video.h>
   #include <libmpcodecs/img_format.h>

   #include <libswscale/swscale.h>
   #include <libswscale/rgb2rgb.h>
}

namespace l2max
{
   CVideoEncoder::CVideoEncoder()
   {
      TRACE_FUN( Routine, "CVideoEncoder::CVideoEncoder" );

      _outFileFormat = MUXER_TYPE_AVI;
      _swsContext = 0;
      _outStream = 0;
      _muxer = 0;
      _vfilter = 0;
   }

   CVideoEncoder::~CVideoEncoder()
   {
      TRACE_FUN( Routine, "CVideoEncoder::~CVideoEncoder" );
   }

   bool CVideoEncoder::Open( const std::string& outFile, int width, int height )
   {
      TRACE_FUN( Frequently, "CVideoEncoder::Open" );

      bool ret( false );

      Close();

      _outFile = outFile;

      mp_msg_init();

      static int parserRet( is_cfg_parsed() ? 1 : parse_codec_cfg( 0 ) );

      if( parserRet && ( _outStream = open_output_stream( const_cast< char* >( _outFile.c_str() ), 0 ) ) )
      {
         _muxer = muxer_new_muxer( _outFileFormat, _outStream );

         if( _muxer )
         {
            _muxerStream = muxer_new_stream( _muxer, MUXER_TYPE_VIDEO );

            _muxerStream->buffer_size = 0x200000;
            _muxerStream->buffer = new unsigned char[ _muxerStream->buffer_size ];
            _muxerStream->bih = 0;

            _muxerStream->h.dwSampleSize = 0;
            _muxerStream->h.dwScale = 1;
            _muxerStream->h.dwRate = 15;
            _muxerStream->h.dwSampleSize = 0;

            _vfilter = vf_open_encoder( 0, "xvid", ( char* )_muxerStream );
            if( _vfilter && _vfilter->config( _vfilter, width, height, width, height, 0, 0 ) )
            {
               _mpimage = new mp_image_s;
               memset( _mpimage, 0, sizeof( *_mpimage ) );

               _mpimage->imgfmt = IMGFMT_YV12;

               _swsContext = sws_getContext( width, height, PIX_FMT_RGB32,
                                             width, height, PIX_FMT_YUV420P, SWS_BILINEAR, 0, 0, 0 );

               if( _swsContext )
               {
                  _sourceStride[ 0 ] = width * 4;
                  _sourceStride[ 1 ] = 0;
                  _sourceStride[ 2 ] = 0;

                  _mpimage->stride[ 0 ] = width;
                  _mpimage->stride[ 1 ] = width / 2;
                  _mpimage->stride[ 2 ] = width / 2;

                  _mpimage->planes[ 0 ] = new unsigned char[ width * height * 3 ];
                  _mpimage->planes[ 1 ] = _mpimage->planes[ 0 ] + width * height;
                  _mpimage->planes[ 2 ] = _mpimage->planes[ 1 ] + width * height;

                  sws_rgb2rgb_init( SWS_CPU_CAPS_MMX * 0 );
               }

               ret = true;
            }
         }
      }

      return ret;
   }

   bool CVideoEncoder::Write( CImage& image )
   {
      TRACE_FUN(  Frequently, "CVideoEncoder::Write" );

      bool ret( false );

      uint8_t* sourceData[ 3 ] = { image.data(), 0, 0 };

      sws_scale( _swsContext, sourceData, _sourceStride, 0, image.height(), _mpimage->planes, _mpimage->stride );

      int blit_frame = _vfilter->put_image( _vfilter, _mpimage, MP_NOPTS_VALUE );

      return ret;
   }

   void CVideoEncoder::Close()
   {
      TRACE_FUN(  Frequently, "CVideoEncoder::Close" );

      if( _swsContext )
      {
         sws_freeContext( _swsContext );

         _swsContext = 0;
      }

      if( _vfilter )
      {
         _vfilter->control( _vfilter, VFCTRL_FLUSH_FRAMES, 0 );

         _vfilter = 0;
      }

      if( _muxer )
      {
         if( _muxer->cont_write_index )
         {
            muxer_write_index( _muxer );
         }

         stream_seek( _muxer->stream, 0 );

         if( _muxer->cont_write_header )
         {
            muxer_write_header( _muxer );
         }

         free( _muxer );

         _muxer = 0;
      }

      if( _outStream )
      {
         free_stream( _outStream );

         _outStream = 0;
      }
   }
}
