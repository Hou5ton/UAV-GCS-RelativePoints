#include "PatternConvertor.h"

void PatternConvertor::Convert(MImage *mImg, QImage &qImg)
{
	QSize size(mImg->width(), mImg->height());
	qImg = QImage(size, (QImage::Format)mImg->format());

	qImg.setColorCount(mImg->numColors());
	mImg->getColorTable((uint *)qImg.colorTable().data());
	mImg->getBits(qImg.bits());
	qImg.setDotsPerMeterX(mImg->dotsPerMeterX());
	qImg.setDotsPerMeterY(mImg->dotsPerMeterY());
}

void PatternConvertor::Convert(QImage qImg, MImage *mImg)
{
	mImg->setWidth(qImg.width());
	mImg->setHeight(qImg.height());
	mImg->setFormat(qImg.format());
    mImg->setBits(qImg.bits(), qImg.byteCount());
	mImg->setColorTable((uint *) qImg.colorTable().data(), qImg.colorCount());
	mImg->setDotsPerMeterX(qImg.dotsPerMeterX());
	mImg->setDotsPerMeterY(qImg.dotsPerMeterY());
}
