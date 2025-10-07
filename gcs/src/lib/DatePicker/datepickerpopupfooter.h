#ifndef DATEPICKERPOPUPFOOTER_H
#define DATEPICKERPOPUPFOOTER_H

#include <QWidget>
#include "datepicker_common.h"

class DatePickerPopupFooterPrivate;

class DatePickerPopupFooter : public QWidget {
    Q_OBJECT
    Q_DECLARE_PRIVATE(DatePickerPopupFooter)

    DatePickerPopupFooterPrivate * const d_ptr;
public:
    explicit DatePickerPopupFooter(QWidget *parent = 0);
    ~DatePickerPopupFooter();

    DatePickerType pickerType() const;
signals:
    void datePickerTypeChanged(DatePickerType picker_type);
    void applyClicked();
    void cancelClicked();
public slots:
    void setAllowedPickerTypes(DatePickerTypes picker_types);
    void reset();
private slots:
    void onDatePickerTypeButtonClicked();
    void onApplyButtonClicked();
    void onCancelButtonClicked();
};

#endif // DATEPICKERPOPUPFOOTER_H
