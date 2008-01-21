//
//  Copyright (C) 2003-2007 by Autodesk, Inc.
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


#ifndef _DWFCORE_ITERATOR_H
#define _DWFCORE_ITERATOR_H



///
///\file        dwfcore/Iterator.h
///\brief       This header contains the declarations for iterator interfaces and implementations.
///


#include "dwfcore/Core.h"
#include "dwfcore/STL.h"
#include "dwfcore/String.h"
#include "dwfcore/Exception.h"


namespace DWFCore
{


///
///\interface   DWFIterator     dwfcore/Iterator.h  "dwfcore/Iterator.h"
///\brief       An interface template for iterators.
///\since       1.0.1
///
///             This iterator interface is used to return objects that
///             are contained in a collection.
///             
///             Typically, this interface is used in a \e for loop.
///             Also, this interface may be returned as a pointer
///             that must be managed by the caller.  The following code
///             snippet shows the standard use of these objects:
///         
///             \code 
///             DWFIterator* pIterator = function_returning_iterator();
///             if (pIterator)
///             {
///                 for (; pIterator->valid(); pIterator->next())
///                 {
///                     function_using_object( pIterator->get() );
///                 }
///             
///                 DWFCORE_FREE_OBJECT( pIterator );
///             }
///             \endcode
///
///\param       T   The type of content returned by the iterator.
///
template<class T>
class DWFIterator
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFIterator()
        throw()
    {;}

    ///
    ///         Reset the iterator back to its initial position
    ///
    ///\throw   None
    ///
    virtual void reset()
        throw() = 0;

    ///
    ///         Determines if the iterator points to a valid element.
    ///
    ///\return  \e true if \a get() will return a valid element,
    ///         \e false if the end of the iterator has been reached.
    ///\throw   None
    ///
    virtual bool valid()
        throw() = 0;

    ///
    ///         Move the iterator forward one element.
    ///
    ///\return  \e true if the iterator points to a valid element,
    ///         \e false if the end of the iterator has been reached.
    ///\throw   None
    ///
    virtual bool next()
        throw() = 0;

    ///
    ///         Returns the element at the current position.
    ///
    ///\return  The current element.
    ///\throw   DWFException
    ///
    virtual T& get()
        throw( DWFException ) = 0;

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFIterator()
        throw()
    {;}
};



///
///\class       DWFStringVectorIterator     dwfcore/Iterator.h  "dwfcore/Iterator.h"
///\brief       An iterator implementation for an STL vector of DWFString.
///\since       1.0.1
///
///             This iterator is used to bind a DWFIterator interface
///             to an STL vector collection.
///
class DWFStringVectorIterator : public DWFCoreMemory
                              , public DWFIterator<DWFString>
{

public:

    ///
    ///         Constructor
    ///
    ///\param   rVector The vector over which to iterate.
    ///\throw   None
    ///
    DWFStringVectorIterator( vector<DWFString>& rVector )
        throw()
        : _rVector( rVector )
        , _iCurrent( 0 )
    {;}

    ///
    ///         Copy Constructor
    ///         This constructor will result in a new iterator pointing
    ///         to the same position in the underlying vector as the source iterator.
    ///
    ///\param   i   The iterator to clone.
    ///\throw   None
    ///
    DWFStringVectorIterator( const DWFStringVectorIterator& i )
        throw()
        : _rVector( i._rVector )
        , _iCurrent( i._iCurrent )
    {;}

    ///
    ///         Assignment Operator
    ///         This operator will assign this iterator
    ///         to the same position in the underlying vector as the source iterator.
    ///
    ///\param   i   The iterator to clone.
    ///\throw   None
    ///
    DWFStringVectorIterator& operator=( const DWFStringVectorIterator& i )
        throw()
    {
        _iCurrent = i._iCurrent;

        size_t n = 0;
        for (; n < i._rVector.size(); n++)
        {
            _rVector.push_back( i._rVector[n] );
        }

        return *this;
    }

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFStringVectorIterator()
        throw()
    {;}

    ///
    ///\copydoc DWFIterator::reset()
    ///
    void reset()
        throw()
    {
        _iCurrent = 0;
    }

    ///
    ///\copydoc DWFIterator::valid()
    ///
    bool valid()
        throw()
    {
        return (_iCurrent < _rVector.size());
    }

    ///
    ///\copydoc DWFIterator::next()
    ///
    bool next()
        throw()
    {
        if (valid())
        {
            _iCurrent++;
            return valid();
        }
        else
        {
            return false;
        }
    }

    ///
    ///\copydoc DWFIterator::get()
    ///
    DWFString& get()
        throw( DWFException )
    {
        if (valid())
        {
            return _rVector[_iCurrent];
        }
        else
        {
            _DWFCORE_THROW( DWFDoesNotExistException, L"No more elements" );
        }
    }

private:

    vector<DWFString>& _rVector;
    size_t  _iCurrent;
};

///
///\class       DWFBasicIteratorImpl     dwfcore/Iterator.h  "dwfcore/Iterator.h"
///\brief       An iterator and basic collection implementation template.
///\since       1.0.1
///
///             This implementation provides an iterator interface over
///             a simple array-based collection of \e T type objects.
///             This class is useful when a simple array of objects must
///             be created and a DWFIterator interface must be returned
///             for it.
///             Memory allocation in the array is simple doubling with 
///             additional consideration for the hint provided:
///
///             \code _nAlloc = max(2*_nAlloc, _nAlloc + _nHint) \endcode
///
///             and is intended to reduce memory allocations over the lifetime
///             of the collection.  When a new allocation is required, 
///             the previous array is copied into the new one all at once.
///
template<class T>
class DWFBasicIteratorImpl : public DWFCoreMemory
                           , public DWFIterator<T>
{

public:

    ///
    ///         Constructor
    ///
    ///\param   nHint   Best guess at the number of elements in the collection.
    ///                 The more accurate this number is, the fewer memory allocations
    ///                 will occur over the lifetime of the collection.
    ///\throw   None
    ///
    DWFBasicIteratorImpl( uint16_t nHint = 16 )
        throw()
        : _ppT( NULL )
        , _nCount( 0 )
        , _nAlloc( 0 )
        , _nHint( nHint )
        , _iCurrent( -1 )
    {;}

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFBasicIteratorImpl()
        throw()
    {
        if (_ppT)
        {
            DWFCORE_FREE_MEMORY( _ppT );
        }
    }

    ///
    ///\copydoc DWFIterator::reset()
    ///
    virtual void reset()
        throw()
    {
        _iCurrent = (_nCount > 0) ? 0 : -1;
    }

    ///
    ///\copydoc DWFIterator::valid()
    ///
    virtual bool valid()
        throw()
    {
        return (_iCurrent != -1);
    }

    ///
    ///\copydoc DWFIterator::next()
    ///
    virtual bool next()
        throw()
    {
        if (_iCurrent != -1)
        {
                //
                // cast ok - _iCurrent never less than -1
                //
            if ((size_t)(++_iCurrent) == _nCount)
            {
                _iCurrent = -1;
            }
        }

        return (_iCurrent != -1);
    }

    ///
    ///\copydoc DWFIterator::get()
    ///
    virtual T& get()
        throw( DWFException )
    {
        if (_iCurrent != -1)
        {
            return *(_ppT[_iCurrent]);
        }
        else
        {
            _DWFCORE_THROW( DWFDoesNotExistException, L"No element" );
        }
    }

    ///
    ///         Add an element to the collection.
    ///         This operation does not alter the iterator position unless
    ///         it was previously invalid due to the existence of no elements.
    ///
    ///\param   rT  The element to add.
    ///\throw   DWFException
    ///
    virtual void add( T& rT )
        throw( DWFException )
    {
        if ((_nAlloc - _nCount) == 0)
        {
            _nAlloc = dwf_max(2*_nAlloc, _nAlloc + _nHint);
            T** ppNew = (T**) DWFCORE_ALLOC_MEMORY( T*, _nAlloc );
            if (ppNew == NULL)
            {
                _DWFCORE_THROW( DWFMemoryException, L"Failed to allocate buffer" );
            }

            DWFCORE_COPY_MEMORY( ppNew, _ppT, _nCount*sizeof(T*) );
            DWFCORE_FREE_MEMORY( _ppT );

            _ppT = ppNew;
        }

        _ppT[_nCount++] = &rT;

        if (_iCurrent == -1)
        {
            _iCurrent = 0;
        }
    }

private:

    T**         _ppT;
    size_t      _nCount;
    size_t      _nAlloc;
    uint16_t    _nHint;

    off_t       _iCurrent;
};



///
///\interface   DWFKVIterator     dwfcore/Iterator.h  "dwfcore/Iterator.h"
///\brief       An interface template for \e key-value iterators.
///\since       1.0.1
///
///             This iterator interface is used to return objects that
///             are stored in a keyed collection (a map, for example).
///\param       T   The type of content returned by the iterator.
///
template<class K, class V>
class DWFKVIterator
{

public:

    ///
    ///         Destructor
    ///
    ///\throw   None
    ///
    virtual ~DWFKVIterator()
        throw()
    {;}

    ///
    ///\copydoc DWFIterator::reset()
    ///
    virtual void reset()
        throw() = 0;

    ///
    ///\copydoc DWFIterator::valid()
    ///
    virtual bool valid()
        throw() = 0;

    ///
    ///\copydoc DWFIterator::next()
    ///
    virtual bool next()
        throw() = 0;

    ///
    ///         Returns the key of the element at the current position.
    ///
    ///\return  The current key.
    ///\throw   DWFException
    ///    
    virtual K& key()
        throw( DWFException ) = 0;

    ///
    ///         Returns the value of the element at the current position.
    ///
    ///\return  The current value.
    ///\throw   DWFException
    ///    
    virtual V& value()
        throw( DWFException ) = 0;

protected:

    ///
    ///         Constructor
    ///
    ///\throw   None
    ///
    DWFKVIterator()
        throw()
    {;}
};

}


#endif
