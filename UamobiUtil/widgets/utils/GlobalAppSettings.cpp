#include "GlobalAppSettings.h"
#include <QtCore/QSettings>
#include <QApplication>
// utility
#include "client_defaults.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif


const float VERSION = 0.21f;
#ifdef LINELLA
const char* SUFFIX = "lin-alpha";
#else
const char* SUFFIX = "alpha";
#endif


GlobalAppSettings* GlobalAppSettings::_instanse = Q_NULLPTR;
GlobalAppSettings* GlobalAppSettings::instanse()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new GlobalAppSettings();
		_instanse->setAsCommon();
		_instanse->load();
	}
	return _instanse;
}
void GlobalAppSettings::init()
{
	if (_instanse == Q_NULLPTR)
	{
		_instanse = new GlobalAppSettings();
		_instanse->setAsCommon();
		_instanse->load();
	}
}
p_QSettings GlobalAppSettings::_getSettingsFile()
{
	p_QSettings settings(new QSettings("settings.ini", QSettings::IniFormat));
	initialize_default_file(*settings);
	return settings;
}
void GlobalAppSettings::_save(p_QSettings settings)
{
	if (settings.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("GlobalAppSettings::_save", "NO SETTINGS OBJECT PROVIDED TO APP SETTINGS");
#endif
		return;
	}
	CommonAppSettings::_save(settings);
#ifdef DEBUG
	detrace_METHEXPL("if errors: " << settings->status());
#endif
	settings->beginGroup("network");
	settings->setValue("http_host", HttpUrl);
	settings->setValue("alt_hosts", AlternativeAdresses);
	settings->endGroup();
	settings->beginGroup("label_printer");
	settings->setValue("printer_designation", printerPortDesignation);
	settings->setValue("printer_port", printerPort);
	settings->setValue("printer_type", printerType);
	settings->setValue("printer_alternatives", alternativePrinters);
	settings->setValue("defaultDeviceNameMasks", defaultDeviceNameMasks);
	settings->endGroup();
}
void GlobalAppSettings::_load(p_QSettings settings)
{
	if (settings.isNull())
	{
#ifdef DEBUG
		detrace_METHPERROR("GlobalAppSettings::_load", "NO SETTINGS OBJECT PROVIDED TO COMMON SETTINGS");
#endif
		return;
	}
	CommonAppSettings::_load(settings);
#ifdef DEBUG
	detrace_METHEXPL("if errors: " << settings->status());
#endif
	settings->beginGroup("network");
	HttpUrl = settings->value("http_host", "http://una.md:3323/").toString();
	AlternativeAdresses = settings->value("alt_hosts", QVariant(QStringList())).toStringList();
	settings->endGroup();
	settings->beginGroup("label_printer");
	printerPortDesignation = settings->value("printer_designation", "COM6:").toString();
	printerPort = settings->value("printer_port", 19200).toInt();
	printerType = settings->value("printer_type", "zebra").toString();
	alternativePrinters = settings->value("printer_alternatives", QVariant(QStringList())).toStringList();
	defaultDeviceNameMasks = settings->value("defaultDeviceNameMasks", QStringList()).toStringList();
	settings->endGroup();
}
GlobalAppSettings::GlobalAppSettings() : CommonAppSettings(
	":/translations/uamobiutil_ru.qm",
	":/translations/uamobiutil_ro.qm",
	":/translations/uamobiutil_en.qm")
{
}
