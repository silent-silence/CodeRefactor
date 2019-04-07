//
// Created by gaojian on 19-4-6.
//

#ifndef CODEREFACTOR_STRINGOPENER_H
#define CODEREFACTOR_STRINGOPENER_H

#include <sstream>
#include "Opener.h"

class StringOpener : public Opener {
	friend StringOpener &operator <<(StringOpener &opener, std::string &newInput);
	friend StringOpener &operator <<(StringOpener &opener, std::string &&newInput);
	friend StringOpener &operator >>(StringOpener &opener, std::string &output);

public:
	StringOpener();
	~StringOpener() override = default;

private:
	std::istringstream is;
	std::ostringstream os;
};

StringOpener &operator <<(StringOpener &opener, std::string &newInput);
StringOpener &operator <<(StringOpener &opener, std::string &&newInput);
StringOpener &operator >>(StringOpener &opener, std::string &outputBuf);

#endif //CODEREFACTOR_STRINGOPENER_H
