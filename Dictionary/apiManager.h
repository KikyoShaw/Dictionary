#pragma once

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

enum WorkType
{
	E_Dictionary = 1, //查询
	E_Translate = 2, //翻译
	E_EveryDayText = 3, //每日一句
	E_tts = 4, //封面
	E_Picture = 5, //logo
};

class APIManager : public QObject
{
	Q_OBJECT
public:
	static APIManager& getInstance();
	~APIManager();

	//查询
	void findDictionary(const QString& word);

	//翻译
	void translate(const QString& text);

	//每天一句
	void getEveryDayText();

	//下载图片
	void downLoadImage(const QString& tts, const QString& picture);

	void downLoadImage(const QString& picture);

	//获取json数据
	QString getJsonData(QJsonObject Object, const QString& key);

	//图片圆角化处理
	QPixmap getRoundRectPixmap(QPixmap srcPixMap, const QSize & size, int radius);

private slots:
	void sltRequestFinsh(QNetworkReply * reply);

signals:
	void sigRequestDictionaryFinsh(QNetworkReply * reply);
	void sigRequestTranslateFinsh(QNetworkReply * reply);
	void sigRequestEveryDayTextFinsh(QNetworkReply * reply);
	void sigRequestTtsFinsh(QNetworkReply * reply);
	void sigRequestPictureFinsh(QNetworkReply * reply);

private:
	//避免数据重复请求
	bool m_isWorking;
	//网络组件
	QNetworkAccessManager *m_netWork = nullptr;
	//当前工作
	WorkType m_workType;
	//记录url
	QString m_picture;

private:
	APIManager(QObject *parent = Q_NULLPTR);
	Q_DISABLE_COPY(APIManager)

};

#define apiManager APIManager::getInstance()
