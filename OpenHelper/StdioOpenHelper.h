//
// Created by gaojian on 19-4-6.
//

#ifndef CODEREFACTOR_STDIOOPENHELPER_H
#define CODEREFACTOR_STDIOOPENHELPER_H

#include "OpenHelper.h"

/// @brief Open stander I/O stream
class StdioOpenHelper : public OpenHelper {
public:
	StdioOpenHelper();
	~StdioOpenHelper() override = default;
};

#endif //CODEREFACTOR_STDIOOPENHELPER_H
