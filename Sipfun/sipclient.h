#ifndef SIP_CLIENT_H
#define SIP_CLIENT_H

#include <QObject>
#include <QTimer>

#include "linphone/linphonecore.h"

class SipClient : public QObject
{
	Q_OBJECT

	private:
		SipClient();
		static SipClient *m_instance;

	public:
		static SipClient *instance();
		~SipClient();

		bool setIdentity(const QString &identity, const QString &password);
		bool sendMessage(const QString &to, const QString &message);

		enum RegistrationState {
			RegistrationNone       = LinphoneRegistrationNone,     // Initial state for registrations
			RegistrationInProgress = LinphoneRegistrationProgress, // Registration is in progress
			RegistrationOk         = LinphoneRegistrationOk,       // Registration is successful
			RegistrationCleared    = LinphoneRegistrationCleared,  // Unregistration succeeded
			RegistrationFailed     = LinphoneRegistrationFailed    // Registration failed
		};

		void shutdown();

		// Called from linphone callbacks to emit signals
		void messageReceivedCb(LinphoneChatMessage *message);
		void messageReceivedCb(const QString &from, const QString &text);
		void registrationStateChangedCb(LinphoneRegistrationState cstate);

	private:
		void init();

	signals:
		void messageReceived(const QString &from, const QString &text, const QString &url);
		void registrationStateChanged(SipClient::RegistrationState state);

	private slots:
		void update();

	private:
		LinphoneCore        *lc;
		LinphoneAuthInfo    *m_authInfo;
		LinphoneProxyConfig *m_proxyCfg;
		LinphoneChatRoom    *m_chatRoom;

		QTimer *m_loopTimer;

		QString m_identity;
		QString m_password;

		QString m_msg;

};

#endif
