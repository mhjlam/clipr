#include <QtWidgets>
#include <QVideoWidget>

#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(QWidget* parent) : QWidget(parent)
{
    this->mediaPlayer = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
    QVideoWidget* videoWidget = new QVideoWidget;

    // create menu bar
    QMenuBar* menuBar = new QMenuBar();
    QMenu* fileMenu = menuBar->addMenu("&File");
    QAction* openAction = fileMenu->addAction("&Open");
    QAction* exitAction = fileMenu->addAction("E&xit");
    QMenu* windowMenu = menuBar->addMenu("&Window");
    QAction* fullscreenAction = windowMenu->addAction("&Fullscreen");

    this->playButton = new QToolButton;
    this->playButton->setEnabled(false);
    this->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    this->stopButton = new QToolButton;
    this->stopButton->setEnabled(false);
    this->stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

    this->positionSlider = new QSlider(Qt::Horizontal);
    this->positionSlider->setRange(0, 0);

    this->muteButton = new QToolButton;
    this->muteButton->setEnabled(false);
    this->muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    this->volumeSlider = new QSlider(Qt::Horizontal);
    this->volumeSlider->setRange(0, 100);
    this->volumeSlider->setFixedWidth(100);
    this->volumeSlider->setValue(50);

    this->errorLabel = new QLabel;
    this->errorLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

    QBoxLayout* controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->setAlignment(Qt::AlignLeft);
    controlLayout->addWidget(this->stopButton);
    controlLayout->addWidget(this->playButton);
    controlLayout->addWidget(this->positionSlider);
    controlLayout->addWidget(this->muteButton);
    controlLayout->addWidget(this->volumeSlider);

    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(1);

    QBoxLayout* layout = new QVBoxLayout;
    layout->setMenuBar(menuBar);
    layout->addWidget(videoWidget);
    layout->addWidget(line);
    layout->addLayout(controlLayout);
    layout->addWidget(this->errorLabel);

    setLayout(layout);

    this->mediaPlayer->setVideoOutput(videoWidget);

    connect(openAction, &QAction::triggered, this, &VideoPlayer::openFile);
    connect(exitAction, &QAction::triggered, this, &QApplication::quit);

    //connect(fullscreenAction, &QAction::triggered, this, [this]() { this->mediaPlayer->setFullscreen(); });

    connect(this->stopButton, &QAbstractButton::clicked, this, [this]() { this->mediaPlayer->stop(); });
    connect(this->playButton, &QAbstractButton::clicked, this, [this]() { this->mediaPlayer->state() == QMediaPlayer::PlayingState ? this->mediaPlayer->pause() : this->mediaPlayer->play(); });
    connect(this->muteButton, &QAbstractButton::clicked, this, [this](bool muted) { this->mediaPlayer->setMuted(muted); });
    connect(this->volumeSlider, &QSlider::valueChanged, this, [this](int volume) { this->mediaPlayer->setVolume(volume); });
    connect(this->positionSlider, &QAbstractSlider::sliderMoved, this, [this](int position) {this->mediaPlayer->setPosition(position); });

    connect(this->mediaPlayer, &QMediaPlayer::positionChanged, this, [this](qint64 position) { this->positionSlider->setValue(position); });
    connect(this->mediaPlayer, &QMediaPlayer::durationChanged, this, [this](qint64 duration) { this->positionSlider->setRange(0, duration); });
    connect(this->mediaPlayer, &QMediaPlayer::mutedChanged, this, [this](bool muted) { this->muteButton->setIcon(style()->standardIcon(muted ? QStyle::SP_MediaVolumeMuted : QStyle::SP_MediaVolume)); });
    connect(this->mediaPlayer, &QMediaPlayer::stateChanged, this, [this]() { this->playButton->setIcon(style()->standardIcon(this->mediaPlayer->state() == QMediaPlayer::PlayingState ? QStyle::SP_MediaPause : QStyle::SP_MediaPlay)); });

    connect(this->mediaPlayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), this, &VideoPlayer::handleError);
}

void VideoPlayer::openFile()
{
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setWindowTitle(tr("Open Movie"));

    QStringList supportedMimeTypes = this->mediaPlayer->supportedMimeTypes();

    if (!supportedMimeTypes.isEmpty())
    {
        fileDialog.setMimeTypeFilters(supportedMimeTypes);
    }

    if (windowFilePath().isEmpty() || !QFileInfo::exists(windowFilePath()))
    {
        fileDialog.setDirectory(QStandardPaths::standardLocations(QStandardPaths::MoviesLocation).value(0, QDir::homePath()));
    }

    if (fileDialog.exec() == QDialog::Accepted)
    {
        QUrl url = fileDialog.selectedUrls().first();

        this->errorLabel->setText(QString());
        setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());

        this->stopButton->setEnabled(true);
        this->playButton->setEnabled(true);
        this->muteButton->setEnabled(true);

        this->mediaPlayer->setMedia(url);
        this->mediaPlayer->play();
    }
}

void VideoPlayer::handleError()
{
    this->playButton->setEnabled(false);

    if (this->mediaPlayer->errorString().isEmpty())
    {
        this->errorLabel->setText("Error: #" + QString::number(int(this->mediaPlayer->error())));
    }
    else
    {
        this->errorLabel->setText("Error: " + this->mediaPlayer->errorString());
    }
}
