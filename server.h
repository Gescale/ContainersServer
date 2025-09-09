#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
class QTcpServer;
class QLabel;
class QPushButton;
class QTableView;
class ContainerTableModel;

class Server : public QWidget {
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();

private slots:
    void startListening();
    void newConnection();
    void readData();

private:
    void parseXml(const QByteArray& xmlData);

    QTcpServer* _tcpServer;
    QLabel* _statusLabel;
    QPushButton* _startButton;
    QTableView* _tableView;
    ContainerTableModel* _model;
};

#endif // SERVER_H
