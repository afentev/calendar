#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QCoreApplication>
#include <QtGui>
#include <QGroupBox>
#include <string>
#include "inc/ClickableLabel.h"

int first = 1;
int previous_year = 2019;

bool is_holiday(int month, int day) {
    return (month == 0 && day < 8) || (month == 1 && day == 22) || (month == 2 && day == 7) ||
           (month == 4 && (day == 0 || day == 8)) || (month == 5 && (day == 11 || day == 26)) ||
           (month == 10 && (day == 3 || day == 19));
}

QString day_name(int n) {
    switch (n) {
        case 0: return QString("Monday");
        case 1: return QString("Tuesday");
        case 2: return QString("Wednesday");
        case 3: return QString("Thursday");
        case 4: return QString("Friday");
        case 5: return QString("Saturday");
        case 6: return QString("Sunday");
        default: return QString("wwwwhat");
    }
}

QString month_name(int n) {
    switch (n) {
        case 0: return QString("January");
        case 1: return QString("February");
        case 2: return QString("March");
        case 3: return QString("April");
        case 4: return QString("May");
        case 5: return QString("June");
        case 6: return QString("July");
        case 7: return QString("August");
        case 8: return QString("September");
        case 9: return QString("October");
        case 10: return QString("November");
        case 11: return QString("December");
        default: return QString("WTF?");
    }
}

int month_days_count(int n, int year) {
    if (n == 0 || n == 2 || n == 4 || n == 6 || n == 7 || n == 9 || n == 11) {
        return 31;
    } else if (n == 3 || n == 5 || n == 8 || n == 10) {
        return 30;
    } else {
        return (year % 100 != 0 && year % 4 == 0) || year % 400 == 0 ? 29: 28;
    }
}

inline bool is_leap(int year) {
    return ((year) % 100 != 0 && (year) % 4 == 0) || (year) % 400 == 0;
}

int delta_years(int cur_year, int new_year) {
    int leaped = 0;
    int delta = new_year - cur_year;
    int offset;
    if (delta > 0) {
        --delta;
        for (int i = cur_year + 1; i < new_year; ++i) {
            if (is_leap(i)) {
                ++leaped;
            }
        }
        offset = (delta + leaped) % 7;
    } else {
        for (int i = cur_year; i >= new_year; --i) {
            if (is_leap(i)) {
                ++leaped;
            }
        }
        offset = (delta - leaped) % 7;
    }
    return offset;
}

void draw(QWidget* widget, QGridLayout* layout, int year) {
    bool leap = is_leap(year);
    bool leap1 = is_leap(year + 1);
    int delta = year - previous_year;

    if (delta == -1) {
        first -= 2 + (leap || leap1);
        if (first < 0)
            first = 7 + first;
    }

    while (auto ch = layout->takeAt(0)) {
        ch->setGeometry(QRect(0, 0, 0, 0));
        layout->removeItem(ch);
        delete ch;
    }
    EnterLineEdit* yearedit = new EnterLineEdit("2019", widget, layout);
    QObject::connect(yearedit, SIGNAL(returnPressed()), yearedit, SLOT(wasPressed()));
    layout->addWidget(yearedit, 0, 2);

    QLabel* label = new QLabel("Year");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, 0, 1);

    PreviousYearButton* yearback = new PreviousYearButton("Previous year", yearedit, widget, layout, year, true);
    QObject::connect(yearback, SIGNAL(clicked()), yearback, SLOT(mousePressEvent()));
    layout->addWidget(yearback, 0, 0);

    PreviousYearButton* yearforward = new PreviousYearButton("Next year", yearedit, widget, layout, year, false);
    QObject::connect(yearforward, SIGNAL(clicked()), yearforward, SLOT(mousePressEvent()));
    layout->addWidget(yearforward, 0, 3);

    for (int month_hor = 0; month_hor < 3; ++month_hor) {
        for (int month_vert = 0; month_vert < 4; ++month_vert) {
            int month = 4 * month_hor + month_vert;
            QGroupBox* box = new QGroupBox(month_name(month), widget);
            box->setAlignment(Qt::AlignCenter);
            layout->addWidget(box, month_hor + 1, month_vert);
            QHBoxLayout* boxLayout = new QHBoxLayout(box);

            QWidget* day_name_widget = new QWidget(box);
            boxLayout->addWidget(day_name_widget);
            QVBoxLayout* day_name_layout = new QVBoxLayout(day_name_widget);

            for (int day = 0; day < 7; ++day) {
                day_name_layout->addWidget(new QLabel(day_name(day)));
            }
            int another_first = 1;
            QWidget* day_number = new QWidget(box);
            boxLayout->addWidget(day_number);
            QVBoxLayout* day_number_layout = new QVBoxLayout(day_number);
            for (int pass = 0; pass < first; ++pass) {
                day_number_layout->addWidget(new QLabel(QString("")));
            }
            for (int day = 0; day < 7 - first; ++day){
                ClickableDateLabel* lbl = new ClickableDateLabel(QString::number(another_first), year, month, day);
                QObject::connect(lbl, SIGNAL(clicked()), lbl, SLOT(mousePressEvent()));
                if ((first + day) % 7 > 4 || is_holiday(month, day)) {
                    lbl->setStyleSheet("color: rgb(255, 0, 0)");
                }
                day_number_layout->addWidget(lbl);
                ++another_first;
            }
            for (int week = 0; week < 5; ++week) {
                QWidget* day_number = new QWidget(box);
                boxLayout->addWidget(day_number);
                QVBoxLayout* day_number_layout = new QVBoxLayout(day_number);
                for (int day = 0; day < 7; ++day){
                    bool flag =  another_first <= month_days_count(month, year);
                    ClickableDateLabel* lbl = new ClickableDateLabel(flag?QString::number(another_first):QString(""),
                                                                     year, month, week * 7 + day);
                    QObject::connect(lbl, SIGNAL(clicked()), lbl, SLOT(mousePressEvent()));
                    if (day % 7 > 4 || is_holiday(month, another_first - 1)) {
                        lbl->setStyleSheet("color: rgb(255, 0, 0)");
                    }
                    day_number_layout->addWidget(lbl);
                    if (flag) ++another_first;
                }
            }
            first = (first + another_first - 1) % 7;
        }
    }
    yearedit->setText(QString::number(year));
    previous_year = year;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow window;

    QWidget* widget = new QWidget(&window);
    QGridLayout* layout = new QGridLayout(widget);

    window.setCentralWidget(widget);
    window.setWindowTitle("Calendar");
    widget->setLayout(layout);

    draw(widget, layout, 2019);

    window.show();

    a.exec();
    return 0;
}
