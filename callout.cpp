#include "callout.h"

callout::callout(QChart *chart):
        QGraphicsItem(chart),
       chartFrom(chart)
{

}

void callout::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* wgt)
{
    Q_UNUSED(option);
    Q_UNUSED(wgt);
    QPainterPath path;
    path.addRoundedRect(m_rect, 5,5);

    QPointF anchor= mapFromParent(chartFrom->mapToPosition(anchorFrom));
    if (!m_rect.contains(anchor)) {
        QPointF point1, point2;

        // establish the position of the anchor point in relation to m_rect
        bool above = anchor.y() <= m_rect.top();
        bool aboveCenter = anchor.y() > m_rect.top() && anchor.y() <= m_rect.center().y();
        bool belowCenter = anchor.y() > m_rect.center().y() && anchor.y() <= m_rect.bottom();
        bool below = anchor.y() > m_rect.bottom();

        bool onLeft = anchor.x() <= m_rect.left();
        bool leftOfCenter = anchor.x() > m_rect.left() && anchor.x() <= m_rect.center().x();
        bool rightOfCenter = anchor.x() > m_rect.center().x() && anchor.x() <= m_rect.right();
        bool onRight = anchor.x() > m_rect.right();

        // get the nearest m_rect corner.
        qreal x = (onRight + rightOfCenter) * m_rect.width();
        qreal y = (below + belowCenter) * m_rect.height();
        bool cornerCase = (above && onLeft) || (above && onRight) || (below && onLeft) || (below && onRight);
        bool vertical = qAbs(anchor.x() - x) > qAbs(anchor.y() - y);

        qreal x1 = x + leftOfCenter * 10 - rightOfCenter * 20 + cornerCase * !vertical * (onLeft * 10 - onRight * 20);
        qreal y1 = y + aboveCenter * 10 - belowCenter * 20 + cornerCase * vertical * (above * 10 - below * 20);;
        point1.setX(x1);
        point1.setY(y1);

        qreal x2 = x + leftOfCenter * 20 - rightOfCenter * 10 + cornerCase * !vertical * (onLeft * 20 - onRight * 10);;
        qreal y2 = y + aboveCenter * 20 - belowCenter * 10 + cornerCase * vertical * (above * 20 - below * 10);;
        point2.setX(x2);
        point2.setY(y2);

        path.moveTo(point1);
        path.lineTo(anchor);
        path.lineTo(point2);
        path = path.simplified();
    }
    painter->setBrush(QColor(255, 255, 255));
    painter->drawPath(path);
    painter->drawText(textRectFrom, textFrom);
}

void callout::setText(QString text)
{
    textFrom = text;
    QFontMetrics metrics(fontFrom);
    textRectFrom = metrics.boundingRect(QRect(0, 0, 150, 150), Qt::AlignLeft,textFrom);
    textRectFrom.translate(5, 5);
    prepareGeometryChange();
    m_rect = textRectFrom.adjusted(-5, -5, 5, 5);
}

void callout::setAnchor(QPointF point)
{
    anchorFrom = point;
}
