//
// Created by gaojian on 19-4-6.
//

#include <iostream>
#include "FileOpenHelper.h"

FileOpenHelper::FileOpenHelper(std::string readFileName)
	: m_inputFileName{readFileName}, m_ifstream{readFileName},
	OpenHelper(m_ifstream, std::cout, readFileName)
{}

FileOpenHelper::FileOpenHelper(std::string readFileName, std::string outputFileName)
	: m_inputFileName{readFileName}, m_outputFileName{outputFileName},
	m_ifstream(readFileName), m_ofstream(outputFileName),
	OpenHelper(m_ifstream, m_ofstream, readFileName)
{}

FileOpenHelper::~FileOpenHelper()
{
	m_ifstream.close();
	m_ofstream.close();
}