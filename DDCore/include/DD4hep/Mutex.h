// $Id$
//==========================================================================
//  AIDA Detector description implementation for LCD
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
// Author     : M.Frank
//
//==========================================================================

#ifndef DD4HEP_MUTEX_H
#define DD4HEP_MUTEX_H

// C/C++ include files
#if __cplusplus >= 201103L
#include <mutex>
#endif

/// Namespace for the AIDA detector description toolkit
namespace DD4hep  {
#if __cplusplus >= 201103L
  typedef std::recursive_mutex dd4hep_mutex_t;
  typedef std::lock_guard<dd4hep_mutex_t> dd4hep_lock_t;
#else
  struct dd4hep_mutex_t {
    dd4hep_mutex_t() {}
    ~dd4hep_mutex_t() {}
    int lock() { return 0;}
    int trylock() { return 0;}
    int unlock() { return 0;}
  };
  struct dd4hep_lock_t {
    dd4hep_lock_t(dd4hep_mutex_t&, bool=true) {}
    ~dd4hep_lock_t() {}
  };
#endif  
}
#endif  // DD4HEP_MUTEX_H
