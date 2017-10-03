/***************************************************************************
                          cgnsvalued.h  -  description
                             -------------------
    begin                : Thu Sep 20 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSVALUED_H
#define CGNSVALUED_H

#include "cgnsarrayed.h"

/** Special subnode containing data arrays with only scalar values
  */
struct CGNS::Private::Valued : CGNS::Private::Arrayed {
COMMON_NODE_INTERFACE(Valued, Dimensioned, Arrayed);
	typedef child_container<double> values_t;
	values_t mValues;

    USING_NEEDS(Arrayed);
    static void Needs(Valued * me, child_container<double> & cDest);
    USING_READ(Arrayed);
    static void Read(Valued * me, child_container<double> & cDest);
DISTINCT_CHILDREN1(double);
};

#endif
