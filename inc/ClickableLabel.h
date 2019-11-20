#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#pragma once

#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QPushButton>
#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <iostream>

class ClickableDateLabel : public QLabel{
Q_OBJECT
public:
    explicit ClickableDateLabel( QString text,  int year, int month, int day, QWidget* parent=0 );
    int year, month, day;
    ~ClickableDateLabel();

    signals:
        void clicked();

private slots:
    void mousePressEvent(QMouseEvent* event);
};

class PreviousYearButton : public QPushButton{
Q_OBJECT
public:
    explicit PreviousYearButton(QString text, QLineEdit* line, QWidget* widget, QGridLayout* layout, int year, bool iss_prev);
    QWidget* widget;
    QLineEdit* line;
    QGridLayout* layout;
    int year;
    bool is_prev;
    ~PreviousYearButton();

private slots:
    void mousePressEvent(QMouseEvent* event);
};

class EnterLineEdit : public QLineEdit{
Q_OBJECT
public:
    explicit EnterLineEdit(QString text, QWidget* widget, QGridLayout* layout);
    QWidget* widget;
    QGridLayout* layout;
    ~EnterLineEdit();

private slots:
    void wasPressed();
};

class SaveThinkButton : public QPushButton{
Q_OBJECT
public:
    explicit SaveThinkButton(QString text, QWidget *wdg, QTextEdit* txt, int year, int month, int day);
    int year, month, day;
    QWidget* wdg;
    QTextEdit* txt;
    ~SaveThinkButton();

private slots:
    void mousePressEvent(QMouseEvent* event);
};

#endif // CLICKABLELABEL_H
