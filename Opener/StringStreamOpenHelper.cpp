//
// Created by gaojian on 19-4-6.
//

#include "StringStreamOpenHelper.h"

using std::string;

StringStreamOpenHelper::StringStreamOpenHelper()
	: OpenHelper(is, os, "string")
{}

StringStreamOpenHelper &operator <<(StringStreamOpenHelper &opener, std::string &newInput)
{
	opener.is.str(newInput);
	return opener;
}

StringStreamOpenHelper &operator <<(StringStreamOpenHelper &opener, std::string &&newInput)
{
	opener.is.str(newInput);
	return opener;
}

StringStreamOpenHelper &operator >>(StringStreamOpenHelper &opener, std::string &outputBuf)
{
	outputBuf = opener.os.str();
	return opener;
}
