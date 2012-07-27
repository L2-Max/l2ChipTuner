#ifndef L2VIDEOENCODER_H_
#define L2VIDEOENCODER_H_

#include <string>

struct stream_st;
struct muxer_t;
struct muxer_stream_s;
struct vf_instance_s;
struct mp_image_s;
struct sh_video_st;

struct SwsContext;

namespace l2max
{
   class CImage;
   
   /*!
    * \brief The CVideoEncoder class produce interface to simlify video encoding. The encoder
    * is use Xvid mp4 encoder. No other encoders are supported now. Encoded mp4 video is writen into 
    * AVI.
    * */
   class CVideoEncoder
   {
      std::string _outFile;
      
      int _outFileFormat;
      int _sourceStride[ 3 ];
      
      stream_st* _outStream;
      muxer_t* _muxer;
      muxer_stream_s* _muxerStream;
      vf_instance_s* _vfilter;
      mp_image_s* _mpimage;
      
      SwsContext* _swsContext;
      
   public:
   	CVideoEncoder();
   	virtual ~CVideoEncoder();
   	
   	/*!
   	 * Initialize internal context of CVideoEncoder object.
   	 * \param outFile file where encoded video will be saved in.
   	 * \param width width of source images
   	 * \param height height of source image
   	 * \return true if successed otherwize false
   	 * */
   	bool Open( const std::string& outFile, int width, int height );
   	
   	/*!
   	 * Encodes and writes image into output file
   	 * \param image source image
   	 * \return true if successed otherwize false
   	 * */
   	bool Write( CImage& image );
   	
      /*!
       * Closes internal context of CVideoEncoder object
       * */
   	void Close();
   };
}

#endif /*L2VIDEOENCODER_H_*/
