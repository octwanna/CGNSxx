/***************************************************************************
                          cgnsexception.cpp  -  description
                             -------------------
    begin                : Mon Mar 12 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgns++.h"

#include "cgnsutil.h"

#include "adf++.h"

#include <assert.h>

using CGNS::Exception;


static char const * getErrorMessage(int iCode) {
	if (iCode<Exception::CGNSPP_FIRST_ERROR) return "";
	if (iCode>Exception::INTERNAL_ERROR) return "";
	switch (iCode) {
	case Exception::CGNSPP_FIRST_ERROR: return "";
	case Exception::NULL_POINTER: return "CGNS++: Null pointer";
	case Exception::NULL_STRING_POINTER: return "CGNS++: Null string pointer";
	case Exception::EMPTY_STRING: return "CGNS++: Empty string";
	case Exception::STRING_TOO_LONG: return "CGNS++: String too long";
	case Exception::NUMBER_LESS_THAN_MINIMUM: return "CGNS++: Number less than minimum";
	case Exception::NUMBER_GREATER_THAN_MAXIMUM: return "CGNS++: Number greater than maximum";
	case Exception::INDEX_TOO_LOW: return "CGNS++: Index too low";
	case Exception::INDEX_TOO_HIGH: return "CGNS++: Index too high";
	case Exception::NO_CGNS_PARENT: return "CGNS++: No CGNS parent node";
	case Exception::NO_CGNS_NODE: return "CGNS++: No CGNS node";
	case Exception::ILLEGAL_OPEN_MODE: return "CGNS++: Illegal open mode";
	case Exception::MULTIPLE_LIBRARY_VERSION_NODES: return "CGNS++: Multiple library version nodes";
	case Exception::UNKNOWN_VERSION: return "CGNS++: Unknown file version";
	case Exception::NO_NAMED_CHILD : return "CGNS++: No child of given name";
	case Exception::NO_LABELED_CHILD : return "CGNS++: No child of given label";
	case Exception::UNKNOWN_DATA_ARRAY: return "CGNS++: Unknown data array subnode";
	case Exception::UNKNOWN_QUANTITY: return "CGNS++: Unknown field quantity";
	case Exception::INVALID_LOCATION: return "CGNS++: Invalid location";
	case Exception::NO_QUANTITY: return "CGNS++: No quantity in DataArray node";
	case Exception::NO_EXPONENTS: return "CGNS++: No exponents for user data";
	case Exception::NO_UNITS: return "CGNS++: No units for user data";
	case Exception::NO_LOCATION: return "CGNS++: Illegal grid location in FlowSolution or DiscreteData node";
	case Exception::NO_ELEMENT: return "CGNS++: Illegal element type in Elements node";
	case Exception::NO_SIMULATIONTYPE: return "CGNS++: Illegal simulation type in Base node";
	case Exception::NO_CONNECTIVITY: return "CGNS++: Illegal connectivity type in GridConnectivity node";
	case Exception::NO_BCTYPE: return "CGNS++: Illegal boundary condition type in BC node";
	case Exception::NO_EQUATIONS: return "CGNS++: Illegal governing equations type in governing equations node";
	case Exception::NO_GASMODEL: return "CGNS++: Illegal gas model type in gas model node";
	case Exception::NO_VISCOSITYMODEL: return "CGNS++: Illegal viscosity model type in viscosity model node";
	case Exception::NO_THERMALCONDUCTIVITYMODEL:
		return "CGNS++: Illegal thermal conductivity model type in thermal conductivity model node";
	case Exception::NO_TURBULENCECLOSURE:
		return "CGNS++: Illegal turbulence closure type in turbulence closure node";
	case Exception::NO_TURBULENCEMODEL: return "CGNS++: Illegal turbulence model type in turbulence model node";
	case Exception::NO_THERMALRELAXATIONMODEL: return "CGNS++: Illegal thermal relaxation model type";
	case Exception::NO_CHEMICALKINETICSMODEL: return "CGNS++: Illegal chemical kinetics model type";
	case Exception::NO_GEOMETRYFORMAT: return "CGNS++: Illegal geometry format in geometry reference node";
	case Exception::NO_RIGIDGRIDMOTION: return "CGNS++: Illegal rigid grid motion type";
	case Exception::NO_ARBITRARYGRIDMOTION: return "CGNS++: Illegal arbitrary grid motion type";
	case Exception::NO_DATACLASS: return "CGNS++: No dataclass in database";
	case Exception::NO_PARENT_NODE: return "CGNS++: No parent node for base";
	case Exception::WRONG_DATATYPE: return "CGNS++: Wrong datatype in ADF node";
	case Exception::WRONG_NUM_DIMENSIONS: return "CGNS++: Wrong number of dimensions in ADF node";
	case Exception::WRONG_DIMENSION: return "CGNS++: Wrong dimension in ADF node";
	case Exception::ILLEGAL_DATA: return "CGNS++: Illegal data in ADF node";
	case Exception::ILLEGAL_RANGE: return "CGNS++: Illegal range";
//	case Exception::: return "CGNS++: ";
	case Exception::TODO: return "CGNS++: Todo, still unimplemented, sorry";
	case Exception::INTERNAL_ERROR: return "CGNS++: Internal error";
	default: return "";
	}
}

void CGNS::Private::Check(int iErr) {
	if (iErr!=Exception::NO_ERROR) Emit(iErr);
}

void CGNS::Private::Emit(int iErr) {
	// throw only if no exception is active
	if (!std::uncaught_exception()) throw Exception(iErr);
	exit(-1);
}

Exception::Exception(int iErrorCode) throw()
	: mErrorCode(iErrorCode) /*, mErrorString(0)*/ {
//	if (iErrorCode==CGNSLIB_ERROR) mErrorString=cg_get_error();
}

Exception::~Exception() throw() {}

int Exception::getErrorCode() const throw() {
	return mErrorCode;
}

char const * Exception::getErrorMessage() const throw() {
//	if (mErrorString) return mErrorString;
	if (mErrorCode==NO_ERROR) return "";
	if (mErrorCode>=CGNSPP_FIRST_ERROR) return ::getErrorMessage(mErrorCode);
	// delegate hoping they can resolve
	return ADF::Exception(mErrorCode).getErrorMessage();
}

char const * Exception::what() const throw() {
	return getErrorMessage();
}
