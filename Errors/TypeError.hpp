//
// Created by 17271 on 2019/5/25.
//

#ifndef CODEREFACTOR_TYPEERROR_H
#define CODEREFACTOR_TYPEERROR_H

#include <string>
#include <exception>

/// @brief Throw this while type specifiers are illegal
class TypeError : public std::exception {
public:
	explicit TypeError(const char *error)
			: m_msg{error}
	{}
	explicit TypeError(std::string error)
	: m_msg{error.data()}
	{}

	~TypeError() override = default;

	const char *what() const noexcept override
	{
		return m_msg.data();
	}

private:
	const std::string m_msg;
};


#endif //CODEREFACTOR_TYPEERROR_H
