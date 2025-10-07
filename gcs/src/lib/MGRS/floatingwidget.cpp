#include "floatingwidget.h"
#include <QStyleOption>
#include <QEvent>
#include <QPainter>

FloatingWidget::FloatingWidget(QWidget *parent)
    : QWidget( parent )
{
    if ( parent )
    {
        mParentEventFilter = new FloatingWidgetEventFilter( parent );
        parent->installEventFilter( mParentEventFilter );
        connect( mParentEventFilter, &FloatingWidgetEventFilter::anchorPointChanged, this, &FloatingWidget::onAnchorPointChanged );
    }
}

void FloatingWidget::setAnchorWidget(QWidget *widget)
{
    if ( widget == mAnchorWidget )
        return;

    // remove existing event filter
    if ( mAnchorWidget )
    {
        mAnchorWidget->removeEventFilter( mAnchorEventFilter );
        delete mAnchorEventFilter;
        mAnchorEventFilter = nullptr;
    }

    mAnchorWidget = widget;
    if ( mAnchorWidget )
    {
        mAnchorEventFilter = new FloatingWidgetEventFilter( mAnchorWidget );
        mAnchorWidget->installEventFilter( mAnchorEventFilter );
        connect( mAnchorEventFilter, &FloatingWidgetEventFilter::anchorPointChanged, this, &FloatingWidget::onAnchorPointChanged );
        //onnect( mAnchorEventFilter, &FloatingWidgetEventFilter::focusChanged, this, &FloatingWidget::focusChanged );
    }

    onAnchorPointChanged();
    emit anchorWidgetChanged( mAnchorWidget );
}

QWidget *FloatingWidget::anchorWidget()
{
    return mAnchorWidget;
}

FloatingWidget::AnchorPoint FloatingWidget::anchorPoint() const
{
    return mFloatAnchorPoint;
}

void FloatingWidget::setAnchorPoint(FloatingWidget::AnchorPoint point)
{
    if ( point == mFloatAnchorPoint )
        return;

    mFloatAnchorPoint = point;
    onAnchorPointChanged();
    emit anchorPointChanged( mFloatAnchorPoint );
}

FloatingWidget::AnchorPoint FloatingWidget::anchorWidgetPoint() const
{
    return mAnchorWidgetAnchorPoint;
}

void FloatingWidget::setAnchorWidgetPoint(FloatingWidget::AnchorPoint point)
{
    if ( point == mAnchorWidgetAnchorPoint )
        return;

    mAnchorWidgetAnchorPoint = point;
    onAnchorPointChanged();
    emit anchorWidgetPointChanged( mAnchorWidgetAnchorPoint );
}

void FloatingWidget::resizeFloatingWidget()
{
    int w = mAnchorWidget->width();
    resize(w, w/4);
}

void FloatingWidget::showEvent(QShowEvent *e)
{
    QWidget::showEvent( e );
    onAnchorPointChanged();
}

void FloatingWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED( e );
    QStyleOption opt;
    opt.init( this );
    QPainter p( this );
    style()->drawPrimitive( QStyle::PE_Widget, &opt, &p, this );
}

void FloatingWidget::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent( e );
    onAnchorPointChanged();
}

void FloatingWidget::onAnchorPointChanged()
{
    QWidget* pW = parentWidget();
    if ( ! pW)
        return;

    if ( mAnchorWidget )
    {
        QPoint anchorWidgetOrigin;

        switch ( mAnchorWidgetAnchorPoint )
        {
        case TopLeft:
            anchorWidgetOrigin = QPoint( 0, 0 );
            break;
        case TopMiddle:
            anchorWidgetOrigin = QPoint( mAnchorWidget->width() / 2, 0 );
            break;
        case TopRight:
            anchorWidgetOrigin = QPoint( mAnchorWidget->width(), 0 );
            break;
        case MiddleLeft:
            anchorWidgetOrigin = QPoint( 0, mAnchorWidget->height() / 2 );
            break;
        case Middle:
            anchorWidgetOrigin = QPoint( mAnchorWidget->width() / 2, mAnchorWidget->height() / 2 );
            break;
        case MiddleRight:
            anchorWidgetOrigin = QPoint( mAnchorWidget->width(), mAnchorWidget->height() / 2 );
            break;
        case BottomLeft:
            anchorWidgetOrigin = QPoint( 0, mAnchorWidget->height() );
            break;
        case BottomMiddle:
            anchorWidgetOrigin = QPoint( mAnchorWidget->width() / 2, mAnchorWidget->height() );
            break;
        case BottomRight:
            anchorWidgetOrigin = QPoint( mAnchorWidget->width(), mAnchorWidget->height() );
            break;
        }

        anchorWidgetOrigin = mAnchorWidget->mapTo( parentWidget(), anchorWidgetOrigin );
        int anchorX = anchorWidgetOrigin.x();
        int anchorY = anchorWidgetOrigin.y();

        switch ( mFloatAnchorPoint )
        {
        case TopLeft:
            break;
        case TopMiddle:
            anchorX = anchorX - width() / 2;
            break;
        case TopRight:
            anchorX = anchorX - width();
            break;
        case MiddleLeft:
            anchorY = anchorY - height() / 2;
            break;
        case Middle:
            anchorY = anchorY - height() / 2;
            anchorX = anchorX - width() / 2;
            break;
        case MiddleRight:
            anchorX = anchorX - width();
            anchorY = anchorY - height() / 2;
            break;
        case BottomLeft:
            anchorY = anchorY - height();
            break;
        case BottomMiddle:
            anchorX = anchorX - width() / 2;
            anchorY = anchorY - height();
            break;
        case BottomRight:
            anchorX = anchorX - width();
            anchorY = anchorY - height();
            break;
        }

        // constrain x so that widget floats within parent widget
        anchorX = qBound( 0, anchorX, parentWidget()->width() - width() );

        resizeFloatingWidget();

        move( anchorX, anchorY );
    }
}

FloatingWidgetEventFilter::FloatingWidgetEventFilter(QWidget *parent)
    : QObject( parent )
{

}

bool FloatingWidgetEventFilter::eventFilter(QObject *object, QEvent *event)
{
    switch ( event->type() )
    {
    case QEvent::Move:
    case QEvent::Resize:
        emit anchorPointChanged();
        return false;
    default:
        return false;
    }
}

//void createFloatingWdgt(FloatingWidget **pFloating, QWidget **form, QWidget *anchorWidget, FloatingWidget::AnchorPoint _anchorPoint,
//                        FloatingWidget::AnchorPoint _anchorWidgetPoint, QWidget *parent)
//{
//    if(!form) return;
//    if(!parent) return;

//    if(*pFloating)
//    {
//        delete *pFloating;
//        *pFloating = nullptr;
//    }

////    (*form)->setWindowFlag(Qt::Widget);
////    (*form)->layout()->setContentsMargins(0, 0, 0, 0);

//    (*pFloating) = new FloatingWidget(parent);
//    (*pFloating)->setAnchorWidget(anchorWidget);
//    (*pFloating)->setAnchorPoint(_anchorPoint);
//    (*pFloating)->setAnchorPoint(_anchorWidgetPoint);

//    QHBoxLayout *containerLayout = new QHBoxLayout();
//    containerLayout->setMargin(0);
//    containerLayout->setContentsMargins(0, 0, 0, 0);
//    containerLayout->addWidget(*form);
//    (*pFloating)->setLayout(containerLayout);
//    (*pFloating)->hide();
//}
