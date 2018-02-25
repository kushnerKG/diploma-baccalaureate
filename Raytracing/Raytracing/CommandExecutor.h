#pragma once
#include <vector>
#include <map>
#include "IModelListener.h"

class Controller;

class CommandExecutor
{
public:
	~CommandExecutor();
	
	void startExecute(int commandNumber);
	void finishExecute(int commandNumber);
	explicit CommandExecutor(std::string fileName, Controller& controller);

	int getCommandQuantity() const;

private:
	enum CommandType
	{
		ClassicRGB,
		ClassicSpectr,
		FlatSpectr,
		FlatPolarization
	};

	static const std::string RGB_IDENTIFIER;
	static const std::string CLASSIC_SPECTR_IDENTIFIER;
	static const std::string SPECTR_WITH_FLAT_LIGHT_IDENTIFIER;
	static const std::string POLARIZATION_WITH_FLAT_LIGHT_IDENTIFIER;

	std::map<std::string, CommandType> mapStringValues;
	
	std::string fileName;
	Controller& controller;

	class Command
	{
	public:
		Command(CommandType type, const std::string& sceneFilePath, const std::string& renderFilePath, 
			const std::string& saveImagePath, int height);

		const CommandType& getType() const;
		const std::string& getSceneFilePath() const;
		const std::string& getRenderFilePath() const;
		const std::string& getSaveImagePath() const;
		int getHeight() const;

	private:
		CommandType type;
		std::string sceneFilePath;
		std::string renderFilePath;
		std::string saveImagePath;

		int height;
	};

	class CommandParser
	{
	public:
		explicit CommandParser(std::string pathToCommandFile);

		std::vector<Command> parse(CommandExecutor& commandExecutor) const;
	private:
		std::string pathToCommandFile;
	};

	std::vector<Command> commands;
	
	std::string subPath;
};
