//
// Created by gaojian on 19-4-6.
//

#ifndef CODEREFACTOR_FILEOPENHELPER_H
#define CODEREFACTOR_FILEOPENHELPER_H

#include <fstream>
#include "OpenHelper.h"

/// @brief Open file stream
class FileOpenHelper : public OpenHelper {
public:
	FileOpenHelper(std::string readFileName);
	FileOpenHelper(std::string readFileName, std::string outputFileName);
	~FileOpenHelper() override;

private:
	std::string m_inputFileName;
	std::string m_outputFileName;
	std::ifstream m_ifstream;
	std::ofstream m_ofstream;
};

#endif //CODEREFACTOR_FILEOPENHELPER_H
