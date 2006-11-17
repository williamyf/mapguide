// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_math.h
 *
 *  OS_NS_math.h,v 1.3 2003/11/01 23:42:24 dhinton Exp
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_MATH_H
# define ACE_OS_NS_MATH_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/ACE_export.h"

#if defined (ACE_EXPORT_MACRO)
#  undef ACE_EXPORT_MACRO
#endif
#define ACE_EXPORT_MACRO ACE_Export

namespace ACE_OS {

  /// This method computes the largest integral value not greater than x.
  ACE_NAMESPACE_INLINE_FUNCTION
  double floor (double x);

  /// This method computes the smallest integral value not less than x.
  ACE_NAMESPACE_INLINE_FUNCTION
  double ceil (double x);


} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_math.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_MATH_H */
