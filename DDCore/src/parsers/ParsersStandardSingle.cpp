// $Id: $
//==========================================================================
//  AIDA Detector description implementation for LCD
//--------------------------------------------------------------------------
// Copyright (C) Organisation europeenne pour la Recherche nucleaire (CERN)
// All rights reserved.
//
// For the licensing terms see $DD4hepINSTALL/LICENSE.
// For the list of contributors see $DD4hepINSTALL/doc/CREDITS.
//
//==========================================================================
#include "DD4hep/Parsers.h"
#include "ParsersFactory.h"
// ============================================================================
#define PARSERS_DEF_FOR_SINGLE(Type)                                  \
  int DD4hep::Parsers::parse(Type& result, const std::string& input)  \
  {  return DD4hep::Parsers::parse_(result, input);  }
// ============================================================================

PARSERS_DEF_FOR_SINGLE(bool)
PARSERS_DEF_FOR_SINGLE(int)
PARSERS_DEF_FOR_SINGLE(long)
PARSERS_DEF_FOR_SINGLE(double)
PARSERS_DEF_FOR_SINGLE(float)
PARSERS_DEF_FOR_SINGLE(std::string)

#if defined(DD4HEP_HAVE_ALL_PARSERS)
PARSERS_DEF_FOR_SINGLE(char)
PARSERS_DEF_FOR_SINGLE(short)
PARSERS_DEF_FOR_SINGLE(unsigned char)
PARSERS_DEF_FOR_SINGLE(signed char)
PARSERS_DEF_FOR_SINGLE(unsigned short)
PARSERS_DEF_FOR_SINGLE(unsigned int)
PARSERS_DEF_FOR_SINGLE(unsigned long)
PARSERS_DEF_FOR_SINGLE(long long)
PARSERS_DEF_FOR_SINGLE(unsigned long long)
PARSERS_DEF_FOR_SINGLE(long double)
#endif
