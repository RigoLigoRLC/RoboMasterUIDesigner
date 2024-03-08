#ifndef JIGHANDLEANCHOR_H
#define JIGHANDLEANCHOR_H

#include <QGraphicsRectItem>

class JigHandle;

class JigHandleAnchor : public QGraphicsRectItem
{
public:
    JigHandleAnchor(QGraphicsItem *parent = nullptr) : QGraphicsRectItem(parent) {
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemHasNoContents, true);
        setRect({-1, -1, 1, 1});
        setHandlesChildEvents(false);
    }
    void setCenter(QPoint p)
    {
        setPos(p);
    }
    QPoint center() { return pos().toPoint(); }
    JigHandle* handle() {
        if (childItems().empty()) {
            return nullptr;
        } else {
            return (JigHandle*)childItems().first();
        }
    }

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) {

    }
};


#endif // JIGHANDLEANCHOR_H
