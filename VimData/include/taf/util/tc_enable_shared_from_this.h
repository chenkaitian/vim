#ifndef __TC_ENABLE_SHARED_FROM_THIS_H__
#define __TC_ENABLE_SHARED_FROM_THIS_H__

#include "tc_shared_ptr.h"

namespace taf
{

    template <class T>
    class TC_EnableSharedFromThis 
	{
    public:

        /**
         * \pre `TC_EnableSharedFromThis<T>' must be an accessible base class of `T'. `*this'
         *     must be a subobject of an instance `t' of type `T'.
         * \returns A `TC_SharedPtr<T>' instance that shares onwership with `p' if there is a
         *     `TC_SharedPtr' instance `p' that owns `t', or an NULL `TC_SharedPtr<T>' instance.
         */
        TC_SharedPtr<T> sharedFromThis()
        {
            TC_SharedPtr<T> p(m_this, m_owner_use_count);
            return p;
        }

        /**
         * Same as above.
         */
        TC_SharedPtr<const T> sharedFromThis() const
        {
            TC_SharedPtr<const T> p(m_this, m_owner_use_count);
            return p;
        }
        
    protected:
        TC_EnableSharedFromThis()
        : m_this(NULL)
		, m_owner_use_count(NULL)
        { }

        TC_EnableSharedFromThis(const TC_EnableSharedFromThis&)
        { }

        TC_EnableSharedFromThis& operator=(const TC_EnableSharedFromThis&)
        {
            return *this;
        }

        // User can not delete a pointer of type TC_EnableSharedFromThis* so it is not
        // necessary to be virtual.
        ~TC_EnableSharedFromThis()
        { }
        
    private:
        
        template <typename U>
        void acceptOwner(const TC_SharedPtr<U>& p) const
        {
            if (m_owner_use_count == NULL) 
			{
                m_owner_use_count = p.m_pn;
                m_this = p.get();
            }
        }

        mutable T *m_this;
        mutable detail::tc_shared_count_base *m_owner_use_count;

        template <class X, class U>
        friend void detail::tc_sp_enable_shared_from_this(const TC_SharedPtr<X> *pp, const TC_EnableSharedFromThis<U> *px);
    };
    
}

#endif
