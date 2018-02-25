#pragma once
#include "qdialog.h"
#include "ui_rendersettingdialogform.h"

class Controller;

class RenderSettingsDialog : public QDialog
{
	Q_OBJECT

public:
	RenderSettingsDialog(QWidget* parent, Controller& controller, int currentHeight);
	~RenderSettingsDialog();
	
	int getHeight() const;

private:
	Ui::Dialog dialogForm;
	Controller& controller;
	int height;
private slots:
	void heightChange(int val);
	void znChange(double val);
	void zfChange(double val);
	void swChange(double val);
	void shChange(double val);
	void gammaChange(double val);
	void depthChange(int val);
};

