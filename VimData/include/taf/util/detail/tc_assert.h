#ifndef __TC_ASSERT_H__
#define __TC_ASSERT_H__

#include <cstdlib>
#include <iostream>

namespace taf 
{
    namespace detail 
	{        
        inline void tc_assertion_failed(const char *expr, const char *function, const char *file, int line)
        {
            std::cerr << "at " << file << ':' << line << ": in " << function
                      << "(): Assertion `" << expr << "' failed" << std::endl;
            std::abort();
        }

		template<bool> struct TC_CompileTimeError;
        template<> struct TC_CompileTimeError<true> {};

    }

}

#define TC_ASSERT(expr)                                                \
    ((expr) ? ((void)0) : ::taf::detail::tc_assertion_failed(#expr, __func__, __FILE__, __LINE__))

#define TC_STATIC_ASSERT(exp, msg)                                         \
    { taf::detail::TC_CompileTimeError<((exp) != 0)> ERROR_##msg; (void) ERROR_##msg; }

#endif
