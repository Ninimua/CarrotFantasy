#include "tower.h"
#include "enemy.h"
#include "bullet.h"
#include "mainwindow.h"
#include "utility.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>



Tower::Tower(QString type,QPoint pos, MainWindow *game)
    : m_attacking(false)
    , m_attackRange(175)
    , m_damage(10)
    , m_fireRate(500)
    , m_rotationSprite(0.0)
    , m_chooseEnemy(NULL)
    , m_game(game)
    , m_pos(pos)
{
    m_fireRateTimer = new QTimer(this);
    connect(m_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
    if(type==QString::fromLocal8Bit("weapon1"))
    {
        m_sprite=QPixmap(":/image/tower1.png");
        ms_fixedSize.setHeight(68);
        ms_fixedSize.setWidth(77);
        m_cost=220;
        m_fireRate/=2;
        m_damage/=2;
    }
    if(type==QString::fromLocal8Bit("weapon2"))
    {
        m_sprite=QPixmap(":/image/tower2.png");
        ms_fixedSize.setHeight(74);
        ms_fixedSize.setWidth(72);
        m_cost=260;
        m_damage*=1.5;
    }
}

Tower::~Tower()
{
	delete m_fireRateTimer;
	m_fireRateTimer = NULL;
}

void Tower::checkEnemyInRange()
{
	if (m_chooseEnemy)
	{
		// 这种情况下,需要旋转炮台对准敌人
		// 向量标准化
		QVector2D normalized(m_chooseEnemy->pos() - m_pos);
		normalized.normalize();
        m_rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) +90;

		// 如果敌人脱离攻击范围
		if (!collisionWithCircle(m_pos, m_attackRange, m_chooseEnemy->pos(), 1))
			lostSightOfEnemy();
	}
	else
	{
		// 遍历敌人,看是否有敌人在攻击范围内
		QList<Enemy *> enemyList = m_game->enemyList();
		foreach (Enemy *enemy, enemyList)
		{
			if (collisionWithCircle(m_pos, m_attackRange, enemy->pos(), 1))
			{
				chooseEnemyForAttack(enemy);
				break;
			}
		}
	}
}

void Tower::draw(QPainter *painter) const
{
	painter->save();

	// 绘制偏转坐标,由中心+偏移=左上
	static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
	// 绘制炮塔并选择炮塔
	painter->translate(m_pos);
	painter->rotate(m_rotationSprite);
	painter->drawPixmap(offsetPoint, m_sprite);
    painter->restore();
}

void Tower::attackEnemy()
{
	m_fireRateTimer->start(m_fireRate);
}

void Tower::chooseEnemyForAttack(Enemy *enemy)
{
	m_chooseEnemy = enemy;
	attackEnemy();
	m_chooseEnemy->getAttacked(this);
}

void Tower::getRemoved()
{
    m_game->removeTower(this);
}

void Tower::shootWeapon()
{
	Bullet *bullet = new Bullet(m_pos, m_chooseEnemy->pos(), m_damage, m_chooseEnemy, m_game);
	bullet->move();
	m_game->addBullet(bullet);
}

void Tower::targetKilled()
{
	if (m_chooseEnemy)
		m_chooseEnemy = NULL;

	m_fireRateTimer->stop();
	m_rotationSprite = 0.0;
}

void Tower::lostSightOfEnemy()
{
	m_chooseEnemy->gotLostSight(this);
	if (m_chooseEnemy)
		m_chooseEnemy = NULL;

	m_fireRateTimer->stop();
	m_rotationSprite = 0.0;
}
int Tower::getcost()
{
    return m_cost;
}

