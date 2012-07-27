#include "l2VideoDecoder.h"

#include <l2Trace.h>
#include <l2Image.h>

extern "C"
{
	#include <stream/stream.h>
	#include <libmpdemux/demuxer.h>
	#include <libmpdemux/stheader.h>
	#include <libvo/video_out.h>
	#include <libmpcodecs/mp_image.h>
	#include <libmpcodecs/vf.h>
	#include <libmpcodecs/dec_video.h>
	#include <codec-cfg.h>
}

#include <math.h>
#include <stdio.h>

namespace l2max
{

	struct SFrameItem
	{
		CImage _image;
		unsigned int _ref;
	};

   CVideoDecoder::CVideoDecoder( const std::string& file )
   {
      TRACE_FUN( Routine, "CVideoDecoder::CVideoDecoder" );

      _file = file;

      _fileStream = 0;
      _demuxer = 0;
      _videoStream = 0;

      _frameCache = 0;

      _curKeyFrame = -1;
      _nextKeyFrame = -1;

      _lastReadFrame = 0;
      _lastReadFrameSize = 0;

      _cacheUpdated = false;

      _previewItem = 0;
   }

   CVideoDecoder::~CVideoDecoder()
   {
      TRACE_FUN( Routine, "CVideoDecoder::~CVideoDecoder" );

      if( _videoStream )
      {
      	uninit_video( _videoStream );
      }

      if( _demuxer )
      {
      	free_demuxer( _demuxer );
      }

      if( _fileStream )
      {
      	free_stream( _fileStream );
      }

      if( _videoStream && _videoStream->video_out )
      {
      	reinterpret_cast< vo_functions_t* >( _videoStream->video_out )->uninit();
      }

      UninitFrameCache();

      delete _lastReadFrame;

      if( _previewItem && --_previewItem->_ref )
      {
         delete _previewItem;
      }
   }

   bool CVideoDecoder::Open( const std::string& file )
   {
      TRACE_FUN( Frequently, "CVideoDecoder::Open");

      bool ret( false);

      mp_msg_init();

      static int parserRet( is_cfg_parsed() ? 1 : parse_codec_cfg( 0 ) );

      int fileFormat( 0 );
      if( parserRet && ( _fileStream = open_stream(
            const_cast< char* >( file.c_str() ), 0, &fileFormat ) ) )
      {
         _demuxer = demux_open(_fileStream, fileFormat, -1, -1, -1, 0 );

         if( _demuxer )
         {
            _videoStream = reinterpret_cast< sh_video_t* >( _demuxer->video->sh );

            if( video_read_properties( _videoStream ) )
            {
               _bpp = 4;

               const char* vo_list[] = { "byte" };
               _videoStream->video_out = init_best_video_out( const_cast< char** >( vo_list ) );

               if( _videoStream->video_out )
               {
                  const char* vf_arg[] = { "_oldargs_", reinterpret_cast< char* >( _videoStream->video_out ), NULL };

                  _videoStream->vfilter = reinterpret_cast< void* >( vf_open_filter( NULL, "vo", const_cast< char** >( vf_arg ) ) );
                  if( _videoStream->vfilter )
                  {
                     _videoStream->vfilter
                           = reinterpret_cast< void* >( append_filters( reinterpret_cast< vf_instance_t* >( _videoStream->vfilter ) ) );

                     init_best_video_codec( _videoStream, NULL, NULL );

                     _videoStream->last_pts = MP_NOPTS_VALUE;
                     _videoStream->num_buffered_pts = 0;
                     _videoStream->next_frame_time = 0;

                     _width = _videoStream->disp_w;
                     _height = _videoStream->disp_h;

                     ret = true;
                  }
               }
            }
         }
      }
      return ret;
   }

   const CImage& CVideoDecoder::frame( int frameNum )
   {
      TRACE_FUN(  Frequently, "CVideoDecoder::frame" );

      PreloadFrames( frameNum );

      return _frameCache[ frameNum - static_cast< unsigned int >( round( ( _curKeyFrame * _videoStream->fps ) ) ) ]->_image;
   }

   void CVideoDecoder::InitFrameCache()
   {
   	TRACE_FUN(  Frequently, "CVideoDecoder::InitFrameCache" );

   	_frameCache = new SFrameItem*[ frameCacheSize() ];

		memset( _frameCache, 0, frameCacheSize() * sizeof( *_frameCache ) );
   }

   void CVideoDecoder::UninitFrameCache()
   {
      TRACE_FUN(  Frequently, "CVideoDecoder::UninitFrameCache" );

   	if( _frameCache )
   	{
	   	for( int i( 0 ); i < frameCacheSize(); ++i )
	   	{
	   		if( _frameCache[ i ] && !--_frameCache[ i ]->_ref )
	   		{
	   			delete _frameCache[ i ];
	   		}
	   	}
   	}

   	delete[] _frameCache;

   	_frameCache = 0;
   	_cacheUpdated = true;
   }

   unsigned int CVideoDecoder::frameCacheSize()const
   {
   	return _frameCacheSize;
   }

   void CVideoDecoder::LoadKeyFramesPos()
   {
   	TRACE_FUN(  Frequently, "CVideoDecoder::LoadKeyFramesPos" );

   	_keyFrames.clear();

   	float nextPts( .0 );
   	float lastPts( demuxer_get_time_length( _demuxer ) );

   	do
   	{
   		DemuxerSeek( nextPts );

	   	unsigned char* start( 0 );
  	   	int frameSize( video_read_frame( _videoStream, &_videoStream->next_frame_time, &start, 0 ) );

	   	nextPts = _videoStream->pts;

	   	_keyFrames.push_back( nextPts );

	   	nextPts += 1.;
   	}
   	while( nextPts < lastPts );

   	_keyFrames.back() = lastPts;
   }

   void CVideoDecoder::PreloadFrames( int frame )
   {
   	TRACE_FUN(  Frequently, "CVideoDecoder::preloadFrames" );

   	_cacheUpdated = false;

   	float keyFrame( frame / _videoStream->fps );

   	if( _curKeyFrame != -1 )
   	{
   		if( _curKeyFrame <= keyFrame && keyFrame < _nextKeyFrame )
   		{
   			return;
   		}
   	}
   	else
   	{
   		LoadKeyFramesPos();
   	}

   	UninitFrameCache();

   	for( TKeyFrames::iterator kfItr( _keyFrames.begin() ); kfItr != _keyFrames.end(); ++kfItr )
   	{
   		if( *kfItr > keyFrame )
   		{
   			_nextKeyFrame = *kfItr;

   			_frameCacheSize = static_cast< unsigned int >( round( ( _nextKeyFrame - _curKeyFrame ) * _videoStream->fps ) );

   			break;
   		}

   		_curKeyFrame = *kfItr;
   	}

   	InitFrameCache();

   	DemuxerSeek( _curKeyFrame );

   	_curPosInCache = 0;

   	bool frameCached( false );
   	do
   	{
   		frameCached = CacheNextFrame();
   	}
   	while( frameCached );
   }

   bool CVideoDecoder::CacheNextFrame()
   {
   	TRACE_FUN(  Frequently, "CVideoDecoder::CacheNextFrame" );

   	unsigned char* start( 0 );

   	_videoStream->last_pts = _videoStream->pts;

   	double frame_time = _videoStream->next_frame_time;

   	int frameSize( video_read_frame( _videoStream, &_videoStream->next_frame_time, &start, 0 ) );

   	_videoStream->timer += frame_time;

		if( frameSize == 6 )
		{
			if( _curPosInCache && _frameCache[ _curPosInCache ] )
			{
				SFrameItem* frameItem( _frameCache[ _curPosInCache++ ] );

				_frameCache[ _curPosInCache ] = frameItem;

				++frameItem->_ref;
			}
			else
			{
				start = _lastReadFrame;
				frameSize = _lastReadFrameSize;
			}
		}
		else if( frameSize != -1 )
		{
			delete _lastReadFrame;

			_lastReadFrameSize = frameSize;
			_lastReadFrame = new unsigned char[ _lastReadFrameSize ];

			memcpy( _lastReadFrame, start, _lastReadFrameSize );
		}

   	mp_image_t* decodedFrame( reinterpret_cast< mp_image_t* >( decode_video( _videoStream, start, frameSize, 0, _videoStream->pts ) ) );
   	if( decodedFrame )
   	{
   		if( filter_video( _videoStream, decodedFrame, _videoStream->pts ) )
   		{
   			if( _frameCache[ _curPosInCache ] )
   			{
   				++_curPosInCache;
   			}

   			if( !_curPosInCache && _previewItem && !_curKeyFrame )
   			{
   			   _frameCache[ _curPosInCache ] = _previewItem;
   			   ++_frameCache[ _curPosInCache ]->_ref;
   			}
   			else
   			{
      			_frameCache[ _curPosInCache ] = new SFrameItem();
      			_frameCache[ _curPosInCache ]->_ref = 1;

      			_frameCache[ _curPosInCache ]->_image.SetWidth( _width );
      			_frameCache[ _curPosInCache ]->_image.SetHeight( _height );
      			_frameCache[ _curPosInCache ]->_image.SetFormat( CImage::fRGB32 );
      			_frameCache[ _curPosInCache ]->_image.SetData( g_imageByteBuf );

      			if( !_previewItem && !_curKeyFrame )
      			{
      			   _previewItem = _frameCache[ _curPosInCache ];

      			   ++_previewItem->_ref;
      			}
   			}
   		}
   	}

   	return frameSize != -1 && _curPosInCache < frameCacheSize() - 1;
   }

   void CVideoDecoder::DemuxerSeek( float to )
   {
   	if( demux_seek( _demuxer, to, 0, 0x01 ) )
   	{
   		resync_video_stream( _videoStream );
   		_videoStream->num_buffered_pts = 0;
   		_videoStream->last_pts = MP_NOPTS_VALUE;
   	}
   }

   void CVideoDecoder::ClearCache()
   {
      UninitFrameCache();

      _nextKeyFrame = .0;
   }

   unsigned int CVideoDecoder::frameBufferSize()const
   {
      return width() * height() * bpp();
   }

   unsigned short CVideoDecoder::width()const
   {
      return _width;
   }

   unsigned short CVideoDecoder::height()const
   {
      return _height;
   }

   unsigned char CVideoDecoder::bpp() const
   {
      return _bpp;
   }

   unsigned int CVideoDecoder::framesCount()const
   {
      return _videoStream->video.dwLength;
   }

   bool CVideoDecoder::isCacheUpdated()const
   {
   	return _cacheUpdated;
   }

   unsigned int CVideoDecoder::curKeyFrame()const
   {
   	return static_cast< unsigned int >( round( _curKeyFrame * _videoStream->fps ) );
   }
}
