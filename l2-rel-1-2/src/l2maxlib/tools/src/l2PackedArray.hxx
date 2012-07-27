#ifndef L2PACKEDARRAY_HXX_
#define L2PACKEDARRAY_HXX_

namespace l2max
{
   class CFile;

   /*!
    * \brief The CPackedArray class is the class which represents compressed data sequence with given
    * precission.
    * The compression algorithm uses redundancy of data length to build indexes. The main idea is to
    * take as long memory as required. To implement index packing has used bit compression algorithm
    * with constant key length.
    * The constant key length allow quick access to any place where data is. It could be combined with other
    * compression methods like huffman but while develop additional compression give bad perfomance and
    * not recomended for use. Bad productivity tuns out because of these algorithm uses frequency tables
    * and doesn't produce random access to the data. Only consecutive access is allowed.
    *
    * \warning If difference between data will be less than precission in \b Compress() method then
    * compression will work with losses of data
    * */
	template< class TValue >
		class CPackedArray
		{
			TValue* _data;
			unsigned int _size;

			unsigned char* _indexTable;
			unsigned char _indexLenght;
			unsigned int _indexTableSize;

			struct SCompressionPredicate;

			unsigned int MakeIndex( unsigned int index )const;

			bool IsCompressed()const;

		public:
		   typedef TValue ValueType;

			CPackedArray();
			CPackedArray( const CPackedArray& pa );

			virtual ~CPackedArray();

			CPackedArray& operator = ( const CPackedArray& pa );

			TValue& operator [] ( unsigned int index );
			const TValue& operator [] ( unsigned int index )const;

			/*!
			 * Allocates memory to use raw array. You need to allocate memory
			 * any time before you use [] operator.
			 * \param size amount of memory in bytes
			 * */
			void SetSize( unsigned int size );

			/*!
			 * Compresses raw array.
			 * \param precission any data which after sorting will differ to precission
			 * will be removed after array will be compressed.
			 * */
			void Compress( TValue precission = 1 );

			/*!
			 * Serialize array
			 * */
			bool Read( CFile& f );

			/*!
			 * Deserialize array
			 * */
			void Write( CFile& f )const;
		};
}

#endif /*L2PACKEDARRAY_H_*/
