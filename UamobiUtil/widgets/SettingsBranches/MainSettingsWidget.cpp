#include "MainSettingsWidget.h"
#include "widgets/utils/ElementsStyles.h"
#include "submodules/UNAQtCommons/barcodeHandling/BarcodeObserver.h"
#include "widgets/utils/GlobalAppSettings.h"
#include <QLineEdit>

MainSettingsWidget::MainSettingsWidget(QWidget* parent)
	:inframedWidget(parent),
	mainLayout(new QVBoxLayout(this)),
	innerWidget(new QTabWidget(this)),
	wrkflTab(new QWidget(innerWidget)),
	wrkflinnLayout(new QFormLayout(wrkflTab)),
	scanModeSelector(new QComboBox(wrkflTab)),
	fontMin(new QSpinBox(wrkflTab)),
	fontMax(new QSpinBox(wrkflTab)),
	fontDec(new QSpinBox(wrkflTab)),
    notificationsVolume(new QDoubleSpinBox(wrkflTab)),
	sysTab(new QWidget(innerWidget)),
	sysinnLayout(new QFormLayout(sysTab)),
	topExplLabel(new QLabel(sysTab)),
	addressField(new QComboBox(sysTab)),
	langField(new QComboBox(sysTab)),
	createSTableButton(new MegaIconButton(sysTab)),
	scaningTab(new QWidget(innerWidget)),
	scaninnlayout(new QFormLayout(scaningTab)),
	prefix(new QSpinBox(sysTab)),
	suffix(new QSpinBox(sysTab)),
	printTab(new QWidget(innerWidget)),
	printinnLayout(new QFormLayout(printTab)),
	buildState(new QLabel(printTab)),
	portDesignation(new QComboBox(printTab)),
	portNumber(new QSpinBox(printTab)),
	portType(new QComboBox(printTab)),
	btDeviceName(new QComboBox(printTab)),
	footerLayout(new QHBoxLayout(this)),
	saveButton(new MegaIconButton(this)), backButton(new MegaIconButton(this))
{
	this->setLayout(mainLayout);
#ifdef Q_OS_WINCE
	this->setFixedSize(calculateAdaptiveSize(1));
	this->setFixedHeight(calculateAdaptiveHeight(0.9));
#endif
	mainLayout->setSpacing(0);		//	spacing removed to avoid space loss
	mainLayout->setContentsMargins(0, 0, 0, 0);

	mainLayout->addWidget(innerWidget);
	innerWidget->addTab(sysTab, tr("system"));
	innerWidget->addTab(wrkflTab, tr("workflow"));
	innerWidget->addTab(printTab, tr("printer"));
	innerWidget->addTab(scaningTab, tr("Scan"));

	sysTab->setLayout(sysinnLayout);
	sysinnLayout->addRow(topExplLabel);
	sysinnLayout->addRow(tr("HTTP address"), addressField);
	sysinnLayout->addRow(tr("language"), langField);
	sysinnLayout->addRow(tr("create style table"), createSTableButton);
	sysinnLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);


	scaninnlayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	scaninnlayout->addRow(tr("prefix ") + QChar(AppSettings->scanPrefix), prefix);
	scaninnlayout->addRow(tr("suffix ") + QChar(AppSettings->scanSuffix), suffix);

	wrkflTab->setLayout(wrkflinnLayout);
	wrkflinnLayout->addRow(tr("scanMode"), scanModeSelector);
	wrkflinnLayout->addRow(tr("font min"), fontMin);
	wrkflinnLayout->addRow(tr("font max"), fontMax);
	wrkflinnLayout->addRow(tr("font dec"), fontDec);
    wrkflinnLayout->addRow(tr("Notifications volume"), notificationsVolume);
	wrkflinnLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	printTab->setLayout(printinnLayout);
    printinnLayout->addRow(tr("Printer support"), buildState);
    printinnLayout->addRow(tr("Printer"), portType);
#ifndef Q_OS_WINCE
    portDesignation->hide();
    portNumber->hide();
    printinnLayout->addRow(tr("Printer name"), btDeviceName);
#else
    btDeviceName->hide();
    printinnLayout->addRow(tr("Port name"), portDesignation);
    printinnLayout->addRow(tr("Port"), portNumber);
#endif
	
	printinnLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);
	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);


	const QFont & scf = *FontAdapter::general();
	

	QStringList temp;
	temp << tr("settings_scmode_one") << tr("settings_scmode_autos") << tr("settings_scmode_simple");
	scanModeSelector->addItems(temp);

	scanModeSelector->setFont(scf);
	scanModeSelector->setMinimumHeight(calculateAdaptiveHeight());

	topExplLabel->setAlignment(Qt::AlignCenter);
	topExplLabel->setFont(scf);
	
	addressField->addItem(AppSettings->HttpUrl);
	addressField->addItems(AppSettings->AlternativeAdresses);
	addressField->setCurrentIndex(0);
	addressField->setFont(scf);

	temp.clear();
	temp << "Russian" << "Romanian" << "English";
	langField->addItems(temp);
	if (AppSettings->language == "Russian")
		langField->setCurrentIndex(0);
	else if (AppSettings->language == "Romanian")
		langField->setCurrentIndex(1);
	else
		langField->setCurrentIndex(2);

	saveButton->setIcon(QIcon(":/resources/with"));
	saveButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);

	backButton->setIcon(QIcon(":/resources/back"));
	backButton->setStyleSheet(BACK_BUTTONS_STYLESHEET);

	addressField->setEditable(true);
	addressField->setInsertPolicy(QComboBox::InsertAtTop);
	langField->setEditable(false);
	langField->setFont(scf);
	langField->setInsertPolicy(QComboBox::NoInsert);
#ifdef QT_VERSION5X
	langField->setCurrentText(AppSettings->language);
#else
	langField->setCurrentIndex(temp.indexOf(AppSettings->language));
#endif
	scanModeSelector->setEditable(false);
	scanModeSelector->setInsertPolicy(QComboBox::NoInsert);
	scanModeSelector->setFont(scf);
	mainLayout->addLayout(footerLayout);
	footerLayout->addWidget(saveButton);
	footerLayout->addWidget(backButton);
	prefix->setValue(AppSettings->scanPrefix);
	prefix->setMaximum(1112063);
	suffix->setValue(AppSettings->scanSuffix);
	suffix->setMaximum(1112063);
	fontMin->setValue(AppSettings->fontMinHeight);
	fontMin->setMaximum(500);
	fontMax->setValue(AppSettings->fontMaxHeight);
	fontMax->setMaximum(500);
    fontDec->setValue(int(AppSettings->fontPercent * 100));
	fontDec->setMaximum(100);
    notificationsVolume->setMaximum(1.0);
    notificationsVolume->setValue(AppSettings->notificationsVolume);
	createSTableButton->setIcon(QIcon(":/resources/add"));

#ifdef FTR_COM
	buildState->setPixmap(QIcon(":/resources/with").pixmap(calculateAdaptiveSize(0.15, 0.2)));
#else
#ifdef Q_OS_ANDROID
	buildState->setPixmap(QIcon(":/resources/with").pixmap(calculateAdaptiveSize(0.15, 0.2)));
#else
	buildState->setPixmap(QIcon(":/resources/without").pixmap(calculateAdaptiveSize(0.15, 0.2)));
#endif
#endif
	temp.clear();
	temp << AppSettings->printerPortDesignation << "COM4:" << "COM2:" << "COM6:" << "BSP2:";
	portDesignation->addItems(temp);
	portDesignation->setEditable(true);
	portNumber->setMaximum(35565);
	portNumber->setValue(AppSettings->printerPort);
	portType->addItem(AppSettings->printerType);
	portType->addItems(AppSettings->alternativePrinters);
	portType->setEditable(true);
	btDeviceName->setEditable(true);
	btDeviceName->addItem(AppSettings->labelPrinterName);
	btDeviceName->addItems(AppSettings->defaultDeviceNameMasks);
	btDeviceName->setCurrentIndex(0);
	btDeviceName->setInputMethodHints(Qt::InputMethodHint::ImhNoPredictiveText);


#ifdef QT_VERSION5X
	QObject::connect(saveButton, &MegaIconButton::clicked, this, &MainSettingsWidget::saveClicked);
	QObject::connect(backButton, &MegaIconButton::clicked, this, &MainSettingsWidget::backRequired);
	QObject::connect(langField, QOverload<const QString&>::of(&QComboBox::activated), this, &MainSettingsWidget::langSelected);
	QObject::connect(addressField, QOverload<const QString&>::of(&QComboBox::activated), this, &MainSettingsWidget::AddressSelected);
	QObject::connect(prefix, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainSettingsWidget::setPSLabels);
	QObject::connect(suffix, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainSettingsWidget::setPSLabels);
	QObject::connect(fontMin, &QSpinBox::editingFinished, this, &MainSettingsWidget::applyFonts);
	QObject::connect(fontMax, &QSpinBox::editingFinished, this, &MainSettingsWidget::applyFonts);
	QObject::connect(fontDec, &QSpinBox::editingFinished, this, &MainSettingsWidget::applyFonts);
	QObject::connect(createSTableButton, &MegaIconButton::clicked, this, &MainSettingsWidget::createStyleTable);
#else
    QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
    QObject::connect(backButton, SIGNAL(clicked()), this, SIGNAL(backRequired()));
    QObject::connect(langField, SIGNAL(activated(QString)), this, SLOT(langSelected(QString)));
    QObject::connect(addressField, SIGNAL(activated(QString)), this, SLOT(AddressSelected(QString)));
    QObject::connect(prefix, SIGNAL(valueChanged(int)), this, SLOT(setPSLabels(int)));
    QObject::connect(suffix, SIGNAL(valueChanged(int)), this, SLOT(setPSLabels(int)));
    QObject::connect(fontMin, SIGNAL(editingFinished()), this, SLOT(applyFonts()));
    QObject::connect(fontMax, SIGNAL(editingFinished()), this, SLOT(applyFonts()));
    QObject::connect(fontDec, SIGNAL(editingFinished()), this, SLOT(applyFonts()));
    QObject::connect(createSTableButton, SIGNAL(clicked()), this, SLOT(createStyleTable()));
#endif
}


void MainSettingsWidget::saveClicked()
{
	AppSettings->HttpUrl = (addressField->currentText().isEmpty()) ? AppSettings->HttpUrl : addressField->currentText();
	if (!AppSettings->AlternativeAdresses.contains(AppSettings->HttpUrl))
	{
		AppSettings->AlternativeAdresses.push_back(AppSettings->HttpUrl);
		addressField->addItem(AppSettings->HttpUrl);
	}
	AppNetwork->setUrl(AppSettings->HttpUrl);
	AppSettings->scanSuffix = suffix->value();
	AppSettings->scanPrefix = prefix->value();
    BarcodeObs->resetCapture(AppSettings->scanPrefix, AppSettings->scanSuffix);
	AppSettings->fontMinHeight = fontMin->value();
	AppSettings->fontMaxHeight = fontMax->value();
	AppSettings->fontPercent = ((fontDec->value() > 0) ? fontDec->value() : 1) / 100;
	if (!AppSettings->alternativePrinters.contains(portType->currentText()))
	{
		AppSettings->alternativePrinters << portType->currentText();
	}
	AppSettings->printerType = portType->currentText();
	AppSettings->printerPort = portNumber->value();
	AppSettings->printerPortDesignation = portDesignation->currentText();
	AppSettings->labelPrinterName = btDeviceName->currentText();
	if (!AppSettings->defaultDeviceNameMasks.contains(AppSettings->labelPrinterName))
	{
		AppSettings->defaultDeviceNameMasks.push_back(AppSettings->labelPrinterName);
		btDeviceName->addItem(AppSettings->labelPrinterName);
	}
    AppSettings->notificationsVolume = notificationsVolume->value();
	BarcodeObs->resetCapture(AppSettings->scanPrefix, AppSettings->scanSuffix);
	AppSettings->save();
	emit saveConfirmed();
	emit backRequired();
}

void MainSettingsWidget::langSelected(const QString& lang)
{
	AppSettings->language = lang;
	langChanged();
	emit languageChanged();
}

void MainSettingsWidget::langChanged()
{
	AppSettings->setTranslator();
	QStringList temp;
	temp << tr("settings_scmode_one") << tr("settings_scmode_autos") << tr("settings_scmode_simple");
	scanModeSelector->clear();
	scanModeSelector->addItems(temp);
	topExplLabel->setText(tr("settings_system_title"));
	saveButton->setText(tr("settings_save_button"));
	backButton->setText(tr("settings_back_button"));
	innerWidget->setTabText(0, tr("system"));
	innerWidget->setTabText(1, tr("workflow"));
	innerWidget->setTabText(2, tr("printer"));
	innerWidget->setTabText(3, tr("Scan"));

	static_cast<QLabel*>(sysinnLayout->labelForField(addressField))->setText(tr("HTTP address"));
	static_cast<QLabel*>(sysinnLayout->labelForField(langField))->setText(tr("language"));	
	static_cast<QLabel*>(sysinnLayout->labelForField(createSTableButton))->setText(tr("create style table"));
	static_cast<QLabel*>(scaninnlayout->labelForField(prefix))->setText(tr("prefix ") + QChar(AppSettings->scanPrefix));
	static_cast<QLabel*>(scaninnlayout->labelForField(suffix))->setText(tr("suffix ") + QChar(AppSettings->scanSuffix));
	static_cast<QLabel*>(wrkflinnLayout->labelForField(scanModeSelector))->setText(tr("scanMode"));
	static_cast<QLabel*>(wrkflinnLayout->labelForField(fontMin))->setText(tr("font min"));
	static_cast<QLabel*>(wrkflinnLayout->labelForField(fontMax))->setText(tr("font max"));
	static_cast<QLabel*>(wrkflinnLayout->labelForField(fontDec))->setText(tr("font dec"));
	static_cast<QLabel*>(wrkflinnLayout->labelForField(notificationsVolume))->setText(tr("Notifications volume"));
	static_cast<QLabel*>(printinnLayout->labelForField(buildState))->setText(tr("Printer support"));
#ifndef Q_OS_WINCE
    static_cast<QLabel*>(printinnLayout->labelForField(btDeviceName))->setText(tr("Printer name"));
#else
    static_cast<QLabel*>(printinnLayout->labelForField(portDesignation))->setText(tr("Port name"));
    static_cast<QLabel*>(printinnLayout->labelForField(portNumber))->setText(tr("Port"));
#endif
    static_cast<QLabel*>(printinnLayout->labelForField(portType))->setText(tr("Printer"));
}

void MainSettingsWidget::AddressSelected(const QString& activated)
{
	AppSettings->HttpUrl = activated;
}

void MainSettingsWidget::setPSLabels(int /*val*/)
{
    qobject_cast<QLabel*>(scaninnlayout->labelForField(prefix))->setText(tr("prefix ") + QChar(prefix->value()));
    qobject_cast<QLabel*>(scaninnlayout->labelForField(suffix))->setText(tr("suffix ") + QChar(suffix->value()));
}

void MainSettingsWidget::applyFonts()
{
	FontAdapter::instanse()->reset(fontMin->value(), fontMax->value(), 
		(((fontDec->value()>0)? fontDec->value() : 1) / 100));
}

void MainSettingsWidget::createStyleTable()
{
	StyleManager::pushCurrentStylesheets();
}
