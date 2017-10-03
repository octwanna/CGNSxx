/***************************************************************************
                          example_main.cpp  -  description
                             -------------------
    begin                : Mon Oct 29 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "../cgns++.h"

#include <iostream>

#ifdef EXAMPLE

extern void EXAMPLE();

int main() {
	try {
		EXAMPLE();
		return 0;
	}
	catch (CGNS::Exception const & e) {
		std::cerr << "Caught CGNS::Exception: " << e.what() << std::endl;
	}
	catch (ADF::Exception const & e) {
		std::cerr << "Caught ADF::Exception: " << e.what() << std::endl;
	}
	catch (std::exception const & e) {
		std::cerr << "Caught std::exception: " << e.what() << std::endl;
	}
	catch (char const * e) {
		std::cerr << "Caught char const *: " << e << std::endl;
	}
	catch (...) {
		std::cerr << "Caught unknown exception\n";
	}
	return 1;
}

#endif
