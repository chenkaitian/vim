#ifndef __TC_TEMPLATE_UTIL_H__
#define __TC_TEMPLATE_UTIL_H__

namespace taf 
{
    template <typename T, T v>
    struct tc_integral_constant 
	{
        typedef T value_type;
        typedef tc_integral_constant<T, v> type;
        static const T value = v;
    };

    template <typename T, T v> const T tc_integral_constant<T, v>::value;
    
    typedef tc_integral_constant<bool, true> true_type;
    typedef tc_integral_constant<bool, false> false_type;

    template <typename T> struct tc_is_void : false_type {};
    template <> struct tc_is_void<void> : true_type {};
    
    template <typename T> struct tc_is_non_const_reference : false_type {};
    template <typename T> struct tc_is_non_const_reference<T&> : true_type {};
    template <typename T> struct tc_is_non_const_reference<const T&> : false_type {};

    template <typename T> struct tc_is_raw_pointer : false_type {};
    template <typename T> struct tc_is_raw_pointer<T*> : true_type {};

	// Check if two types are the same
    template <typename U, typename V>
    struct tc_is_same_type : tc_integral_constant<bool, false> {};

    template <typename U>
    struct tc_is_same_type<U, U> : tc_integral_constant<bool, true> {};

    // Select type1 if true, type2 otherwise.
    template <bool, typename type1, typename type2>
    struct tc_select_type_c;

    template <typename type1, typename type2>
    struct tc_select_type_c<true, type1, type2> 
	{
        typedef type1 type;
    };

    template <typename type1, typename type2>
    struct tc_select_type_c<false, type1, type2> 
	{
        typedef type2 type;
    };

    // Select T1 if Cond::value is true.
    template <typename Cond, typename T1, typename T2>
    struct tc_select_type : tc_select_type_c<Cond::value, T1, T2> {};
    
    namespace detail 
	{
        // Types Yes and No are guaranteed such that sizeof(Yes) != sizeof(No). The reason
        // why we declare Yes and No as a type of reference-to-array is because functions
        // can not return array types.
        typedef char (&Yes)[1];
        typedef char (&No)[2];

        // Used to determine if a type From can convert to a type To. See Alexandrescu's
        // "Modern C++ Design" for more details on this trick.
        struct TC_ConvertHelper 
		{
            template <typename To>
            static Yes Test(To);

            template <typename To>
            static No Test(...);
            
            template <typename From>
            static From Make();
        };

        // Used to determine if a type is a struct/class/union.
        // See http://src.chromium.org/viewvc/chrome/trunk/src/base/template_util.h?content-type=text%2Fplain
        struct TC_IsClassHelper 
		{
            template <typename C>
            static Yes Test(void (C::*)(void));

            template <typename C>
            static No Test(...);
        };

    }  // namespace detail

    // Inherits from true_type if From is convertible to To, false_type otherwise.
    // Note: From and To must be not abstract type.
    template <typename From, typename To>
    struct tc_is_convertible : tc_integral_constant<bool,
		sizeof(detail::TC_ConvertHelper::Test<To>(detail::TC_ConvertHelper::Make<From>())) == sizeof(detail::Yes)> 
	{
    };
    
    template <typename T>
    struct tc_is_class : tc_integral_constant<bool,
		sizeof(detail::TC_IsClassHelper::Test<T>(0)) == sizeof(detail::Yes)> 
	{
    };

    // Check if a type is derived from another type.
    template <typename D, typename B>
    struct tc_is_derived_from 
	{
        enum { value = tc_is_class<D>::value && tc_is_class<B>::value && tc_is_convertible<D*, B*>::value };
    }; 
    
    // Use SFINAE to check if a class type has the specified member function. Note this
    // trick only checks its existence, not its signature.
    //
    // See also:
    //   http://en.wikipedia.org/wiki/Substitution_failure_is_not_an_error
    //   http://stackoverflow.com/questions/1966362/sfinae-to-check-for-inherited-member-functions
    template <typename T>
    class tc_has_method_foo 
	{
        struct BasMixin 
		{
            void foo();
        };

        struct Base : public T, public BasMixin {};

        template <void (BasMixin::*)(void)> struct Helper {};
        
        template <typename C>
        static detail::No Test(Helper<&C::foo>*);

        template <typename>
        static detail::Yes Test(...);

    public:

        enum { value = sizeof(Test<Base>(0)) == sizeof(detail::Yes) };
    };

    // The `tc_enable_if' family of templates is used to control the inclusion or exclusion
    // from a set of matching functions or specializations during overload resolution.
    template <bool, typename T = void>
    struct tc_enable_if_c 
	{
        typedef T type;
    };

    template <typename T>
    struct tc_enable_if_c<false, T> {};

    template <typename Cond, typename T = void>
    struct tc_enable_if : public tc_enable_if_c<Cond::value, T> {};

    template <bool, typename T = void>
    struct tc_disable_if_c 
	{
        typedef T type;
    };

    template <typename T>
    struct tc_disable_if_c<true, T> {};

    template <typename Cond, typename T = void>
    struct disable_if : public tc_disable_if_c<Cond::value, T> {};
    
}

#endif
