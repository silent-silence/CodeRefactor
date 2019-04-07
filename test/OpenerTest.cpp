//
// Created by gaojian on 19-4-6.
//

#ifdef ENV_TEST

#include <string>
#include <sstream>
#include <gtest/gtest.h>
#include <Opener/FileOpener.h>
#include <Opener/StdioOpener.h>
#include <Opener/StringOpener.h>

TEST(OpenerTest, fileOpenerReadonly)
{
	FileOpener opener("a");
	EXPECT_EQ("a", opener.getOpenedName());

	std::string readBuf;
	opener.getInputStream() >> readBuf;
	EXPECT_EQ("int", readBuf);
	opener.getInputStream() >> readBuf;
	EXPECT_EQ("a;", readBuf);

	std::ostringstream os;
	auto backup = std::cout.rdbuf();
	std::cout.rdbuf(os.rdbuf());
	opener.getOutputStream() << "output";
	EXPECT_EQ("output", os.str());
	os.flush();
	std::cout.rdbuf(backup);
}

TEST(OpenerTest, fileOpenerReadWrite)
{
	FileOpener opener("a", "b");
	EXPECT_EQ("a", opener.getOpenedName());

	std::string readBuf;
	opener.getInputStream() >> readBuf;
	EXPECT_EQ("int", readBuf);
	opener.getInputStream() >> readBuf;
	EXPECT_EQ("a;", readBuf);

	opener.getOutputStream() << "output";
	opener.getOutputStream().flush();
	std::ifstream ifstream("b");
	ifstream >> readBuf;
	EXPECT_EQ("output", readBuf);
}

TEST(OpenerTest, stdio)
{
	StdioOpener opener;

	std::istringstream istringstream("input");
	opener.getInputStream().rdbuf(istringstream.rdbuf());
	std::string readBuf;
	std::cin >> readBuf;
	EXPECT_EQ("input", readBuf);

	std::ostringstream ostringstream("output");
	auto backup = std::cout.rdbuf();
	opener.getOutputStream().rdbuf(ostringstream.rdbuf());
	EXPECT_EQ("output", ostringstream.str());
	std::cout.rdbuf(backup);

	EXPECT_EQ("stdio", opener.getOpenedName());
}

TEST(OpenerTest, stringOpener)
{
	StringOpener opener;

	std::string readBuf;
	opener << std::string("input");
	opener.getInputStream() >> readBuf;
	EXPECT_EQ("input", readBuf);

	dynamic_cast<std::ostringstream &>(opener.getOutputStream()).str("output");
	opener >> readBuf;
	EXPECT_EQ("output", readBuf);

	EXPECT_EQ("string", opener.getOpenedName());
	opener.getOutputStream().flush();
}
#endif