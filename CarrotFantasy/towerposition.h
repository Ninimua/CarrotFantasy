#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include <QPoint>
#include <QSize>
#include <QPixmap>

class QPainter;

class TowerPosition
{
public:
	TowerPosition(QPoint pos, const QPixmap &sprite = QPixmap(":/image/open_spot.png"));

	void setHasTower(bool hasTower = true);
	bool hasTower() const;
	const QPoint centerPos() const;
	bool containPoint(const QPoint &pos) const;
    bool	m_hasTower;
	void draw(QPainter *painter) const;

private:

	QPoint		m_pos;
	QPixmap		m_sprite;

	static const QSize ms_fixedSize;
};

#endif // TOWERPOSITION_H
