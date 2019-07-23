#ifndef STRINGUTIL_INCLUDED_H
#define STRINGUTIL_INCLUDED_H

#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>

namespace CommonCode
{
	
using namespace std;

class StringUtil
{

public:

	static string ToUpper(string& str)
	{		
		string result;
		std::transform(str.begin(), str.end(), std::back_inserter(result), ::toupper);
		return result;
	}

	template <class ContainerT>
	static void Tokenize(const std::string& str, ContainerT& tokens,
		const std::string& delimiters = " ", bool trimEmpty = false)
	{
		std::string::size_type pos, lastPos = 0;

		using value_type = typename ContainerT::value_type;
		using size_type = typename ContainerT::size_type;

		while (true)
		{
			pos = str.find_first_of(delimiters, lastPos);
			if (pos == std::string::npos)
			{
				pos = str.length();

				if (pos != lastPos || !trimEmpty)
					tokens.push_back(value_type(str.data() + lastPos,
					(size_type)pos - lastPos));

				break;
			}
			else
			{
				if (pos != lastPos || !trimEmpty)
					tokens.push_back(value_type(str.data() + lastPos,
					(size_type)pos - lastPos));
			}

			lastPos = pos + 1;
		}
	}

	static std::string ReplaceString(std::string subject, const std::string& search, const std::string& replace) 
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos) 
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
		return subject;
	}

	static void ReplaceStringInPlace(std::string& subject, const std::string& search, const std::string& replace)
	{
		size_t pos = 0;
		while ((pos = subject.find(search, pos)) != std::string::npos) 
		{
			subject.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}
};

}

#endif 