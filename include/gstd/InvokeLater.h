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
  typedef std::function<Args...> Functor;
  InvokeLater(Functor t) : f(t), mDoInvoke(true) { }
  InvokeLater(const InvokeLater& c) : f(c.f), mDoInvoke(c.mDoInvoke) {}
  ~InvokeLater() { if(mDoInvoke) invoke(); }

  void clear()  { mDoInvoke = false; }
  void remove() { mDoInvoke = false; }
private:
  InvokeLater& operator=(const InvokeLater& c) = delete;
  inline void invoke() { try{ f(); } catch (...) {} }
  Functor f;
  bool mDoInvoke;
};


template<typename... Args>
class InvokeLaterQueue
{
public:
  typedef std::function<Args...> Functor;
  InvokeLaterQueue() { }
  InvokeLaterQueue(const Functor& f) { push(f); }
  InvokeLaterQueue(const InvokeLaterQueue& c) : q(c.q) {};
  ~InvokeLaterQueue() { while (!q.empty()) { invoke(); } }
  void push(const Functor& f) { q.push(f); }
  inline void invoke() { try{ q.front()(); } catch (...) {} q.pop(); }
  void remove() { q.pop(); }
  void clear() { while (!q.empty()) q.pop(); }
private:
  InvokeLaterQueue& operator=(const InvokeLaterQueue& c) = delete;
  std::queue<Functor> q;
};

template<typename... Args>
class InvokeLaterStack
{
public:
  typedef std::function<Args...> Functor;
  InvokeLaterStack() { }
  InvokeLaterStack(const Functor& f) { push(f); }
  InvokeLaterStack(const InvokeLaterStack& c) : s(c.s) {};
  ~InvokeLaterStack() { while (!s.empty()) { invoke(); } }
  void push(const Functor& f) { s.push(f); }
  inline void invoke() { try { s.top()(); } catch (...) {} s.pop(); }
  void remove() { s.pop(); }
  void clear() { while (!s.empty()) s.pop(); }
private:
  InvokeLaterStack& operator=(const InvokeLaterStack& c) = delete;
  std::stack<Functor> s;
};

template<typename... Args>
class InvokeLaterList
{
public:
  typedef std::function<Args...> Functor;
  typedef std::map<int, Functor> FunctorMap;
  typedef std::pair<int, Functor> FunctorPair;
  enum enInvocationOrder { UNDEFINED, FIFO, LIFO };
  InvokeLaterList(const enInvocationOrder io = LIFO) : nIdx(1), invoOrder(io) {}
  InvokeLaterList(const Functor& f, const enInvocationOrder io = LIFO) : InvokeLater(io) { push(f); }
  ~InvokeLaterList() { while (!m.empty()) { invoke(); } }
  int push(const Functor& f) { m.insert(m.end(), FunctorPair(nIdx, f)); return nIdx++; }
  void remove(const int idx) { auto it = m.find(idx); remove(it); }
  inline void invoke(const int idx) {  auto it = m.find(idx); invoke(it); }
  void clear() { q.clear(); }
  void setInvocationOrder(const enInvocationOrder io) { invoOrder = io; }
  enInvocationOrder getInvocationOrder() const { return invoOrder; }
private:
  inline void remove(typename FunctorMap::iterator it) { if(it != m.end()) m.erase(it); }
  inline void invoke() { if (invoOrder==FIFO) invoke(m.begin()); else invoke(--m.end()); }
  inline void invoke(typename FunctorMap::iterator it) { if(it != m.end()) try{ it->second(); } catch (...) {} remove(it); }
  InvokeLaterList(const InvokeLaterList& c) = delete;
  InvokeLaterList& operator=(const InvokeLaterList& c) = delete;
  FunctorMap m;
  int nIdx;
  enInvocationOrder invoOrder;
};

}
}
