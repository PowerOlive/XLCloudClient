#include "mediaplayer.h"

MediaPlayer *mediaPlayer = 0;

MediaPlayer::MediaPlayer(QObject *parent) :
    QObject(parent),
    mediaObject(new Phonon::MediaObject(this)),
    audioOutput(new Phonon::AudioOutput(Phonon::VideoCategory, this))
{
    mediaObject->setTransitionTime(1);

    Phonon::createPath(mediaObject, audioOutput);
    audioOutput->setVolume(100);
}

MediaPlayer::~MediaPlayer()
{
}

void MediaPlayer::stop()
{
    mediaObject->stop();
}

bool MediaPlayer::isPlaying()
{
    return mediaObject->state() == Phonon::PlayingState;
}

void MediaPlayer::play(const QString & filePath)
{
#if QT_VERSION > QT_VERSION_CHECK(4, 8, 4)
    mediaObject->setCurrentSource(QUrl::fromLocalFile(filePath));
#else
    mediaObject->setCurrentSource (Phonon::MediaSource (filePath));
#endif
    mediaObject->play();
}

void MediaPlayer::setVolume(int vol)
{
    audioOutput->setVolume(vol);
}
