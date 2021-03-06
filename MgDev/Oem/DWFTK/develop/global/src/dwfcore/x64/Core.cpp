//
//  Copyright (c) 2003-2006 by Autodesk, Inc.
//
//  By using this code, you are agreeing to the terms and conditions of
//  the License Agreement included in the documentation for this code.
//
//  AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED,
//  AS TO THE CORRECTNESS OF THIS CODE OR ANY DERIVATIVE
//  WORKS WHICH INCORPORATE IT.
//
//  AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS
//  AND EXPLICITLY DISCLAIMS ANY LIABILITY, INCLUDING
//  CONSEQUENTIAL AND INCIDENTAL DAMAGES FOR ERRORS,
//  OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
//
//  Use, duplication, or disclosure by the U.S. Government is subject to
//  restrictions set forth in FAR 52.227-19 (Commercial Computer Software
//  Restricted Rights) and DFAR 252.227-7013(c)(1)(ii) (Rights in Technical
//  Data and Computer Software), as applicable.
//


#include "dwfcore/Core.h"

//
// Intel and AMD x64 architecture implementation
//
#ifdef  _DWFCORE_X64_SYSTEM

#ifdef  _DWFCORE_WIN64_SYSTEM

_DWFCORE_API
int
DWFCore::AtomicIncrement( volatile int* pValue )
throw()
{
   //
   // An assertion failure results in error C2118: negative subscript.
   //
   C_ASSERT(sizeof(int) == sizeof(long));

   return InterlockedIncrement((volatile long*)pValue);
}

_DWFCORE_API
int
DWFCore::AtomicDecrement( volatile int* pValue )
throw()
{
   return InterlockedDecrement((volatile long*)pValue);
}

#elif defined(_DWFCORE_LINUX_SYSTEM)

//
//  TODO: Need 64-bit Linux
//  for AtomicIncrement and AtomicDecrement
//
		#ifdef  HAVE_CONFIG_H
		#include "dwfcore/config.h"

			#if !defined( _DWFCORE_USE_ASM_ATOMIC_H ) && !defined( _DWFCORE_USE_ALSA_IATOMIC_H )

				#if     defined( HAVE_ASM_ATOMIC_H )
				#include <asm/atomic.h>
				#elif   defined( HAVE_ALSA_IATOMIC_H )
				#include <alsa/iatomic.h>
				#else
				#error  Missing atomic header file containing definition of atomic_t - cannot continue
				#endif


				//
				// change LOCK to _DWFCORE_ATOMIC_LOCK
				// to prevent any unexpected redefinitions
				//
				#ifdef __SMP__
				#define _DWFCORE_ATOMIC_LOCK "lock ; "
				#else
				#define _DWFCORE_ATOMIC_LOCK ""
				#endif

				namespace DWFCore
				{

					_DWFCORE_API
						int
						AtomicIncrement( volatile int* pValue )
						throw()
					{
						atomic_t tValue;
						atomic_set( &tValue, *pValue );

						__asm__ __volatile__(
							_DWFCORE_ATOMIC_LOCK "incl %0"
							:"=m" (tValue.counter)
							:"m" (tValue.counter));

						*pValue = atomic_read( &tValue );
						return *pValue;
					}

					_DWFCORE_API
						int
						AtomicDecrement( volatile int* pValue )
						throw()
					{
						atomic_t tValue;
						atomic_set( &tValue, *pValue );

						__asm__ __volatile__(
							_DWFCORE_ATOMIC_LOCK "decl %0"
							:"=m" (tValue.counter)
							:"m" (tValue.counter));

						*pValue = atomic_read( &tValue );
						return *pValue;
					}

				}
#endif
#endif
#endif

#endif


