#include "server.h"
#include "containertablemodel.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QVBoxLayout>
#include <QDomDocument>
#include <QRegularExpression>
#include <QHeaderView>

Server::Server(QWidget *parent)
    : QWidget(parent), _tcpServer(new QTcpServer(this))
{
    setWindowTitle("Container Server");
    setMinimumSize(800, 400);

    _statusLabel = new QLabel("Not listening.");
    _startButton = new QPushButton("Start Listening");
    _model = new ContainerTableModel(this);
    _tableView = new QTableView();
    _tableView->setModel(_model);
    _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(_startButton);
    topLayout->addWidget(_statusLabel);
    topLayout->addStretch();
    layout->addLayout(topLayout);
    layout->addWidget(_tableView);

    connect(_startButton, &QPushButton::clicked, this, &Server::startListening);
    connect(_tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);
}

Server::~Server() = default;

void Server::startListening() {
    if (!_tcpServer->listen(QHostAddress::LocalHost, 6164)) {
        _statusLabel->setText("Error: " + _tcpServer->errorString());
        return;
    }
    _statusLabel->setText("Listening on port 6164");
    _startButton->setEnabled(false);
}

void Server::newConnection() {
    QTcpSocket* clientConnection = _tcpServer->nextPendingConnection();
    connect(clientConnection, &QTcpSocket::readyRead, this, &Server::readData);
    connect(clientConnection, &QTcpSocket::disconnected, clientConnection, &QTcpSocket::deleteLater);
    _statusLabel->setText("Client connected.");
}

void Server::readData() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;
    QByteArray data = socket->readAll();
    parseXml(data);
}

void Server::parseXml(const QByteArray& xmlData) {
    QDomDocument doc;
    if (!doc.setContent(xmlData)) {
        return;
    }

    QList<QList<QVariant>> parsedData;
    QRegularExpression codeRegex("^20[0-9]{2}/(0[1-9]|1[0-2])/[BC][1-9][0-9]{0,3}$");

    QDomElement root = doc.documentElement();
    QDomNodeList palletNodes = root.elementsByTagName("pallet");

    for (int i = 0; i < palletNodes.count(); ++i) {
        QDomNode palletNode = palletNodes.at(i);
        QString palletNum = palletNode.toElement().attribute("number");

        QDomNodeList containerNodes = palletNode.childNodes();
        for (int j = 0; j < containerNodes.count(); ++j) {
            QDomElement containerElem = containerNodes.at(j).toElement();
            QString type = containerElem.tagName();
            QString code = containerElem.firstChildElement("code").text();
            QString height = containerElem.firstChildElement("height").text();
            QString weight = containerElem.firstChildElement("weight").text();
            QString breadthOrDiameter = "";
            QString length = "N/A";

            if (type == "Box") {
                breadthOrDiameter = containerElem.firstChildElement("breadth").text();
                length = containerElem.firstChildElement("length").text();
            } else if (type == "Cylinder") {
                breadthOrDiameter = containerElem.firstChildElement("diameter").text();
            }

            QList<QVariant> row;
            row << palletNum << type
                << (codeRegex.match(code).hasMatch() ? code : "****")
                << height << breadthOrDiameter << length << weight;
            parsedData.append(row);
        }
    }
    _model->updateData(parsedData);
    _statusLabel->setText("Data received and updated.");
}
