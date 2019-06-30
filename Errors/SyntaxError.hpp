//
// Created by gaojian on 19-6-29.
//

#ifndef CODEREFACTOR_SYNTAXERROR_HPP
#define CODEREFACTOR_SYNTAXERROR_HPP

#include <exception>
#include <string>

/// @brief Throw this when not find a name in symbol table
class SyntaxError : public std::exception {
public:
	explicit SyntaxError(const char *error)
			: m_msg{error}
	{}
	explicit SyntaxError(std::string error)
			: m_msg{error}
	{}

	~SyntaxError() override = default;

	const char *what() const noexcept override
	{
		return m_msg.data();
	}

private:
	const std::string m_msg;
};

#endif //CODEREFACTOR_SYNTAXERROR_HPP
