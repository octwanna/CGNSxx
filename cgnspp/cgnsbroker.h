/***************************************************************************
                          cgnsbroker.h  -  description
                             -------------------
    begin                : Thu Sep 20 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef CGNSBROKER_H
#define CGNSBROKER_H

#include "cgns++.h"

#include "cgnstraits.h"

/** Broker for access to implementation data. */
struct CGNS::Private::Broker {
public:
	static Node * GetImplementation(CGNS::Node & iNode) { return iNode.mImpl; }
	static Node const * GetImplementation(CGNS::Node const & iNode) {
		return iNode.mImpl; }
private:
	/** Forbidden */
	Broker();
	/** Forbidden */
	Broker(Broker const &);
	/** Forbidden */
	Broker const & operator=(Broker const &);
	/** Forbidden */
	~Broker();
};

//template<>
//inline CGNS::Private::Node & CGNS::Private::Broker::getRealImpl<CGNS::Node,true>(
//	CGNS::Node & iT) { return iT.mImpl; }

#endif
