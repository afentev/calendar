#include <QLabel>
#include <QMouseEvent>
#include <QGridLayout>
#include <QMainWindow>
#include <QTextEdit>
#include <string>
#include <vector>
#include "inc/ClickableLabel.h"

void draw(QWidget* widget, QGridLayout* layout, int year);
int delta_years(int cur_year, int new_year);

std::vector<std::string>* notes = new std::vector<std::string>;

ClickableDateLabel::ClickableDateLabel(QString text, int yyear, int mmonth, int dday, QWidget* parent) : QLabel(parent)
{
    year = yyear;
    month = mmonth;
    day = dday;
    setCursor(Qt::PointingHandCursor);
    setText(text);
}

ClickableDateLabel::~ClickableDateLabel() = default;

void ClickableDateLabel::mousePressEvent(QMouseEvent* event)
{
    QWidget *wdg = new QWidget;
    QGridLayout* layout = new QGridLayout(wdg);
    QTextEdit* txt = new QTextEdit;
    QPushButton* btn = new SaveThinkButton("Save", wdg, txt,year, month, day);

    wdg->setLayout(layout);
    wdg->setWindowTitle("Add note...");
    layout->addWidget(txt);
    layout->addWidget(btn);

    int date = day + month * 31 + year * 365;
    std::string str_date = std::to_string(date);
    while (str_date.length() < 12) {
        str_date.insert(str_date.begin(), '0');
    }

    for (int i = 0; i < notes->size(); ++i) {
        std::string data = notes->at(i);
        std::string code = data.substr(0, 12);
        if (code == str_date) {
            txt->setText(QString::fromStdString(data.substr(12, data.length() - 12)));
        }
    }

    wdg->show();
}


PreviousYearButton::PreviousYearButton(QString text, QLineEdit* lline,
        QWidget* wwidget, QGridLayout* llayout, int yyear, bool iss_prev) : QPushButton(0)
{
    widget = wwidget;
    layout = llayout;
    is_prev = iss_prev;
    if (is_prev) year = yyear - 1;
    else year = yyear + 1;
    line = lline;
    setCursor(Qt::PointingHandCursor);
    setText(text);
}

PreviousYearButton::~PreviousYearButton() = default;

void PreviousYearButton::mousePressEvent(QMouseEvent* event)
{
    draw(widget, layout, year);
    if (is_prev) --year;
    else ++year;
}

EnterLineEdit::EnterLineEdit(QString text, QWidget* wwidget, QGridLayout* llayout) : QLineEdit(0)
{
    widget = wwidget;
    layout = llayout;
    setText(text);
}

EnterLineEdit::~EnterLineEdit() = default;

void EnterLineEdit::wasPressed()
{
    std::string text = this->text().toStdString();
    if (!text.empty() && std::find_if(text.begin(),
        text.end(), [](char c) { return !std::isdigit(c); }) == text.end()) {
        if (text.length() <= 9) {
            int string2int = std::stoi(text);
            extern int previous_year, first;
            if (string2int - previous_year == 0) {
                return;
            }
            int dd = delta_years(previous_year, string2int);
            if (previous_year > string2int)
                --first;
            first += dd;
            if (first < 0) {
                first = 7 + first;
            }
            first %= 7;
            previous_year = -3;
            draw(widget, layout, string2int);
        }
    }
}

SaveThinkButton::SaveThinkButton(QString text, QWidget *wdgg, QTextEdit* txtt, int yyear, int mmonth,
                                 int dday) : QPushButton(0)
{
    txt = txtt;
    wdg = wdgg;
    year = yyear;
    month = mmonth;
    day = dday;
    setCursor(Qt::PointingHandCursor);
    setText(text);
}

SaveThinkButton::~SaveThinkButton() = default;

void SaveThinkButton::mousePressEvent(QMouseEvent* event)
{
    int date = day + month * 31 + year * 365;
    std::cout << notes->size() << " ";
    std::string str_date = std::to_string(date);
    while (str_date.length() < 12) {
        str_date.insert(str_date.begin(), '0');
    }
    std::string text = txt->toPlainText().toStdString();
    for (int i = 0; i < notes->size(); ++i) {
        std::string data = notes->at(i);
        std::string code = data.substr(0, 12);
        if (code == str_date) {
            if (!text.empty()) {
                notes->at(i) = str_date + text;
            } else {
                notes->erase(notes->begin() + i);
            }
            std::cout << notes->size() << std::endl;
            wdg->deleteLater();
            return;
        }
    }
    if (!text.empty()) {
        notes->push_back(str_date + text);
    }
    std::cout << notes->size() << std::endl;
    wdg->deleteLater();
}
