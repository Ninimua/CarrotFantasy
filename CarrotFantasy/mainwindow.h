#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "towerposition.h"
#include "tower.h"
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
#include <QGraphicsWidget>
#include <QMediaPlayer>
#include <QScreen>
#include <QDir>
#include <QVector>
#include <iostream>
#include <QMessageBox>
#include "mybutton.h"

namespace Ui {
class MainWindow;
}

class WayPoint;
class Enemy;
class Bullet;
class AudioPlayer;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void getHpDamage(int damage = 1);
    void getHp(int damage = 1);
	void removedEnemy(Enemy *enemy);
	void removedBullet(Bullet *bullet);
	void addBullet(Bullet *bullet);
    void removeTower(Tower *tower);
    void awardGold();
	AudioPlayer* audioPlayer() const;
	QList<Enemy *> enemyList() const;
    void startMenu();
void ini(QWidget *parent=0);
protected:
	void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
private slots:
	void updateMap();
	void gameStart();
    void screenShotSlot();
    void change_interface();
    void gamereStart();
    void pause();
    void help();
    void fireball();
    void freeze();
private:
    void loadTowerPositions123();
    void loadTowerPositions4();
    void addWayPoints123();
    void addWayPoints4();
	bool loadWave();
	bool canBuyTower() const;
	void drawWave(QPainter *painter);
	void drawHP(QPainter *painter);
	void drawPlayerGold(QPainter *painter);
	void doGameOver();
    void preLoadWavesInfo(int level);
    bool con;
private:
	Ui::MainWindow *		ui;
	int						m_waves;
	int						m_playerHp;
    int						m_playerGold;
	bool					m_gameEnded;
	bool					m_gameWin;
	AudioPlayer *			m_audioPlayer;
	QList<QVariant>			m_wavesInfo;
	QList<TowerPosition>	m_towerPositionsList;
	QList<Tower *>			m_towersList;
	QList<WayPoint *>		m_wayPointsList;
	QList<Enemy *>			m_enemyList;
	QList<Bullet *>			m_bulletList;
    QPushButton * startButton;// 游戏开始按钮
    QPushButton * optionButton;// 选择项按钮
    QPushButton * helpButton;// 帮助按钮
    QPushButton * exitButton;// 游戏退出按钮
    QPushButton * exitButton_1;//游戏进行中退出按钮
    QPushButton * replay;// 重新游戏
    QPushButton * pauseButton; // 游戏暂停按钮
    QPushButton * StartButton; // 游戏继续按钮
    mybutton * damageskill; // 伤害技能
    mybutton * slowdown;
    QPixmap screenShot;
    QPushButton * screenShotButton;
    bool gameStatus;
    int flag;                                               //判断鼠标操作到了第几层
    int times;                                              //记录上次点的是第几个点
    QPoint temp;    //记录上一层的鼠标位置信息
    QPoint currentPos;  //记录鼠标的实时位置
    QTimer * m_timer;
    static int level;                                       //记录目前的关卡
    int start_flag;                                         //判断是否开始游戏
    int option_flag;                                        //判断是否为关卡选择
    int restart_flag;                                       //判断是否为重新开始游戏


};

#endif // MAINWINDOW_H
