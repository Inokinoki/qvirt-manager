/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version).
 */

#include "GraphWidget.h"
#include <QPainter>
#include <QTimer>
#include <QPainterPath>
#include <QDebug>
#include <QPixmap>

namespace QVirt {

GraphWidget::GraphWidget(GraphType type, QWidget *parent)
    : QWidget(parent)
    , m_type(type)
    , m_maxDataPoints(60)
    , m_updateTimer(nullptr)
    , m_updateInterval(1000)
    , m_minValue(0.0f)
    , m_maxValue(100.0f)
{
    setMinimumSize(200, 100);
    setAutoFillBackground(true);

    // Set default colors based on type
    switch (type) {
    case CPU:
        m_graphColor = QColor(52, 152, 219);  // Blue
        m_title = "CPU Usage";
        break;
    case Memory:
        m_graphColor = QColor(46, 204, 113);  // Green
        m_title = "Memory Usage";
        break;
    case Disk:
        m_graphColor = QColor(155, 89, 182);  // Purple
        m_title = "Disk I/O";
        break;
    case Network:
        m_graphColor = QColor(230, 126, 34);  // Orange
        m_title = "Network I/O";
        break;
    }

    // Set background color
    setPalette(Qt::white);
}

void GraphWidget::addValue(float value)
{
    m_data.append(value);

    // Keep only the last maxDataPoints values
    while (m_data.size() > m_maxDataPoints) {
        m_data.removeFirst();
    }

    // Update min/max values
    m_minValue = 0.0f;
    m_maxValue = 100.0f;

    if (!m_data.isEmpty()) {
        float max = *std::max_element(m_data.begin(), m_data.end());
        if (max > m_maxValue) {
            m_maxValue = max;
        }
    }

    update();
}

void GraphWidget::clear()
{
    m_data.clear();
    update();
}

void GraphWidget::setUpdateInterval(int milliseconds)
{
    m_updateInterval = milliseconds;

    if (m_updateTimer) {
        m_updateTimer->setInterval(m_updateInterval);
    }
}

void GraphWidget::setMaxDataPoints(int points)
{
    m_maxDataPoints = points;

    // Trim existing data if needed
    while (m_data.size() > m_maxDataPoints) {
        m_data.removeFirst();
    }
}

QString GraphWidget::title() const
{
    return m_title;
}

void GraphWidget::setTitle(const QString &title)
{
    m_title = title;
    update();
}

QColor GraphWidget::graphColor() const
{
    return m_graphColor;
}

void GraphWidget::setGraphColor(const QColor &color)
{
    m_graphColor = color;
    update();
}

void GraphWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBackground(&painter);
    drawGrid(&painter);

    if (!m_data.isEmpty()) {
        drawGraph(&painter);
    }

    drawLabels(&painter);
}

void GraphWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    update();
}

void GraphWidget::updateGraph()
{
    // This would be called by timer if we were auto-updating
    // For now, updates are triggered externally via addValue()
}

void GraphWidget::drawBackground(QPainter *painter)
{
    painter->fillRect(rect(), palette().window());
}

void GraphWidget::drawGrid(QPainter *painter)
{
    painter->setPen(QPen(QColor(200, 200, 200), 1, Qt::DotLine));

    // Draw horizontal grid lines
    for (int i = 0; i <= 4; ++i) {
        int y = height() - (height() * i / 4);
        painter->drawLine(0, y, width(), y);
    }

    // Draw vertical grid lines
    for (int i = 0; i <= 10; ++i) {
        int x = width() * i / 10;
        painter->drawLine(x, 0, x, height());
    }
}

void GraphWidget::drawGraph(QPainter *painter)
{
    if (m_data.size() < 2) {
        return;
    }

    // Create path for the graph
    QPainterPath path;

    float xStep = static_cast<float>(width()) / (m_maxDataPoints - 1);

    // Start the path
    float y = height() - (m_data[0] / m_maxValue * height());
    path.moveTo(0, y);

    // Add points to the path
    for (int i = 1; i < m_data.size(); ++i) {
        float x = i * xStep;
        float value = m_data[i];
        y = height() - (value / m_maxValue * (height() - 20)) - 10;  // Leave padding
        path.lineTo(x, y);
    }

    // Draw the line
    QPen pen(m_graphColor, 2);
    painter->setPen(pen);
    painter->drawPath(path);

    // Fill area under the graph
    QPainterPath fillPath = path;
    fillPath.lineTo(width(), height());
    fillPath.lineTo(0, height());
    fillPath.closeSubpath();

    QColor fillColor = m_graphColor;
    fillColor.setAlpha(50);
    painter->fillPath(fillPath, fillColor);

    // Draw current value indicator
    float lastX = (m_data.size() - 1) * xStep;
    float lastValue = m_data.last();
    float lastY = height() - (lastValue / m_maxValue * (height() - 20)) - 10;

    painter->setBrush(QBrush(m_graphColor));
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(lastX, lastY), 4, 4);
}

void GraphWidget::drawLabels(QPainter *painter)
{
    painter->setPen(QColor(80, 80, 80));

    // Draw title
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);

    QRect titleRect(10, 5, width() - 20, 20);
    painter->drawText(titleRect, Qt::AlignLeft, m_title);

    // Draw current value if available
    if (!m_data.isEmpty()) {
        float currentValue = m_data.last();
        QString valueText = QString::number(currentValue, 'f', 1) + "%";

        font.setBold(false);
        font.setPointSize(9);
        painter->setFont(font);

        QRect valueRect(width() - 80, 5, 70, 20);
        painter->drawText(valueRect, Qt::AlignRight, valueText);
    }

    // Draw Y-axis labels
    font.setPointSize(7);
    painter->setFont(font);

    for (int i = 0; i <= 4; ++i) {
        int y = height() - (height() * i / 4);
        float value = (m_maxValue * i / 4);
        QString label = QString::number(value, 'f', 0) + "%";

        QRect labelRect(2, y - 10, 35, 20);
        painter->drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, label);
    }
}

// ============================================================================
// SparklineDelegate
// ============================================================================

QPixmap SparklineDelegate::generateSparkline(const QVector<float> &data,
                                             const QSize &size,
                                             const QColor &color)
{
    if (data.isEmpty()) {
        return QPixmap();
    }

    QPixmap pixmap(size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    if (data.size() < 2) {
        // Draw single point
        painter.setBrush(QBrush(color));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(QPointF(size.width() / 2, size.height() / 2), 2, 2);
        return pixmap;
    }

    // Create path
    QPainterPath path;

    float minVal = *std::min_element(data.begin(), data.end());
    float maxVal = *std::max_element(data.begin(), data.end());
    float range = maxVal - minVal;

    if (range == 0) {
        range = 1.0f;
    }

    float xStep = static_cast<float>(size.width()) / (data.size() - 1);

    // Start the path
    float y = size.height() - ((data[0] - minVal) / range * size.height());
    path.moveTo(0, y);

    // Add points
    for (int i = 1; i < data.size(); ++i) {
        float x = i * xStep;
        float value = data[i];
        y = size.height() - ((value - minVal) / range * size.height());
        path.lineTo(x, y);
    }

    // Draw the line
    QPen pen(color, 1.5);
    painter.setPen(pen);
    painter.drawPath(path);

    // Fill area under the graph
    QPainterPath fillPath = path;
    fillPath.lineTo(size.width(), size.height());
    fillPath.lineTo(0, size.height());
    fillPath.closeSubpath();

    QColor fillColor = color;
    fillColor.setAlpha(80);
    painter.fillPath(fillPath, fillColor);

    return pixmap;
}

} // namespace QVirt
