#ifndef __TAF_PROMISE_EXCEPTION_PTR_H__
#define __TAF_PROMISE_EXCEPTION_PTR_H__

#include "shared_ptr.h"
#include "promise_exception.h"
#include <stdexcept>
#include <new>
#include <typeinfo>
#include <string>

using namespace std;

namespace promise 
{
namespace detail 
{
	class ExceptionBase : public std::exception
	{
    public:
		ExceptionBase() {}

        virtual ~ExceptionBase() throw() {}

		virtual ExceptionBase* clone() const { return new ExceptionBase(*this); }

        virtual void rethrow() const { throw *this; }

		virtual const char* what() const throw() { return "ExceptionBase"; }
		
		virtual int getErrCode() const { return -1; }
    };

    class ExceptionBaseImpl : public ExceptionBase
	{
    public:
        ExceptionBaseImpl() {}
        
		ExceptionBaseImpl(const std::string& s)
        : _sInfo(s)
        , _error(-1)
        {}
        
        ExceptionBaseImpl(const std::string& s, int err)
        : _sInfo(s)
        , _error(err)
        {}

        virtual ~ExceptionBaseImpl() throw() {}

        virtual ExceptionBase* clone() const
        {
            return new ExceptionBaseImpl(*this);
        }

        virtual void rethrow() const
        {
            throw *this;
        }

		virtual const char* what() const throw()
		{
			return _sInfo.c_str();
		}
		
		virtual int getErrCode() const
		{ 
			return _error; 
		}
	private:
		std::string _sInfo;
		int			_error;
    };

}  // namespace detail

typedef SharedPtr<detail::ExceptionBase> ExceptionPtr;

class CurrentExceptionUnknownException : PromiseException 
{
private:

    const char *name() const
    { 
		return "CurrentExceptionUnknownException"; 
	}
};

/**
 * \pre This function must be called in a catch block.
 * \throws nothing
 */
inline ExceptionPtr currentException()
{
    try 
	{
        try 
		{
            throw;
        } 
		catch (detail::ExceptionBaseImpl& e) 
		{
            return ExceptionPtr(e.clone());
        } 
		catch (detail::ExceptionBase& e) 
		{
            return ExceptionPtr(e.clone());
        } 
		catch (std::domain_error& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        } 
		catch (std::invalid_argument& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        } 
		catch (std::length_error& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        } 
		catch (std::out_of_range& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        } 
		catch (std::range_error& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));                
        } 
		catch (std::overflow_error& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        } 
		catch (std::underflow_error& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        } 
		catch (std::logic_error& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        } 
		catch (std::bad_alloc& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        } 
		catch (std::bad_cast& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        } 
		catch (std::bad_typeid& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        } 
		catch (std::bad_exception& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        } 
		catch (PromiseException& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        } 
		catch (std::exception& e) 
		{
            return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
        }
    } 
	catch (std::bad_alloc& e) 
	{
        //return detail::ExceptionPtrStaticExceptionObject<std::bad_alloc>::e;
		return ExceptionPtr(new detail::ExceptionBaseImpl(string(e.what(), strlen(e.what())) ));
    } 
	catch (...) 
	{
        //return detail::ExceptionPtrStaticExceptionObject<CurrentExceptionUnknownException>::e;
		return ExceptionPtr(new detail::ExceptionBaseImpl(string("PromiseUnknownException") ));
    }
}


inline detail::ExceptionBaseImpl enableCurrentException(const string& t)
{
    return detail::ExceptionBaseImpl(t);
}

inline detail::ExceptionBaseImpl enableCurrentException(const string& t, int err)
{
    return detail::ExceptionBaseImpl(t, err);
}

template <typename E>
inline void throwException(const E& e)
{
    throw enableCurrentException(string(e.what(), strlen(e.what())));
}

inline ExceptionPtr copyException(const string& e)
{
    try 
	{
        throw enableCurrentException(e);
    } 
	catch (...) 
	{
        return currentException();
    }
}

inline ExceptionPtr copyException(const string& e, int err)
{
    try 
	{
        throw enableCurrentException(e, err);
    } 
	catch (...) 
	{
        return currentException();
    }
}
    
}  // namespace promise

#endif  // __EXCEPTION_PTR_H__
