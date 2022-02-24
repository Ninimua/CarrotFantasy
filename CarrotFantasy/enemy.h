#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>

class WayPoint;
class QPainter;
class MainWindow;
class Tower;

class Enemy : public QObject
{
	Q_OBJECT
public:
    Enemy(QString type,WayPoint *startWayPoint, MainWindow *game);
        Enemy(const QPixmap &sprite/* = QPixmap(":/image/enemy.png")*/);
        //设置一个开始位置节点（用于生成路径），一个窗口管理，一个怪物图像

	~Enemy();

	void draw(QPainter *painter) const;
	void move();
	void getDamage(int damage);
	void getRemoved();
	void getAttacked(Tower *attacker);
	void gotLostSight(Tower *attacker);
	QPoint pos() const;

public slots:
	void doActivate();

public:
	bool			m_active;
	int				m_maxHp;
	int				m_currentHp;
	qreal			m_walkingSpeed;
	qreal			m_rotationSprite;

	QPoint			m_pos;
	WayPoint *		m_destinationWayPoint;
	MainWindow *	m_game;
	QList<Tower *>	m_attackedTowersList;

    QPixmap	m_sprite;
    QSize ms_fixedSize;
};

#endif // ENEMY_H
