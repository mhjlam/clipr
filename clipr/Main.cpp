#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>

#include "VideoPlayer.h"

int main(int argc, char* argv[])
{
    QApplication application(argc, argv);

    QCoreApplication::setApplicationName("Video Widget Example");
    QGuiApplication::setApplicationDisplayName(QCoreApplication::applicationName());

    VideoPlayer videoPlayer;
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&videoPlayer);
    videoPlayer.resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
    videoPlayer.move(availableGeometry.center().x() - videoPlayer.size().width() / 2, availableGeometry.center().y() - videoPlayer.size().height() / 2);
    videoPlayer.show();

    return application.exec();
}
