//
// Created by 17271 on 2019/5/24.
//

#ifndef CODEREFACTOR_SYMBOLERROR_HPP
#define CODEREFACTOR_SYMBOLERROR_HPP

#include <exception>
#include <string>

/// @brief Throw this when not find a name in symbol table
class SymbolNotFound : public std::exception {
public:
	explicit SymbolNotFound(const char *error)
			: m_msg{error}
	{}
	explicit SymbolNotFound(std::string error)
			: m_msg{error.data()}
	{}

	~SymbolNotFound() override = default;

	const char *what() const noexcept override
	{
		return m_msg;
	}

private:
	const char *m_msg;
};


#endif //CODEREFACTOR_SYMBOLERROR_HPP
