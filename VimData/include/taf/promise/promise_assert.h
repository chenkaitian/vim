#ifndef __TAF_PROMISE_ASSERT_H__
#define __TAF_PROMISE_ASSERT_H__

#include <cstdlib>
#include <iostream>

namespace promise 
{
    namespace detail 
	{        
        inline void assertion_failed(const char *expr, const char *function, const char *file, int line)
        {
            std::cerr << "at " << file << ':' << line << ": in " << function
                      << "(): Assertion `" << expr << "' failed" << std::endl;
            std::abort();
        }

    }  // namespace detail

}  // namespace promise

#define PROMISE_ASSERT(expr)                                                \
    ((expr) ? ((void)0) : ::promise::detail::assertion_failed(#expr, __func__, __FILE__, __LINE__))

namespace promise 
{
    namespace detail 
	{        
        template<bool> struct CompileTimeError;
        template<> struct CompileTimeError<true> {};

    }  // namespace detail

}  // namespace promise

#define TAF_PROMISE_STATIC_ASSERT(exp, msg)                                         \
	{ promise::detail::CompileTimeError<((exp) != 0)> ERROR_##msg; (void) ERROR_##msg; }

#endif  // __PROMISE_ASSERT_H__
