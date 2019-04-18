//Base class QObject

#include <QUdpSocket>
#include <QFile>
#include <QDataStream>

class MyUDP : public QObject
{
  Q_OBJECT

  public:
      explicit MyUDP(QObject *parent = 0);

      void RevResp();

  private:
      QUdpSocket *socket;
      QByteArray Buffer;
      QUdpSocket *socket_send;

      struct Client
      {
          bool RecFlag;
          quint16 senderPort;
          QHostAddress senderIP;
      };

      Client Client = {0,0,QHostAddress::Any};

      struct Missing
      {
          uint Index[100];
          uint Diff[100];
          uint old_data;
          uint MissCnt;
          QFile RecData;
          QFile MissData;
      };

      Missing Miss = {{0},{0},0,0};


  signals:

  public slots:
      void ReadThread();

};
