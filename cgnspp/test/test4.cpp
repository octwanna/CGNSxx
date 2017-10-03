/***************************************************************************
                          test4.cpp  -  description
                             -------------------
    begin                : Thu Mar 28 2002
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "cgns++.h"

#include "test.h"

#include <iostream>

#if _MSC_VER > 1300
	#include "process.h"
#else
	#include <unistd.h>
#endif

using std::cerr;
using std::endl;

void CreateTest4(TempFile & file) {
	cerr << "Now we try to create a new CGNS database.\n";
	CGNS::File tempFile(file.mName, CGNS::File::WRITE);
	cerr << "CGNS Open of temporary file (" << file.mName
		<< ") seems to have been successfull.\n";
	CGNS::Base base=tempFile.writeBase("Base", 1, 1);
	CGNS::Family family=base.writeFamily("Family");
	CGNS::BC::BCType bcType=family.writeFamilyBC(CGNS::BC::DIRICHLET);
//cerr << "Please copy file " << file.mName << endl;
//sleep(10);
	cerr << "########### Test4: Finished . Ok! ###########\n";
}

