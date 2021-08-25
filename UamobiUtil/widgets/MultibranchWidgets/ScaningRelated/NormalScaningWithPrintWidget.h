#pragma once
#include "widgets/MultibranchWidgets/ScaningRelated/NormalScaningWidget.h"


class SemaphorLabel;
class AbsPrinterWrapper;
class NormalScaningWithPrintWidget : public NormalScaningWidget
{
	Q_OBJECT
protected:
	MegaIconButton* printLabelButton;
	SemaphorLabel* connectionStateLabel;
	AbsPrinterWrapper* printerWrapper;

	void _print(QString text);
	virtual void _makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads) override;

public:
	explicit NormalScaningWithPrintWidget(QWidget* parent = Q_NULLPTR, IndependentBranchNode* resScreen = Q_NULLPTR,
		IndependentBranchNode* searchScr = Q_NULLPTR);

protected slots:
	void labelPrintRequired();
	void onLabelArrived();
	void wrapperOk();
	void wrapperError(QString errtext);
	void reshowError();
};