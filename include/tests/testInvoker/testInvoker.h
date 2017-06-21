#pragma once

#include <gstd/InvokeLater.h>

namespace gris
{
  namespace gstd
  {
    void testInvokeLater();

    void testInvokeLaterException();

    bool testValueInvokeLater();

    template<template<typename...> typename T>
    void testInvokeLaterTemplate();

    bool testValueInvokeStack();

    bool testValueInvokeQueue();

    void testInvokeLaterList();

  }
}