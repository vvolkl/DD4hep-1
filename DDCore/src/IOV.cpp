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

// Framework includes
#include "DD4hep/IOV.h"

// C/C++ include files
#include <climits>
#include <iomanip>
#include <cstring>

using namespace std;
using namespace DD4hep;

std::string IOVType::str()  const   {
  char text[256];
  ::snprintf(text,sizeof(text),"%s(%d)",name.c_str(),int(type));
  return text;
}

/// Initializing constructor: Does not set reference to IOVType !
IOV::IOV() : iovType(0), keyData(0,0), optData(0)  {
  type = int(IOVType::UNKNOWN_IOV);
}

/// Initializing constructor
IOV::IOV(const IOVType* t) : iovType(t), keyData(0,0), optData(0)  {
  type = t ? t->type : int(IOVType::UNKNOWN_IOV);
}

/// Copy constructor
IOV::IOV(const IOV& c) 
  : iovType(c.iovType), keyData(c.keyData), optData(c.optData), type(c.type)
{
}

/// Copy constructor
IOV::IOV(const IOVType* t, const Key& k)
  : iovType(t), keyData(k), optData(0)
{
  type = t ? t->type : int(IOVType::UNKNOWN_IOV);
}

/// Standard Destructor
IOV::~IOV()  {
}

/// Set discrete IOV value
void IOV::set(const Key& value)  {
  keyData = value;
}

/// Set discrete IOV value
void IOV::set(Key::first_type value)  {
  keyData.first = keyData.second = value;
}

/// Set range IOV value
void IOV::set(Key::first_type val_1, Key::second_type val_2)  {
  keyData.first  = val_1;
  keyData.second = val_2;
}

/// Set keys to unphysical values (LONG_MAX, LONG_MIN)
IOV& IOV::reset()  {
  keyData.first  = LONG_MAX;
  keyData.second = LONG_MIN;
  return *this;
}

/// Set keys to unphysical values (LONG_MAX, LONG_MIN)
IOV& IOV::invert()  {
  Key::first_type tmp = keyData.first;
  keyData.first  = keyData.second;
  keyData.second = tmp;
  return *this;
}

void IOV::iov_intersection(const IOV& validity)   {
  if ( !iovType )
    *this = validity;
  else if ( validity.keyData.first > keyData.first ) 
    keyData.first = validity.keyData.first;
  if ( validity.keyData.second < keyData.second )
    keyData.second = validity.keyData.second;
}

void IOV::iov_intersection(const IOV::Key& validity)   {
  if ( validity.first > keyData.first ) 
    keyData.first = validity.first;
  if ( validity.second < keyData.second )
    keyData.second = validity.second;
}

void IOV::iov_union(const IOV& validity)   {
  if ( !iovType )
    *this = validity;
  else if ( validity.keyData.first < keyData.first ) 
    keyData.first = validity.keyData.first;
  if ( validity.keyData.second > keyData.second )
    keyData.second = validity.keyData.second;
}

void IOV::iov_union(const IOV::Key& validity)   {
  if ( validity.first < keyData.first ) 
    keyData.first = validity.first;
  if ( validity.second > keyData.second )
    keyData.second = validity.second;
}

/// Move the data content: 'from' will be reset to NULL
void IOV::move(IOV& from)   {
  ::memcpy(this,&from,sizeof(IOV));
  from.keyData.first = from.keyData.second = from.optData = 0;
  from.type = int(IOVType::UNKNOWN_IOV);
  from.iovType = 0;
}

/// Create string representation of the IOV
string IOV::str()  const  {
  char text[256];
  if ( iovType )  {
    if ( iovType->name[0] != 'e' )   {
      ::snprintf(text,sizeof(text),"%s(%d):[%ld-%ld]",
		 iovType->name.c_str(),int(iovType->type),keyData.first, keyData.second);
    }
    else if ( iovType->name == "epoch" )  {
      time_t since = keyData.first;
      time_t until = keyData.second;
      char c_since[64], c_until[64];
      struct tm time_buff;
      ::strftime(c_since,sizeof(c_since),"%d-%m-%Y %H:%M:%S",::gmtime_r(&since,&time_buff));
      ::strftime(c_until,sizeof(c_until),"%d-%m-%Y %H:%M:%S",::gmtime_r(&until,&time_buff));
      ::snprintf(text,sizeof(text),"%s(%d):[%s - %s]",
		 iovType->name.c_str(),iovType->type,
		 c_since, c_until);
    }
    else   {
      ::snprintf(text,sizeof(text),"%s(%d):[%ld-%ld]",
		 iovType->name.c_str(),int(iovType->type),keyData.first, keyData.second);
    }
  }
  else  {
    ::snprintf(text,sizeof(text),"%d:[%ld-%ld]",type,keyData.first, keyData.second);
  }
  return text;
}

/// Check for validity containment
bool IOV::contains(const IOV& iov)  const   {
  if ( key_is_contained(iov.keyData,keyData) )  {
    unsigned int typ1 = iov.iovType ? iov.iovType->type : iov.type;
    unsigned int typ2 = iovType ? iovType->type : type;
    return typ1 == typ2;
  }
  return false;
}