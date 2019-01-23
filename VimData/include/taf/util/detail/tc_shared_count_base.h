#ifndef __TC_SHARED_COUNT_BASE_H__
#define __TC_SHARED_COUNT_BASE_H__

#include "../tc_atomic.h"

namespace taf 
{
    namespace detail 
	{
        class tc_shared_count_base 
		{
        public:

            tc_shared_count_base()
            : m_use_count(1)
            {}

            virtual ~tc_shared_count_base()
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
			tc_shared_count_base(const tc_shared_count_base&);
			void operator=(const tc_shared_count_base&);
            
            taf::TC_Atomic m_use_count;
        };

    }
}

#endif
