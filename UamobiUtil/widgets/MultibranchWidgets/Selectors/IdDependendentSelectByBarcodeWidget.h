#pragma once
#include "widgets/MultibranchWidgets/IdDependentSelectWidget.h"



class IdDependentSelectByBarcodeWidget : public IdDependentSelectWidget
{
	Q_OBJECT
protected:
#ifdef Q_OS_ANDROID
	MegaIconButton* switchKeyboardTypeButton;
	SwitchableIMDecorator* switchDecorator;
#endif
	QHBoxLayout* barcodeLayout;
	QLineEdit* barcodeField;

	void _sendByIdField();
public:
	IdDependentSelectByBarcodeWidget(RecEntity proto, QWidget* = Q_NULLPTR);
	virtual void show() override;
	virtual void hide() override;

protected slots:
	void barcodeCaught(QString barcode);
	virtual void pickClicked() override;
};