#ifndef QLINPHONE_CORE_H
#define QLINPHONE_CORE_H

#include <QObject>
#include "linphone/linphonecore.h"


class QTimer;

class QLinPhoneCore : public QObject
{
	Q_OBJECT

	Q_PROPERTY(RegistrationState state READ   registrationState
									   WRITE  setRegistrationState
									   NOTIFY registrationStateChanged);
	Q_ENUMS(RegistrationState)

	public:

		enum RegistrationState {
			RegistrationNone       = LinphoneRegistrationNone,     // Initial state for registrations
			RegistrationInProgress = LinphoneRegistrationProgress, // Registration is in progress
			RegistrationOk         = LinphoneRegistrationOk,       // Registration is successful
			RegistrationCleared    = LinphoneRegistrationCleared,  // Unregistration succeeded
			RegistrationFailed     = LinphoneRegistrationFailed    // Registration failed
		};

		QLinPhoneCore(int loopInterval = 50, QObject *parent = 0);
		~QLinPhoneCore();

		void messageReceivedCb(LinphoneChatMessage *msg);
		void messageReceivedCb(const QString &from, const QString &text);

		QLinPhoneCore::RegistrationState registrationState() const;
		void setRegistrationState(QLinPhoneCore::RegistrationState state);
		bool isRegistered() const;
		QString registrationStateString() const;


		bool registerToNetwork(const QString &identity, const QString &password);
		void unregisterFromNetwork();

		bool sendMessage(const QString &to, const QString &message);
		bool sendPicture(const QString &to, const QString &fileName);

	signals:
		void messageReceived(const QString &from, const QString &text, const QString &url);
		void registrationStateChanged(QLinPhoneCore::RegistrationState state);

	private slots:
		void update();

	private:
		LinphoneCore *m_lc;

		QTimer *m_loopTimer;

		RegistrationState m_rstate;

		LinphoneAuthInfo    *m_authInfo;
		LinphoneProxyConfig *m_proxyCfg;
		LinphoneChatRoom    *m_chatRoom;

		QString m_extUrl;
};

#endif
