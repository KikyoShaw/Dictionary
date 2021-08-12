#pragma once

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

enum WorkType
{
	E_Dictionary = 1, //��ѯ
	E_Translate = 2, //����
	E_EveryDayText = 3, //ÿ��һ��
	E_tts = 4, //����
	E_Picture = 5, //logo
};

class APIManager : public QObject
{
	Q_OBJECT
public:
	static APIManager& getInstance();
	~APIManager();

	//��ѯ
	void findDictionary(const QString& word);

	//����
	void translate(const QString& text);

	//ÿ��һ��
	void getEveryDayText();

	//����ͼƬ
	void downLoadImage(const QString& tts, const QString& picture);

	void downLoadImage(const QString& picture);

	//��ȡjson����
	QString getJsonData(QJsonObject Object, const QString& key);

	//ͼƬԲ�ǻ�����
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
	//���������ظ�����
	bool m_isWorking;
	//�������
	QNetworkAccessManager *m_netWork = nullptr;
	//��ǰ����
	WorkType m_workType;
	//��¼url
	QString m_picture;

private:
	APIManager(QObject *parent = Q_NULLPTR);
	Q_DISABLE_COPY(APIManager)

};

#define apiManager APIManager::getInstance()
