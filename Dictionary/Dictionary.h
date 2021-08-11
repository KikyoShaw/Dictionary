#pragma once

#include <QtWidgets/QWidget>
#include "ui_Dictionary.h"

class Dictionary : public QWidget
{
    Q_OBJECT

public:
    Dictionary(QWidget *parent = Q_NULLPTR);
	~Dictionary();

private slots:
	void sltMaxOrNormal();

private:
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::DictionaryClass ui;
	//Œª÷√“∆∂Ø
	QPoint m_point;
	bool m_bMove = false;
};
