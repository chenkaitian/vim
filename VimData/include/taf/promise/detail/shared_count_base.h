#ifndef __TAF_PROMISE_SHARED_COUNT_BASE_H__
#define __TAF_PROMISE_SHARED_COUNT_BASE_H__

#include "util/tc_atomic.h"

namespace promise 
{
    namespace detail 
	{
        class shared_count_base 
		{
        public:

            shared_count_base()
            : m_use_count(1)
            {}

            virtual ~shared_count_base()
            {}
            
            int use_count() const
            {
                return m_use_count.get();
            }

            void increment()
            {
                ++m_use_count;
            }

            void release()
            {
                if (m_use_count.dec_and_test()) 
				{
                    dispose();
                    delete this;
                }
            }

            // dispose() is called when m_use_count drops to zero, to release resources
            // managed by this object.
            virtual void dispose() = 0;  // never throws
            
        private:
			shared_count_base(const shared_count_base&);
			void operator=(const shared_count_base&);
            
            taf::TC_Atomic m_use_count;
        };

    }  // namespace detail

}  // namespace promise

#endif  // __TAF_PROMISE_SHARED_COUNT_BASE_H__
