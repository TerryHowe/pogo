
#include <fstream>
#include "ttFileLexer.h"
#include "ttException.h"

#define TT_NODEBUG 1
#include "ttInfraLog.h"


namespace tt {

FileLexer::FileLexer(const char* fileName) throw(Exception)
	: Lexer(), _fileName((fileName)?fileName:"fileNameNotSet"), _data("")
{
	

	//
	// Open the file
	//
	
#if sun
	std::fstream inFile(fileName, std::ios::in);
#else
	std::fstream inFile(fileName, std::ios_base::in);
#endif
	if (! inFile.is_open()) {
		std::string eStr("Error opening file <");
		eStr += fileName;
		eStr += ">";
		throw tt::ExceptionErrno(__FILE__, __LINE__, eStr);
	}

	//
	// Populate the buffer
	//
	char nl[2];
	char buf[2046];
	nl[0] = '\012'; nl[1] = '\000';
	
	while (inFile.getline(buf, sizeof(buf))) {

		_data += buf;
		_data += nl;
	}

	//
	// Set
	//
	
	set_Buffer(_data.c_str());

	
}

FileLexer::~FileLexer()
{
	
	
}


};
