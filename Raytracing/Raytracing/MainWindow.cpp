#include <QDir>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>
#include <memory>
#include <QMessageBox>

#include "MainWindow.h"
#include "RenderSettingsDialog.h"
#include "Controller.h"
#include <qicon.h>
#include "CommandExecutor.h"
#include "RayTransfer.h"
#include <qevent.h>


MainWindow::MainWindow(Controller* controller, QWidget *parent)
	: QMainWindow(parent), controller(controller)
{
	ui.setupUi(this);

	ui.canvasWidget->setController(controller);
	ui.openSceneFileAction->setIcon(QIcon::fromTheme(":/Raytracing/Resources/folder_open.png"));
	ui.showCoordSystemAction->setIcon(QIcon::fromTheme(":/Raytracing/Resources/axis.png"));
	ui.quitAction->setIcon(QIcon::fromTheme(":/Raytracing/Resources/icon_close.png"));
	ui.renderSettingDiolagAction->setIcon(QIcon::fromTheme(":/Raytracing/Resources/setting.png"));
	ui.renderAction->setIcon(QIcon::fromTheme(":/Raytracing/Resources/render.png"));
	ui.saveImageAction->setIcon(QIcon::fromTheme(":/Raytracing/Resources/save.png"));
	ui.spectrumRenderAction->setIcon(QIcon::fromTheme(":/Raytracing/Resources/spectrum.png"));
	ui.polarizatiomRenderAction->setIcon(QIcon::fromTheme(":/Raytracing/Resources/polarization.png"));
	ui.dispersionRenderSurfaceAction->setIcon(QIcon::fromTheme(":/Raytracing/Resources/dispersion.png"));
	ui.polarizationTransferAction->setIcon(QIcon::fromTheme(":/Raytracing/Resources/rayTransfer.png"));
	ui.openCommandsFileAction->setIcon(QIcon::fromTheme(":/Raytracing/Resources/commands.png"));

	ui.openRenderFileAction->setEnabled(false);
	ui.saveRenderSettingAction->setEnabled(false);
	ui.saveImageAction->setEnabled(false);
	ui.renderSettingDiolagAction->setEnabled(false);
	ui.initStateAction->setEnabled(false);
	ui.chooseViewAction->setEnabled(false);
	ui.showCoordSystemAction->setEnabled(false);
	ui.renderAction->setEnabled(false);
	ui.spectrumRenderAction->setEnabled(false);
	ui.polarizatiomRenderAction->setEnabled(false);
	ui.dispersionRenderSurfaceAction->setEnabled(false);

	connect(ui.openSceneFileAction, SIGNAL(triggered()), SLOT(slotOpenSceneFile()));
	connect(ui.openRenderFileAction, SIGNAL(triggered()), SLOT(slotOpenRenderFile()));
	connect(ui.openCommandsFileAction, SIGNAL(triggered()), SLOT(slotOpenCommandsFile()));
	connect(ui.saveRenderSettingAction, SIGNAL(triggered()), SLOT(slotSaveRenderSettings()));
	connect(ui.saveImageAction, SIGNAL(triggered()), SLOT(slotSaveImage()));
	connect(ui.quitAction, SIGNAL(triggered()), SLOT(slotQuit()));
	connect(ui.renderSettingDiolagAction, SIGNAL(triggered()), SLOT(slotRenderSetting()));
	connect(ui.initStateAction, SIGNAL(triggered()), SLOT(slotInit()));
	connect(ui.renderAction, SIGNAL(triggered()), SLOT(slotRender()));
	connect(ui.chooseViewAction, SIGNAL(triggered()), SLOT(slotChooseViewScene()));
	connect(ui.helpMenu, SIGNAL(triggered()), SLOT(slotHelp()));
	connect(ui.showCoordSystemAction, SIGNAL(triggered()), SLOT(slotShowSystemCoordinates()));
	connect(ui.spectrumRenderAction, SIGNAL(triggered()), SLOT(slotSpectrumRender()));
	connect(ui.polarizatiomRenderAction, SIGNAL(triggered()), SLOT(slotSpectrumRenderWithPOlarization()));
	connect(ui.dispersionRenderSurfaceAction, SIGNAL(triggered()), SLOT(slotRenderWithPolarizationSutfaceLightSource()));
	connect(ui.polarizationTransferAction, SIGNAL(triggered()), SLOT(slotRayTransfer()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::handleUpdateImage(std::shared_ptr<QImage> image) //тут тип делаем чтобы отрисовывалось
{
	if (!includedCommandMode){
		ui.canvasWidget->setImage(image);
		ui.canvasWidget->repaint();
	}
}

void MainWindow::handleRGBMode(bool included)
{
	ui.renderAction->setEnabled(included);
}

void MainWindow::handleSpectrumMode(bool included)
{
	ui.spectrumRenderAction->setEnabled(included);
}

void MainWindow::handleSpectrumWithPolarizationMode(bool included)
{
	ui.polarizatiomRenderAction->setEnabled(included);
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	if (!isIncludedRenderMode()){
		//controller->updateWorkSpaceSize(ui.canvasWidget->size().width(), ui.canvasWidget->size().height());
		controller->updateWorkSpaceSize(currentHeight, currentHeight);
	}
}

void MainWindow::paintEvent(QPaintEvent* event)
{
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if (isIncludedRenderMode()){
		if (genetateMessageBox("Message", "Rendered image will be lost") == QMessageBox::No)
		{
			event->ignore();
		}
	}
	if (includedCommandMode)
	{
		if (genetateMessageBox("Message", "Last task' progress will be lost") == QMessageBox::No)
		{
			event->ignore();
		}
	}
}

void MainWindow::includeRenderMenuMode(bool included)
{
	ui.openSceneFileAction->setEnabled(!included);
	ui.openRenderFileAction->setEnabled(!included);
	ui.saveRenderSettingAction->setEnabled(!included);
	ui.initStateAction->setEnabled(!included);
	ui.chooseViewAction->setEnabled(!included);
	ui.showCoordSystemAction->setEnabled(!included);
	ui.openCommandsFileAction->setEnabled(!included);
}

bool MainWindow::isIncludedRenderMode() const
{
	if (!ui.renderAction->isChecked() && !ui.spectrumRenderAction->isChecked() && !ui.polarizatiomRenderAction->isChecked() && !ui.dispersionRenderSurfaceAction->isChecked())
	{
		return false;
	}
	return true;
}

int MainWindow::genetateMessageBox(QString title, QString message) const
{
	QMessageBox* pmbx = new QMessageBox(QMessageBox::Information, title, message, QMessageBox::Yes | QMessageBox::No);
	int n = pmbx->exec();
	delete pmbx;

	return n;
}

void MainWindow::slotOpenSceneFile()
{
	if (isIncludedRenderMode()){
		if (genetateMessageBox("Message", "Rendered image will be lost") == QMessageBox::No)
		{
			return;
		}
	}

	QString fileName = nullptr;
	fileName = QFileDialog::getOpenFileName(this, "Select a scene file...", currentDirectory, tr("*.scene *.sceneSpectrum"));
	if (fileName == nullptr)
	{
		return;
	}

	currentDirectory = fileName.left(fileName.lastIndexOf("/"));

	std::string tmp = std::string(fileName.toLocal8Bit().constData());
	//controller->updateWorkSpaceSize(ui.canvasWidget->size().width(), ui.canvasWidget->size().height());
	controller->updateWorkSpaceSize(currentHeight, currentHeight);
	controller->onOpenSceneFile(tmp);
	ui.openRenderFileAction->setEnabled(true);
	ui.saveRenderSettingAction->setEnabled(true);
	ui.saveImageAction->setEnabled(true);
	ui.renderSettingDiolagAction->setEnabled(true);
	ui.showCoordSystemAction->setEnabled(true);
	
	if (CURRENT_RENDER_MODE == RGB_RENDER)
	{
		ui.renderAction->setEnabled(true);
		ui.spectrumRenderAction->setEnabled(false);
		ui.polarizatiomRenderAction->setEnabled(false);
		ui.dispersionRenderSurfaceAction->setEnabled(false);
	}
	else
	{
		ui.renderAction->setEnabled(false);
		ui.spectrumRenderAction->setEnabled(true);
		ui.polarizatiomRenderAction->setEnabled(true);
		ui.dispersionRenderSurfaceAction->setEnabled(true);
	}
}

void MainWindow::slotOpenRenderFile()
{
	QString fileName = nullptr;
	fileName = QFileDialog::getOpenFileName(this, "Select a render file...", currentDirectory, "*.render");
	if (fileName == nullptr)
	{
		return;
	}

	currentDirectory = fileName.left(fileName.lastIndexOf("/"));

	std::string tmp = std::string(fileName.toLocal8Bit().constData());

	controller->updateWorkSpaceSize(ui.canvasWidget->size().width(), ui.canvasWidget->size().height());
	controller->updateWorkSpaceSize(currentHeight, currentHeight);
	controller->onOpenRenderFile(tmp, CURRENT_RENDER_MODE);
}

void MainWindow::slotOpenCommandsFile()
{
	QString fileName = nullptr;
	fileName = QFileDialog::getOpenFileName(this, "Select a render file...", currentDirectory, "*.commands");
	if (fileName == nullptr)
	{
		return;
	}

	currentDirectory = fileName.left(fileName.lastIndexOf("/"));

	includedCommandMode = true;
	ui.openSceneFileAction->setEnabled(false);
	ui.openRenderFileAction->setEnabled(false);
	ui.openCommandsFileAction->setEnabled(false);
	ui.saveRenderSettingAction->setEnabled(false);
	ui.saveImageAction->setEnabled(false);
	ui.renderSettingDiolagAction->setEnabled(false);
	ui.initStateAction->setEnabled(false);
	ui.chooseViewAction->setEnabled(false);
	ui.showCoordSystemAction->setEnabled(false);
	ui.renderAction->setEnabled(false);
	ui.spectrumRenderAction->setEnabled(false);
	ui.polarizatiomRenderAction->setEnabled(false);
	ui.dispersionRenderSurfaceAction->setEnabled(false);

	executor = std::make_shared<CommandExecutor>(std::string(fileName.toLocal8Bit().constData()), *controller);
	executor->startExecute(0);
	currentCommand = 0;
}

void MainWindow::slotSaveRenderSettings()
{
	QString pathToRenderFile = nullptr;
	pathToRenderFile = QFileDialog::getSaveFileName(this, "Select directory for save...", currentDirectory, "*.render");
	if (pathToRenderFile == nullptr)
	{
		return;
	}

	currentDirectory = pathToRenderFile.left(pathToRenderFile.lastIndexOf("/"));

	std::string tmp = std::string(pathToRenderFile.toLocal8Bit().constData());

	controller->onSaveRenderSettings(tmp);
}

void MainWindow::slotSaveImage()
{
	QString fileName = nullptr;
	fileName = QFileDialog::getSaveFileName(this, "Select directory for save...", currentDirectory, tr("Images (*.png *.bmp *.jpg)"));
	if (fileName == nullptr)
	{
		return;
	}

	currentDirectory = fileName.left(fileName.lastIndexOf("/"));

	controller->onSaveImage(fileName);
}

void MainWindow::slotQuit()
{
	if (isIncludedRenderMode()){
		if (genetateMessageBox("Message", "Rendered image will be lost") == QMessageBox::No)
		{
			return;
		}
	}

	if (includedCommandMode)
	{
		if (genetateMessageBox("Message", "Last task' progress will be lost") == QMessageBox::No)
		{
			return;
		}
	}
	exit(0);
}

void MainWindow::slotRayTransfer()
{
	QString pathToTransferFile = nullptr;
	pathToTransferFile = QFileDialog::getOpenFileName(this, "Select trnasfer file", currentDirectory, "*.transfer");
	if (pathToTransferFile == nullptr)
	{
		return;
	}

	RayTransfer rayTransfer = RayTransfer(std::string(pathToTransferFile.toLocal8Bit().constData()));
	rayTransfer.startTransfer();
	//rayTransfer.transfer();
}

void MainWindow::slotRenderSetting()
{
	if (isIncludedRenderMode())
	{
		QMessageBox::information(nullptr, "Information", "You cannot open settings dialog, while render mode included");
		return;
	}
	RenderSettingsDialog dialog(this, *controller, ui.canvasWidget->height());
	int height = 0;
	switch (dialog.exec()) {
	case QDialog::Accepted:
		qDebug("Accepted");
		height = dialog.getHeight();

		//m_edit->setText(dlg.getInput());
		break;
	case QDialog::Rejected:
		qDebug("Rejected");
		break;
	default:
		qDebug("Unexpected");
	}

	if (height > 0 && height != currentHeight)
	{
		currentHeight = height;
		controller->updateWorkSpaceSize(height, height);
	}
}

void MainWindow::slotInit()
{
}

void MainWindow::slotRender()
{
	if (!ui.renderAction->isChecked()){
		if (genetateMessageBox("Message", "Rendered image will be lost") == QMessageBox::No)
		{
			ui.renderAction->setChecked(true);
			return;
		}
	}
	includeRenderMenuMode(ui.renderAction->isChecked());
	controller->toRender(ui.renderAction->isChecked());
}

void MainWindow::slotSpectrumRender()
{
	if (!ui.spectrumRenderAction->isChecked()){
		if (genetateMessageBox("Message", "Rendered image will be lost") == QMessageBox::No)
		{
			ui.spectrumRenderAction->setChecked(true);
			return;
		}
	}

	includeRenderMenuMode(ui.spectrumRenderAction->isChecked());
	controller->toSpectrumRender(ui.spectrumRenderAction->isChecked());

	ui.polarizatiomRenderAction->setEnabled(!ui.spectrumRenderAction->isChecked());
	ui.dispersionRenderSurfaceAction->setEnabled(!ui.spectrumRenderAction->isChecked());
}

void MainWindow::slotSpectrumRenderWithPOlarization()
{
	if (!ui.polarizatiomRenderAction->isChecked()){
		if (genetateMessageBox("Message", "Rendered image will be lost") == QMessageBox::No)
		{
			ui.polarizatiomRenderAction->setChecked(true);
			return;
		}
	}
	includeRenderMenuMode(ui.polarizatiomRenderAction->isChecked());
	controller->toPolarizationRender(ui.polarizatiomRenderAction->isChecked());

	ui.spectrumRenderAction->setEnabled(!ui.polarizatiomRenderAction->isChecked());
	ui.dispersionRenderSurfaceAction->setEnabled(!ui.polarizatiomRenderAction->isChecked());
}

void MainWindow::slotRenderWithPolarizationSutfaceLightSource()
{
	if (!ui.dispersionRenderSurfaceAction->isChecked()){
		if (genetateMessageBox("Message", "Rendered image will be lost") == QMessageBox::No)
		{
			ui.dispersionRenderSurfaceAction->setChecked(true);
			return;
		}
	}
	includeRenderMenuMode(ui.dispersionRenderSurfaceAction->isChecked());
	controller->toSpectrumRenderWithSurfaceLightSource(ui.dispersionRenderSurfaceAction->isChecked());

	ui.spectrumRenderAction->setEnabled(!ui.dispersionRenderSurfaceAction->isChecked());
	ui.polarizatiomRenderAction->setEnabled(!ui.dispersionRenderSurfaceAction->isChecked());
}

void MainWindow::slotShowSystemCoordinates()
{
	controller->showCoordinatesSystem();
}

void MainWindow::slotChooseViewScene()
{
	qDebug("ChooseScene()");
}

void MainWindow::slotHelp()
{
	qDebug("Help()");
}

void MainWindow::slotRenderImageFromAnotherThread(QImage image)
{
	if (includedCommandMode)
	{
		executor->finishExecute(currentCommand);
		currentCommand++;
		if (currentCommand < executor->getCommandQuantity())
		{
			executor->startExecute(currentCommand);
		}
		else
		{
			includedCommandMode = false;
			
			ui.openSceneFileAction->setEnabled(true);
			ui.openCommandsFileAction->setEnabled(true);
		}
	}
	else{
		ui.canvasWidget->setImage(std::make_shared<QImage>(image));
		ui.canvasWidget->repaint();
	}
}

void MainWindow::slotUpdateStatusBar(QString str)
{
	if (!includedCommandMode){
		ui.statusbar->showMessage(str + " percents");
	}
	else
	{
		ui.statusbar->showMessage(QString::number(currentCommand + 1) + " command: " + str + " percent");
	}
}