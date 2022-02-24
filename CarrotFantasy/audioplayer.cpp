#include "audioplayer.h"
#include <QDir>
#include <QMediaPlayer>
#include <QMediaPlaylist>

static const QString s_curDir = QDir::currentPath() + "/";

AudioPlayer::AudioPlayer(QObject *parent)
	: QObject(parent)
	, m_backgroundMusic(NULL)
{
	// 创建一直播放的背景音乐
    QUrl backgroundMusicUrl = QUrl::fromLocalFile("C:/cc/CarrotFantasy/music/back.mp3");//解析读文件
	if (QFile::exists(backgroundMusicUrl.toLocalFile()))
	{
		m_backgroundMusic = new QMediaPlayer(this);
        QMediaPlaylist *backgroundMusicList = new QMediaPlaylist();//创建播放列表

        QMediaContent media(backgroundMusicUrl);//设置接收读取
		backgroundMusicList->addMedia(media);
		backgroundMusicList->setCurrentIndex(0);
		// 设置背景音乐循环播放
		backgroundMusicList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
		m_backgroundMusic->setPlaylist(backgroundMusicList);
	}
}

void AudioPlayer::startBGM()
{
	if (m_backgroundMusic)
		m_backgroundMusic->play();
}

void AudioPlayer::playSound(SoundType soundType)
{
	static const QUrl mediasUrls[] =
	{
        QUrl::fromLocalFile("C:/cc/CarrotFantasy/music/tower_place.wav"),
        QUrl::fromLocalFile("C:/cc/CarrotFantasy/music/life_lose.wav"),
        QUrl::fromLocalFile("C:/cc/CarrotFantasy/music/laser_shoot.wav"),
        QUrl::fromLocalFile("C:/cc/CarrotFantasy/music/enemy_destroy.wav")
	};
	static QMediaPlayer player;

	if (QFile::exists(mediasUrls[soundType].toLocalFile()))
	{
		player.setMedia(mediasUrls[soundType]);
		player.play();
	}
}
void AudioPlayer::endBGM()
{
    if (m_backgroundMusic)
            m_backgroundMusic->stop();
}
