#ifndef JIGHANDLE_H
#define JIGHANDLE_H

#include <QGraphicsRectItem>

class JigHandle : public QGraphicsRectItem
{
public:
    JigHandle(QGraphicsItem *parent = nullptr);
    JigHandle(QRect rect, QGraphicsItem* parent = nullptr);

    void setCenter(QPoint p);

    static constexpr int HandleWidth = 8;
    static QColor HandleColor;

    enum HandleData {
        HandleIndex
    };

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* e) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* e) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* e) override;

    QPointF m_scenePressPos, m_beginDragRectCenter;
    bool m_moveConfirmed;
};

#endif // JIGHANDLE_H
