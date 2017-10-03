/***************************************************************************
                          adfexception.cpp  -  description
                             -------------------
    begin                : Mon Mar 12 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "adf++.h"

#include "ADF.h"

using ADF::Exception;

Exception::Exception(int iErrorCode) : mErrorCode(iErrorCode) {}

Exception::~Exception() throw() {}

int Exception::getErrorCode() const throw() { return mErrorCode; }

char const * Exception::getErrorMessage() const throw() {
	if (mErrorCode<ADFPP_FIRST_ERROR) {
		static char errorMessage[ADF::MAX_ERROR_STR_LENGTH+1];
		ADF_Error_Message(mErrorCode, errorMessage);
		return errorMessage;
	}
	switch (mErrorCode) {
	case ADFPP_FIRST_ERROR: return "ADF++ first error";
	case DIFFERENT_DATA_TYPE: return "ADF++ incompatible data type";
	case DELETED_NODE: return "ADF++ accessing deleted node";
	case INTERNAL_ERROR: return "ADF++ internal error";
	default: return "ADF++ unknown error";
	}
}

char const * Exception::what() const throw() {
	return getErrorMessage();
}
