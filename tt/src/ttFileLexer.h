#ifndef ttFileLexer_h
#define ttFileLexer_h

#include "ttLexer.h"

namespace tt {

class FileLexer : public Lexer {
private:
	std::string _fileName;
	std::string _data;
public:
	FileLexer(const char* fileName) throw(Exception);
	~FileLexer();
};

};
#endif
