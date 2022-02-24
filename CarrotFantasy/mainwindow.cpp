#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "waypoint.h"
#include "enemy.h"
#include "bullet.h"
#include "audioplayer.h"
#include "plistreader.h"
#include "windows.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtGlobal>
#include <QMessageBox>
#include <QTimer>
#include <QXmlStreamReader>
#include <QtDebug>
#include <QIcon>
#include <QPixmap>
#include <QTime>
#include <QApplication>
#include <QProcess>
#include <QSplashScreen>
#include <QGraphicsWidget>
#include <QWidget>
#include <QScreen>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDir>
#include <QVector>
#include <iostream>
#include <QMessageBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGuiApplication>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QList>
#include <QPalette>
#include <QLabel>
#include <QMediaPlayer>
#include <QGridLayout>
#include <QString>
#include <QDesktopServices>
#include <QUrl>
#include "mybutton.h"
QFont font1("Times New Roman",20,QFont::Bold,false);
static const int TowerCost = 300;
static int cnt=2;
static int _level=0;
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
	, m_waves(0)
    , m_playerHp(5)
    , m_playerGold(1000)
	, m_gameEnded(false)
	, m_gameWin(false)
{
    setMouseTracking(true);

    restart_flag=0;
    ui->setupUi(this);
    ui->centralWidget->setMouseTracking(true);
    this->setWindowTitle(QStringLiteral("保卫萝卜"));
    this->setWindowIcon(QIcon(":/image/0.jpg"));
    this->setMinimumSize(1440,840);
    this->setMaximumSize(1440,840);
    preLoadWavesInfo(1);
    loadTowerPositions123();
    //loadTowerPositions4();
    addWayPoints123();
    //addWayPoints4();
    m_audioPlayer = new AudioPlayer(this);
    m_audioPlayer->startBGM();
    this->pauseButton = new QPushButton(QStringLiteral("暂停"),this);
    this->pauseButton->move(9,50);
    pauseButton->setStyleSheet(tr("background-image: url(:/image/07.PNG);"));
    QObject::connect(this->pauseButton,SIGNAL(clicked(bool)),this,SLOT(pause()));
    this->exitButton_1 = new QPushButton(QStringLiteral("结束"),this);
    this->exitButton_1->move(9,287);
    exitButton_1->setStyleSheet(tr("background-image: url(:/image/07.PNG);"));
    QObject::connect(this->exitButton_1,SIGNAL(clicked(bool)),this,SLOT(close()));
    this->replay = new QPushButton(QStringLiteral("重玩"),this);
    this->replay->move(850,647);
    replay->setStyleSheet(tr("background-image: url(:/image/07.PNG);"));
    QObject::connect(this->replay,SIGNAL(clicked(bool)),this,SLOT(gamereStart()));
    this->screenShotButton = new QPushButton(QStringLiteral("截图"),this);
    this->screenShotButton->move(1090,647);
    screenShotButton->setStyleSheet(tr("background-image: url(:/image/07.PNG);"));
    QObject::connect(this->screenShotButton,SIGNAL(clicked(bool)),this,SLOT(screenShotSlot()));
    QTimer *timer = new QTimer(this);
    m_timer = timer;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
    this->StartButton = new QPushButton(QStringLiteral("继续"),this);
    StartButton->move(9,50);
    StartButton->setStyleSheet(tr("background-image: url(:/image/07.PNG);"));
    QObject::connect(StartButton,SIGNAL(clicked(bool)),m_timer,SLOT(start()));
    StartButton->hide();
    this->damageskill = new mybutton(":/image/fireball2.png",":/image/fireball.png","",this);
    this->damageskill->move(1300,400);
    QObject::connect(damageskill,SIGNAL(clicked(bool)),this,SLOT(fireball()));
    damageskill->hide();
    this->slowdown = new mybutton(":/image/freeze2.png",":/image/freeze1.png","",this);
    this->slowdown->move(1300,260);
    QObject::connect(slowdown,SIGNAL(clicked(bool)),this,SLOT(freeze()));
    slowdown->hide();
    this->startMenu();

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::fireball()
{
    foreach (Enemy *enemy, m_enemyList)
    {
        (enemy->m_currentHp)/=2;
    }
    getHpDamage(1);
}

void MainWindow::freeze()
{
    foreach (Enemy *enemy, m_enemyList)
    {
        (enemy->m_walkingSpeed)/=2.0;
    }
    getHpDamage(1);
}

void MainWindow::pause()
{
    StartButton->show();
    m_timer->stop();
    QObject::connect(StartButton,SIGNAL(clicked(bool)),StartButton,SLOT(hide()));
}

void MainWindow::startMenu()
{
    option_flag=0;
    if(!restart_flag)
    {
        QSplashScreen* splash1 = new QSplashScreen;
        QSplashScreen* splash2= new QSplashScreen;
        QSplashScreen* splash3 = new QSplashScreen;
        splash1->setPixmap(QPixmap(":/image/3.jpg"));
            splash1->show();
            Sleep(1000);
            splash2->setPixmap(QPixmap(":/image/4.jpg"));
            splash2->show();
            Sleep(1000);
            splash3->setPixmap(QPixmap(":/image/t.jpg"));
            splash3->show();
            Sleep(1000);
            delete splash1;
            delete splash2;
            delete splash3;
    }

    this->pauseButton->hide();
    this->exitButton_1->hide();
    this->replay->hide();
    this->screenShotButton->hide();
    this->damageskill->hide();
    this->slowdown->hide();
    this->startButton = new QPushButton("开始",this);
    this->optionButton = new QPushButton("选关",this);
    this->helpButton = new QPushButton("帮助",this);
    this->exitButton = new QPushButton("退出",this);

    this->startButton->setGeometry(660,280,100,50);
    startButton->setStyleSheet(tr("background-image: url(:/image/06.PNG);"));
    this->optionButton->setGeometry(660,345,100,50);
    optionButton->setStyleSheet(tr("background-image: url(:/image/06.PNG);"));
    this->helpButton->setGeometry(660,410,100,50);
    helpButton->setStyleSheet(tr("background-image: url(:/image/06.PNG);"));
     this->exitButton->setGeometry(660,475,100,50);
    exitButton->setStyleSheet(tr("background-image: url(:/image/06.PNG);"));
    start_flag=1;
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(gameStart()));
    connect(optionButton, SIGNAL(clicked(bool)), this, SLOT(change_interface()));
    connect(helpButton,SIGNAL(clicked(bool)),this,SLOT(help()));
    QObject::connect(exitButton,SIGNAL(clicked(bool)),this,SLOT(close()));

    return;
    // 设置300ms后游戏启动
}
void MainWindow::loadTowerPositions123()
{
    QFile file;
    if(_level!=4)
        file.setFileName(":/config/TowersPosition.plist");
    else {
        file.setFileName(":/config/TowersPosition4.plist");
    }
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
		return;
	}

	PListReader reader;
	reader.read(&file);

	QList<QVariant> array = reader.data();
	foreach (QVariant dict, array)
	{
		QMap<QString, QVariant> point = dict.toMap();
		int x = point.value("x").toInt();
		int y = point.value("y").toInt();
		m_towerPositionsList.push_back(QPoint(x, y));
	}

	file.close();
}
void MainWindow::loadTowerPositions4()
{
    QFile file(":/config/TowersPosition.plist");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }

    PListReader reader;
    reader.read(&file);

    QList<QVariant> array = reader.data();
    foreach (QVariant dict, array)
    {
        QMap<QString, QVariant> point = dict.toMap();
        int x = point.value("x").toInt();
        int y = point.value("y").toInt();
        m_towerPositionsList.push_back(QPoint(x, y));
    }

    file.close();
}
void MainWindow::help()
{
    QDesktopServices::openUrl(QUrl(QString("https://blog.csdn.net/weixin_44651434/article/details/91045822")));
}
void MainWindow::paintEvent(QPaintEvent *)
{
    if(option_flag)
        {
            QPixmap level1(":/image/level1.png");
            QPixmap level2(":/image/level2.png");
            QPixmap level3(":/image/level3.png");
            QPixmap level4(":/image/map4.jpg");
            QPixmap img = level4.scaled(720,420);
            QPainter levelpainter(this);
            levelpainter.drawPixmap(0,0,level1);
            levelpainter.drawPixmap(720,0,level2);
            levelpainter.drawPixmap(0,420,level3);
            levelpainter.drawPixmap(720,420,img);
            return;
        }
    if(start_flag)
    {
        QPixmap start_picture(":/image/screen.jpg");
        QPainter startpainter(this);
        QPixmap img=start_picture.scaled(1440,840);
        startpainter.drawPixmap(0,0,img);
        this->startButton ->show();
        this->optionButton ->show();
        this->helpButton ->show();
        this->exitButton ->show();
        if(restart_flag)
                {
                    m_waves=0;
                    m_playerGold=1000;
                    m_wavesInfo.clear();
                    m_towerPositionsList.clear();
                    m_towersList.clear();
                    m_wayPointsList.clear();
                    m_enemyList.clear();
                    m_bulletList.clear();
                    preLoadWavesInfo(1);
                    loadTowerPositions123();
                    //loadTowerPosition4();
                    addWayPoints123();
                    //addWayPoints4();
                    m_gameEnded=0;
                    m_gameWin=0;
                    m_playerHp=1;
                }

        return;
    }
	if (m_gameEnded || m_gameWin)
	{
        this->pauseButton->hide();
        this->exitButton_1->hide();
        this->replay->hide();
        this->screenShotButton->hide();
        this->damageskill->hide();
        this->slowdown->hide();
        QPainter painter(this);
        QPixmap pix;
        if(m_gameWin)
        {

            pix.load(":/image/25.jpg");
            painter.drawPixmap(0,0,1440,840,pix);
        }
        if(m_gameEnded)
        {
            pix.load(":/image/fail.jpg");
            painter.drawPixmap(0,0,1440,840,pix);
        }

       return ;
    }

    QPixmap cachePix;
    if(_level!=4){
        cachePix.load(":/image/Bg.png");
    }
    else{
        cachePix.load(":/image/Bg2.png");
    }
        QPainter cachePainter(&cachePix);
        foreach (const Tower *tower, m_towersList)
            tower->draw(&cachePainter);
        //draw the towers
        foreach (const WayPoint *wayPoint, m_wayPointsList)
            wayPoint->draw(&cachePainter);
        //draw the waypoints
        foreach (const Enemy *enemy, m_enemyList)
            enemy->draw(&cachePainter);
        //draw the enemies
        foreach (const Bullet *bullet, m_bulletList)
            bullet->draw(&cachePainter);
        //draw the bullets
        QPainter painter(this);
        painter.drawPixmap(0, 0, cachePix);
        QPixmap Pix1;
        QPixmap Pix2;
        if(flag==1)
        {
            if(m_playerGold<220)
                Pix1=QPixmap(":/image/tower1_nopayment.png");
            else
                Pix1=QPixmap(":/image/tower1_payment.png");
            if(m_playerGold<260)
                Pix2=QPixmap(":/image/tower2_nopayment.png");
            else
                Pix2=QPixmap(":/image/tower2_payment.png");
            QPainter mypainter(this);
            int x=temp.rx();
            int y=temp.ry();
            mypainter.drawPixmap(x-110,y-120,Pix1);
            mypainter.drawPixmap(x+50,y-120,Pix2);
        }
        else if(flag==2)
        {
            if(m_playerGold<100)
                Pix1=QPixmap(":/image/noupgrade.png");
            else
                Pix1=QPixmap(":/image/upgrade.png");

            Pix2=QPixmap(":/image/sell.png");

            QPainter mypainter(this);
            int x=temp.rx();
            int y=temp.ry();
            mypainter.drawPixmap(x-90,y-90,Pix1);
            mypainter.drawPixmap(x+30,y-90,Pix2);
        }
        else
        {
            foreach (const TowerPosition tower, m_towerPositionsList)
            {
                if(tower.containPoint(currentPos) && !tower.hasTower())
                {
                    QPoint Temp=tower.centerPos();
                    painter.save();
                    painter.setPen(QPen(Qt::green, 5));
                    painter.drawEllipse(Temp, 40, 15);
                    painter.restore();
                    break;
                }
            }
        }
        QPixmap coinbar=QPixmap(":/image/bar.png");
        QPainter mybar(this);
        mybar.drawPixmap(740,785,coinbar);
        drawPlayerGold(&mybar);
        drawHP(&mybar);
        drawWave(&mybar);

}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint mypoint;
    QPoint pressPos = event->pos();
    auto it = m_towerPositionsList.begin();//auto 自动匹配类型
    if(option_flag)
        {
            if(pressPos.rx()>=0&&pressPos.rx()<=720&&pressPos.ry()>=0&&pressPos.ry()<=420)
            {
                preLoadWavesInfo(1);
                this->con=false;
                _level=1;
            }
            if(pressPos.rx()>720&&pressPos.rx()<=1440&&pressPos.ry()>=0&&pressPos.ry()<=420)
            {
                preLoadWavesInfo(2);
                this->con=false;
                _level=2;
            }
            if(pressPos.rx()>=0&&pressPos.rx()<=720&&pressPos.ry()>420&&pressPos.ry()<=840)
            {
                preLoadWavesInfo(3);
                this->con=false;
                _level=3;
            }
            if(pressPos.rx()>720&&pressPos.rx()<=1440&&pressPos.ry()>420&&pressPos.ry()<=840)
            {
                preLoadWavesInfo(4);
                this->con=true;
                _level=4;
                this->m_wayPointsList.clear();
                this->m_towerPositionsList.clear();
                this->loadTowerPositions123();
                this->addWayPoints123();
            }
            option_flag=0;
            gameStart();
            return;
        }

    if(flag==1)
    {
        it+=times;
        mypoint.setX(pressPos.rx()+110-40);
        mypoint.setY(pressPos.ry()+120-40);
        if(it->containPoint(mypoint))
        {
            Tower *tower = new Tower(QString::fromLocal8Bit("weapon1"),it->centerPos(), this);
            if(m_playerGold<tower->getcost())
            {
                update();
                return;
            }
            m_playerGold -= tower->getcost();
            m_towersList.push_back(tower);
            flag=0;
            m_audioPlayer->playSound(TowerPlaceSound);
            it->setHasTower();
            update();
            return;
        }
        mypoint.setX(pressPos.rx()-50-40);
        mypoint.setY(pressPos.ry()+120-40);
        if(it->containPoint(mypoint))
        {
            Tower *tower = new Tower(QString::fromLocal8Bit("weapon2"),it->centerPos(), this);
            if(m_playerGold<tower->getcost())
            {
                update();
                return;
            }
            m_playerGold -= tower->getcost();
            m_towersList.push_back(tower);
            flag=0;
            m_audioPlayer->playSound(TowerPlaceSound);
            it->setHasTower();
            update();
            return;
        }
        flag=0;
        update();
        return;
    }
    if(flag==2)
    {
        it+=times;
        mypoint.setX(pressPos.rx()+110-40);
        mypoint.setY(pressPos.ry()+120-40);
        if(it->containPoint(mypoint))
        {
            if(m_playerGold<100)
            {
                update();
                return;
            }
            foreach(Tower * tower,m_towersList)
            {
                if(it->centerPos()==tower->m_pos)
                {
                    tower->m_damage+=5;
                }
            }
            m_playerGold -= 100;
            flag=0;
            update();
            return;
        }
        mypoint.setX(pressPos.rx()-50-40);
        mypoint.setY(pressPos.ry()+120-40);
        if(it->containPoint(mypoint))
        {

            foreach(Tower * tower,m_towersList)
            {
                if(it->centerPos()==tower->m_pos)
                {
                    tower->getRemoved();
                    it->m_hasTower=false;
                }
            }
            m_playerGold+=100;
            flag=0;
            update();
            return;
        }
        flag=0;
        update();
        return;
    }
    times=0;
    while (it != m_towerPositionsList.end())
    {
        if (it->containPoint(pressPos) && !it->hasTower())
        {
            flag=1;
            temp=it->centerPos();
            break;
        }
        if (it->containPoint(pressPos) && it->hasTower())
        {
            flag=2;
            temp=it->centerPos();

            break;
        }
        ++times;
        ++it;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    currentPos=event->pos();
}

bool MainWindow::canBuyTower() const
{
    if (m_playerGold >= TowerCost)
		return true;
	return false;
}

void MainWindow::drawWave(QPainter *painter)
{
    painter->setPen(QPen(Qt::white));
    painter->setFont(font1);
    painter->drawText(1340, 825, QString("%1").arg(m_waves + 1));
}

void MainWindow::drawHP(QPainter *painter)
{
    painter->setPen(QPen(Qt::white));
    painter->setFont(font1);
    painter->drawText(1080, 825,QString("%1").arg(m_playerHp));
}

void MainWindow::drawPlayerGold(QPainter *painter)
{
    painter->setPen(QPen(Qt::white));
    painter->setFont(font1);
    painter->drawText(820, 825, QString("%1").arg(m_playerGold));
}

void MainWindow::doGameOver()
{
    if (!m_gameEnded)
	{
		m_gameEnded = true;
        start_flag=0;
        repaint();
        Sleep(1500);
        restart_flag=1;
        start_flag=1;
        repaint();
        // 此处应该切换场景到结束场景
		// 暂时以打印替代,见paintEvent处理
	}

}

void MainWindow::awardGold()
{
    m_playerGold += 150;
	update();
}

AudioPlayer *MainWindow::audioPlayer() const
{
	return m_audioPlayer;
}

void MainWindow::addWayPoints123()
{
    if(_level!=4)
    {
        WayPoint *wayPoint11 = new WayPoint(QPoint(1260, 540));
        WayPoint *wayPoint10 = new WayPoint(QPoint(660, 540));
        wayPoint10->setNextWayPoint(wayPoint11);
        WayPoint *wayPoint9 = new WayPoint(QPoint(660, 642));
        wayPoint9->setNextWayPoint(wayPoint10);
        WayPoint *wayPoint8 = new WayPoint(QPoint(420, 642));
        wayPoint8->setNextWayPoint(wayPoint9);
        WayPoint *wayPoint7 = new WayPoint(QPoint(420, 295));
        wayPoint7->setNextWayPoint(wayPoint8);
        WayPoint *wayPoint6 = new WayPoint(QPoint(175, 295));
        wayPoint6->setNextWayPoint(wayPoint7);
        WayPoint *wayPoint5 = new WayPoint(QPoint(175, 180));
        wayPoint5->setNextWayPoint(wayPoint6);
        WayPoint *wayPoint4 = new WayPoint(QPoint(780, 175));
        wayPoint4->setNextWayPoint(wayPoint5);
        WayPoint *wayPoint3 = new WayPoint(QPoint(780, 325));
        wayPoint3->setNextWayPoint(wayPoint4);
        WayPoint *wayPoint2 = new WayPoint(QPoint(1260, 325));
        wayPoint2->setNextWayPoint(wayPoint3);
        WayPoint *wayPoint1 = new WayPoint(QPoint(1260, 205));
        wayPoint1->setNextWayPoint(wayPoint2);

        m_wayPointsList.push_back(wayPoint11);
        m_wayPointsList.push_back(wayPoint10);
        m_wayPointsList.push_back(wayPoint9);
        m_wayPointsList.push_back(wayPoint8);
        m_wayPointsList.push_back(wayPoint7);
        m_wayPointsList.push_back(wayPoint6);
        m_wayPointsList.push_back(wayPoint5);
        m_wayPointsList.push_back(wayPoint4);
        m_wayPointsList.push_back(wayPoint3);
        m_wayPointsList.push_back(wayPoint2);
        m_wayPointsList.push_back(wayPoint1);
        return;
    }
    if(_level==4){
        //WayPoint *wayPoint11 = new WayPoint(QPoint(1209, 615));
        //WayPoint *wayPoint10 = new WayPoint(QPoint(993, 615));
        //wayPoint10->setNextWayPoint(wayPoint11);
        //WayPoint *wayPoint9 = new WayPoint(QPoint(993, 464));
        //wayPoint9->setNextWayPoint(wayPoint10);
        //WayPoint *wayPoint8 = new WayPoint(QPoint(993, 332));
        //wayPoint8->setNextWayPoint(wayPoint9);
        WayPoint *wayPoint7 = new WayPoint(QPoint(1200, 630));
        //wayPoint7->setNextWayPoint(wayPoint8);
        WayPoint *wayPoint6 = new WayPoint(QPoint(1000, 630));
        wayPoint6->setNextWayPoint(wayPoint7);
        WayPoint *wayPoint5 = new WayPoint(QPoint(1000, 320));
        wayPoint5->setNextWayPoint(wayPoint6);
        WayPoint *wayPoint4 = new WayPoint(QPoint(800, 320));
        wayPoint4->setNextWayPoint(wayPoint5);
        WayPoint *wayPoint3 = new WayPoint(QPoint(800, 400));
        wayPoint3->setNextWayPoint(wayPoint4);
        WayPoint *wayPoint2 = new WayPoint(QPoint(340, 400));
        wayPoint2->setNextWayPoint(wayPoint3);
        WayPoint *wayPoint1 = new WayPoint(QPoint(340, 235));
        wayPoint1->setNextWayPoint(wayPoint2);
        WayPoint *wayPoint0 = new WayPoint(QPoint(144, 235));
        wayPoint0->setNextWayPoint(wayPoint1);


        //m_wayPointsList.push_back(wayPoint11);
       // m_wayPointsList.push_back(wayPoint10);
        //m_wayPointsList.push_back(wayPoint9);
        //m_wayPointsList.push_back(wayPoint8);
        m_wayPointsList.push_back(wayPoint7);
        m_wayPointsList.push_back(wayPoint6);
        m_wayPointsList.push_back(wayPoint5);
        m_wayPointsList.push_back(wayPoint4);
        m_wayPointsList.push_back(wayPoint3);
        m_wayPointsList.push_back(wayPoint2);
        m_wayPointsList.push_back(wayPoint1);
        m_wayPointsList.push_back(wayPoint0);

        return;
   }
}

void MainWindow::addWayPoints4()
{
    WayPoint *wayPoint11 = new WayPoint(QPoint(1260, 540));
    WayPoint *wayPoint10 = new WayPoint(QPoint(660, 540));
    wayPoint10->setNextWayPoint(wayPoint11);
    WayPoint *wayPoint9 = new WayPoint(QPoint(660, 642));
    wayPoint9->setNextWayPoint(wayPoint10);
    WayPoint *wayPoint8 = new WayPoint(QPoint(420, 642));
    wayPoint8->setNextWayPoint(wayPoint9);
    WayPoint *wayPoint7 = new WayPoint(QPoint(420, 295));
    wayPoint7->setNextWayPoint(wayPoint8);
    WayPoint *wayPoint6 = new WayPoint(QPoint(175, 295));
    wayPoint6->setNextWayPoint(wayPoint7);
    WayPoint *wayPoint5 = new WayPoint(QPoint(175, 180));
    wayPoint5->setNextWayPoint(wayPoint6);
    WayPoint *wayPoint4 = new WayPoint(QPoint(780, 175));
    wayPoint4->setNextWayPoint(wayPoint5);
    WayPoint *wayPoint3 = new WayPoint(QPoint(780, 325));
    wayPoint3->setNextWayPoint(wayPoint4);
    WayPoint *wayPoint2 = new WayPoint(QPoint(1260, 325));
    wayPoint2->setNextWayPoint(wayPoint3);
    WayPoint *wayPoint1 = new WayPoint(QPoint(1260, 205));
    wayPoint1->setNextWayPoint(wayPoint2);

    m_wayPointsList.push_back(wayPoint11);
    m_wayPointsList.push_back(wayPoint10);
    m_wayPointsList.push_back(wayPoint9);
    m_wayPointsList.push_back(wayPoint8);
    m_wayPointsList.push_back(wayPoint7);
    m_wayPointsList.push_back(wayPoint6);
    m_wayPointsList.push_back(wayPoint5);
    m_wayPointsList.push_back(wayPoint4);
    m_wayPointsList.push_back(wayPoint3);
    m_wayPointsList.push_back(wayPoint2);
    m_wayPointsList.push_back(wayPoint1);
}

void MainWindow::getHpDamage(int damage/* = 1*/)
{
	m_audioPlayer->playSound(LifeLoseSound);
	m_playerHp -= damage;
	if (m_playerHp <= 0)
		doGameOver();

}

void MainWindow::removedEnemy(Enemy *enemy)
{
	Q_ASSERT(enemy);

	m_enemyList.removeOne(enemy);
	delete enemy;

	if (m_enemyList.empty())
	{
		++m_waves;
		if (!loadWave())
		{
            m_gameWin = true;
            start_flag=0;
            repaint();
            Sleep(1500);
            restart_flag=1;
            start_flag=1;
            repaint();

			// 游戏胜利转到游戏胜利场景
			// 这里暂时以打印处理
		}
	}
}

void MainWindow::removedBullet(Bullet *bullet)
{
	Q_ASSERT(bullet);

	m_bulletList.removeOne(bullet);
	delete bullet;
}

void MainWindow::removeTower(Tower *tower)
{
    Q_ASSERT(tower);

    m_towersList.removeOne(tower);
    delete tower;
}

void MainWindow::addBullet(Bullet *bullet)
{
	Q_ASSERT(bullet);

	m_bulletList.push_back(bullet);
}

void MainWindow::updateMap()
{
	foreach (Enemy *enemy, m_enemyList)
		enemy->move();
	foreach (Tower *tower, m_towersList)
		tower->checkEnemyInRange();
	update();
}

void MainWindow::preLoadWavesInfo(int level)
{
    QFile file;
    switch(level)
    {
    case 1:
        file.setFileName(":/config/Waves.plist");
        break;
    case 2:
        file.setFileName(":/config/waves2.plist");
        break;
    case 3:
        file.setFileName(":/config/waves3.plist");
        break;
    case 4:
        file.setFileName(":/config/waves4.plist");
        break;
    }


    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "TowerDefense", "Cannot Open TowersPosition.plist");
        return;
    }

    PListReader reader;
    reader.read(&file);

    // 获取波数信息
    m_wavesInfo = reader.data();

    file.close();
}


bool MainWindow::loadWave()
{
	if (m_waves >= m_wavesInfo.size())
		return false;

	WayPoint *startWayPoint = m_wayPointsList.back();
	QList<QVariant> curWavesInfo = m_wavesInfo[m_waves].toList();


           for (int i = 0; i < curWavesInfo.size(); ++i)
           {
               QMap<QString, QVariant> dict = curWavesInfo[i].toMap();
               int spawnTime = dict.value("spawnTime").toInt();

               Enemy *enemy=new Enemy(dict.value("data").toString(),startWayPoint,this);

               m_enemyList.push_back(enemy);
               QTimer::singleShot(spawnTime, enemy, SLOT(doActivate()));
           }


	return true;
}

QList<Enemy *> MainWindow::enemyList() const
{
	return m_enemyList;
}

void MainWindow::gameStart()
{
    start_flag=0;
	loadWave();
    this->startButton->hide();
    this->optionButton->hide();
    this->helpButton->hide();
    this->exitButton->hide();
    this->pauseButton->show();
    this->exitButton_1->show();
    this->replay->show();
    this->slowdown->show();
    this->screenShotButton->show();
    this->damageskill->show();
}

void MainWindow::screenShotSlot()
{
    QPixmap pix, bmp;
    pix =bmp.grabWindow(this->winId(),0,0,frameGeometry().width(),frameGeometry().height());
        if (pix.isNull())
        {
            QMessageBox::information(this, "Error", "截屏失败 !", QMessageBox::Ok);

        }
        else
        {
            if(!pix.save(QString(":/image/")+QString("pic")+QString::number(cnt-2),"BMP"))
            {
                QMessageBox::information(this, "Right", "保存错误 !", QMessageBox::Ok);
            }
            else
            {
                QMessageBox::information(this, "Grab", "保存成功!",QMessageBox::Ok);

            }
        }
}
void MainWindow::change_interface()
{
    option_flag=1;
    update();
    this->startButton->hide();
    this->optionButton->hide();
    this->helpButton->hide();
    this->exitButton->hide();
}
void MainWindow::gamereStart()
{
    qApp->closeAllWindows();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
}



