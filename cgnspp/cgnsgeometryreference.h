/***************************************************************************
                          cgnsgeometryreference.h  -  description
                             -------------------
    begin                : Tue May 28 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSGEOMETRYREFERENCE_H
#define CGNSGEOMETRYREFERENCE_H

#include "cgnsnode.h"

/** Implementation data for a GeometryReference_t subnode
  */
struct CGNS::Private::GeometryReference : CGNS::Private::Node  {
COMMON_SIMPLE_NODE_INTERFACE(GeometryReference, Family, Node);
	std::string mFile;
	CGNS::GeometryReference::GeometryFormat mFormat;
	child_container<CGNS::GeometryReference::GeometryEntity> mEntities;
	
	USING_READ(Node);
	static void Read(GeometryReference * me);

	void Modify(std::string const & iFile,
		CGNS::GeometryReference::GeometryFormat iFormat);
DISTINCT_CHILDREN3(std::string, CGNS::GeometryReference::GeometryEntity,
	CGNS::GeometryReference::GeometryFormat);
};

#endif
