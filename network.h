#ifndef NETWORK_H
#define NETWORK_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

class Network
{
public:
    static QNetworkAccessManager& instance()
    {
        static QNetworkAccessManager net;
        return net;
    }
private:
    Network() = delete;
    ~Network() = delete;
    Network(const Network&) = delete;
    Network& operator=(const Network&) = delete;
};

#endif // NETWORK_H
