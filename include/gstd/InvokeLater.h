#pragma once

#include "gstd_api.h"

#include <stack>
#include <queue>
#include <map>
#include <functional>

namespace gris
{
  namespace gstd
  {

    template<typename... Args>
    class InvokeLater
    {
    public:
      typedef std::function<void(Args...)> Functor;
      InvokeLater(Functor t) : f(t), mDoInvoke(true) { }
      InvokeLater(const InvokeLater& c) : f(c.f), mDoInvoke(c.mDoInvoke) {}
      ~InvokeLater() { if (mDoInvoke) invoke(); }

      void clear() { mDoInvoke = false; }
      void remove() { mDoInvoke = false; }
    private:
      InvokeLater& operator=(const InvokeLater& c) = delete;
      inline void invoke() { try { f(); } catch (...) {} }
      Functor f;
      bool mDoInvoke;
    };


    template<template<typename...> typename Container, typename... Args>
    class InvokeLaterTemplate
    {
    public:
      typedef std::function<void(Args...)> Functor;
      InvokeLaterTemplate() { }
      InvokeLaterTemplate(const Functor& f) { push(f); }
      InvokeLaterTemplate(const InvokeLaterTemplate& r) : c(r.c) {};
      ~InvokeLaterTemplate() { while (!c.empty()) { invoke(); } }
      void push(const Functor& f) { c.push(f); }
      inline void invoke() { try { next()(); } catch (...) {} c.pop(); }
      void remove() { c.pop(); }
      void clear() { while (!c.empty()) c.pop(); }
    protected:
      virtual Functor& next() = 0;
      InvokeLaterTemplate& operator=(const InvokeLaterTemplate& c) = delete;
      Container<Functor> c;
    };

    template<typename...Args>
    class InvokeLaterQueue : public InvokeLaterTemplate<std::queue, Args...>
    {
    public:
      InvokeLaterQueue() : InvokeLaterTemplate<std::queue, Args...>() {}
      InvokeLaterQueue(const Functor& f) : InvokeLaterTemplate<std::queue, Args...>(f) {}
    protected:
      virtual Functor& next() { return c.front(); }
    };

    template<typename...Args>
    class InvokeLaterStack : public InvokeLaterTemplate<std::stack, Args...>
    {
    public:
      InvokeLaterStack() : InvokeLaterTemplate<std::stack, Args...>() {}
      InvokeLaterStack(const Functor& f) : InvokeLaterTemplate<std::stack, Args...>(f) {}
    protected:
      virtual Functor& next() { return c.top(); }
    };

    template<template<typename...> typename Container, typename...Args>
    class InvokeLaterContainer : public InvokeLaterTemplate<Container, Args...>
    {
    public:
      InvokeLaterContainer() : InvokeLaterTemplate<Container, Args...>() { it = c.begin(); }
      InvokeLaterContainer(const Functor& f) : InvokeLaterTemplate<Container, Args...>(f) { it = c.begin(); }
      InvokeLaterContainer(const InvokeLaterContainer& r) : InvokeLaterTemplate<Container, Args...>(r), it(c.begin())
      { // set the iterator to the same position as in the other container
        int pos(0);  auto r_it = r.c.begin();
        while (r.it != r_it) { ++pos; if (r_it == r.c.end()) { it = c.end();  return; } ++r_it; }
        for (int i = 0; i < pos; ++i) ++it;
      };
      void reset() { it = c.begin(); }
    protected:
      virtual Functor& next() { return *(it++); }
      typename Container<Functor>::iterator it;
    };

    template<typename Key, typename... Args>
    class InvokeLaterMap
    {
    public:
      typedef std::function<void(Args...)> Functor;
      typedef std::map<Key, Functor> FunctorMap;
      typedef std::pair<Key, Functor> FunctorPair;
      enum enInvocationOrder { UNDEFINED, FIFO, LIFO };
      InvokeLaterMap(const enInvocationOrder io = LIFO) : invoOrder(io) {}
      InvokeLaterMap(const Key& position, const Functor& f, const enInvocationOrder io = LIFO) : invoOrder(io) { insert(position, f); }
      ~InvokeLaterMap() { while (!m.empty()) { invoke(); } }
      bool insert(const Key& idx, const Functor& f) { if (m.find(idx) == m.end()) { m.insert(m.end(), FunctorPair(idx, f)); return true; } return false; }
      void remove(const Key idx) { auto it = m.find(idx); remove(it); }
      inline void invoke(const Key idx) { auto it = m.find(idx); invoke(it); }
      void clear() { m.clear(); }
      void setInvocationOrder(const enInvocationOrder io) { invoOrder = io; }
      enInvocationOrder getInvocationOrder() const { return invoOrder; }
    private:
      inline void remove(typename FunctorMap::iterator it) { if (it != m.end()) m.erase(it); }
      inline void invoke() { if (invoOrder == FIFO) invoke(m.begin()); else invoke(--m.end()); }
      inline void invoke(typename FunctorMap::iterator it) { if (it != m.end()) try { it->second(); } catch (...) {} remove(it); }
      InvokeLaterMap(const InvokeLaterMap& c) = delete;
      InvokeLaterMap& operator=(const InvokeLaterMap& c) = delete;
      FunctorMap m;
      enInvocationOrder invoOrder;
    };

    template<typename Key, typename... Args>
    class InvokeLaterList : public InvokeLaterMap<typename std::enable_if<std::is_integral<Key>::value, Key>::type, Args...>
    {
    public:
      InvokeLaterList(const enInvocationOrder io = LIFO) : nIdx(0), InvokeLaterMap<Key, Args...>(io) {}
      InvokeLaterList(const Functor& f, const enInvocationOrder io = LIFO) : nIdx(1), InvokeLaterMap<Key, Args...>(0, f, io) {}
      Key push(const Functor& f) { while (!insert(nIdx, f)) ++nIdx; return nIdx; }
    private:
      Key nIdx;
    };

  }
}
