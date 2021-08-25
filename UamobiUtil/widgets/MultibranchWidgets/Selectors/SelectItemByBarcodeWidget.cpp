#include "SelectItemByBarcodeWidget.h"
#include "submodules/UNAQtCommons/barcodeHandling/BarcodeObserver.h"

SelectItemByBarcodeWidget::SelectItemByBarcodeWidget(QWidget* parent, RecEntity proto)
	: SelectItemFromListWidget(parent, proto)
#ifdef Q_OS_ANDROID
	,
	switchKeyboardTypeButton(new MegaIconButton(this)),
	switchDecorator(new SwitchableIMDecorator(userinputField)),
#endif
{
#ifdef Q_OS_ANDROID
	headerLayout->insertWidget(1, switchKeyboardTypeButton);
	QVector<Qt::InputMethodHints> hints;
	hints.push_back(Qt::ImhDigitsOnly);
	hints.push_back(Qt::ImhNone);
	switchDecorator->initiate(hints, true);
	switchKeyboardTypeButton->setIcon(QIcon(":/resources/key"));
	switchKeyboardTypeButton->setMaximumHeight(barcodeField->maximumHeight());
	switchKeyboardTypeButton->setMinimumHeight(barcodeField->minimumHeight());
	switchKeyboardTypeButton->setMinimumWidth(calculateAdaptiveWidth(0.1));
	QObject::connect(switchKeyboardTypeButton, &QPushButton::clicked, switchDecorator, &SwitchableIMDecorator::nextIM);
#endif
}

void SelectItemByBarcodeWidget::show()
{
	SelectItemFromListWidget::show();
#ifdef QT_VERSION5X
	QObject::connect(BarcodeObs, &BarcodeObserver::barcodeCaught, this, &SelectItemByBarcodeWidget::barcodeCaught);
#else
	QObject::connect(BarcodeObs, SIGNAL(barcodeCaught(QString)), this, SLOT(barcodeCaught(QString)));
#endif
	BarcodeObs->activate();
}

void SelectItemByBarcodeWidget::hide()
{
	SelectItemFromListWidget::hide();
#ifdef QT_VERSION5X
	QObject::disconnect(BarcodeObs, &BarcodeObserver::barcodeCaught, this, &SelectItemByBarcodeWidget::barcodeCaught);
#else
	QObject::disconnect(BarcodeObs, SIGNAL(barcodeCaught(QString)), this, SLOT(barcodeCaught(QString)));
#endif
	BarcodeObs->deactivate();
}

void SelectItemByBarcodeWidget::barcodeCaught(QString barcode)
{
	userinputField->setText(barcode);
	searchPrimed();
}
