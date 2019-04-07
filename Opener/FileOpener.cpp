//
// Created by gaojian on 19-4-6.
//

#include <iostream>
#include "FileOpener.h"

FileOpener::FileOpener(std::string readFileName)
	: m_inputFileName{readFileName}, m_ifstream{readFileName},
	Opener(m_ifstream, std::cout, readFileName)
{}

FileOpener::FileOpener(std::string readFileName, std::string outputFileName)
	: m_inputFileName{readFileName}, m_outputFileName{outputFileName},
	m_ifstream(readFileName), m_ofstream(outputFileName),
	Opener(m_ifstream, m_ofstream, readFileName)
{}

FileOpener::~FileOpener()
{
	m_ifstream.close();
	m_ofstream.close();
}