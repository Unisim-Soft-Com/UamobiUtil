#pragma once
#include "widgets/MultibranchWidgets/SelectItemFromListWidget.h"



class SelectItemByBarcodeWidget : public SelectItemFromListWidget
{
	Q_OBJECT
protected:
#ifdef Q_OS_ANDROID
	MegaIconButton* switchKeyboardTypeButton;
	SwitchableIMDecorator* switchDecorator;
#endif

public:
	SelectItemByBarcodeWidget(QWidget* parent,
		RecEntity proto);
	virtual void show() override;
	virtual void hide() override;

protected slots:
	void barcodeCaught(QString barcode);
	
};