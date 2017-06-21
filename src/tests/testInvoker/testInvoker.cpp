#include "testInvoker.h"

#include <functional>
#include <boost/test/auto_unit_test.hpp>

namespace gris
{
  namespace gstd
  {
    void testInvokeLater()
    {
      int a = 1;
      InvokeLater<> invoker([&a]() { ++a; });
    }
    void testInvokeLaterException()
    {
      InvokeLater<> invoker([]() { throw std::exception(); });
    }
    bool testValueInvokeLater()
    {
      int a = 1;
      {
        InvokeLater<> invoker([&a]() { ++a; });
      }
      return a == 2;
    }
    template<template<typename...> typename T>
    void testInvokeLaterTemplate()
    {
      int a = 1;
      std::function<void()> f = [&a]() { ++a; };
      T<> invoker(f);
      invoker.push(f);
      invoker.remove();
      invoker.clear();
      invoker.push(f);
    }

    template void testInvokeLaterTemplate<InvokeLaterStack>();
    template void testInvokeLaterTemplate<InvokeLaterQueue>();

    bool testValueInvokeStack()
    {
      std::string testString = "";
      {
        std::function<void(const std::string&)> f = [&testString](const std::string& s) { testString += s; };
        InvokeLaterStack<> invoker;
        invoker.push(std::bind(f, "a"));
        invoker.clear();
        invoker.push(std::bind(f, "b"));
        invoker.push(std::bind(f, "c"));
        invoker.push(std::bind(f, "d"));
        invoker.remove();
      }
      return testString == "cb";
    }

    bool testValueInvokeQueue()
    {
      std::string testString = "";
      {
        std::function<void(const std::string&)> f = [&testString](const std::string& s) { testString += s; };
        InvokeLaterQueue<> invoker;
        invoker.push(std::bind(f, "a"));
        invoker.clear();
        invoker.push(std::bind(f, "b"));
        invoker.push(std::bind(f, "c"));
        invoker.push(std::bind(f, "d"));
        invoker.remove();
      }
      return testString == "bc";
    }
    void testInvokeLaterList()
    {
      int a = 1;
      std::function<void()> f = [&a]() { ++a; };
      InvokeLaterList<int> invoker(f);
      invoker.insert(0, f);
      invoker.clear();
      invoker.push(f);
    }
  }
}