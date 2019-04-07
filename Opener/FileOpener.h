//
// Created by gaojian on 19-4-6.
//

#ifndef CODEREFACTOR_FILEOPENER_H
#define CODEREFACTOR_FILEOPENER_H

#include <fstream>
#include "Opener.h"

class FileOpener : public Opener {
public:
	FileOpener(std::string readFileName);
	FileOpener(std::string readFileName, std::string outputFileName);
	~FileOpener() override;

private:
	std::string m_inputFileName;
	std::string m_outputFileName;
	std::ifstream m_ifstream;
	std::ofstream m_ofstream;
};

#endif //CODEREFACTOR_FILEOPENER_H
