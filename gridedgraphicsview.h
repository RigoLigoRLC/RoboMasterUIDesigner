#ifndef GRIDEDGRAPHICSVIEW_H
#define GRIDEDGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QBitmap>
#include <QPainter>

class GridedGraphicsView : public QGraphicsView
{
public:
    GridedGraphicsView(QWidget *parent = nullptr);;

protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect);

private:
    QPixmap m_clientBackground;
};

#endif // GRIDEDGRAPHICSVIEW_H
