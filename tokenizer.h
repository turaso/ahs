#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QObject>

#include <QString>
#include <QNetworkRequest>
#include <QWidget>

class Tokenizer : public QObject
{
    Q_OBJECT
    static QString token_;
public:
    void setToken(QString t)
    {
        token_ = t;
        emit this->tokenChanged();
    }
    void setToken(QString t, QString r)
    {
        token_ = t;
        role = r;
        emit this->tokenChanged();
    }
    static void resetToken()
    {
        token_ = "";
    }
    static QNetworkRequest& tokenize(QNetworkRequest& req)
    {
        req.setRawHeader("Authorization", token_.toUtf8());
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        return req;
    }
    static QString role;
    Tokenizer() {};
    ~Tokenizer() {};
private:
    Tokenizer(const Tokenizer&) = delete;
    Tokenizer& operator=(const Tokenizer&) = delete;

signals:
    void tokenChanged();
};

#endif // TOKENIZER_H
