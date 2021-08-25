#include "MainPageWidget.h"
#include "networking/Parsers/RequestParser.h"
#include "widgets/ElementWidgets/ProcessingOverlay.h"
#include "widgets/utils/ElementsStyles.h"
#ifdef DEBUG
#include "submodules/UNAQtCommons/debug/debugtrace.h"
#endif
#include "widgets/ExtendedDelegates/ZebraListItemDelegate.h"
#include "submodules/UNAQtCommons/widgets/Scaling/ScalingUtilities.h"
void MainPageWidget::show_login_widget(User u)
{
	manualLogin->set_user(u);
	_hideAny(manualLogin);
}

MainPageWidget::MainPageWidget(QWidget* parent)
	: inframedWidget(parent), abstractNode(), innerModel(new DataEntityListModel(this)),
	mainLayout(new QVBoxLayout(this)),
	innerWidget(new inframedWidget(this)), innerLayout(new QVBoxLayout(innerWidget)),
	topPanelLayout(new QHBoxLayout(innerWidget)), bottomPanelLayout(new QHBoxLayout(innerWidget)),
	versionLabel(new QLabel(innerWidget)), hostLabel(new QLabel(innerWidget)),
	userHelpLabel(new QLabel(innerWidget)), loginsView(new QListView(this)), userIdInfo(new QLabel(innerWidget)),
	exitButton(new MegaIconButton(innerWidget)), settingsButton(new MegaIconButton(innerWidget)), refreshButton(new MegaIconButton(innerWidget)),
	userid(new QLineEdit(innerWidget)), manualLogin(new LoginWidget( this)),
	settingsScreen(new MainSettingsWidget( this)), awaiter(AppSettings->timeoutInt, this)
{
#ifdef DEBUG
	detrace_DCONSTR("MainPageWidget");
#endif
	current = innerWidget;
	untouchable = innerWidget;
	main = this;
	this->setLayout(mainLayout);
	mainLayout->addWidget(innerWidget);
	mainLayout->addWidget(settingsScreen);
	mainLayout->addWidget(manualLogin);
	manualLogin->hide();
	settingsScreen->hide();
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);		//	spacing removed to avoid space loss
	mainLayout->setContentsMargins(0, 0, 0, 0);

	innerLayout->setSpacing(0);
	innerLayout->setContentsMargins(0, 0, 0, 0);

	innerWidget->setLayout(innerLayout);
	innerLayout->addLayout(topPanelLayout);
	innerLayout->addWidget(userHelpLabel);

	innerLayout->addWidget(loginsView);

	innerLayout->addWidget(userIdInfo);
	innerLayout->addWidget(userid);

	innerLayout->addLayout(bottomPanelLayout);

	topPanelLayout->addWidget(versionLabel);
	topPanelLayout->addStretch();
	topPanelLayout->addWidget(hostLabel);
	bottomPanelLayout->addWidget(exitButton);
	bottomPanelLayout->addWidget(settingsButton);
	bottomPanelLayout->addWidget(refreshButton);

    versionLabel->setText( normalizeFloatString(QString::number(double(VERSION)), 2) + " " + SUFFIX);
	versionLabel->setFont(GENERAL_FONT);
	hostLabel->setText(AppSettings->HttpUrl.section("/", 4, 4));
	hostLabel->setFont(GENERAL_FONT);

	userHelpLabel->setFont(GENERAL_FONT);
	userIdInfo->setFont(GENERAL_FONT);
	userHelpLabel->setAlignment(Qt::AlignCenter);
	userIdInfo->setAlignment(Qt::AlignCenter);

	exitButton->setIcon(QIcon(":/resources/exit"));
	exitButton->setStyleSheet(CANCEL_BUTTONS_STYLESHEET);
	exitButton->setFont(GENERAL_FONT);
	settingsButton->setIcon(QIcon(":/resources/settings"));
	settingsButton->setStyleSheet(SETTINGS_BUTTONS_STYLESHEET);
	settingsButton->setFont(GENERAL_FONT);
	refreshButton->setIcon(QIcon(":/resources/refresh"));
	refreshButton->setStyleSheet(COMMIT_BUTTONS_STYLESHEET);
	refreshButton->setFont(GENERAL_FONT);
	userid->setFont(GENERAL_FONT);
	languageChanged();

	loginsView->setModel(innerModel);
	loginsView->setItemDelegate(new ZebraItemDelegate(this));

#ifdef QT_VERSION5X
	QObject::connect(settingsButton, &QPushButton::clicked, this, &MainPageWidget::settinsPressed);
	QObject::connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
	QObject::connect(manualLogin, &LoginWidget::loginConfirmed, this, &MainPageWidget::userIdOk);
	QObject::connect(manualLogin, &LoginWidget::backRequired, this, &MainPageWidget::hideCurrent);
	QObject::connect(settingsScreen, &MainSettingsWidget::backRequired, this, &MainPageWidget::hideCurrent);
	QObject::connect(settingsScreen, &MainSettingsWidget::languageChanged, this, &MainPageWidget::languageChanged);
	QObject::connect(settingsScreen, &MainSettingsWidget::saveConfirmed, this, &MainPageWidget::settingsSaved);
	QObject::connect(innerModel, &DataEntityListModel::dataEntityClicked, this, &MainPageWidget::userPicked);
	QObject::connect(userid, &QLineEdit::returnPressed, this, &MainPageWidget::userIdSearch);
	QObject::connect(this, &MainPageWidget::backRequired, qApp, &QApplication::quit);
	QObject::connect(loginsView, &QListView::clicked, innerModel, &DataEntityListModel::mapClickToEntity);
	QObject::connect(refreshButton, &MegaIconButton::clicked, this, &MainPageWidget::loadUsers);
	QObject::connect(&awaiter, &RequestAwaiter::requestTimeout, this, &MainPageWidget::wasTimeout);
	QObject::connect(&awaiter, &RequestAwaiter::requestReceived, this, &MainPageWidget::parseUsers);
#else
	QObject::connect(settingsButton, SIGNAL(clicked()), this, SLOT(settinsPressed()));
	QObject::connect(exitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
	QObject::connect(manualLogin, SIGNAL(loginConfirmed(QString, QString)), this, SLOT(userIdOk(QString, QString)));
	QObject::connect(manualLogin, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(settingsScreen, SIGNAL(backRequired()), this, SLOT(hideCurrent()));
	QObject::connect(settingsScreen, SIGNAL(languageChanged()), this, SLOT(languageChanged()));
	QObject::connect(settingsScreen, SIGNAL(saveConfirmed()), this, SLOT(settingsSaved()));
	QObject::connect(innerModel, SIGNAL(dataEntityClicked(RecEntity)), this, SLOT(userPicked(RecEntity)));
	QObject::connect(userid, SIGNAL(returnPressed()), this, SLOT(userIdSearch()));
	QObject::connect(this, SIGNAL(backRequired()), qApp, SLOT(quit()));
	QObject::connect(loginsView, SIGNAL(clicked(const QModelIndex&)), innerModel, SLOT(mapClickToEntity(const QModelIndex&)));
	QObject::connect(refreshButton, SIGNAL(clicked()), this, SLOT(loadUsers()));
	QObject::connect(&awaiter, SIGNAL(requestTimeout()), this, SLOT(wasTimeout()));
	QObject::connect(&awaiter, SIGNAL(requestReceived()), this, SLOT(parseUsers()));
#endif
}

void MainPageWidget::_numberReaction(int val)
{
	if (val < 9 && val >= -1)
	{
		if (val == -1)
		{
			qApp->quit();
			return;
		}
		else
		{
			QModelIndex index = innerModel->index(val);
			if (index.isValid())
			{
				innerModel->mapClickToEntity(index);
			}
		}
	}
}

void MainPageWidget::settinsPressed()
{
	_hideAny(settingsScreen);
}

void MainPageWidget::userIdOk(const QString /*log*/, const  QString /*pass*/)
{
	hideCurrent();
	emit loggedIn();
}

void MainPageWidget::userPicked(RecEntity up)
{
	if (awaiter.isAwaiting())
		return;
	User temp = upcastRecord<UserEntity>(up);
	show_login_widget(temp);
}

void MainPageWidget::hideCurrent()
{
	if (!_hideCurrent(innerWidget))
	{
		innerWidget->stopListeningKeyboard();
		emit backRequired();
	}
}

void MainPageWidget::languageChanged()
{
	userHelpLabel->setText(tr("main_page_select_profile_tip"));
	userIdInfo->setText(tr("main_page_enter_profile_id_tip"));
	exitButton->setText(tr("main_page_exit_button"));
	settingsButton->setText(tr("main_page_settings_button"));
	refreshButton->setText(tr("refresh"));
	settingsButton->setText(tr("main_page_settings_button"));
	manualLogin->langCh();
}

void MainPageWidget::userIdSearch()
{
	if (awaiter.isAwaiting())
		return;
	innerModel->lookForEntity(User(new UserEntity(userid->text())));
}

void MainPageWidget::parseUsers()
{
	hideProcessingOverlay();

	NetRequestResponse<UserEntity> result = RequestParser::parseResponse<UserEntity>(
		ResponseParser(new TwoStateListParser(awaiter.restext, awaiter.errtext))
		);
	if (result.isError)
	{
		userHelpLabel->setText(result.errtext);
#ifdef DEBUG
		detrace_NRESPERR(result.errtext);
#endif
		return;
	}
	switch (result.alternative_result)
	{
	case 0:
        innerModel->insertData(downcastRecords(result.objects));
		if (!result.additionalObjects.isEmpty())
			hostLabel->setText(result.additionalObjects.first()->value("servicename"));
		break;
	case 1:
		show_login_widget(User(new UserEntity()));
		break;
	default:
		userHelpLabel->setText(tr("response error - wrong interpretation"));
		break;
	}
}

void MainPageWidget::wasTimeout()
{
	hideProcessingOverlay();
	userHelpLabel->setText(tr("timeout_with_delay:") + QString::number(AppSettings->timeoutInt));
}

void MainPageWidget::settingsSaved()
{
	hostLabel->setText(AppSettings->HttpUrl.section("/", 4, 4));
	loadUsers();
}

void MainPageWidget::loadUsers()
{
	if (awaiter.isAwaiting())
		return;
	showProcessingOverlay();
	UserEntity e;
	e.sendAssociatedGetRequest(QStringList(), &awaiter);
}
