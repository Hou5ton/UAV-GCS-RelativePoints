#ifndef FLOATINGWIDGET_H
#define FLOATINGWIDGET_H
#include <QWidget>
#include <QLayout>

class FloatingWidgetEventFilter;

class FloatingWidget : public QWidget
{
    Q_OBJECT

public:
    enum AnchorPoint
    {
        TopLeft, //!< Top-left of widget
        TopMiddle, //!< Top center of widget
        TopRight, //!< Top-right of widget
        MiddleLeft, //!< Middle left of widget
        Middle, //!< Middle of widget
        MiddleRight, //!< Middle right of widget
        BottomLeft, //!< Bottom-left of widget
        BottomMiddle, //!< Bottom center of widget
        BottomRight, //!< Bottom-right of widget
    };

    FloatingWidget( QWidget *parent  = nullptr );
    void setAnchorWidget( QWidget *widget );

    QWidget *anchorWidget();

    AnchorPoint anchorPoint() const;

    void setAnchorPoint( AnchorPoint point );

    AnchorPoint anchorWidgetPoint() const;

    void setAnchorWidgetPoint( AnchorPoint point );

    virtual void resizeFloatingWidget();

signals:

    //! Emitted when the anchor widget changes
    void anchorWidgetChanged( QWidget *widget );

    //! Emitted when the anchor point changes
    void anchorPointChanged( FloatingWidget::AnchorPoint point );

    //! Emitted when the anchor widget point changes
    void anchorWidgetPointChanged( FloatingWidget::AnchorPoint point );

protected:
    void showEvent( QShowEvent *e ) override;

    void paintEvent( QPaintEvent *e ) override;

    void resizeEvent( QResizeEvent *e ) override;

public slots:

    //! Repositions the floating widget to a changed anchor point
    void onAnchorPointChanged();

private:
    QWidget *mAnchorWidget = nullptr;
    FloatingWidgetEventFilter *mParentEventFilter = nullptr;
    FloatingWidgetEventFilter *mAnchorEventFilter = nullptr;
    AnchorPoint mFloatAnchorPoint = BottomMiddle;
    AnchorPoint mAnchorWidgetAnchorPoint = TopMiddle;

};

class FloatingWidgetEventFilter: public QObject
{
    Q_OBJECT

public:

    FloatingWidgetEventFilter( QWidget *parent = nullptr );

    bool eventFilter( QObject *object, QEvent *event ) override;

signals:
    //! Emitted when the filter's parent is moved or resized
    void anchorPointChanged();
};


void createFloatingWdgt(FloatingWidget** pFloating,
                        QWidget **form, QWidget* anchorWidget,
                        FloatingWidget::AnchorPoint _anchorPoint,
                        FloatingWidget::AnchorPoint _anchorWidgetPoint,
                        QWidget *parent);

#endif // FLOATINGWIDGET_H
