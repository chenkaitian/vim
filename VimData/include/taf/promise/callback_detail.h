#ifndef __TAF_PROMISE_CALLBACK_DETAIL_H__
#define __TAF_PROMISE_CALLBACK_DETAIL_H__

#include "scoped_ptr.h"

namespace promise {

    namespace detail {

        class BindStateBase {
        public:
            virtual ~BindStateBase() { }
        };

        // This is a typetraits object that's used to take an argument type, and
        // extract a suitable type for storing and forwarding arguments.
        template <typename T>
        struct CallbackParamTraits {
            typedef const T& ForwardType;
            typedef T StorageType;
        };

        template <typename T>
        struct CallbackParamTraits<T&> {
            typedef T& ForwardType;
            typedef T StorageType;
        };

        template <typename T>
        struct CallbackParamTraits<const T&> {
            typedef const T& ForwardType;
            typedef T StorageType;
        };

        // Note that for array types, we implicitly add a const in the conversion. This
        // means that it is not possible to bind array arguments to functions that take
        // a non-const pointer. Trying to specialize the template based on a "const
        // T[n]" does not seem to match correctly, so we are stuck with this
        // restriction.
        template <typename T>
        struct CallbackParamTraits<T[]> {
            typedef const T* ForwardType;
            typedef const T* StorageType;
        };

        template <typename T, size_t n>
        struct CallbackParamTraits<T[n]> {
            typedef const T* ForwardType;
            typedef const T* StorageType;
        };

        // Parameter traits for movable-but-not-copyable scopers.
        template <typename T>
        struct CallbackParamTraits<ScopedPtr<T> > {
            typedef ScopedPtr<T> ForwardType;
            typedef ScopedPtr<T> StorageType;
        };

    } // namespace detail

} // namespace promise

#endif  // __PROMISE_CALLBACK_DETAIL_H__
