//
// Created by gaojian on 19-4-6.
//

#include "OpenHelper.h"

OpenHelper::OpenHelper(std::istream &is, std::ostream &os, std::string streamName)
	: m_inputStream{is}, m_outputStream{os}, m_openedStreamName{std::move(streamName)}
{}

OpenHelper::~OpenHelper()
{}

std::istream& OpenHelper::getInputStream()
{
	return m_inputStream;
}

std::ostream& OpenHelper::getOutputStream()
{
	return m_outputStream;
}

std::string &OpenHelper::getOpenedName()
{
	return m_openedStreamName;
}
