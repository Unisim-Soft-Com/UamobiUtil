#include "CountingDelegate.h"
#include "networking/things.h"
#include "widgets/utils/ElementsStyles.h"
#include "submodules/UNAQtCommons/widgets/Scaling/ScalingUtilities.h"

static QColor shadeOfGray(225, 226, 227);

QRect CountingItemDelegate::drawNameRect(QPainter* painter, const QStyleOptionViewItem& option, bool isOdd) const
{
	QRect textbox(option.rect.topLeft(), QSize(

		option.rect.width() * 0.85, option.rect.height()));
	painter->setPen(Qt::PenStyle::SolidLine);
	if (isOdd)
	{
		painter->setBrush(QBrush(shadeOfGray));
	}
	else
	{
		painter->setBrush(QBrush(Qt::white));
	}
	painter->setFont(option.font);
	painter->drawRect(textbox);
	return textbox;
}
QRect & CountingItemDelegate::drawCountRect(QPainter* painter, const QStyleOptionViewItem& option, QRect &textbox) const
{
	textbox.setTopLeft(textbox.topRight());
	textbox.setBottomRight(option.rect.bottomRight());
	painter->setBrush(StyleManager::instanse()->EnumerablesFieldsColor);
	painter->drawRect(textbox);
	return textbox;
}

void CountingItemDelegate::_drawCountingRect(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index, RecEntity& ent) const
{
	QRect textbox = drawNameRect(painter, option, index.row() & 1);
	painter->drawText(textbox, Qt::AlignLeft | Qt::AlignTop /*| Qt::TextWordWrap*/,
		index.data(DataEntityListModel::DirectAccess).value<RecEntity>()->getTitle());
	drawCountRect(painter, option, textbox);
	painter->setFont(quantityFont);
	painter->drawText(textbox, Qt::AlignCenter | Qt::TextWrapAnywhere, normalizeFloatString(QString::number(ent->getAttachedNumber())));
}

void CountingItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	// drawing rectangle for first date
	RecEntity ent = index.data(DataEntityListModel::DirectAccess).value<RecEntity >();
	if (ent.isNull())
		return;
	painter->save();
	switch (ent->myType())
	{
	default:
	case UniformXmlObject::Separator:
		_drawSeparator(painter, option);
		break;
	case UniformXmlObject::Invoice:
		ZebraItemDelegate::_drawRect(painter, option, index);
		painter->drawText(option.rect, Qt::AlignLeft | Qt::AlignTop /*| Qt::TextWordWrap*/, ent->getTitle());
		break;
	case UniformXmlObject::Item:
		_drawCountingRect(painter, option, index, ent);
		break;
	}
	_drawSelection(painter, option, index);
	painter->restore();
}

CountingItemDelegate::CountingItemDelegate(QObject* parent)
	: ZebraItemDelegate(parent), quantityFont(AppFonts->makeCustomFont(0.03))
{
}
QSize CountingItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	if (index.isValid())
	{
		int hght = (index.data(Qt::SizeHintRole).toSize().height());

		return  QSize(option.rect.width(), hght * option.fontMetrics.height());
	}
	return QItemDelegate::sizeHint(option, index);
}
