#include "apiManager.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QPixmap>
#include <QPainter>
#include <QNetworkConfiguration>

APIManager & APIManager::getInstance()
{
	static APIManager m_instance;
	return m_instance;
}

APIManager::APIManager(QObject *parent)
	:QObject(parent), m_isWorking(false), m_picture(QString())
{
	m_netWork = new QNetworkAccessManager();
	m_netWork->activeConfiguration();
	connect(m_netWork, &QNetworkAccessManager::finished, this, &APIManager::sltRequestFinsh, Qt::DirectConnection);
}

APIManager::~APIManager()
{
}

void APIManager::findDictionary(const QString & word)
{
	/*if (m_isWorking)
		return;
	m_isWorking = true;*/
	m_workType = E_Dictionary;
	QString url = QString("http://fanyi.youdao.com/openapi.do?keyfrom=YouDaoCV&key=659600698&type=data&doctype=json&version=1.1" \
					"&q=%1").arg(word);
	QNetworkRequest request;
	request.setUrl(QUrl(url));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWork->get(request);
}

void APIManager::translate(const QString & text)
{
	/*if (m_isWorking)
		return;
	m_isWorking = true;*/
	m_workType = E_Translate;
	QString url = QString("http://fanyi.youdao.com/openapi.do?keyfrom=YouDaoCV&key=659600698&type=data&doctype=json&version=1.1&only=translate" \
		"&q=%1").arg(text);
	QNetworkRequest request;
	request.setUrl(QUrl(url));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWork->get(request);
}

void APIManager::getEveryDayText()
{
	/*if (m_isWorking)
		return;
	m_isWorking = true;*/
	m_workType = E_EveryDayText;
	QString url = QString("http://open.iciba.com/dsapi/");
	QNetworkRequest request;
	request.setUrl(QUrl(url));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWork->get(request);
}

void APIManager::downLoadImage(const QString & tts, const QString & picture)
{
	if (tts.isEmpty())
		return;
	/*if (m_isWorking)
		return;
	m_isWorking = true;*/
	m_picture = picture;
	m_workType = E_tts;
	QNetworkRequest request;
	request.setUrl(QUrl(tts));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWork->get(request);
}

void APIManager::downLoadImage(const QString & picture)
{
	if (picture.isEmpty())
		return;
	/*if (m_isWorking)
		return;
	m_isWorking = true;*/
	m_workType = E_Picture;
	QNetworkRequest request;
	request.setUrl(QUrl(picture));
	request.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
	m_netWork->get(request);
}

QString APIManager::getJsonData(QJsonObject Object, const QString & key)
{
	if (Object.contains(key)) {
		QJsonValue value = Object.take(key);
		if (value.isString()) {
			return value.toString();
		}
	}
	return QString();
}

QPixmap APIManager::getRoundRectPixmap(QPixmap srcPixMap, const QSize & size, int radius)
{
	//不处理空数据或者错误数据
	if (srcPixMap.isNull()) {
		return srcPixMap;
	}

	//获取图片尺寸
	int imageWidth = size.width();
	int imageHeight = size.height();

	//处理大尺寸的图片,保证图片显示区域完整
	QPixmap newPixMap = srcPixMap.scaled(imageWidth, (imageHeight == 0 ? imageWidth : imageHeight), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPixmap destImage(imageWidth, imageHeight);
	destImage.fill(Qt::transparent);
	QPainter painter(&destImage);
	// 抗锯齿
	painter.setRenderHints(QPainter::Antialiasing, true);
	// 图片平滑处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	// 将图片裁剪为圆角
	QPainterPath path;
	QRect rect(0, 0, imageWidth, imageHeight);
	path.addRoundedRect(rect, radius, radius);
	painter.setClipPath(path);
	painter.drawPixmap(0, 0, imageWidth, imageHeight, newPixMap);
	return destImage;
}

void APIManager::sltRequestFinsh(QNetworkReply * reply)
{
	m_isWorking = false;
	switch (m_workType)
	{
	case E_Dictionary: {
		emit sigRequestDictionaryFinsh(reply);
		break;
	}
	case E_Translate: {
		emit sigRequestTranslateFinsh(reply);
		break;
	}
	case E_EveryDayText: {
		emit sigRequestEveryDayTextFinsh(reply);
		break;
	}
	case E_tts: {
		emit sigRequestTtsFinsh(reply);
		downLoadImage(m_picture);
		break;
	}
	case E_Picture: {
		emit sigRequestPictureFinsh(reply);
		break;
	}
	default:
		break;
	}
}
