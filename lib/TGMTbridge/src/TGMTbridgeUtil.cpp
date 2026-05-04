#include "TGMTbridgeUtil.h"

using namespace System::Text::RegularExpressions;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

System::String^ TGMTbridgeUtil::RemoveSpecialCharacter(System::String^ str)
{
	return Regex::Replace(str, "[^0-9a-zA-Z]+", "");
}