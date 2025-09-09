#include "server.h"
#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QTimer>
#include <QScreen>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Resources/images/logo.png"));

    QPixmap pixmap(":/Resources/images/splashscreen.png");
    if (pixmap.isNull()) {
        qWarning("Failed to load splashscreen image");
        return -1;
    }

    QSplashScreen splash(pixmap);

    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->geometry();
        int screenWidth = screenGeometry.width();
        int screenHeight = screenGeometry.height();

        int splashWidth = screenWidth * 0.75;
        int splashHeight = screenHeight * 0.75;

        QPixmap scaledPixmap = pixmap.scaled(splashWidth, splashHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        splash.setPixmap(scaledPixmap);

        splash.move((screenWidth - scaledPixmap.width()) / 2, (screenHeight - scaledPixmap.height()) / 2);
    }

    splash.show();
    splash.showMessage("Loading...", Qt::AlignBottom | Qt::AlignCenter, Qt::white);
    QTimer::singleShot(1500, &splash, &QWidget::close);

    Server s;
    s.show();
    return a.exec();
}
