#include "ParserUtils.h"
#include <locale>
#include <fstream>
#include "PointVector3D.h"

using std::string;

namespace RayTracingUtils
{
	ParserUtils::ParserUtils()
	{
	}


	ParserUtils::~ParserUtils()
	{
	}


	std::vector<std::string> ParserUtils::splitBySpace(const string& inputString)
	{
		std::vector<string> tokens;
		string token;

		for (auto c : inputString)
		{
			if (!isspace(c))
			{
				token += c;
			}
			else
			{
				if (!token.empty())
				{
					tokens.push_back(token);
				}
				token.clear();
			}
		}

		if (!token.empty())
		{
			tokens.push_back(token);
		}

		return tokens;
	}

	void ParserUtils::removeComment(std::string& str)
	{
		str = str.substr(0, str.find_first_of('//'));

		int first = 0, last = str.length();
		bool state = false;

		for (int i = 0; i < str.length(); i++)
		{
			if (i == 0 && str.at(i) != ' ')
			{
				first = 0;
				break;
			}
			if (i == 0 && str.at(i) == ' ' && state == false)
			{
				first = i;
				state = true;
				continue;
			}
			if (str.at(i) == ' ' && state)
			{
				first = i;
				continue;
			}
			if (state && str.at(i) != ' ')
			{
				first = i;
				break;
			}
		}

		state = false;
		for (int i = str.length() - 1; i > 0; i--)
		{
			if (i == str.length() - 1 && str.at(i) != ' ')
			{
				last = str.length();
				break;
			}
			if (i == str.length() - 1 && str.at(i) == ' ' && state == false)
			{
				last = i;
				state = true;
				continue;
			}
			if (str.at(i) == ' ' && state)
			{
				last = i;
				continue;
			}
			if (state && str.at(i) != ' ')
			{
				last = i + 1;
				break;
			}
		}

		str = str.substr(first, last - first);
	}

	bool ParserUtils::readNextNotEmptyString(std::ifstream& inputFile, std::string& str)
	{
		string fileStr;
		getline(inputFile, fileStr);

		removeComment(fileStr);

		while (fileStr.length() == 0)
		{
			if (!getline(inputFile, fileStr))
			{
				return false;
			}
			removeComment(fileStr);
		}
		removeComment(fileStr);
		str = fileStr;

		return true;
	}

	PointVector3D ParserUtils::readVertex(std::ifstream& inputFile)
	{
		string fileStr;

		readNextNotEmptyString(inputFile, fileStr);
		ParserUtils::removeComment(fileStr);
		while (fileStr.length() == 0)
		{
			readNextNotEmptyString(inputFile, fileStr);
			ParserUtils::removeComment(fileStr);
		}
		std::vector<string> subStrings = ParserUtils::splitBySpace(fileStr);
		return  PointVector3D(atof(subStrings.at(0).c_str()), atof(subStrings.at(1).c_str()), atof(subStrings.at(2).c_str()));
	}
}
