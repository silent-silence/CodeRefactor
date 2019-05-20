//
// Created by gaojian on 19-4-6.
//

#ifndef CODEREFACTOR_OPENHELPER_H
#define CODEREFACTOR_OPENHELPER_H

#include <istream>
#include <ostream>

class OpenHelper {
public:
	OpenHelper(std::istream &is, std::ostream &os, std::string streamName);
	virtual ~OpenHelper() = 0;

	std::istream &getInputStream();
	std::ostream &getOutputStream();
	std::string &getOpenedName();

private:
	std::istream &m_inputStream;
	std::ostream &m_outputStream;
	std::string m_openedStreamName;
};

#endif //CODEREFACTOR_OPENHELPER_H
