#include "Dictionary.h"
#include <QMouseEvent>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include "apiManager.h"
#include <QDebug>
#include <QDesktopServices>

Dictionary::Dictionary(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint);

	initPanel();

	connect(ui.pushButton_min, &QPushButton::clicked, this, &Dictionary::showMinimized);
	connect(ui.pushButton_close, &QPushButton::clicked, this, &Dictionary::close);
	connect(ui.pushButton_max, &QPushButton::clicked, this, &Dictionary::sltMaxOrNormal);

	connect(ui.pushButton, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.page);
	});

	connect(ui.pushButton_dictionary, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.page_dictionary);
	});

	connect(ui.pushButton_translate, &QPushButton::clicked, this, [=]() {
		ui.stackedWidget->setCurrentWidget(ui.page_translate);
	});

	//��ѯ
	connect(ui.pushButton_findWord, &QPushButton::clicked, this, &Dictionary::sltFindDictionary);
	//����
	connect(ui.pushButton_translate_t, &QPushButton::clicked, this, &Dictionary::sltTranslate);

	//api�������
	connect(&apiManager, &APIManager::sigRequestDictionaryFinsh, this, &Dictionary::sltRequestDictionaryFinsh);
	connect(&apiManager, &APIManager::sigRequestTranslateFinsh, this, &Dictionary::sltRequestTranslateFinsh);
	connect(&apiManager, &APIManager::sigRequestEveryDayTextFinsh, this, &Dictionary::sltRequestEveryDayTextFinsh);
	connect(&apiManager, &APIManager::sigRequestTtsFinsh, this, &Dictionary::sltRequestTtsFinsh);
	connect(&apiManager, &APIManager::sigRequestPictureFinsh, this, &Dictionary::sltRequestPictureFinsh);
}

Dictionary::~Dictionary()
{
}

void Dictionary::initPanel()
{
	//����ÿ��һ��api
	apiManager.getEveryDayText();

	//����
	auto csdnText = QStringLiteral("CSDN��<a style='color:#01EEC3;text-decoration:none;'href = 'https://blog.csdn.net/qq_36651243'>%1</a>").arg(QStringLiteral("�������"));
	auto githubText = QStringLiteral("GitHub��<a style='color:#01EEC3;text-decoration:none;'href = 'https://github.com/KikyoShaw'>%1</a>").arg(QStringLiteral("�������"));
	ui.label_csdn->setText(csdnText);
	ui.label_github->setText(githubText);
	connect(ui.label_csdn, &QLabel::linkActivated, this, [=](const QString & text) {
		QDesktopServices::openUrl(QUrl(text));
	});
	connect(ui.label_github, &QLabel::linkActivated, this, [=](const QString & text) {
		QDesktopServices::openUrl(QUrl(text));
	});
}

void Dictionary::parseDictionaryJson(const QString & json)
{
	QByteArray byte_array;
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
	if (parse_doucment.isNull()) {
		return;
	}
	if (json_error.error != QJsonParseError::NoError) {
		return;
	}
	ui.textEdit_result->clear();
	if (parse_doucment.isObject()) {
		QJsonObject object = parse_doucment.object();
		QString text = apiManager.getJsonData(object, "query");
		if (!text.isEmpty()) {
			ui.textEdit_result->append(text);
			ui.textEdit_result->append("\n");
		}
		//��������
		if (object.contains("translation")) {
			QJsonValue valueArray = object.value("translation");
			if (valueArray.isArray()) {
				for (auto & each : valueArray.toArray()) {
					QString text = each.toString();
					ui.textEdit_result->append(text);
				}
			}
			ui.textEdit_result->append("\n");
		}
		//��������
		if (object.contains("basic")) {
			QJsonValue basicValue = object.value("basic");
			if (basicValue.isObject()) {
				QJsonObject basicObj = basicValue.toObject();
				QString us = apiManager.getJsonData(basicObj, "us-phonetic");
				QString uk = apiManager.getJsonData(basicObj, "uk-phonetic");
				if (!us.isEmpty() && !uk.isEmpty()) {
					ui.textEdit_result->append(QStringLiteral("��/%1/  Ӣ/%2/").arg(us).arg(uk));
					ui.textEdit_result->append("\n");
				}
				//�������ʺ���
				if (basicObj.contains("explains")) {
					QJsonValue explainsValue = basicObj.value("explains");
					if (explainsValue.isArray()) {
						for (auto & each : explainsValue.toArray()) {
							QString text = each.toString();
							ui.textEdit_result->append(text);
						}
					}
				}
			}
		}
	}
}

void Dictionary::parseTranslateJson(const QString & json)
{
	QByteArray byte_array;
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
	if (parse_doucment.isNull()) {
		return;
	}
	if (json_error.error != QJsonParseError::NoError) {
		return;
	}
	ui.textEdit_result_translate->clear();
	if (parse_doucment.isObject()) {
		QJsonObject object = parse_doucment.object();
		QString text = apiManager.getJsonData(object, "query");
		if (!text.isEmpty()) {
			ui.textEdit_result_translate->append(text);
			ui.textEdit_result_translate->append("\n");
		}
		//��������
		if (object.contains("translation")) {
			QJsonValue valueArray = object.value("translation");
			if (valueArray.isArray()) {
				for (auto & each : valueArray.toArray()) {
					QString text = each.toString();
					ui.textEdit_result_translate->append(text);
				}
			}
		}
	}
}

void Dictionary::parseEveryDayTextJson(const QString & json)
{
	QByteArray byte_array;
	QJsonParseError json_error;
	QJsonDocument parse_doucment = QJsonDocument::fromJson(byte_array.append(json), &json_error);
	if (parse_doucment.isNull()) {
		return;
	}
	if (json_error.error != QJsonParseError::NoError) {
		return;
	}

	ui.textEdit_day->clear();
	if (parse_doucment.isObject()) {
		QJsonObject object = parse_doucment.object();
		auto tts = apiManager.getJsonData(object, "tts");
		auto picture = apiManager.getJsonData(object, "picture");
		apiManager.downLoadImage(tts, picture);
		auto content = apiManager.getJsonData(object, "content");
		if (!content.isEmpty()) {
			ui.textEdit_day->append(content);
			ui.textEdit_day->append("\n");
		}
		auto note = apiManager.getJsonData(object, "note");
		if (!note.isEmpty()) {
			ui.textEdit_day->append(note);
		}
	}

}

void Dictionary::sltMaxOrNormal()
{
	if (isMaximized())
		showNormal();
	else
		showMaximized();
}

void Dictionary::sltFindDictionary()
{
	auto word = ui.textEdit->toPlainText();
	if (word.isEmpty())
		return;
	apiManager.findDictionary(word);
}

void Dictionary::sltTranslate()
{
	auto text = ui.textEdit_translate->toPlainText();
	if (text.isEmpty())
		return;
	apiManager.translate(text);
}

void Dictionary::sltRequestDictionaryFinsh(QNetworkReply * reply)
{
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		QString result(bytes);
		qInfo() << "1111";
		parseDictionaryJson(result);
	}
}

void Dictionary::sltRequestTranslateFinsh(QNetworkReply * reply)
{
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		QString result(bytes);
		parseTranslateJson(result);
	}
}

void Dictionary::sltRequestEveryDayTextFinsh(QNetworkReply * reply)
{
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		QString result(bytes);
		parseEveryDayTextJson(result);
	}
}

void Dictionary::sltRequestTtsFinsh(QNetworkReply * reply)
{
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		QPixmap pixmap = QPixmap();
		pixmap.loadFromData(bytes);
		if (!pixmap.isNull()) {
			pixmap = apiManager.getRoundRectPixmap(pixmap, QSize(600, 182), 6);
		}
		if (!pixmap.isNull()) {
			ui.label_image->setPixmap(pixmap);
		}
	}
}

void Dictionary::sltRequestPictureFinsh(QNetworkReply * reply)
{
	//��ȡ��Ӧ����Ϣ��״̬��Ϊ200��ʾ����
	QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray bytes = reply->readAll();  //��ȡ�ֽ�
		QPixmap pixmap = QPixmap();
		pixmap.loadFromData(bytes);
		if (!pixmap.isNull()) {
			pixmap = apiManager.getRoundRectPixmap(pixmap, QSize(260, 160), 6);
		}
		if (!pixmap.isNull()) {
			ui.label_icon->setPixmap(pixmap);
		}
	}
}

void Dictionary::mouseMoveEvent(QMouseEvent * event)
{
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);//�ƶ�����
	}
	QWidget::mouseMoveEvent(event);
}

void Dictionary::mousePressEvent(QMouseEvent * event)
{
	//����¼��а�������ס���������
	if ((event->button() == Qt::LeftButton))
	{
		m_bMove = true;
		//��ȡ�ƶ���λ����
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void Dictionary::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}
