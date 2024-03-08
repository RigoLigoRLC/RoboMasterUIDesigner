#ifndef JIGHANDLE_H
#define JIGHANDLE_H

#include <QGraphicsRectItem>
#include "jighandleanchor.h"

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

    /**
     * @brief Make Create a jig handle with specified ID
     * @param parent parent item
     * @param id handle ID
     * @return jig handle anchor item, whose only child is the jig handle item
     */
    static JigHandleAnchor* Make(QGraphicsItem *parent = nullptr, int id = -1) {
        auto anchor = new JigHandleAnchor(parent);
        new JigHandle(anchor, id); // Create handle attached to anchor
        return anchor;
    }

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* e) override;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* e) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* e) override;

    QGraphicsItem* parentJig();

    QPointF m_scenePressPos, m_beginDragRectCenter;
    bool m_moveConfirmed;

    friend class ElementJig;
};

#endif // JIGHANDLE_H
