#include "RenderSettingsDialog.h"

#include <QDebug>

RenderSettingsDialog::RenderSettingsDialog(QWidget* parent, Controller& controller, int currentHeight) : 
						QDialog(parent), controller(controller), height(height)
{
	dialogForm.setupUi(this);

	dialogForm.normalRadioButton->setChecked(true);
	dialogForm.heightSpinBox->setValue(currentHeight);
	connect(dialogForm.znSpinBox, SIGNAL(valueChanged(double)), this, SLOT(znChange(double)));
	connect(dialogForm.zfSpinBox, SIGNAL(valueChanged(double)), this, SLOT(zfChange(double)));
	connect(dialogForm.swSpinBox, SIGNAL(valueChanged(double)), this, SLOT(swChange(double)));
	connect(dialogForm.shSpinBox, SIGNAL(valueChanged(double)), this, SLOT(shChange(double)));
	connect(dialogForm.gammaSpinBox, SIGNAL(valueChanged(double)), this, SLOT(gammaChange(double)));
	connect(dialogForm.depthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(depthChange(int)));
	connect(dialogForm.heightSpinBox, SIGNAL(valueChanged(int)), this, SLOT(heightChange(int)));
}


RenderSettingsDialog::~RenderSettingsDialog()
{
	
}

int RenderSettingsDialog::getHeight() const
{
	return dialogForm.heightSpinBox->value();
}

void RenderSettingsDialog::heightChange(int val)
{
	qDebug() << val;
}

void RenderSettingsDialog::znChange(double val)
{
	qDebug() << "znChange " << QString::number(val);	
}

void RenderSettingsDialog::zfChange(double val)
{
	qDebug() << "zfChange " << QString::number(val);
}

void RenderSettingsDialog::swChange(double val)
{
	qDebug() << "swChange " << QString::number(val);
}

void RenderSettingsDialog::shChange(double val)
{
	qDebug() << "shChange " << QString::number(val);
}

void RenderSettingsDialog::gammaChange(double val)
{
	qDebug() << "gammaChange " << QString::number(val);
}

void RenderSettingsDialog::depthChange(int val)
{
	qDebug() << "depthChange " << QString::number(val);
}
