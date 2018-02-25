#pragma once
#include <qimage.h>
#include <memory>
class IModelListener
{
public:
	virtual ~IModelListener()
	{
	}

	virtual void handleUpdateImage(std::shared_ptr<QImage> image) = 0;
	
	virtual void handleRGBMode(bool included) = 0;
	virtual void handleSpectrumMode(bool included) = 0;
	virtual void handleSpectrumWithPolarizationMode(bool included) = 0;

	/*
	void updateImage(BufferedImage image);

    void windowPanelRepaint();

    void updateProperties(DialogProperties properties);

    void setUpdatable(boolean isUpdatable);

    void setStatusBarText(String text);
	 */
};

