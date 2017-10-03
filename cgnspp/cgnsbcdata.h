/***************************************************************************
                          cgnsbcdata.h  -  description
                             -------------------
    begin                : Thu May 16 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSBCDATA_H
#define CGNSBCDATA_H

#include "cgnsvalued.h"

#include "cgnsbcdataset.h"

/** Implementation data for the BCData_t subnode
  */
struct CGNS::Private::BCData : CGNS::Private::Valued  {
COMMON_NODE_INTERFACE(BCData, BCDataSet, Valued);
DISTINCT_CHILDREN0();
};

#endif
