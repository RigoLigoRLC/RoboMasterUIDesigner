#ifndef GRIDEDGRAPHICSVIEW_H
#define GRIDEDGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QBitmap>
#include <QPainter>

class GridedGraphicsView : public QGraphicsView
{
public:
    GridedGraphicsView(QWidget *parent = nullptr);

    void setAlawysZoomToFit(bool fit) { m_alwaysZoomToFit = fit; if (fit) zoomToFit(); }

protected:
    virtual void drawBackground(QPainter *painter, const QRectF &rect) override;

    virtual void mousePressEvent(QMouseEvent* e) override;
    virtual void mouseReleaseEvent(QMouseEvent* e) override;
    virtual void mouseMoveEvent(QMouseEvent* e) override;

    virtual void resizeEvent(QResizeEvent* e) override;

private:
    void zoomToFit();

private:
    QPixmap m_clientBackground;

    enum {
        DraggingNone,
        DraggingPrepare,
        DraggingConfirmed,
    } m_backgroundDragging = DraggingNone;
    QPoint m_backgroundDraggingMouseStartPos, m_backgroundDraggingViewStartPos;

    bool m_alwaysZoomToFit = false;
};

#endif // GRIDEDGRAPHICSVIEW_H
