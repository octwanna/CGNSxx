/***************************************************************************
                          cgnsreferencestate.cpp  -  description
                             -------------------
    begin                : Thu Sep 20 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgnsreferencestate.h"

#include "cgnsstated.h"

using CGNS::ReferenceState;

GENERATE_D(ReferenceState);

COMMON_NODE_IMPLEMENTATION(ReferenceState);

INIT_IMPLEMENTATION0(ReferenceState);

bool CGNS::ReferenceState::hasReferenceStateDescription() const {
	return hasDescriptor("ReferenceStateDescription");
}

std::string CGNS::ReferenceState::getReferenceStateDescription() const {
	return getDescriptorText("ReferenceStateDescription");
}

std::string CGNS::ReferenceState::writeReferenceStateDescription(std::string const & iDescription) {
	return writeDescriptor("ReferenceStateDescription", iDescription);
}

void CGNS::ReferenceState::deleteReferenceStateDescription() {
	deleteDescriptor("ReferenceStateDescription");
}

