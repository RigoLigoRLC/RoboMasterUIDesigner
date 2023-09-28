#ifndef ELLIPTICALARCJIG_H
#define ELLIPTICALARCJIG_H

#include "elementjig.h"
#include "jighandle.h"
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <tuple>

class EllipticalArcJig : public ElementJig
{
public:
    EllipticalArcJig(QGraphicsItem* parent = nullptr);

    void setRect(QRect rect);
    void setBeginEndAngles(double begin, double end);

    std::tuple<int, int> qtArcStartSpanAngles();
    std::tuple<double, double> rmArcBeginEndAnglesRadian();

    QRect jigRect() { return m_resultArc->rect().toAlignedRect(); }

    virtual JigType jigType() override { return JigType::EllipticalArcJigType; }

    virtual void remember() override;
    virtual void restore() override;
    virtual void forget() override;

protected:
    virtual bool sceneEvent(QEvent* e) override;

    void invalidateGraphics();
    QPoint desiredHandlePos(double angle); // Calculate handle pos with angle and height, width
    double desiredHandleAngle(QPoint point); // Calculate handle angle with point and height, width

    void handleMoved(int id, QPoint oldPos, QPoint newPos);

    // Convenience, for Qt ellipse
    QRectF boundingRekt();

    // Convenience
    double screenAtan2(double Y, double X);

protected:
    // Angles in the elliptical arc jig is calculated with the ellipse
    // "stretched" up to a circle.
    double m_angleBegin, m_angleEnd;
    int m_halfWidth, m_halfHeight;

    // Center of the elliptical arc is determined by m_handleCenter's rect center entirely
    JigHandle *m_handleBegin, *m_handleEnd, *m_handleCenter;
    QGraphicsEllipseItem *m_resultArc;

    // Remembered state
    struct {
        double angleBegin, angleEnd;
        int halfWidth, halfHeight;
        QPoint center;
    } m_rememberedState;
};

#endif // ELLIPTICALARCJIG_H
