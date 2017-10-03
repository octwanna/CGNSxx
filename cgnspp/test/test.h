/***************************************************************************
                          test.h  -  description
                             -------------------
    begin                : Fri Oct 26 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#ifndef TEST_H
#define TEST_H

#include <string>

struct TempFile {
	TempFile();
	~TempFile();
	std::string mName;
};

void CreateTest1(TempFile & iFile);
void CreateTest2(TempFile & iFile);
void CreateTest3(TempFile & iFile);
void CreateTest4(TempFile & iFile);
void CreateTest5(TempFile & iFile);

#endif
