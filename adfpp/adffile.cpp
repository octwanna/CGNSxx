/***************************************************************************
                          adffile.cpp  -  description
                             -------------------
    begin                : Mon Mar 12 2001
    Copyright (c) 1998-2004 Manuel Kessler, University of Stuttgart
    Copyright (c) 2001-2002 Udo Tremel, EADS Germany

    See the file COPYING in the toplevel directory for details.
 ***************************************************************************/

#include "adf++.h"

#include "adfnode.h"

#include "adfutil.h"

#include "ADF.h"

#include <iostream>

using ADF::File;

#ifndef HAVE_STRNCASECMP
static int strncasecmp(char const * s1, char const * s2, int n) {
	while (*s1 && *s2) {
		if (n--==0) return 0;
		char c1=toupper(*s1), c2=toupper(*s2);
		if (c1!=c2) return c2-c1;
		++s1, ++s2;
	}
	return toupper(*s2)-toupper(*s1);
}
#endif

// check with header file!
static char const * statusString[] = { "UNKNOWN", "READ_ONLY", "OLD", "NEW", "SCRATCH" };
CT_ASSERT(File::UNKNOWN==0 && File::READ_ONLY==1 && File::OLD==2 && File::NEW==3 && File::SCRATCH==4);

// check with header file!
static char const * formatString[] = { "UNDEFINED", "NATIVE", "IEEE_BIG", "IEEE_LITTLE", "CRAY" };
CT_ASSERT(File::UNDEFINED==0 && File::NATIVE==1 && File::IEEE_BIG==2 && File::IEEE_LITTLE==3 && File::CRAY==4);

std::string File::asString(Status iStatus) {
	if (iStatus<UNKNOWN || iStatus>SCRATCH) return "<unknown status>";
	else return statusString[iStatus];
}


std::string File::asString(Format iFormat) {
  if (iFormat<UNDEFINED || iFormat>CRAY) return "<unknown format>";
  else return formatString[iFormat];
}


double doOpen(std::string const & iFileName, File::Status iStatus, File::Format iFormat) {
	double rootId=0;
	ADF_Database_Open(iFileName.c_str(), File::asString(iStatus).c_str(),
		File::asString(iFormat).c_str(), &rootId, ADF::Checker());	
	return rootId;
}

File::File(std::string const & iFileName, Status iStatus, Format iFormat)
  : mStatus(iStatus),
    mRootNode(doOpen(iFileName, iStatus, iFormat)),
    mRootId(mRootNode.getId()) {
}

File::File(Format iFormat)
  : mStatus(SCRATCH),
    mRootNode(doOpen("", SCRATCH, iFormat)),
	mRootId(mRootNode.getId()) {
}

File::~File() {
	ADF_Database_Close(mRootId, Checker());
}

File::Status File::getStatus() const {
	return mStatus;
}

File::Format File::getFormat() const {
	char format[ADF::FORMAT_LENGTH+1];
	ADF_Database_Get_Format(mRootId, format, Checker());

	for (int f=UNDEFINED; f<=CRAY; ++f)
		if (!strcmp(formatString[f], format)) return static_cast<Format>(f);
	if (!strcmp("IEEE_BIG_32", format)) return IEEE_BIG;
	if (!strcmp("IEEE_BIG_64", format)) return IEEE_BIG;
	if (!strcmp("IEEE_LITTLE_32", format)) return IEEE_LITTLE;
	if (!strcmp("IEEE_LITTLE_64", format)) return IEEE_LITTLE;
	Emit(Exception::MACHINE_FORMAT_NOT_RECOGNIZED);
	return UNDEFINED;
}

ADF::Node File::getRoot() const {
	return mRootNode;
}

static std::string versionInfo(std::string const & iStr) {
	std::string::size_type v=iStr.find_first_of("vV");
	while (v!=std::string::npos) {
		if (!strncasecmp("version", iStr.c_str()+v, 7)) {
			v=iStr.find_first_not_of(" \t\n\f\v", v+7);
			if (v!=std::string::npos) return iStr.substr(v);
		}
		else v=iStr.find_first_of("vV", v+1);
	}
	return "A01";
}

static char versionMajor(std::string const & iStr) {
	return versionInfo(iStr)[0];
}

static int versionMinor(std::string const & iStr) {
	std::string info(versionInfo(iStr));
	return (info[1]-'0')*10+(info[2]-'0');
}

std::string File::getVersionString() const {
	static char version[DATE_LENGTH+1];
	char creation[DATE_LENGTH+1];
	char modification[DATE_LENGTH+1];
	ADF_Database_Version(mRootId, version, creation, modification, Checker());
	return version;		
}

char File::getVersionMajor() const {
	return versionMajor(getVersionString());
}

int File::getVersionMinor() const {
	return versionMinor(getVersionString());
}

std::string File::getLibraryVersionString() {
	static char version[DATE_LENGTH+1];
	ADF_Library_Version(version, Checker());
	return version;		
}

char File::getLibraryVersionMajor() {
	return versionMajor(getLibraryVersionString());
}

int File::getLibraryVersionMinor() {
	return versionMinor(getLibraryVersionString());
}
