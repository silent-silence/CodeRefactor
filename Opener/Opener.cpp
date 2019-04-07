//
// Created by gaojian on 19-4-6.
//

#include "Opener.h"

Opener::Opener(std::istream &is, std::ostream &os, std::string streamName)
	: m_inputStream{is}, m_outputStream{os}, m_openedStreamName{std::move(streamName)}
{}

Opener::~Opener()
{}

std::istream& Opener::getInputStream()
{
	return m_inputStream;
}

std::ostream& Opener::getOutputStream()
{
	return m_outputStream;
}

std::string &Opener::getOpenedName()
{
	return m_openedStreamName;
}
