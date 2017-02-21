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


template<typename Container, typename... Args>
class InvokeLaterTemplate
{
public:
  typedef std::function<Args...> Functor;
  InvokeLaterTemplate() { }
  InvokeLaterTemplate(const Functor& f) { push(f); }
  InvokeLaterTemplate(const InvokeLaterTemplate& r) : c(r.c) {};
  ~InvokeLaterTemplate() { while (!c.empty()) { invoke(); } }
  void push(const Functor& f) { c.push(f); }
  inline void invoke() { try{ next()(); } catch (...) {} c.pop(); }
  void remove() { c.pop(); }
  void clear() { while (!c.empty()) c.pop(); }
private:
  InvokeLaterTemplate& operator=(const InvokeLaterTemplate& c) = delete;
  Container<Functor> c;
};

template<typename...Args>
class InvokeLaterTemplate<std::queue, Args...>
{  protected: Functor& next() { return front(); } };

template<typename...Args>
class InvokeLaterTemplate<std::stack, Args...>
{  protected: Functor& next() { return top(); } };

template<typename...Args> typedef InvokeLaterTemplate<std::queue, Args...> InvokeLaterQueue;
template<typename...Args> typedef InvokeLaterTemplate<std::stack, Args...> InvokeLaterStack;

template<typename Key, typename... Args>
class InvokeLaterList
{
public:
  typedef std::function<Args...> Functor;
  typedef std::map<Key, Functor> FunctorMap;
  typedef std::pair<Key, Functor> FunctorPair;
  enum enInvocationOrder { UNDEFINED, FIFO, LIFO };
  InvokeLaterList(const enInvocationOrder io = LIFO) : invoOrder(io) {}
  InvokeLaterList(const Functor& f, const enInvocationOrder io = LIFO) : InvokeLater(io) { push(f); }
  ~InvokeLaterList() { while (!m.empty()) { invoke(); } }
  bool push(const Key& idx, const const Functor& f) { if (m.find(idx) == m.end()) { m.insert(m.end(), FunctorPair(idx, f)); return true; } return false; }
  void remove(const Key idx) { auto it = m.find(idx); remove(it); }
  inline void invoke(const Key idx) {  auto it = m.find(idx); invoke(it); }
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
  enInvocationOrder invoOrder;
};

template<typename... Args>
class InvokeLaterList<int, Args...>
{
  InvokeLaterList(const enInvocationOrder io = LIFO) : nIdx(1), invoOrder(io) {}
  Key push(const Functor& f) { while (!push(nIdx, f)) ++nIdx; return nIdx - 1; }
private:
  int nIdx;
};

}
}
