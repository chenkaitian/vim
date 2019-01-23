#ifndef __TC_CHECKED_DELETE_H__
#define __TC_CHECKED_DELETE_H__

namespace taf 
{
    // verify that types are complete when deleted
    
    template <typename T>
    inline void tc_checked_delete(T *p)
    {
        typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
        (void) sizeof(type_must_be_complete);
        delete p;
    }

    template <typename T>
    inline void tc_checked_array_delete(T *p)
    {
        typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
        (void) sizeof(type_must_be_complete);
        delete[] p;
    }

}

#endif
