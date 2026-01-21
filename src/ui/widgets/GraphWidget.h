/*
 * QVirt-Manager
 *
 * Copyright (C) 2025-2026 Inoki <veyx.shaw@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef QVIRT_UI_WIDGETS_GRAPHWIDGET_H
#define QVIRT_UI_WIDGETS_GRAPHWIDGET_H

#include <QWidget>
#include <QVector>
#include <QString>

class QPainter;
class QTimer;

namespace QVirt {

/**
 * @brief Performance graph widget
 *
 * Displays real-time performance data as a line graph
 */
class GraphWidget : public QWidget
{
    Q_OBJECT

public:
    enum GraphType {
        CPU,
        Memory,
        Disk,
        Network
    };

    explicit GraphWidget(GraphType type, QWidget *parent = nullptr);
    ~GraphWidget() override = default;

    void addValue(float value);
    void clear();
    void setUpdateInterval(int milliseconds);
    void setMaxDataPoints(int points);

    QString title() const;
    void setTitle(const QString &title);

    QColor graphColor() const;
    void setGraphColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateGraph();

private:
    void drawBackground(QPainter *painter);
    void drawGrid(QPainter *painter);
    void drawGraph(QPainter *painter);
    void drawLabels(QPainter *painter);

    GraphType m_type;
    QString m_title;
    QColor m_graphColor;

    QVector<float> m_data;
    int m_maxDataPoints;

    QTimer *m_updateTimer;
    int m_updateInterval;

    float m_minValue;
    float m_maxValue;
};

/**
 * @brief Sparkline delegate for table views
 *
 * Displays mini graphs in table cells
 */
class SparklineDelegate
{
public:
    static QPixmap generateSparkline(const QVector<float> &data,
                                     const QSize &size,
                                     const QColor &color = Qt::blue);
};

} // namespace QVirt

#endif // QVIRT_UI_WIDGETS_GRAPHWIDGET_H
