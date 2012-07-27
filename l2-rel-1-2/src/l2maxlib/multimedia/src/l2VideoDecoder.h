#ifndef L2VIDEODECODER_H_
#define L2VIDEODECODER_H_

#include <string>
#include <vector>

struct stream_st;
struct demuxer_st;
struct sh_video_st;
   
namespace l2max
{
	struct SFrameItem;
	class CImage;
	
	/*!
	 * \brief The CVideoDecoder class produce interface to simlify video decoding. The class used
	 * MPlayer engine. The best video codec for opened AVI file will be choosen from own MPlayer repository.
	 * Note: No one instaled Windows codecs used.
	 * */
   class CVideoDecoder
   {
      std::string _file;

      stream_st* _fileStream;
      demuxer_st* _demuxer;
      sh_video_st* _videoStream;

      unsigned short _width;
      unsigned short _height;
      unsigned char _bpp;
      
      unsigned int _curPosInCache;
      SFrameItem** _frameCache;
      unsigned int _frameCacheSize;
      
      unsigned char* _lastReadFrame;
		unsigned int _lastReadFrameSize;

      typedef std::vector< float > TKeyFrames;
      TKeyFrames _keyFrames;
      
      float _curKeyFrame;
      float _nextKeyFrame;
      
      bool _cacheUpdated;
      
      SFrameItem* _previewItem;
      
      void LoadKeyFramesPos();
      void PreloadFrames( int frame );
      void InitFrameCache();
      void UninitFrameCache();
      
      bool CacheNextFrame();
      void DemuxerSeek( float to );
      
	public:
	   /*!
	    * Constructs CVideoDecoder object and opens data source file
	    * \param file data source file (only AVI supported)
	    * */
      CVideoDecoder( const std::string& file = std::string() );
      virtual ~CVideoDecoder();

      /*!
       * Opens data source file
       * \param file data source file (only AVI supported)
       * \return true if succed otherwise false
       * */
      bool Open( const std::string& file );
      
      /*!
       * Returns decoded frame by frame number.
       * \param frameNum integer index of frame in data source
       * \return decoded frame
       * */
      const CImage& frame( int frameNum );

      /*!
       * Returns frame size
       * \return frame size in bytes
       * */
      unsigned int frameBufferSize() const;
      
      /*!
       * Returns frame count in data source
       * \return frame count
       * */
      unsigned int framesCount()const;
      
      /*!
       * Returns frame cache size
       * \return count frames in cache
       * */
      unsigned int frameCacheSize()const;
      
      /*!
       * Returns frame width
       * \return frame width
       * */
      unsigned short width() const;
      
      /*!
       * Returns frame height
       * \return frame height
       * */
      unsigned short height() const;
      
      /*!
       * Returns frame bytes per pixel
       * \return frame bytes per pixel
       * */
      unsigned char bpp() const;
      
      /*!
       * Tests if cache frame was changed since last request
       * \return true if cache was changed otherwise false
       * */
      bool isCacheUpdated()const;
      
      /*!
       * Clears farame cache
       * */
      void ClearCache();
      
      /*!
       * Returns current key frame. Current key frame is key frame
       * which is found betwen next key frame and current requested frame.
       * \return Key frame number
       * */
      unsigned int curKeyFrame()const;
   };

}

#endif /*CVIDEOFILE_H_*/
