//
// Created by gaojian on 19-4-6.
//

#ifndef CODEREFACTOR_STRINGSTREAMOPENHELPER_H
#define CODEREFACTOR_STRINGSTREAMOPENHELPER_H

#include <sstream>
#include "OpenHelper.h"

class StringStreamOpenHelper : public OpenHelper {
	friend StringStreamOpenHelper &operator <<(StringStreamOpenHelper &opener, std::string &newInput);
	friend StringStreamOpenHelper &operator <<(StringStreamOpenHelper &opener, std::string &&newInput);
	friend StringStreamOpenHelper &operator >>(StringStreamOpenHelper &opener, std::string &output);

public:
	StringStreamOpenHelper();
	~StringStreamOpenHelper() override = default;

private:
	std::istringstream is;
	std::ostringstream os;
};

StringStreamOpenHelper &operator <<(StringStreamOpenHelper &opener, std::string &newInput);
StringStreamOpenHelper &operator <<(StringStreamOpenHelper &opener, std::string &&newInput);
StringStreamOpenHelper &operator >>(StringStreamOpenHelper &opener, std::string &outputBuf);

#endif //CODEREFACTOR_STRINGSTREAMOPENHELPER_H
