#include "datepickercalendar.h"
#include <QVBoxLayout>
#include <QCalendarWidget>
#include <QStackedWidget>
#include <QFrame>
#include <QLabel>
#include "datepickercalendarnavigator.h"
#include "datepickercalendarmonthview.h"
#include "datepickercalendaryearview.h"
#include "datepickercalendardecadeview.h"


class DatePickerCalendarPrivate {
    Q_DECLARE_PUBLIC(DatePickerCalendar)

    DatePickerCalendar *q_ptr;
    DatePickerCalendarNavigator *navigator;
    QStackedWidget *view_stack;
    //QLabel *label;
    QFrame *hline_top;
    QFrame *hline_bottom;
    DatePickerCalendarMonthView *month_view;
    DatePickerCalendarYearView *year_view;
    DatePickerCalendarDecadeView *decade_view;
    bool is_period_shown;
    bool is_period_begin_shown;
    QDate date_begin;
    QDate date_end;
    QDate minimum_date;
    QDate maximum_date;

    DatePickerCalendarPrivate(DatePickerCalendar *q) :
        q_ptr(q), is_period_shown(false), is_period_begin_shown(false) {}
    ~DatePickerCalendarPrivate() {}

    void initUi()
    {
        Q_Q(DatePickerCalendar);

        view_stack = new QStackedWidget(q);
        view_stack->setContentsMargins(QMargins());
        view_stack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        navigator = new DatePickerCalendarNavigator(q);
        navigator->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        month_view = new DatePickerCalendarMonthView(view_stack);
        year_view = new DatePickerCalendarYearView(view_stack);
        decade_view = new DatePickerCalendarDecadeView(view_stack);

        view_stack->insertWidget(MonthView, month_view);
        view_stack->insertWidget(YearView, year_view);
        view_stack->insertWidget(DecadeView, decade_view);

        view_stack->setCurrentIndex(MonthView);

        view_stack->setFixedSize(month_view->minimumSizeHint());

        /*label = new QLabel(q);
        label->setObjectName("label");
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        label->setAlignment(Qt::AlignCenter);
        label->setText(q->m_label);*/


        hline_top = new QFrame(q);
        hline_top->setFrameStyle(QFrame::HLine);

        hline_bottom = new QFrame(q);
        hline_bottom->setFrameStyle(QFrame::HLine);

        QVBoxLayout *main_layout = new QVBoxLayout(q);
        main_layout->setContentsMargins(QMargins());
        main_layout->setSpacing(1);
        //main_layout->addWidget(label);
        main_layout->addWidget(hline_top);
        main_layout->addWidget(navigator);
        main_layout->addWidget(view_stack, 1);
        main_layout->addWidget(hline_bottom);

        navigator->setView(MonthView);
        navigator->setDate(month_view->selectedDate());

        q->setLayout(main_layout);
    }

    void scroll(int direction)
    {
        Q_Q(DatePickerCalendar);

        switch (navigator->view()) {
            case MonthView:
                if (is_period_shown) {
                    if (is_period_begin_shown){
                        QDate date_begin_ = date_begin;
                        date_begin_.setDate(date_begin_.year(),date_begin_.month(),1);
                        date_begin_ = date_begin_.addMonths(1 * direction);
                        if(date_begin_ <= date_end){
                            q->setPeriod(date_begin_, date_end, is_period_begin_shown);
                        }
                    }else{
                        QDate date_end_ = date_end;
                        date_end_.setDate(date_end_.year(),date_end_.month(),date_end_.daysInMonth());
                        date_end_ = date_end_.addMonths(1 * direction);
                        if(date_end_ >= date_begin &&
                                date_end_.month() <= maximum_date.month()){
                            q->setPeriod(date_begin, date_end_, is_period_begin_shown);
                        }
                    }
                }
                else {
                    q->setDate(date_begin.addMonths(1 * direction));
                }
                break;
            case YearView:
                if (is_period_shown) {
                    if (is_period_begin_shown){
                        QDate date_begin_ = date_begin;
                        date_begin_.setDate(date_begin_.year(),date_begin_.month(),1);
                        date_begin_ = date_begin_.addYears(1 * direction);
                        if(date_begin_ <= date_end){
                            q->setPeriod(date_begin_, date_end, is_period_begin_shown);
                        }
                    }else{
                        QDate date_end_ = date_end;
                        date_end_.setDate(date_end_.year(),date_end_.month(),date_end_.daysInMonth());
                        date_end_ = date_end_.addYears(1 * direction);
                        if(date_end_ >= date_begin &&
                                date_end_.month() <= maximum_date.month()){
                            q->setPeriod(date_begin, date_end_, is_period_begin_shown);
                        }
                    }
                }
                else {
                    q->setDate(date_begin.addYears(1 * direction));
                }
                break;
            case DecadeView:
                if (is_period_shown) {
                    if (is_period_begin_shown){
                        QDate date_begin_ = date_begin;
                        date_begin_.setDate(date_begin_.year(),date_begin_.month(),1);
                        date_begin_ = date_begin_.addYears(10 * direction);
                        if(date_begin_ <= date_end){
                            q->setPeriod(date_begin_, date_end, is_period_begin_shown);
                        }
                    }else{
                        QDate date_end_ = date_end;
                        date_end_.setDate(date_end_.year(),date_end_.month(),date_end_.daysInMonth());
                        date_end_ = date_end_.addYears(10 * direction);
                        if(date_end_ >= date_begin){
                            q->setPeriod(date_begin, date_end_, is_period_begin_shown);
                        }
                    }
                }
                else {
                    q->setDate(date_begin.addYears(10 * direction));
                }
                break;
            default:
                break;
        }
    }

    void limitMinimum(const QDate &min_date)
    {
        month_view->setMinimumDate(min_date.isValid() ? min_date : QDate::fromJulianDay(1));
        year_view->setMinimumDate(min_date);
        decade_view->setMinimumDate(min_date);
    }

    void limitMaximum(const QDate &max_date)
    {
        month_view->setMaximumDate(max_date.isValid() ? max_date : QDate(7999, 12, 31));
        year_view->setMaximumDate(max_date);
        decade_view->setMaximumDate(max_date);
    }
};

DatePickerCalendar::DatePickerCalendar(QWidget *parent, QString label) :
    QWidget(parent),
    d_ptr(new DatePickerCalendarPrivate(this))
{
    Q_D(DatePickerCalendar);
    m_label = label;
    d->initUi();

    connect(d->navigator, SIGNAL(viewChanged(DatePickerView)), SLOT(setView(DatePickerView)));
    connect(d->navigator, SIGNAL(toPrevious()), SLOT(previous()));
    connect(d->navigator, SIGNAL(toNext()), SLOT(next()));

    connect(d->month_view, SIGNAL(dateClicked(QDate)), SIGNAL(dateSelected(QDate)));
    connect(d->month_view, SIGNAL(currentPageChanged(int,int)), SLOT(onCurrentMonthChanged(int,int)));
    connect(d->year_view, SIGNAL(monthClicked(int)), SLOT(onYearViewMonthClicked(int)));
    connect(d->decade_view, SIGNAL(yearClicked(int)), SLOT(onDecadeViewYearClicked(int)));
}

DatePickerCalendar::~DatePickerCalendar()
{
    delete d_ptr;
}

QDate DatePickerCalendar::selectedDate() const
{
    Q_D(const DatePickerCalendar);
    return d->month_view->selectedDate();
}

void DatePickerCalendar::setFrameVisible(bool on)
{
    Q_D(DatePickerCalendar);
    d->hline_top->setVisible(on);
    d->hline_bottom->setVisible(on);
}

void DatePickerCalendar::setView(DatePickerView picker_view)
{
    Q_D(DatePickerCalendar);
    d->navigator->setView(picker_view);
    d->view_stack->setCurrentIndex(picker_view);
}

void DatePickerCalendar::setMinimumDate(const QDate &date)
{
    Q_D(DatePickerCalendar);
    d->minimum_date = date;
    d->month_view->setMinimumDate(date.isValid() ? date : QDate::fromJulianDay(1));
    d->year_view->setMinimumDate(date);
    d->decade_view->setMinimumDate(date);
}

void DatePickerCalendar::setMaximumDate(const QDate &date)
{
    Q_D(DatePickerCalendar);
    d->maximum_date = date;
    d->month_view->setMaximumDate(date.isValid() ? date : QDate(7999, 12, 31));
    d->year_view->setMaximumDate(date);
    d->decade_view->setMaximumDate(date);
}

void DatePickerCalendar::setDate(const QDate &date)
{
    Q_D(DatePickerCalendar);
    d->navigator->setDate(date);
    d->month_view->setDate(date);
    d->year_view->setDate(date);
    d->decade_view->setDate(date);

    d->date_begin = date;
    d->date_end = date;
    d->is_period_shown = false;
    d->is_period_begin_shown = false;

    // FIXME: why?????
//    d->limitMinimum(QDate());
//    d->limitMaximum(QDate());
}

void DatePickerCalendar::setDatePeriodFull(const QDate &begin, const QDate &end)
{
    Q_D(DatePickerCalendar);
    d->month_view->setDatePeriodFull(begin, end);
}

void DatePickerCalendar::setPeriod(const QDate &begin, const QDate &end, bool shows_begin)
{
    Q_D(DatePickerCalendar);
    QDate shown_date = shows_begin ? begin : end;
    d->navigator->setDate(shown_date);
    d->month_view->setPeriod(begin, end, shows_begin);
    d->year_view->setDate(shown_date);
    d->decade_view->setDate(shown_date);

    d->date_begin = begin;
    d->date_end = end;
    d->is_period_shown = true;
    d->is_period_begin_shown = shows_begin;

    if (d->is_period_begin_shown) {
        d->limitMinimum(d->minimum_date);
        d->limitMaximum(end);
    }
    else {
        d->limitMinimum(begin);
        d->limitMaximum(d->maximum_date);
    }
}

void DatePickerCalendar::previous()
{
    Q_D(DatePickerCalendar);
    d->scroll(-1);
}

void DatePickerCalendar::next()
{
    Q_D(DatePickerCalendar);
    d->scroll(1);
}

void DatePickerCalendar::reset()
{
    Q_D(DatePickerCalendar);
    d->navigator->reset();
}

void DatePickerCalendar::onYearViewMonthClicked(int month)
{
    Q_D(DatePickerCalendar);

    QDate current_date = d->navigator->date();
    current_date.setDate(current_date.year(), month, current_date.day());

    if (d->is_period_shown) {
        if (d->is_period_begin_shown)
            setPeriod(current_date, d->date_end, d->is_period_begin_shown);
        else
            setPeriod(d->date_begin, current_date, d->is_period_begin_shown);
    }
    else {
        setDate(current_date);
    }

    setView(MonthView);
}

void DatePickerCalendar::onDecadeViewYearClicked(int year)
{
    Q_D(DatePickerCalendar);

    QDate current_date = d->navigator->date();
    current_date.setDate(year, current_date.month(), current_date.day());

    if (d->is_period_shown) {
        if (d->is_period_begin_shown)
            setPeriod(current_date, d->date_end, d->is_period_begin_shown);
        else
            setPeriod(d->date_begin, current_date, d->is_period_begin_shown);
    }
    else {
        setDate(current_date);
    }

    setView(YearView);
}

void DatePickerCalendar::onCurrentMonthChanged(int year, int month)
{
    Q_UNUSED(year);
    Q_UNUSED(month);

    QDate selected_date = selectedDate();

    emit scrolledTo(selected_date);
}
