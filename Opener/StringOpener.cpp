//
// Created by gaojian on 19-4-6.
//

#include "StringOpener.h"

using std::string;

StringOpener::StringOpener()
	: Opener(is, os, "string")
{}

StringOpener &operator <<(StringOpener &opener, std::string &newInput)
{
	opener.is.str(newInput);
	return opener;
}

StringOpener &operator <<(StringOpener &opener, std::string &&newInput)
{
	opener.is.str(newInput);
	return opener;
}

StringOpener &operator >>(StringOpener &opener, std::string &outputBuf)
{
	outputBuf = opener.os.str();
	return opener;
}
