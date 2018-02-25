#pragma once
#include <vector>
#include "PointVector3D.h"

namespace RayTracingUtils
{
	class ParserUtils
	{
	public:

		static std::vector<std::string> splitBySpace(const std::string& inputString);
		static void removeComment(std::string& str);
		static bool readNextNotEmptyString(std::ifstream& inputFile, std::string& str);
		static PointVector3D ParserUtils::readVertex(std::ifstream& inputFile);

	private:
		ParserUtils();
		~ParserUtils();
	};
}


