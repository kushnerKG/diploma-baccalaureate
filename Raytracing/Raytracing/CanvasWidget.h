#pragma once
#include "qwidget.h"
#include <memory>
class Controller;

class CanvasWidget :
	public QWidget
{
public:
	CanvasWidget(QWidget *parent = 0);
	~CanvasWidget();

	void setController(Controller* controller);
	void setImage(std::shared_ptr<QImage> image);

protected:
	void paintEvent(QPaintEvent* event) override;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

private:

	Controller* controller = nullptr;
	std::shared_ptr<QImage> image = nullptr;

	bool isPressed = false;
	int lastX;
	int lastY;
};

