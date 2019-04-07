//
// Created by gaojian on 19-4-6.
//

#ifndef CODEREFACTOR_STDIOOPENER_H
#define CODEREFACTOR_STDIOOPENER_H

#include "Opener.h"

class StdioOpener : public Opener {
public:
	StdioOpener();
	~StdioOpener() override = default;
};

#endif //CODEREFACTOR_STDIOOPENER_H
