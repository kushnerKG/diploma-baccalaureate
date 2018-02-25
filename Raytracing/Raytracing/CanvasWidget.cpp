#include "CanvasWidget.h"
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

#include "Controller.h"

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent), lastX(0), lastY(0)
{
}

CanvasWidget::~CanvasWidget()
{
}

void CanvasWidget::setController(Controller* controller)
{
	this->controller = controller;
}

void CanvasWidget::setImage(std::shared_ptr<QImage> image)
{
	this->image = image;
}

void CanvasWidget::paintEvent(QPaintEvent* event)
{
	if (image != nullptr)
	{	
		setFixedWidth(image->width());
		setFixedHeight(image->height());
		QPainter painter;
		painter.begin(this);
		painter.drawImage(0, 0, *image.get());
		painter.end();
	}
}

void CanvasWidget::mousePressEvent(QMouseEvent* event)
{
	isPressed = true;
	lastX = event->pos().x();
	lastY = event->pos().y();
	qDebug() << "mousePressEvent " << event->pos().x() << " " << event->pos().y();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* event)
{
	isPressed = false;
	qDebug() << "mouseReleaseEvent " << event->pos().x() << " " << event->pos().y();
}

void CanvasWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (isPressed){
		int currentX = event->pos().x();
		int currentY = event->pos().y();
		int dx = lastX - currentX;
		int dy = lastY - currentY;
		double angleY = dx * 0.0001;
		double angleX = dy * 0.0001;
		controller->rotateScene(angleX, angleY);
		qDebug() << "mouseMoveEvent " << event->pos().x() << " " << event->pos().y() << " " << angleX << " " << angleY;
	}
}

void CanvasWidget::wheelEvent(QWheelEvent* event)
{
	qDebug() << "wheelEvent() " << event->delta() ;
}
