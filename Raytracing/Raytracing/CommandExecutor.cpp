#include "CommandExecutor.h"
#include "Controller.h"
#include <fstream>
#include <QDebug>
#include <QString>
#include "ParserUtils.h"

using namespace  std;
using namespace RayTracingUtils;


//CommandExecutor

const std::string CommandExecutor::RGB_IDENTIFIER = "RGB";
const std::string CommandExecutor::CLASSIC_SPECTR_IDENTIFIER = "CLASSIC_SPECTR";
const std::string CommandExecutor::SPECTR_WITH_FLAT_LIGHT_IDENTIFIER = "SPECTR_WITH_FLAT_LIGHT";
const std::string CommandExecutor::POLARIZATION_WITH_FLAT_LIGHT_IDENTIFIER = "POLARIZATION_WITH_FLAT_LIGHT";

CommandExecutor::CommandExecutor(std::string fileName, Controller& controller) : 
	fileName(fileName), controller(controller)
{
	mapStringValues[RGB_IDENTIFIER] = ClassicRGB;
	mapStringValues[CLASSIC_SPECTR_IDENTIFIER] = ClassicSpectr;
	mapStringValues[SPECTR_WITH_FLAT_LIGHT_IDENTIFIER] = FlatSpectr;
	mapStringValues[POLARIZATION_WITH_FLAT_LIGHT_IDENTIFIER] = FlatPolarization;

	CommandParser parser(fileName);
	commands = parser.parse(*this);
	subPath = fileName.substr(0, fileName.find_last_of("/") + 1);
}

int CommandExecutor::getCommandQuantity() const
{
	return  commands.size();
}

CommandExecutor::~CommandExecutor()
{
}

void CommandExecutor::startExecute(int commandNumber)
{
	qDebug() << "execute";

	Command command = commands.at(commandNumber);
	controller.updateWorkSpaceSize(command.getHeight(), command.getHeight());
	controller.onOpenSceneFile(subPath + command.getSceneFilePath(), subPath + command.getRenderFilePath());
	
	switch (command.getType())
	{
	case ClassicRGB:
		controller.toRender(true);
		break;
	case ClassicSpectr:
		controller.toSpectrumRender(true);
		break;
	case FlatSpectr:
		controller.toSpectrumRenderWithSurfaceLightSource(true);
		break;
	case FlatPolarization:
		controller.toPolarizationRender(true);
		break;
	default: break;
	}
}

void CommandExecutor::finishExecute(int commandNumber)
{
	string str = subPath + commands.at(commandNumber).getSaveImagePath();
	QString tmp = QString::fromLocal8Bit(str.c_str());
	controller.onSaveImage(QString::fromLocal8Bit(str.c_str()));
}

//Command

CommandExecutor::Command::Command(CommandType type, const std::string& sceneFilePath, const std::string& renderFilePath, 
	const std::string& saveImagePath, int height) :
						type(type), sceneFilePath(sceneFilePath), renderFilePath(renderFilePath), 
						saveImagePath(saveImagePath), height(height)
{
}



const CommandExecutor::CommandType& CommandExecutor::Command::getType() const
{
	return type;
}

const std::string& CommandExecutor::Command::getSceneFilePath() const
{
	return sceneFilePath;
}

const std::string& CommandExecutor::Command::getRenderFilePath() const
{
	return renderFilePath;
}

const std::string& CommandExecutor::Command::getSaveImagePath() const
{
	return saveImagePath;
}


int CommandExecutor::Command::getHeight() const
{
	return height;
}

// CommandParser

CommandExecutor::CommandParser::CommandParser(std::string pathToCommandFile) : pathToCommandFile(pathToCommandFile)
{
}

std::vector<CommandExecutor::Command> CommandExecutor::CommandParser::parse(CommandExecutor& commandExecutor) const
{
	std::vector<Command> commands;

	ifstream inputFile;
	inputFile.open(pathToCommandFile);

	string fileStr;
	vector<string> separateCommand;
	while (ParserUtils::readNextNotEmptyString(inputFile, fileStr))
	{
		separateCommand = ParserUtils::splitBySpace(fileStr);

		CommandType type = ClassicRGB;
		switch (commandExecutor.mapStringValues[separateCommand.at(0)])
		{
			case ClassicRGB:
				type = ClassicRGB;
				break;
			case ClassicSpectr:
				type = ClassicSpectr;
				break;
			case FlatSpectr:
				type = FlatSpectr;
				break;
			case FlatPolarization:
				type = FlatPolarization;
				break;
			default: break;
		}
	
		Command command(type, separateCommand.at(1), separateCommand.at(2), separateCommand.at(3), 
						stoi(separateCommand.at(4)));
		commands.push_back(command);
		separateCommand.clear();
	}

	return commands;
}
