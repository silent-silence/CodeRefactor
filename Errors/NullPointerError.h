//
// Created by gaojian on 19-7-7.
//

#ifndef CODEREFACTOR_NULLPOINTERERROR_H
#define CODEREFACTOR_NULLPOINTERERROR_H

#include <exception>
#include <string>

/// @brief Throw this when not find a name in symbol table
class NullPointerError : public std::exception {
public:
	explicit NullPointerError(const char *error)
			: m_msg{error}
	{}
	explicit NullPointerError(std::string error)
			: m_msg{error.data()}
	{}

	~NullPointerError() override = default;

	const char *what() const noexcept override
	{
		return m_msg.data();
	}

private:
	const std::string m_msg;
};

#endif //CODEREFACTOR_NULLPOINTERERROR_H
