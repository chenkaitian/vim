#ifndef __TC_CALLBACK_DETAIL_H__
#define __TC_CALLBACK_DETAIL_H__

#include "tc_scoped_ptr.h"

namespace taf
{

    namespace detail 
	{

        class TC_BindStateBase {
        public:
            virtual ~TC_BindStateBase() { }
        };

        // This is a typetraits object that's used to take an argument type, and
        // extract a suitable type for storing and forwarding arguments.
        template <typename T>
        struct TC_CallbackParamTraits {
            typedef const T& ForwardType;
            typedef T StorageType;
        };

        template <typename T>
        struct TC_CallbackParamTraits<T&> {
            typedef T& ForwardType;
            typedef T StorageType;
        };

        template <typename T>
        struct TC_CallbackParamTraits<const T&> {
            typedef const T& ForwardType;
            typedef T StorageType;
        };

        // Note that for array types, we implicitly add a const in the conversion. This
        // means that it is not possible to bind array arguments to functions that take
        // a non-const pointer. Trying to specialize the template based on a "const
        // T[n]" does not seem to match correctly, so we are stuck with this
        // restriction.
        template <typename T>
        struct TC_CallbackParamTraits<T[]> {
            typedef const T* ForwardType;
            typedef const T* StorageType;
        };

        template <typename T, size_t n>
        struct TC_CallbackParamTraits<T[n]> {
            typedef const T* ForwardType;
            typedef const T* StorageType;
        };

        // Parameter traits for movable-but-not-copyable scopers.
        template <typename T>
        struct TC_CallbackParamTraits<TC_ScopedPtr<T> > {
            typedef TC_ScopedPtr<T> ForwardType;
            typedef TC_ScopedPtr<T> StorageType;
        };

    } // namespace detail

}

#endif
