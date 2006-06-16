/*
 * Copyright 1999-2002,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Id: RefVectorOf.c 191054 2005-06-17 02:56:35Z jberry $
 */


// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#if defined(XERCES_TMPLSINC)
#include <xercesc/util/RefVectorOf.hpp>
#endif

XERCES_CPP_NAMESPACE_BEGIN

// ---------------------------------------------------------------------------
//  RefVectorOf: Constructors and Destructor
// ---------------------------------------------------------------------------
template <class TElem>
RefVectorOf<TElem>::RefVectorOf(const unsigned int maxElems,
                                const bool adoptElems,
                                MemoryManager* const manager)
    : BaseRefVectorOf<TElem>(maxElems, adoptElems, manager)
{
}

template <class TElem> RefVectorOf<TElem>::~RefVectorOf()
{
    if (this->fAdoptedElems)
    {
       for (unsigned int index = 0; index < this->fCurCount; index++)
        delete this->fElemList[index];
    }
    this->fMemoryManager->deallocate(this->fElemList);//delete [] this->fElemList;
}


XERCES_CPP_NAMESPACE_END
