/***************************************************************************
                          adfutil.cpp  -  description
                             -------------------
    begin                : Tue Sep 18 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "adfutil.h"

using ADF::Exception;

ADF::Checker::~Checker() {
	/// DANGER: throwing from destructor. We hope this is allowed.
	if (mErrorCode!=Exception::NO_ERROR) {
		// avoid recursion
		int errorCode=mErrorCode;
		mErrorCode=Exception::NO_ERROR;
		Emit(errorCode);
	}
}

void ADF::Check(int iErr) {
	if (iErr!=Exception::NO_ERROR) Emit(iErr);
}

void ADF::Emit(int iErr) {
	// throw only if no exception is active
	if (!std::uncaught_exception()) throw Exception(iErr);
	exit(-1);
}

int ADF::DataSize(DataType iType) {
	switch (iType) {
	case MT: return 0;
	case I4: return 4;
	case I8: return 8;
	case U4: return 4;
	case U8: return 8;
	case R4: return 4;
	case R8: return 8;
	case X4: return 8;
	case X8: return 16;
	case C1: return 1;
	case B1: return 1;
	case LK: Emit(Exception::INVALID_DATA_TYPE); return 0;
	case STRUCT: Emit(Exception::INVALID_DATA_TYPE); return 0;
	default: Emit(Exception::INTERNAL_ERROR); return 0;
	}
}

