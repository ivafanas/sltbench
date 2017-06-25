#include "WarningsMessages.h"

namespace {

const char* const c_msg_debug = "sltbench is compiled in debug mode. Performance results might be not relevant.";
const char* const c_msg_empty = "";

} // namespace

namespace sltbench {

const char * MessageForWarning(RunWarning warning)
{
	switch(warning)
	{
	case RunWarning::DEBUG_BUILD: return c_msg_debug;
	};
	return c_msg_empty;
}

} // namespace sltbench
