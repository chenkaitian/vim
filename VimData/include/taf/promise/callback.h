#ifndef __TAF_PROMISE_CALLBACK_H__
#define __TAF_PROMISE_CALLBACK_H__

#include "shared_ptr.h"
#include "callback_detail.h"

// ---------------------------------------------------------------------------
// Introduction
// ---------------------------------------------------------------------------
//
// The templated Callback class is a generalized function object. Together with bind(),
// they provide a type-safe method of performing curring of arguments.
//
// MEMORY MANAGEMENT AND PASSING
//
// The Callback objects themselves should be passed by const-reference, and stored by
// copy. They internally store their state via a refcounted smart pointer and thus do not
// need to be deleted.
//
// The reason to pass via a const-reference is to avoid unnecessary increment/decrement to
// the internal refcount.
//
// ---------------------------------------------------------------------------
// Quick Reference
// ---------------------------------------------------------------------------
//
// BINDING A BARE FUNCTION
//
//   int foo() { return 3; }
//   promise::Callback<int(void)> cb = promise::bind(&foo);
//   std::cout << cb();  // prints 3
//
// BINDING A CLASS METHOD
//
//   The first argument to bind is the member function to call, the second is the object
//   on which to call it.
//
//   class X {
//   public:
//       int foo() { return 3; }
//   };
//   promise::Callback<int(void)> cb = promise::bind(&X::foo, promise::owned(new X()));
//   std::cout << cb(); // prints 3
//
//   When binding the object you must specify who is responsible for the memory of the
//   object. For the above example, promise::owned() specifies the callback be responsible for
//   the deallocation of the object. If you pass a raw pointer to the object you will get
//   a compiler error. Because passing a raw pointer may cause memory leak.
//
// RUNNING A CALLBACK
//
//   Callbacks have overloaded their function application operator, which has the same
//   signuation as the template argument to the Callback.
//
//   void do_stuff(const promise::Callback<void(int, std::string)>& callback)
//   {
//       callback(5, "hello");
//   }
//
//   Callbacks can be run more than once (they don't get deleted or marked when run).
//
// PASSING UNBOUND INPUT PARAMETERS
//
//   Unbound parameters are specified at the time a callback is run. They are specified in
//   the Callback template type:
//
//   void foo(int a, const std::string& s) { }
//   promise::Callback<void(int, const std::string&)> cb = promise::bind(&foo);
//   cb(1, "hello");
//
// PASSING BOUND INPUT PARAMETERS
//
//   Bound parameters are specified when you create the callback as arguments to
//   bind(). They will be passed to the function when callback is run.
//
//   void foo(int a, const std::string& s) { }
//   promise::Callback<void(void)> cb = promise::bind(&foo, 1, promise::unretained("hello"));
//   cb();
//
//   (see below for why I used promise::unretained() here)
//   
//   When calling member functions, bound parameters just go after the object pointer
//   (indeed the object pointer is the first bound parameter).
//
// PARTIAL BINDING OF PARAMETERS
//
//   You can specify some parameters when you create the callback, and specify the rest
//   when you execute the callback.
//
//   void foo(int a, const std::string& s) { }
//   promise::Callback<void(const std::string& s)> cb = base::bind(&foo, 1);
//   cb("hello");
//
//   When calling a function bound parameters are first followed by unbound parameters.
//
// ---------------------------------------------------------------------------
// Specify memory management semantics when binding parameters
// ---------------------------------------------------------------------------
//
// When binding parameters of raw pointer type we must specify who, the caller or the
// callback, should be responsible for the deallocation of the memory the pointer is
// pointing to.
// 
// BINDING PARAMETERS WITH MANUAL LIFETIME MANAGEMENT
//
//   void foo(int a, const std::string& s) { }
//   promise::Callback<void(void)> cb = promise::bind(&foo, 1, promise::unretained("hello"));
//   cb(); 
//
//   This specifies that the caller owns the memory. And you must make sure the object is
//   alive at the time callback is run.
//
// BINDING PARAMETERS OWNED BY THE CALLBACK
//
//   X *px = new X();
//   promise::Callback<void(void)> cb = promise::bind(&X::foo, promise::owned(px));
//
//   This specifies the callback owns the object. The object will be deleted when the
//   callback is destroyed, even if it's not run.
//
// BINDING PARAMETERS SHARED BY THE CALLER AND THE CALLBACK
//
//   promise::SharedPtr<X> p(new X());
//   promise::Callback<void(void)> cb = promise::bind(&X::foo, promise::shared(p));
//
//   This specifies the object is shared by the caller and the callback. The object will
//   be deleted when the last reference from the caller or the callback is gone.
//
// BINDING PARAMETERS OF ARRAY TYPE
//
//   int bar(int a[2]) { return a[0] + a[1]; }
//   int foo(const int a[2]) { return a[0] + a[1]; }
//   int a[2] = { 3, 4 };
//   promise::Callback<int(int*)> cb = promise::bind(&bar, a);  // Error!
//   promise::Callback<int(const int*)> cb = promise::bind(&foo, a);  // OK!
//   promise::Callback<int(int*)> cb = promise::bind(&bar, promise::unretained(&a[0]));  // OK!
//
//   The template argument deduction logic of C++ deduces the type of "a" as "int[]" and
//   bind() passes all bound parameters as const-reference so "a" is a const-reference to
//   int[]. But the callback "cb" accepts non-const pointer and const-pointer can not be
//   converted to non-const pointer so the first bind() is error and the second bind() is
//   OK. That is, we can't bind array arguments to functions that take a non-const pointer.
//
//   To work around this limitation we can explicitly specify the type of the bound
//   parameter as a pointer to the first element of the array, like what the third bind()
//   did.
//
//   Alternatively I recomend to use std::vector instead to avoid potential memory leak.
//
// ---------------------------------------------------------------------------
// Implementation notes
// ---------------------------------------------------------------------------
//
// The design is adapted from http://src.chromium.org/viewvc/chrome/trunk/src/base/callback.h?content-type=text%2Fplain

namespace promise {

    namespace detail {
        template <typename FunctorType, typename BindRunType, typename BoundArgsType> class BindState;
    } // namespace detail

    template <typename Signature>
    class Callback;

    template <typename R>
    class Callback<R(void)> {
    private:
        typedef void (*GenFuncPointer)(void);
        typedef GenFuncPointer unspecified_bool_type;
        
    public:

        typedef R (RunType)(void);
        
        Callback()
            : m_invoke(NULL)
        { }
        
        // Should be private, but avoids template friendship issues.
        template <typename FunctorType, typename BindRunType, typename BoundArgsType>
        Callback(detail::BindState<FunctorType, BindRunType, BoundArgsType>* bind_state)
            : m_bind_state_ptr(bind_state)
        {
            PolymorphicInvoke invoke = &detail::BindState<FunctorType, BindRunType, BoundArgsType>::InvokerType::invoke;
            m_invoke = reinterpret_cast<GenFuncPointer>(invoke);
        }

        R operator()() const
        {
            PolymorphicInvoke f = reinterpret_cast<PolymorphicInvoke>(m_invoke);
            return f(m_bind_state_ptr.get());
        }

        operator unspecified_bool_type() const
        {
            return m_invoke;
        }
        
    private:
        typedef R (*PolymorphicInvoke)(detail::BindStateBase*);

        SharedPtr<detail::BindStateBase> m_bind_state_ptr;
        GenFuncPointer m_invoke;
    };

    template <typename R, typename A1>
    class Callback<R(A1)> {
    private:
        typedef void (*GenFuncPointer)(void);
        typedef GenFuncPointer unspecified_bool_type;
        
    public:

        typedef R (RunType)(A1);
        
        Callback()
            : m_invoke(NULL)
        { }

        // Should be private, but avoids template friendship issues.
        template <typename FunctorType, typename BindRunType, typename BoundArgsType>
        Callback(detail::BindState<FunctorType, BindRunType, BoundArgsType>* bind_state)
            : m_bind_state_ptr(bind_state)
        {
            PolymorphicInvoke invoke = &detail::BindState<FunctorType, BindRunType, BoundArgsType>::InvokerType::invoke;
            m_invoke = reinterpret_cast<GenFuncPointer>(invoke);
        }

        R operator()(typename detail::CallbackParamTraits<A1>::ForwardType a1) const
        {
            PolymorphicInvoke f = reinterpret_cast<PolymorphicInvoke>(m_invoke);
            return f(m_bind_state_ptr.get(), a1);
        }

        operator unspecified_bool_type() const
        {
            return m_invoke;
        }
        
    private:

        typedef R (*PolymorphicInvoke)(detail::BindStateBase*,
                                       typename detail::CallbackParamTraits<A1>::ForwardType);

        SharedPtr<detail::BindStateBase> m_bind_state_ptr;
        GenFuncPointer m_invoke;
    };
    
    template <typename R, typename A1, typename A2>
    class Callback<R(A1, A2)> {
    private:
        typedef void (*GenFuncPointer)(void);
        typedef GenFuncPointer unspecified_bool_type;
        
    public:

        typedef R (RunType)(A1, A2);
        
        Callback()
            : m_invoke(NULL)
        { }

        // Should be private, but avoids template friendship issues.
        template <typename FunctorType, typename BindRunType, typename BoundArgsType>
        Callback(detail::BindState<FunctorType, BindRunType, BoundArgsType>* bind_state)
            : m_bind_state_ptr(bind_state)
        {
            PolymorphicInvoke invoke = &detail::BindState<FunctorType, BindRunType, BoundArgsType>::InvokerType::invoke;
            m_invoke = reinterpret_cast<GenFuncPointer>(invoke);
        }

        R operator()(typename detail::CallbackParamTraits<A1>::ForwardType a1,
                     typename detail::CallbackParamTraits<A2>::ForwardType a2)
        {
            PolymorphicInvoke f = reinterpret_cast<PolymorphicInvoke>(m_invoke);
            return f(m_bind_state_ptr.get(), a1, a2);
        }

        operator unspecified_bool_type() const
        {
            return m_invoke;
        }
        
    private:

        typedef R (*PolymorphicInvoke)(detail::BindStateBase*,
                                       typename detail::CallbackParamTraits<A1>::ForwardType,
                                       typename detail::CallbackParamTraits<A2>::ForwardType);

        SharedPtr<detail::BindStateBase> m_bind_state_ptr;
        GenFuncPointer m_invoke;
    };

    template <typename R, typename A1, typename A2, typename A3>
    class Callback<R(A1, A2, A3)> {
    private:
        typedef void (*GenFuncPointer)(void);
        typedef GenFuncPointer unspecified_bool_type;
        
    public:

        typedef R (RunType)(A1, A2, A3);
        
        Callback()
            : m_invoke(NULL)
        { }

        // Should be private, but avoids template friendship issues.
        template <typename FunctorType, typename BindRunType, typename BoundArgsType>
        Callback(detail::BindState<FunctorType, BindRunType, BoundArgsType>* bind_state)
            : m_bind_state_ptr(bind_state)
        {
            PolymorphicInvoke invoke = &detail::BindState<FunctorType, BindRunType, BoundArgsType>::InvokerType::invoke;
            m_invoke = reinterpret_cast<GenFuncPointer>(invoke);
        }

        R operator()(typename detail::CallbackParamTraits<A1>::ForwardType a1,
                     typename detail::CallbackParamTraits<A2>::ForwardType a2,
                     typename detail::CallbackParamTraits<A3>::ForwardType a3) const
        {
            PolymorphicInvoke f = reinterpret_cast<PolymorphicInvoke>(m_invoke);
            return f(m_bind_state_ptr.get(), a1, a2, a3);
        }

        operator unspecified_bool_type() const
        {
            return m_invoke;
        }
        
    private:

        typedef R (*PolymorphicInvoke)(detail::BindStateBase*,
                                       typename detail::CallbackParamTraits<A1>::ForwardType a1,
                                       typename detail::CallbackParamTraits<A2>::ForwardType a2,
                                       typename detail::CallbackParamTraits<A3>::ForwardType a3);

        SharedPtr<detail::BindStateBase> m_bind_state_ptr;
        GenFuncPointer m_invoke;
    };

    template <typename R, typename A1, typename A2, typename A3, typename A4>
    class Callback<R(A1, A2, A3, A4)> {
    private:
        typedef void (*GenFuncPointer)(void);
        typedef GenFuncPointer unspecified_bool_type;
        
    public:

        typedef R (RunType)(A1, A2, A3, A4);
        
        Callback()
            : m_invoke(NULL)
        { }

        // Should be private, but avoids template friendship issues.
        template <typename FunctorType, typename BindRunType, typename BoundArgsType>
        Callback(detail::BindState<FunctorType, BindRunType, BoundArgsType>* bind_state)
            : m_bind_state_ptr(bind_state)
        {
            PolymorphicInvoke invoke = &detail::BindState<FunctorType, BindRunType, BoundArgsType>::InvokerType::invoke;
            m_invoke = reinterpret_cast<GenFuncPointer>(invoke);
        }

        R operator()(typename detail::CallbackParamTraits<A1>::ForwardType a1,
                     typename detail::CallbackParamTraits<A2>::ForwardType a2,
                     typename detail::CallbackParamTraits<A3>::ForwardType a3,
                     typename detail::CallbackParamTraits<A4>::ForwardType a4) const
        {
            PolymorphicInvoke f = reinterpret_cast<PolymorphicInvoke>(m_invoke);
            return f(m_bind_state_ptr.get(), a1, a2, a3, a4);
        }

        operator unspecified_bool_type() const
        {
            return m_invoke;
        }
        
    private:

        typedef R (*PolymorphicInvoke)(detail::BindStateBase*,
                                       typename detail::CallbackParamTraits<A1>::ForwardType a1,
                                       typename detail::CallbackParamTraits<A2>::ForwardType a2,
                                       typename detail::CallbackParamTraits<A3>::ForwardType a3,
                                       typename detail::CallbackParamTraits<A4>::ForwardType a4);

        SharedPtr<detail::BindStateBase> m_bind_state_ptr;
        GenFuncPointer m_invoke;
    };

    template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
    class Callback<R(A1, A2, A3, A4, A5)> {
    private:
        typedef void (*GenFuncPointer)(void);
        typedef GenFuncPointer unspecified_bool_type;
        
    public:

        typedef R (RunType)(A1, A2, A3, A4, A5);
        
        Callback()
            : m_invoke(NULL)
        { }

        // Should be private, but avoids template friendship issues.
        template <typename FunctorType, typename BindRunType, typename BoundArgsType>
        Callback(detail::BindState<FunctorType, BindRunType, BoundArgsType>* bind_state)
            : m_bind_state_ptr(bind_state)
        {
            PolymorphicInvoke invoke = &detail::BindState<FunctorType, BindRunType, BoundArgsType>::InvokerType::invoke;
            m_invoke = reinterpret_cast<GenFuncPointer>(invoke);
        }

        R operator()(typename detail::CallbackParamTraits<A1>::ForwardType a1,
                     typename detail::CallbackParamTraits<A2>::ForwardType a2,
                     typename detail::CallbackParamTraits<A3>::ForwardType a3,
                     typename detail::CallbackParamTraits<A4>::ForwardType a4,
                     typename detail::CallbackParamTraits<A5>::ForwardType a5) const
        {
            PolymorphicInvoke f = reinterpret_cast<PolymorphicInvoke>(m_invoke);
            return f(m_bind_state_ptr.get(), a1, a2, a3, a4, a5);
        }

        operator unspecified_bool_type() const
        {
            return m_invoke;
        }
        
    private:

        typedef R (*PolymorphicInvoke)(detail::BindStateBase*,
                                       typename detail::CallbackParamTraits<A1>::ForwardType a1,
                                       typename detail::CallbackParamTraits<A2>::ForwardType a2,
                                       typename detail::CallbackParamTraits<A3>::ForwardType a3,
                                       typename detail::CallbackParamTraits<A4>::ForwardType a4,
                                       typename detail::CallbackParamTraits<A5>::ForwardType a5);

        SharedPtr<detail::BindStateBase> m_bind_state_ptr;
        GenFuncPointer m_invoke;
    };

    template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    class Callback<R(A1, A2, A3, A4, A5, A6)> {
    private:
        typedef void (*GenFuncPointer)(void);
        typedef GenFuncPointer unspecified_bool_type;
        
    public:

        typedef R (RunType)(A1, A2, A3, A4, A5, A6);
        
        Callback()
            : m_invoke(NULL)
        { }

        // Should be private, but avoids template friendship issues.
        template <typename FunctorType, typename BindRunType, typename BoundArgsType>
        Callback(detail::BindState<FunctorType, BindRunType, BoundArgsType>* bind_state)
            : m_bind_state_ptr(bind_state)
        {
            PolymorphicInvoke invoke = &detail::BindState<FunctorType, BindRunType, BoundArgsType>::InvokerType::invoke;
            m_invoke = reinterpret_cast<GenFuncPointer>(invoke);
        }

        R operator()(typename detail::CallbackParamTraits<A1>::ForwardType a1,
                     typename detail::CallbackParamTraits<A2>::ForwardType a2,
                     typename detail::CallbackParamTraits<A3>::ForwardType a3,
                     typename detail::CallbackParamTraits<A4>::ForwardType a4,
                     typename detail::CallbackParamTraits<A5>::ForwardType a5,
                     typename detail::CallbackParamTraits<A6>::ForwardType a6) const
        {
            PolymorphicInvoke f = reinterpret_cast<PolymorphicInvoke>(m_invoke);
            return f(m_bind_state_ptr.get(), a1, a2, a3, a4, a5, a6);
        }

        operator unspecified_bool_type() const
        {
            return m_invoke;
        }
        
    private:

        typedef R (*PolymorphicInvoke)(detail::BindStateBase*,
                                       typename detail::CallbackParamTraits<A1>::ForwardType a1,
                                       typename detail::CallbackParamTraits<A2>::ForwardType a2,
                                       typename detail::CallbackParamTraits<A3>::ForwardType a3,
                                       typename detail::CallbackParamTraits<A4>::ForwardType a4,
                                       typename detail::CallbackParamTraits<A5>::ForwardType a5,
                                       typename detail::CallbackParamTraits<A6>::ForwardType a6);

        SharedPtr<detail::BindStateBase> m_bind_state_ptr;
        GenFuncPointer m_invoke;
    };

    template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    class Callback<R(A1, A2, A3, A4, A5, A6, A7)> {
    private:
        typedef void (*GenFuncPointer)(void);
        typedef GenFuncPointer unspecified_bool_type;
        
    public:

        typedef R (RunType)(A1, A2, A3, A4, A5, A6, A7);
        
        Callback()
            : m_invoke(NULL)
        { }

        // Should be private, but avoids template friendship issues.
        template <typename FunctorType, typename BindRunType, typename BoundArgsType>
        Callback(detail::BindState<FunctorType, BindRunType, BoundArgsType>* bind_state)
            : m_bind_state_ptr(bind_state)
        {
            PolymorphicInvoke invoke = &detail::BindState<FunctorType, BindRunType, BoundArgsType>::InvokerType::invoke;
            m_invoke = reinterpret_cast<GenFuncPointer>(invoke);
        }

        R operator()(typename detail::CallbackParamTraits<A1>::ForwardType a1,
                     typename detail::CallbackParamTraits<A2>::ForwardType a2,
                     typename detail::CallbackParamTraits<A3>::ForwardType a3,
                     typename detail::CallbackParamTraits<A4>::ForwardType a4,
                     typename detail::CallbackParamTraits<A5>::ForwardType a5,
                     typename detail::CallbackParamTraits<A6>::ForwardType a6,
                     typename detail::CallbackParamTraits<A7>::ForwardType a7) const
        {
            PolymorphicInvoke f = reinterpret_cast<PolymorphicInvoke>(m_invoke);
            return f(m_bind_state_ptr.get(), a1, a2, a3, a4, a5, a6, a7);
        }

        operator unspecified_bool_type() const
        {
            return m_invoke;
        }
        
    private:

        typedef R (*PolymorphicInvoke)(detail::BindStateBase*,
                                       typename detail::CallbackParamTraits<A1>::ForwardType a1,
                                       typename detail::CallbackParamTraits<A2>::ForwardType a2,
                                       typename detail::CallbackParamTraits<A3>::ForwardType a3,
                                       typename detail::CallbackParamTraits<A4>::ForwardType a4,
                                       typename detail::CallbackParamTraits<A5>::ForwardType a5,
                                       typename detail::CallbackParamTraits<A6>::ForwardType a6,
                                       typename detail::CallbackParamTraits<A7>::ForwardType a7);

        SharedPtr<detail::BindStateBase> m_bind_state_ptr;
        GenFuncPointer m_invoke;
    };

    template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5,
              typename A6, typename A7, typename A8>
    class Callback<R(A1, A2, A3, A4, A5, A6, A7, A8)> {
    private:
        typedef void (*GenFuncPointer)(void);
        typedef GenFuncPointer unspecified_bool_type;
        
    public:

        typedef R (RunType)(A1, A2, A3, A4, A5, A6, A7, A8);
        
        Callback()
            : m_invoke(NULL)
        { }

        // Should be private, but avoids template friendship issues.
        template <typename FunctorType, typename BindRunType, typename BoundArgsType>
        Callback(detail::BindState<FunctorType, BindRunType, BoundArgsType>* bind_state)
            : m_bind_state_ptr(bind_state)
        {
            PolymorphicInvoke invoke = &detail::BindState<FunctorType, BindRunType, BoundArgsType>::InvokerType::invoke;
            m_invoke = reinterpret_cast<GenFuncPointer>(invoke);
        }

        R operator()(typename detail::CallbackParamTraits<A1>::ForwardType a1,
                     typename detail::CallbackParamTraits<A2>::ForwardType a2,
                     typename detail::CallbackParamTraits<A3>::ForwardType a3,
                     typename detail::CallbackParamTraits<A4>::ForwardType a4,
                     typename detail::CallbackParamTraits<A5>::ForwardType a5,
                     typename detail::CallbackParamTraits<A6>::ForwardType a6,
                     typename detail::CallbackParamTraits<A7>::ForwardType a7,
                     typename detail::CallbackParamTraits<A8>::ForwardType a8) const
        {
            PolymorphicInvoke f = reinterpret_cast<PolymorphicInvoke>(m_invoke);
            return f(m_bind_state_ptr.get(), a1, a2, a3, a4, a5, a6, a7, a8);
        }

        operator unspecified_bool_type() const
        {
            return m_invoke;
        }
        
    private:

        typedef R (*PolymorphicInvoke)(detail::BindStateBase*,
                                       typename detail::CallbackParamTraits<A1>::ForwardType a1,
                                       typename detail::CallbackParamTraits<A2>::ForwardType a2,
                                       typename detail::CallbackParamTraits<A3>::ForwardType a3,
                                       typename detail::CallbackParamTraits<A4>::ForwardType a4,
                                       typename detail::CallbackParamTraits<A5>::ForwardType a5,
                                       typename detail::CallbackParamTraits<A6>::ForwardType a6,
                                       typename detail::CallbackParamTraits<A7>::ForwardType a7,
                                       typename detail::CallbackParamTraits<A8>::ForwardType a8);

        SharedPtr<detail::BindStateBase> m_bind_state_ptr;
        GenFuncPointer m_invoke;
    };

    template <typename R, typename A1, typename A2, typename A3, typename A4, typename A5,
              typename A6, typename A7, typename A8, typename A9>
    class Callback<R(A1, A2, A3, A4, A5, A6, A7, A8, A9)> {
    private:
        typedef void (*GenFuncPointer)(void);
        typedef GenFuncPointer unspecified_bool_type;
        
    public:

        typedef R (RunType)(A1, A2, A3, A4, A5, A6, A7, A8, A9);
        
        Callback()
            : m_invoke(NULL)
        { }

        // Should be private, but avoids template friendship issues.
        template <typename FunctorType, typename BindRunType, typename BoundArgsType>
        Callback(detail::BindState<FunctorType, BindRunType, BoundArgsType>* bind_state)
            : m_bind_state_ptr(bind_state)
        {
            PolymorphicInvoke invoke = &detail::BindState<FunctorType, BindRunType, BoundArgsType>::InvokerType::invoke;
            m_invoke = reinterpret_cast<GenFuncPointer>(invoke);
        }

        R operator()(typename detail::CallbackParamTraits<A1>::ForwardType a1,
                     typename detail::CallbackParamTraits<A2>::ForwardType a2,
                     typename detail::CallbackParamTraits<A3>::ForwardType a3,
                     typename detail::CallbackParamTraits<A4>::ForwardType a4,
                     typename detail::CallbackParamTraits<A5>::ForwardType a5,
                     typename detail::CallbackParamTraits<A6>::ForwardType a6,
                     typename detail::CallbackParamTraits<A7>::ForwardType a7,
                     typename detail::CallbackParamTraits<A8>::ForwardType a8,
                     typename detail::CallbackParamTraits<A9>::ForwardType a9) const
        {
            PolymorphicInvoke f = reinterpret_cast<PolymorphicInvoke>(m_invoke);
            return f(m_bind_state_ptr.get(), a1, a2, a3, a4, a5, a6, a7, a8, a9);
        }

        operator unspecified_bool_type() const
        {
            return m_invoke;
        }
        
    private:

        typedef R (*PolymorphicInvoke)(detail::BindStateBase*,
                                       typename detail::CallbackParamTraits<A1>::ForwardType a1,
                                       typename detail::CallbackParamTraits<A2>::ForwardType a2,
                                       typename detail::CallbackParamTraits<A3>::ForwardType a3,
                                       typename detail::CallbackParamTraits<A4>::ForwardType a4,
                                       typename detail::CallbackParamTraits<A5>::ForwardType a5,
                                       typename detail::CallbackParamTraits<A6>::ForwardType a6,
                                       typename detail::CallbackParamTraits<A7>::ForwardType a7,
                                       typename detail::CallbackParamTraits<A8>::ForwardType a8,
                                       typename detail::CallbackParamTraits<A9>::ForwardType a9);

        SharedPtr<detail::BindStateBase> m_bind_state_ptr;
        GenFuncPointer m_invoke;
    };

} // namespace promise

#endif
