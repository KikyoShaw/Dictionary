#include "Dictionary.h"
#include <QMouseEvent>

Dictionary::Dictionary(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);

	connect(ui.pushButton_min, &QPushButton::clicked, this, &Dictionary::showMinimized);
	connect(ui.pushButton_close, &QPushButton::clicked, this, &Dictionary::close);
	connect(ui.pushButton_max, &QPushButton::clicked, this, &Dictionary::sltMaxOrNormal);

	connect(ui.pushButton_dictionary, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.page_dictionary);
	});

}

Dictionary::~Dictionary()
{
}

void Dictionary::sltMaxOrNormal()
{
	if (isMaximized())
		showNormal();
	else
		showMaximized();
}

void Dictionary::mouseMoveEvent(QMouseEvent * event)
{
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);//移动窗口
	}
	QWidget::mouseMoveEvent(event);
}

void Dictionary::mousePressEvent(QMouseEvent * event)
{
	//鼠标事件中包含“按住的是左键”
	if ((event->button() == Qt::LeftButton))
	{
		m_bMove = true;
		//获取移动的位移量
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void Dictionary::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}
