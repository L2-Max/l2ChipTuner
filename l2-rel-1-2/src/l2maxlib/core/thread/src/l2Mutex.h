#ifndef L2MUTEX_H_
#define L2MUTEX_H_

namespace l2max
{
   class CMutex
   {
     	struct SMutexPrivateHandle;
      SMutexPrivateHandle* _mutexPrivateHandle;
      unsigned int _lockCount;

   public:
      enum ELockType
      {
         ltInfinite = -1
      };

      enum EMutexType
      {
      	mtNonRecursive,
      	mtRecursive
      };

   private:
   	EMutexType _mutexType;

   	CMutex( const CMutex& );
   	CMutex& operator = ( const CMutex& );

   public:
   	CMutex( EMutexType aType = mtRecursive );
   	virtual ~CMutex();

   	bool Lock( unsigned int aTime = ltInfinite );
   	void UnLock();

   	bool ReLock( unsigned int aTime = 0 );

   	unsigned int lockCount()const;
   };
}

#endif /*L2MUTEX_H_*/
