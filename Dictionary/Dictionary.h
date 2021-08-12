#pragma once

#include <QtWidgets/QWidget>
#include "ui_Dictionary.h"

class QNetworkReply;

class Dictionary : public QWidget
{
    Q_OBJECT

public:
    Dictionary(QWidget *parent = Q_NULLPTR);
	~Dictionary();

private:
	void initPanel();
	void parseDictionaryJson(const QString& json);
	void parseTranslateJson(const QString& json);
	void parseEveryDayTextJson(const QString& json);

private slots:
	void sltMaxOrNormal();
	void sltFindDictionary();
	void sltTranslate();

	void sltRequestDictionaryFinsh(QNetworkReply * reply);
	void sltRequestTranslateFinsh(QNetworkReply * reply);
	void sltRequestEveryDayTextFinsh(QNetworkReply * reply);
	void sltRequestTtsFinsh(QNetworkReply * reply);
	void sltRequestPictureFinsh(QNetworkReply * reply);

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
