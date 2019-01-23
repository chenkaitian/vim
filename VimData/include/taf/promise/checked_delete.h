#ifndef __TAF_PROMISE_CHECKED_DELETE_H__
#define __TAF_PROMISE_CHECKED_DELETE_H__

namespace promise 
{
    // verify that types are complete when deleted
    
    template <typename T>
    inline void checked_delete(T *p)
    {
        typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
        (void) sizeof(type_must_be_complete);
        delete p;
    }

    template <typename T>
    inline void checked_array_delete(T *p)
    {
        typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
        (void) sizeof(type_must_be_complete);
        delete[] p;
    }

}  // namespace promise

#endif  // __PROMISE_CHECKED_DELETE_H__
