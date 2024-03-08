#ifndef RECTANGULARJIG_H
#define RECTANGULARJIG_H

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QVector>
#include "elementjig.h"
#include "jighandle.h"

// In rectangular jig, the m_boundary holds the exact rectangle the jig controls.
// All other handles' positions are calculated based on its rect.

class RectangularJig : public ElementJig
{
public:
    RectangularJig(QGraphicsItem* parent = nullptr);
    ~RectangularJig();

    QRect jigRect();
    void setJigRect(QRect rect);

    virtual JigType jigType() override { return JigType::RectangularJigType; }

    virtual void remember() override;
    virtual void restore() override;
    virtual void forget() override;

protected:
    // Boundary geometry manipulation convenience functions
    QPoint pointAtIndex(int i);
    void setPointAtIndex(int i, QPoint p);

    // Handle point manipulation convenience functions
    QPoint handleCenter(int i);
    void setHandleCenter(int i, QPoint p);

    void invalidateHandlePositions();

    // Point manipulation convenience function
    QPoint withX(QPoint p, int x) { p.setX(x); return p; }
    QPoint withY(QPoint p, int y) { p.setY(y); return p; }

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *e) override;
    virtual bool sceneEvent(QEvent* e) override;

    void jigHandleMoved(int id, QPointF oldPos, QPointF newPos);

protected:
    QVector<JigHandleAnchor*> m_handles;
    JigHandleAnchor* m_centerHandle;
    QGraphicsRectItem* m_boundary;

    QRectF m_rememberedBoundary;
};

#endif // RECTANGULARJIG_H
