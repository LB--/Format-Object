#ifndef __StringConverter_HeaderPlusPlus__
#define __StringConverter_HeaderPlusPlus__
#include <sstream>
namespace StringConverterUtil
{
#ifdef UNICODE
	typedef std::wistringstream istr;
	typedef std::wostringstream ostr;
	typedef std::wstring str;
#else
	typedef std::istringstream istr;
	typedef std::ostringstream ostr;
	typedef std::string str;
#endif
}
template<typename T>
struct ConvTo
{
	static T f(const StringConverterUtil::str &from)
	{using namespace StringConverterUtil;
		T t;
		istr(from) >> t;
		return t;
	}
};
template<>
struct ConvTo<StringConverterUtil::str>
{
	template<typename F>
	static StringConverterUtil::str f(const F &from)
	{using namespace StringConverterUtil;
		ostr s;
		return (s << from), s.str();
	}
};

#endif
