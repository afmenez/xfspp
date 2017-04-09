#pragma once
#ifndef __SYNCH_HPP__
#define __SYNCH_HPP__

#include <windows.h>

#include "win32/handle.hpp"

namespace Windows
{
  namespace Synch
  {
    class Lockable : public Handle<>
    {
    public:
      explicit Lockable(HANDLE h);

      void lock();
    };
  
    class Mutex : public Lockable
    {
    public:
      explicit Mutex();
  
      void unlock();
    };
  
    class Semaphore : public Lockable
    {
    public:
      explicit Semaphore(LONG start, LONG max);
  
      void unlock();
    };
  
    class Event : public Handle<>
    {
    public:
      explicit Event();

      void waitFor();
      void set();
    };
  
    template< typename T >
    class Locker
    {
      T &_ref;

      Locker(const Locker &);
      Locker &operator=(const Locker &);
  
    public:
      explicit Locker(T &ref) : _ref(ref) { _ref.lock(); }
      ~Locker() { _ref.unlock(); }
    };

    template<>
    class Locker< HANDLE >
    {
      HANDLE _ref;

      Locker(const Locker &);
      Locker &operator=(const Locker &);
  
    public:
      explicit Locker(HANDLE ref) : _ref(ref) { WaitForSingleObjectEx(_ref,INFINITE,FALSE); }
      Locker(Locker &&o) :
        _ref(o._ref)
      {
        o._ref = NULL;
      }

      ~Locker()
      {
        if (_ref != NULL)
          ReleaseMutex(_ref);
      }

      Locker &operator=(Locker &&o)
      {
        _ref = o._ref;
        o._ref = NULL;

        return *this;
      }
    };
  }
}

#endif
