#ifndef __TAF_PROMISE_SHARED_COUNT_IMPL_H__
#define __TAF_PROMISE_SHARED_COUNT_IMPL_H__

#include "shared_count_base.h"
#include "../checked_delete.h"

namespace promise 
{
    namespace detail 
	{    
        template <typename T>
        class shared_count_impl_p : public shared_count_base 
		{
        public:

            template <typename U>
            shared_count_impl_p(U *p)
            : m_px(p)
            {}

            virtual void dispose()
            {
                checked_delete(m_px);
            }
            
        private:

            T *m_px;
        };

        template <typename T, typename D>
        class shared_count_impl_pd : public shared_count_base 
		{
        public:

            // Requirements: d(p) must not throw
            shared_count_impl_pd(T *p, D& d)
            : m_px(p)
			, m_deleter(d)
            {}

            virtual void dispose()  // no throw
            {
                m_deleter(m_px);
            }

        private:

            T *m_px;
            D m_deleter;  // copy constructor must not throw
        };

    }  // namespace detail

}  // namespace promise

#endif  // __TAF_PROMISE_SHARED_COUNT_IMPL_H__
