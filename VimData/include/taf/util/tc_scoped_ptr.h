#ifndef __TC_SCOPED_PTR_H__
#define __TC_SCOPED_PTR_H__

#include "detail/tc_assert.h"
#include "detail/tc_checked_delete.h"

namespace taf
{

    template <typename T>
    class TC_ScopedPtr 
	{
    public:
        typedef T element_type;
        typedef T* (TC_ScopedPtr::*unspecified_bool_type)() const;
        
        explicit TC_ScopedPtr(T *p = NULL)
        : m_p(p)
        { }
            
        // Never throw. The destructor of pointee is required to never throw.
        ~TC_ScopedPtr()
        {
            tc_checked_delete(m_p);
        }

        /**
         * Return the object this pointer is pointing to.
         *
         * \pre `this->get() != NULL`
         */
        T& operator*() const
        {
            TC_ASSERT(m_p != NULL);
            return *m_p;
        }
                
        /**
         * Returns `this->get()`.
         *
         * \pre `this->get() != NULL`.
         */
        T *operator->() const
        {
            TC_ASSERT(m_p != NULL);
            return m_p;
        }
        
        /**
         * Returns the stored pointer.
         */
        T *get() const
        {
            return m_p;
        }

        /**
         * Returns an value that, when used in boolean contexts, is equivalent to
         * `get() != NULL'.
         */
        operator unspecified_bool_type() const
        {
            return m_p ? &TC_ScopedPtr::get : 0;
        }

        void reset(T *p = NULL)
        {
            TC_ScopedPtr(p).swap(*this);
        }

        void swap(TC_ScopedPtr& other)
        {
            std::swap(m_p, other.m_p);
        }

    private:
		TC_ScopedPtr(const TC_ScopedPtr&);
		void operator=(const TC_ScopedPtr&);
        
        T *m_p;
    };

}

#endif
