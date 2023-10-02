#ifndef JIGHANDLE_H
#define JIGHANDLE_H

#include <QGraphicsRectItem>

class ElementJig;

class JigHandle : public QGraphicsRectItem
{
public:
    JigHandle(QGraphicsItem *parent = nullptr, int id = -1);
    JigHandle(QRect rect, QGraphicsItem* parent = nullptr, int id = -1);

    void setCenter(QPoint p);
    QPoint center() { return rect().center().toPoint(); }

    int id() { return data(HandleIndex).toInt(); }

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

    friend class ElementJig;
};

#endif // JIGHANDLE_H
