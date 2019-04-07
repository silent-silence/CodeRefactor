//
// Created by gaojian on 19-4-6.
//

#ifndef CODEREFACTOR_OPENER_H
#define CODEREFACTOR_OPENER_H

#include <istream>
#include <ostream>

class Opener {
public:
	Opener(std::istream &is, std::ostream &os, std::string streamName);
	virtual ~Opener() = 0;

	std::istream &getInputStream();
	std::ostream &getOutputStream();
	std::string &getOpenedName();

private:
	std::istream &m_inputStream;
	std::ostream &m_outputStream;
	std::string m_openedStreamName;
};

#endif //CODEREFACTOR_OPENER_H
