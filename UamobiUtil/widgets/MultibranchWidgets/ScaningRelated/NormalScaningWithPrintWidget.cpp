#include "NormalScaningWithPrintWidget.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#include "widgets/utils/ElementsStyles.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedLabels.h"
#include "submodules/UNAQtCommons/widgets/UtilityElements/ExtendedDialogs.h"
#include "submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/AbsPrinterWrapper.h"
#include "submodules/UNAQtCommons/wrappers/DirectConnectionPrinters/PrinterWrapperFactory.h"


void NormalScaningWithPrintWidget::_print(QString text)
{
	if (printerWrapper->isValid() && printerWrapper->isReady())
	{
		printerWrapper->print(text);
	}
	else
	{
		printerWrapper->retryOpeningConnection();
	}
}

void NormalScaningWithPrintWidget::_makeOverloads(const QVector<QueryTemplates::OverloadableQuery>& overloads)
{
	NormalScaningWidget::_makeOverloads(overloads);
	if (overloads.count() >= 5)
	{
		QStringList t;
		t << "barcode" << "quantity" << "printer";
		localCache.insert(docGetItemLabel, overloads.at(4).assertedAndMappedCopy(
			addIdToParentDocs, t, t
		));
	}
	else
	{
		QStringList t;
		t << "barcode" << "quantity" << "printer";
		localCache.insert(docGetItemLabel, OverloadableQuery(
			addIdToParentDocs, t, t
		));
	}
}

NormalScaningWithPrintWidget::NormalScaningWithPrintWidget(QWidget* parent, IndependentBranchNode* resScreen, IndependentBranchNode* searchScr)
	: NormalScaningWidget(parent, resScreen, searchScr),
	printLabelButton(new MegaIconButton(this)),
	connectionStateLabel(new SemaphorLabel(this)),
	printerWrapper(PrinterWrapperFactory::fabricate(this))
{
#ifdef DEBUG
	detrace_DCONSTR("NormalScaningWithPrintWidget");
#endif
	buttonPanel->insertWidget(2, printLabelButton);
	topPanelLayout->addWidget(connectionStateLabel);
	printLabelButton->setText(tr("print"));
	printLabelButton->setStyleSheet(NAVIGATE_BUTTONS_STYLESHEET);
	printLabelButton->setIcon(QIcon(":/res/text_files"));
	connectionStateLabel->setText(tr("status"));

#ifdef QT_VERSION5X
	QObject::connect(printerWrapper, &AbsPrinterWrapper::error, this, &NormalScaningWithPrintWidget::wrapperError);
	QObject::connect(printerWrapper, &AbsPrinterWrapper::connected, this, &NormalScaningWithPrintWidget::wrapperOk);
	QObject::connect(printLabelButton, &QPushButton::clicked, this, &NormalScaningWithPrintWidget::labelPrintRequired);
#else
	QObject::connect(printerWrapper, SIGNAL(error(QString)), this, SLOT(wrapperError(QString)));
	QObject::connect(printerWrapper, SIGNAL(connected()), this, SLOT(wrapperOk()));
	QObject::connect(printLabelButton, SIGNAL(clicked()), this, SLOT(labelPrintRequired()));
#endif
	printerWrapper->establishConnection();
}

void NormalScaningWithPrintWidget::labelPrintRequired()
{
	if (awaiter->isAwaiting())
	{
		return;
	}
	showProcessingOverlay();
	QStringList buffer;
	buffer << barcodeField->text();
	buffer << "1";
	buffer << AppSettings->printerType;
	AppNetwork->execQueryByTemplate(localCache[docGetItemLabel], 3, buffer, awaiter);
	awaiter->deliverResultTo(docGetItemLabel);
	wipe();
}

void NormalScaningWithPrintWidget::onLabelArrived()
{
	if (!awaiter->deliverHere(docGetItemLabel))
		return;
	ResponseParser parser(new RichtextResponseParser(awaiter->restext, awaiter->errtext));
	if (!assertAndShowError(this, parser, tr("Error!")))
	{
		_print(parser->read().first()->value("code"));
	}
	hideProcessingOverlay();
}

void NormalScaningWithPrintWidget::wrapperOk()
{
	connectionStateLabel->setState(SemaphorLabel::opsuccess);
}

void NormalScaningWithPrintWidget::wrapperError(QString errtext)
{
	connectionStateLabel->setState(SemaphorLabel::opfail);
	if (!isVisible())
		QTimer::singleShot(100, this, SLOT(reshowError()));
	else
		ErrorMessageDialog::showErrorInfo(tr("Printer error"), errtext);
}

void NormalScaningWithPrintWidget::reshowError()
{
	ErrorMessageDialog::showErrorInfo(tr("Printer error"), printerWrapper->errors());
}
