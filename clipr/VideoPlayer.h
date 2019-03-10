#pragma once

#include <QWidget>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QSlider;
class QLabel;
class QUrl;
QT_END_NAMESPACE

class VideoPlayer : public QWidget
{
    Q_OBJECT

public:
    VideoPlayer(QWidget* parent = nullptr);

public slots:
    void openFile();

private slots:
    void handleError();

private:
    bool muted;

    QMediaPlayer* mediaPlayer;

    QAbstractButton* playButton;
    QAbstractButton* stopButton;
    QAbstractButton* muteButton;

    QSlider* volumeSlider;
    QSlider* positionSlider;

    QLabel* errorLabel;
};
