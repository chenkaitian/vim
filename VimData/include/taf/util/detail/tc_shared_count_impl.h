#ifndef __TC_SHARED_COUNT_IMPL_H__
#define __TC_SHARED_COUNT_IMPL_H__

#include "tc_shared_count_base.h"
#include "tc_checked_delete.h"

namespace taf 
{
    namespace detail 
	{    
        template <typename T>
        class tc_shared_count_impl_p : public tc_shared_count_base 
		{
        public:

            template <typename U>
            tc_shared_count_impl_p(U *p)
            : m_px(p)
            {}

            virtual void dispose()
            {
                tc_checked_delete(m_px);
            }
            
        private:

            T *m_px;
        };

        template <typename T, typename D>
        class tc_shared_count_impl_pd : public tc_shared_count_base 
		{
        public:

            // Requirements: d(p) must not throw
            tc_shared_count_impl_pd(T *p, D& d)
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

    }
}

#endif
