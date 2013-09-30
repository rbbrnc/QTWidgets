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

		void shutdown();

		// Called from linphone callbacks to emit signals
		void messageReceivedCb(LinphoneChatMessage *message);

	private:
		void init();

	signals:
		void messageReceived(const QString &from, const QString &text, const QString &url);

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
