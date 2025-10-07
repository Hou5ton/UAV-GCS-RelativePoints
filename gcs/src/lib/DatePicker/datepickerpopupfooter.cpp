#include "datepickerpopupfooter.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>


class DatePickerPopupFooterPrivate {
    Q_DECLARE_PUBLIC(DatePickerPopupFooter)

    DatePickerPopupFooter *q_ptr;
    DatePickerTypes allowed_picker_types;
    QPushButton  *apply_button;
    QPushButton  *cancel_button;

    DatePickerPopupFooterPrivate(DatePickerPopupFooter *q) :
        q_ptr(q), allowed_picker_types(/*DayType |*/ PeriodType) {}
    ~DatePickerPopupFooterPrivate() {}

    void initUi()
    {
        Q_Q(DatePickerPopupFooter);

        apply_button = new QPushButton(QObject::tr("Apply"), q);
        cancel_button = new QPushButton(QObject::tr("Cancel"), q);

        QHBoxLayout *buttons_layout = new QHBoxLayout();
        buttons_layout->addStretch();
        buttons_layout->addWidget(apply_button);
        buttons_layout->addWidget(cancel_button);

        QVBoxLayout *main_layout = new QVBoxLayout();
        main_layout->setContentsMargins(QMargins(3, 6, 3, 6));
        main_layout->setSpacing(6);

        QFrame *top_line = new QFrame(q);
        top_line->setFrameStyle(QFrame::HLine | QFrame::Plain);

        main_layout->addWidget(top_line);
        main_layout->addLayout(buttons_layout);

        q->setLayout(main_layout);

        q->setAllowedPickerTypes(allowed_picker_types);
    }
};


DatePickerPopupFooter::DatePickerPopupFooter(QWidget *parent) :
    QWidget(parent),
    d_ptr(new DatePickerPopupFooterPrivate(this))
{
    setObjectName("datepicker_popup_footer");
    Q_D(DatePickerPopupFooter);
    d->initUi();
    connect(d->apply_button, &QPushButton::clicked, this, &DatePickerPopupFooter::onApplyButtonClicked);
    connect(d->cancel_button, &QPushButton::clicked, this, &DatePickerPopupFooter::onCancelButtonClicked);
}

DatePickerPopupFooter::~DatePickerPopupFooter()
{
    delete d_ptr;
}

DatePickerType DatePickerPopupFooter::pickerType() const
{
    return PeriodType;
}

void DatePickerPopupFooter::setAllowedPickerTypes(DatePickerTypes picker_types)
{
    Q_D(DatePickerPopupFooter);
    d->allowed_picker_types = picker_types;
    onDatePickerTypeButtonClicked();
}

void DatePickerPopupFooter::reset()
{
    Q_D(DatePickerPopupFooter);
    setAllowedPickerTypes(d->allowed_picker_types);
}

void DatePickerPopupFooter::onDatePickerTypeButtonClicked()
{
    DatePickerType picker_type = PeriodType;
    emit datePickerTypeChanged(picker_type);
}
void DatePickerPopupFooter::onApplyButtonClicked()
{
    emit applyClicked();
}
void DatePickerPopupFooter::onCancelButtonClicked()
{
    emit cancelClicked();
}
