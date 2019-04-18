#include "myudp.h"
#include <mainwindow.h>
#include <ui_mainwindow.h>
#include <QDataStream>
 
MyUDP::MyUDP(QObject *parent) :
    QObject(parent)
{
  socket = new QUdpSocket(this);
  socket_send = new QUdpSocket(this);

  //We need to bind the UDP socket to an address and a port
  socket->bind(QHostAddress::Any,5555);         //ex. Address localhost, port 1234

  connect(socket,SIGNAL(readyRead()),this,SLOT(ReadThread()));
  Miss.RecData.setFileName("RecData");
  if(!Miss.RecData.open(QIODevice::WriteOnly))
      qDebug() << "Open RecData fail";
  Miss.MissData.setFileName("MissData.txt");
  if(!Miss.MissData.open(QIODevice::WriteOnly))
      qDebug() << "Open MissData fail";
  Miss.MissData.write("Missing number   Missing index    Missing difference\r\n");
}

void MyUDP::RevResp()      //Just spit out some data
{
  QByteArray Data;
  Data.append((char)0x00);
  Data.append((char)0x88);
  
  socket->writeDatagram(Data,Client.senderIP,Client.senderPort);
  
  //If you want to broadcast something you send it to your broadcast address
  //ex. 192.2.1.255
}


void MyUDP::ReadThread()     //Read something
{
    while(socket->hasPendingDatagrams())
    {
        Buffer.resize(socket->pendingDatagramSize());

        socket->readDatagram(Buffer.data(),Buffer.size(),&Client.senderIP,&Client.senderPort);

        if(!Client.RecFlag && ((int)Buffer[0] & 0xFF) == 0x00 && ((int)Buffer[1] & 0xFF) == 0x88)
        {
            this->RevResp();
            Client.RecFlag = true;
        }
        //for (int i = 0; i < Buffer.size(); i++) {
        //qDebug() << QString("0x%1").arg((ushort)Buffer[i], 0, 16);
        //}
        //The address will be sender.toString()
        else
        {
            //QDataStream out(&Miss.file);
            for(int i = 0; i < Buffer.size(); i = i+4)
            {
                uint tmp = (Buffer[i] & 0xFF) | (Buffer[i+1] & 0xFF) << 8 | (Buffer[i+2] & 0xFF) << 16 | (Buffer[i+3] & 0xFF) << 24;
                if(tmp != 0)
                {
                    if(tmp != Miss.old_data + 1)
                    {
                        Miss.Index[Miss.MissCnt] = Miss.old_data;
                        Miss.Diff[Miss.MissCnt] = tmp - Miss.old_data;
                        char Buf[20];
                        sprintf(Buf, "%d\t\t%d\t\t%d\r\n",Miss.MissCnt, Miss.Index[Miss.MissCnt], Miss.Diff[Miss.MissCnt]);
                        Miss.MissData.write(Buf);
                        Miss.MissCnt++;
                        //qDebug() << "Packet missing";
                        //qDebug() << "cnt : " << Miss.MissCnt;
                        //qDebug() << "Index : " << Miss.Index[Miss.MissCnt] << endl;
                    }
                    //qDebug() <<"tmp = " << tmp << endl;
                    //out<<tmp;
                    Miss.old_data = tmp;
                    Miss.RecData.write(reinterpret_cast<const char*>(&tmp), sizeof (uint));
                }
            }
        }
    }
}
