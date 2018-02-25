#pragma once
#include "RenderInfo.h"
#include <memory>
#include "RenderSetting.h"
class QString;

class RenderFileReader
{
public:
	explicit RenderFileReader(std::string path, RenderType renderType);
	explicit RenderFileReader(QString path, RenderType renderType);
	~RenderFileReader();
	std::shared_ptr<RenderInfo> read() const;
private:
	std::string pathToFile;
	RenderType renderType;
};

