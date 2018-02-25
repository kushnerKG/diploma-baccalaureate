#pragma once

#include "ui_mainwindowform.h"
#include "IModelListener.h"
#include <memory>
#include "CommandExecutor.h"

class Controller;

class MainWindow : public QMainWindow, public IModelListener
{
	Q_OBJECT

public:

	MainWindow(Controller* controller, QWidget *parent = nullptr);
	~MainWindow();

	Q_INVOKABLE virtual void handleUpdateImage(std::shared_ptr<QImage> image) override;

	Q_INVOKABLE virtual void handleRGBMode(bool included) override;
	Q_INVOKABLE virtual void handleSpectrumMode(bool included) override;
	Q_INVOKABLE virtual void handleSpectrumWithPolarizationMode(bool included) override;
protected:
	void resizeEvent(QResizeEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
	void closeEvent(QCloseEvent *event) override;
private:
	Ui::MainWindow ui;
	
	Controller* controller;

	void includeRenderMenuMode(bool included);

	int currentCommand;
	int includedCommandMode = false;
	std::shared_ptr<CommandExecutor> executor;

	QString currentDirectory = "./Data";
	int currentHeight = 400;

	bool isIncludedRenderMode() const;

	int genetateMessageBox(QString title, QString message) const;

private slots:
	void slotOpenSceneFile();
	void slotOpenRenderFile();
	void slotOpenCommandsFile();
	void slotSaveRenderSettings();
	void slotSaveImage();
	void slotQuit();
	void slotRayTransfer();
	void slotRenderSetting();
	void slotInit();
	void slotRender();
	void slotSpectrumRender();
	void slotSpectrumRenderWithPOlarization();
	void slotRenderWithPolarizationSutfaceLightSource();
	void slotShowSystemCoordinates();
	void slotChooseViewScene();
	void slotHelp();

	void slotRenderImageFromAnotherThread(QImage image);
	void slotUpdateStatusBar(QString str);
};
