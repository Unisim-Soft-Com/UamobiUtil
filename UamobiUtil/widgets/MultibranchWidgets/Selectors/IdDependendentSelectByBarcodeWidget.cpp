#include "IdDependendentSelectByBarcodeWidget.h"
#include "submodules/UNAQtCommons/barcodeHandling/BarcodeObserver.h"
#include <QLineEdit>


IdDependentSelectByBarcodeWidget::IdDependentSelectByBarcodeWidget(RecEntity proto, QWidget*parent)
	: IdDependentSelectWidget(proto, parent)
#ifdef Q_OS_ANDROID
	,
	switchKeyboardTypeButton(new MegaIconButton(this)),
	switchDecorator(new SwitchableIMDecorator(userinputField)),
#endif
	, barcodeLayout(new QHBoxLayout(this))
	, barcodeField(new QLineEdit(this))
{
	mainLayout->insertLayout(1, barcodeLayout);
	barcodeLayout->addWidget(barcodeField);
#ifdef Q_OS_ANDROID
	barcodeLayout->addWidget(switchKeyboardTypeButton);
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
#ifdef QT_VERSION5X
	QObject::connect(barcodeField, &QLineEdit::returnPressed, this, &IdDependentSelectByBarcodeWidget::pickClicked);
#else
	QObject::connect(barcodeField, SIGNAL(returnPressed()), this, SLOT(pickClicked()));
#endif
}

void IdDependentSelectByBarcodeWidget::_sendByIdField()
{
	if (!barcodeField->text().isEmpty())
	{
		RecEntity t_entity(prototype->clone());
		t_entity->setId(barcodeField->text());
		itemSelected(t_entity);
	}
}

void IdDependentSelectByBarcodeWidget::show()
{
	IdDependentSelectWidget::show();
#ifdef QT_VERSION5X
	QObject::connect(BarcodeObs, &BarcodeObserver::barcodeCaught, this, &IdDependentSelectByBarcodeWidget::barcodeCaught);
#else
	QObject::connect(BarcodeObs, SIGNAL(barcodeCaught(QString)), this, SLOT(barcodeCaught(QString)));
#endif
	BarcodeObs->activate();
}

void IdDependentSelectByBarcodeWidget::hide()
{
	IdDependentSelectWidget::hide();
#ifdef QT_VERSION5X
	QObject::disconnect(BarcodeObs, &BarcodeObserver::barcodeCaught, this, &IdDependentSelectByBarcodeWidget::barcodeCaught);
#else
	QObject::disconnect(BarcodeObs, SIGNAL(barcodeCaught(QString)), this, SLOT(barcodeCaught(QString)));
#endif
	BarcodeObs->deactivate();
}

void IdDependentSelectByBarcodeWidget::barcodeCaught(QString barcode)
{
	barcodeField->setText(barcode);
	_sendByIdField();
}

void IdDependentSelectByBarcodeWidget::pickClicked()
{
	if (barcodeField->text().isEmpty())
		IdDependentSelectWidget::pickClicked();
	else
		_sendByIdField();
}
