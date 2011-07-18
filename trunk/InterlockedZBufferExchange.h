
#ifndef INTERLOCKED_Z_BUFFER_EXCHANGE_H_
#define INTERLOCKED_Z_BUFFER_EXCHANGE_H_

#include <Windows.h>
#include "Pixel.h"
#include "Alignment.h"

//http://msdn.microsoft.com/en-us/library/ms683568(v=VS.85).aspx and http://blogs.msdn.com/b/doronh/archive/2006/12/06/creating-your-own-interlockedxxx-operation.aspx

template<typename ColorComponentDepth, typename zDepth, typename int dataSize>
inline bool InterlockedZBufferExchange(
    __inout  Pixel<ColorComponentDepth,zDepth,dataSize>* volatile *pixelInBuffer,
    __in     Pixel<ColorComponentDepth,zDepth,dataSize>* pixelToCompare	
    )
{
	PVOID volatile* pixInBuffer = (PVOID volatile*)pixelInBuffer;
    PVOID prevValue, prevCopy, pixelToExchange;
    
    prevValue = *pixInBuffer;
    pixelToExchange = pixelToCompare;
    do {
        if ( pixelToCompare->z >= (*pixelInBuffer)->z ) {
            return false;
        }
    
        prevCopy = prevValue;
    
        //
        // prevValue will be the value that used to be Target if the exchange was made
        // or its current value if the exchange was not made.
        //
        prevValue = InterlockedCompareExchangePointer(pixInBuffer, pixelToExchange, prevValue);
    
        //
        // If prevCopy == prevValue, then no one updated Target in between the deref at the top
        // and the InterlockecCompareExchange afterward and we are done
        //
    } while (prevCopy != prevValue);
    
    return true;
}

template<typename ColorComponentDepth, typename zDepth, typename int dataSize>
inline bool InterlockedZExchange(
    __inout  volatile Pixel<ColorComponentDepth,zDepth,dataSize>* pixelInBuffer,
    __in     Pixel<ColorComponentDepth,zDepth,dataSize>* pixelToCompare	
    )
{
	volatile LONG* zInBuffer = (volatile LONG*)(&pixelInBuffer->z);
    LONG prevValue, prevCopy, zToExchange;
    
    prevValue = *zInBuffer ;
    zToExchange = pixelToCompare->z;
    do {
        if ( pixelToCompare->z >= pixelInBuffer->z ) {
            return false;
        }
    
        prevCopy = prevValue;
    
        //
        // prevValue will be the value that used to be Target if the exchange was made
        // or its current value if the exchange was not made.
        //
        prevValue = InterlockedCompareExchange(zInBuffer, zToExchange, prevValue);
    
        //
        // If prevCopy == prevValue, then no one updated Target in between the deref at the top
        // and the InterlockecCompareExchange afterward and we are done
        //
    } while (prevCopy != prevValue);
    
    return true;
}

#endif
