#include <QDateTime>
#include <QDebug>

#include "sipclient.h"
#include <stdio.h>

SipClient *SipClient::m_instance = 0;

SipClient *SipClient::instance()
{
	if (m_instance == 0) {
		m_instance = new SipClient();
	}

	return m_instance;
}

static void registration_state_changed(struct _LinphoneCore *lc, LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message)
{
	Q_UNUSED(lc)
	Q_UNUSED(message)

	SipClient *sc = SipClient::instance();
	sc->registrationStateChangedCb(cstate);

	printf("New registration state %s for user id [%s] at proxy [%s]\n",
		linphone_registration_state_to_string(cstate),
		linphone_proxy_config_get_identity(cfg),
		linphone_proxy_config_get_addr(cfg));
}

void SipClient::registrationStateChangedCb(LinphoneRegistrationState cstate)
{
	emit registrationStateChanged(static_cast<SipClient::RegistrationState>(cstate));
}

/* ONLY for Text messages */
static void text_received(LinphoneCore *lc, LinphoneChatRoom *room, const LinphoneAddress *from, const char *message)
{
	Q_UNUSED(lc)
	Q_UNUSED(room)

	qDebug() << __PRETTY_FUNCTION__
			 <<	"Text Message:" << message
			 << "received from:" << linphone_address_as_string(from);

	SipClient *sc = SipClient::instance();
	sc->messageReceivedCb(QString(message), QString(linphone_address_as_string(from)));
}

/* Messages with attachments */
static void message_received(LinphoneCore *lc, LinphoneChatRoom *room, LinphoneChatMessage *message)
{
	Q_UNUSED(lc)
	Q_UNUSED(room)

	qDebug() << "Unread:" << linphone_chat_room_get_unread_messages_count(room);
	linphone_chat_room_mark_as_read(room);

	SipClient *sc = SipClient::instance();
	sc->messageReceivedCb(message);
}

// Called from text_received callback
void SipClient::messageReceivedCb(const QString &from, const QString &text)
{
	emit messageReceived(from, text, QString());
}

// Called from message_received callback
void SipClient::messageReceivedCb(LinphoneChatMessage *message)
{
	QString url  = QString(linphone_chat_message_get_external_body_url(message));
	QString text = QString(linphone_chat_message_get_text(message));
	QString from = QString(linphone_address_as_string(linphone_chat_message_get_from(message)));

	qDebug() << __PRETTY_FUNCTION__
		 << QDateTime::fromTime_t(linphone_chat_message_get_time(message))
		 << "From:" << from << "Text:" << text << "Url:" << url;

	linphone_chat_message_destroy(message);

	emit messageReceived(from, text, url);
}

SipClient::SipClient()
{
	lc = 0;
	m_authInfo = 0;
	m_loopTimer = new QTimer(this);
	m_loopTimer->setInterval(50);

	init();

}

SipClient::~SipClient()
{}

void SipClient::shutdown()
{
	qDebug() << __PRETTY_FUNCTION__;

	if (m_authInfo) {
		linphone_auth_info_destroy(m_authInfo);

		// Get default proxy config
		linphone_core_get_default_proxy(lc, &m_proxyCfg);
		// start editing proxy configuration
		linphone_proxy_config_edit(m_proxyCfg);
		// De-activate registration for this proxy config
		linphone_proxy_config_enable_register(m_proxyCfg, FALSE);
		linphone_proxy_config_done(m_proxyCfg);

		while (linphone_proxy_config_get_state(m_proxyCfg) != LinphoneRegistrationCleared) {
			//...to make sure we receive call backs before shutting down
			linphone_core_iterate(lc);
			ms_usleep(50000);
		}
		m_authInfo = 0;
	}

//	delete m_loopTimer;

	if (lc) {
		linphone_core_destroy(lc);
		lc = 0;
	}
}

void SipClient::update()
{
	/* Main loop for receiving notifications and doing background
	 * linphonecore work:
	 */
	if (lc) {
		linphone_core_iterate(lc);
	}

	if (!m_msg.isEmpty()) {
		qDebug() << "Mess:" << m_msg;
		m_msg.clear();
	}
}


bool SipClient::setIdentity(const QString &identity, const QString &password)
{
	m_identity = identity;
	m_password = password;

        /* Create Proxy Config*/
	m_proxyCfg = linphone_core_create_proxy_config(lc);

        /*parse identity*/
        LinphoneAddress *from = linphone_address_new(identity.toLatin1().data());
	if (from == NULL) {
                qDebug() << __PRETTY_FUNCTION__ << " " << identity << " is not a valid sip uri, must be like sip:toto@sip.linphone.org";
		return false;
        }

	m_authInfo = linphone_auth_info_new(linphone_address_get_username(from), NULL, password.toLatin1().data(), NULL, NULL, NULL);
	linphone_core_add_auth_info(lc, m_authInfo);

	/* Configure proxy entries */

	/* Set identity with user name and domain*/
	linphone_proxy_config_set_identity(m_proxyCfg, identity.toLatin1().data());

	/* Extract domain address from identity*/
	const char *server_addr = linphone_address_get_domain(from);

	/* We assume domain = proxy server address*/
	linphone_proxy_config_set_server_addr(m_proxyCfg, server_addr);

	/* Activate registration for this proxy config*/
	linphone_proxy_config_enable_register(m_proxyCfg, TRUE);

	 /* Release resource*/
	linphone_address_destroy(from);

	 /* Add proxy config to linphone core*/
	linphone_core_add_proxy_config(lc, m_proxyCfg);

	 /* Set to default proxy*/
	linphone_core_set_default_proxy(lc, m_proxyCfg);

	return true;
}

void SipClient::init()
{
	qDebug() << "linphone core version:" << linphone_core_get_version();

	m_loopTimer->setInterval(50);
	connect(m_loopTimer, SIGNAL(timeout()), this, SLOT(update()));

	LinphoneCoreVTable vtable = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

#ifdef DEBUG
	/* Enable liblinphone logs. */
	linphone_core_enable_logs(NULL);
#endif

	/* Fill the LinphoneCoreVTable with application callbacks. All are optional. */
	vtable.registration_state_changed = registration_state_changed;
	vtable.text_received              = text_received;
	vtable.message_received           = message_received;
//	vtable.call_state_changed
//	vtable.notify_presence_recv
//	vtable.new_subscription_request
//	vtable.auth_info_requested
//	vtable.call_log_updated
//	vtable.dtmf_received
//	vtable.refer_received
//	vtable.call_encryption_changed
//	vtable.transfer_state_changed
//	vtable.buddy_info_updated
//	vtable.notify_recv
//	vtable.call_stats_updated
//	vtable.display_status
//	vtable.display_message
//	vtable.display_warning
//	vtable.display_url
//	vtable.show

	/* Instantiate a LinphoneCore object given the LinphoneCoreVTable */
	lc = linphone_core_new(&vtable, NULL, NULL, NULL);

	if (lc) {
		m_loopTimer->start();
	}
}

//
// use sip:user@sip.linphone.org or for local net without internet
//  sip.<ipaddr> e.g. sip:192.168.0.1
//
bool SipClient::sendMessage(const QString &to, const QString &message)
{
	if (to.isNull() || to.isEmpty()) {
		return false;
	}

	m_chatRoom = linphone_core_create_chat_room(lc, to.toLatin1().data());

	/* Sending message */
	linphone_chat_room_send_message(m_chatRoom, message.toLatin1().data());

	linphone_chat_room_destroy(m_chatRoom);

	return true;
}

