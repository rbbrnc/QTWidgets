#include <QTimer>
#include <QDateTime>
#include <QDebug>

#include "QLinPhoneCore.h"

static LinphoneCoreVTable vtable = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

// LinPhone Core VTable Callbacks

static void registration_state_changed(struct _LinphoneCore *lc, LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message)
{
//	Q_UNUSED(message)

	qDebug() << __func__
			 << "New registration state:" << linphone_registration_state_to_string(cstate)
			 << "User:" << linphone_proxy_config_get_identity(cfg)
			 << "Proxy:" << linphone_proxy_config_get_addr(cfg)
			 << "Message:" << message;

	QLinPhoneCore *lcc = static_cast<QLinPhoneCore *>(linphone_core_get_user_data(lc));
	lcc->setRegistrationState(static_cast<enum QLinPhoneCore::RegistrationState>(cstate));
}

// Messages Received (with attachments)
static void message_received(LinphoneCore *lc, LinphoneChatRoom *room, LinphoneChatMessage *msg)
{
	qDebug() << "Unread:" << linphone_chat_room_get_unread_messages_count(room);
	linphone_chat_room_mark_as_read(room);


	QLinPhoneCore *lcc = static_cast<QLinPhoneCore *>(linphone_core_get_user_data(lc));
	lcc->messageReceivedCb(msg);
}

/* [DEPRECATED] ONLY for Text messages */
static void text_received(LinphoneCore *lc, LinphoneChatRoom *room, const LinphoneAddress *from, const char *msg)
{
	Q_UNUSED(room)

	qDebug() << __PRETTY_FUNCTION__
			 << "Message:" << msg
			 << "from:" << linphone_address_as_string(from);

	QLinPhoneCore *lcc = static_cast<QLinPhoneCore *>(linphone_core_get_user_data(lc));
	lcc->messageReceivedCb(QString(msg), QString(linphone_address_as_string(from)));
}

QLinPhoneCore::QLinPhoneCore(int loopInterval, QObject *parent)
	: QObject(parent)
{
	m_rstate = QLinPhoneCore::RegistrationNone;
	m_loopTimer = new QTimer(this);
	m_loopTimer->setInterval(loopInterval);

	connect(m_loopTimer, SIGNAL(timeout()), this, SLOT(update()));

#ifdef DEBUG
	qDebug() << "linphone core version:" << linphone_core_get_version();
	/* Enable liblinphone logs. */
	linphone_core_enable_logs(NULL);
#endif

	// Fill the LinphoneCoreVTable with application callbacks. All are optional.
	vtable.registration_state_changed = registration_state_changed;
    vtable.text_received              = text_received;
    vtable.message_received           = message_received;
//  vtable.call_state_changed
//  vtable.notify_presence_recv
//  vtable.new_subscription_request
//  vtable.auth_info_requested
//  vtable.call_log_updated
//  vtable.dtmf_received
//  vtable.refer_received
//  vtable.call_encryption_changed
//  vtable.transfer_state_changed
//  vtable.buddy_info_updated
//  vtable.notify_recv
//  vtable.call_stats_updated
//  vtable.display_status
//  vtable.display_message
//  vtable.display_warning
//  vtable.display_url
//  vtable.show

	// Instantiate a LinphoneCore object given the LinphoneCoreVTable
	m_lc = linphone_core_new(&vtable, NULL, NULL, NULL);

	linphone_core_set_user_data(m_lc, static_cast<void *>(this));

	m_loopTimer->start();
}

QLinPhoneCore::~QLinPhoneCore()
{
	m_loopTimer->stop();

	if (m_lc) {
		linphone_core_destroy(m_lc);
		m_lc = 0;
	}
}

void QLinPhoneCore::update()
{
	/* Main loop for receiving notifications and doing background
     * linphonecore work:
     */
    if (m_lc) {
        linphone_core_iterate(m_lc);
    }
}

// Called from message_received callback
void QLinPhoneCore::messageReceivedCb(LinphoneChatMessage *msg)
{
    QString url  = QString(linphone_chat_message_get_external_body_url(msg));
    QString text = QString(linphone_chat_message_get_text(msg));
    QString from = QString(linphone_address_as_string(linphone_chat_message_get_from(msg)));

    qDebug() << __PRETTY_FUNCTION__
         << QDateTime::fromTime_t(linphone_chat_message_get_time(msg))
         << "From:" << from << "Text:" << text << "Url:" << url;

    linphone_chat_message_destroy(msg);

    emit messageReceived(from, text, url);
}

// Called from text_received callback
void QLinPhoneCore::messageReceivedCb(const QString &from, const QString &text)
{
	emit messageReceived(from, text, QString());
}

QLinPhoneCore::RegistrationState QLinPhoneCore::registrationState() const
{
	return m_rstate;
}

void QLinPhoneCore::setRegistrationState(RegistrationState state)
{
	m_rstate = state;
	emit registrationStateChanged(state);
}

bool QLinPhoneCore::isRegistered() const
{
	return linphone_proxy_config_is_registered(m_proxyCfg);
}

QString QLinPhoneCore::registrationStateString() const
{
	LinphoneRegistrationState s = linphone_proxy_config_get_state(m_proxyCfg);
	return QString(linphone_registration_state_to_string(s));
}


bool QLinPhoneCore::registerToNetwork(const QString &identity, const QString &password)
{
	/* Create Proxy Config*/
	m_proxyCfg = linphone_core_create_proxy_config(m_lc);

	/*parse identity*/
	LinphoneAddress *from = linphone_address_new(identity.toLatin1().data());
	if (from == NULL) {
		qDebug() << __PRETTY_FUNCTION__ << " " << identity << " is not a valid sip uri, must be like sip:toto@sip.linphone.org";
		return false;
	}

	m_authInfo = linphone_auth_info_new(linphone_address_get_username(from), NULL, password.toLatin1().data(), NULL, NULL, NULL);
	linphone_core_add_auth_info(m_lc, m_authInfo);

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
	linphone_core_add_proxy_config(m_lc, m_proxyCfg);

	 /* Set to default proxy*/
	linphone_core_set_default_proxy(m_lc, m_proxyCfg);

	return true;
}

void QLinPhoneCore::unregisterFromNetwork()
{
    qDebug() << __PRETTY_FUNCTION__;

    if (m_authInfo) {
        linphone_auth_info_destroy(m_authInfo);

        // Get default proxy config
        linphone_core_get_default_proxy(m_lc, &m_proxyCfg);
        // start editing proxy configuration
        linphone_proxy_config_edit(m_proxyCfg);
        // De-activate registration for this proxy config
        linphone_proxy_config_enable_register(m_proxyCfg, FALSE);
        linphone_proxy_config_done(m_proxyCfg);

        while (linphone_proxy_config_get_state(m_proxyCfg) != LinphoneRegistrationCleared) {
            //...to make sure we receive call backs before shutting down
            linphone_core_iterate(m_lc);
            ms_usleep(50000);
        }
        m_authInfo = 0;
    }
}

//
// use sip:user@sip.linphone.org or for local net without internet
//  sip.<ipaddr> e.g. sip:192.168.0.1
//
bool QLinPhoneCore::sendMessage(const QString &to, const QString &message)
{
    if (to.isNull() || to.isEmpty()) {
        return false;
    }

    m_chatRoom = linphone_core_create_chat_room(m_lc, to.toLatin1().data());

    /* Sending message */
    linphone_chat_room_send_message(m_chatRoom, message.toLatin1().data());
    linphone_chat_room_destroy(m_chatRoom);

    return true;
}

static void on_chat_message_state_changed(LinphoneChatMessage *msg, LinphoneChatMessageState state, void *ud)
{
	Q_UNUSED(ud)

	qDebug() << linphone_chat_message_state_to_string(state)
			 << "From:" <<
			 << linphone_chat_message_get_external_body_url(msg);


	//LinphoneChatRoom* linphone_chat_message_get_chat_room(LinphoneChatMessage *msg);


//	linphone_chat_message_destroy(msg);
}

bool QLinPhoneCore::sendPicture(const QString &to, const QString &fileName)
{
    if (to.isNull() || to.isEmpty()) {
        return false;
    }

	qDebug() << __PRETTY_FUNCTION__ << "To:" << to << "File:" << fileName;

    m_chatRoom = linphone_core_create_chat_room(m_lc, to.toLatin1().data());

	LinphoneChatMessage *msg = linphone_chat_room_create_message(m_chatRoom, "Test");

	m_extUrl = "https://www.linphone.org:444/download/" + fileName;

	linphone_chat_message_set_external_body_url(msg, m_extUrl.toLatin1().data());

	linphone_chat_room_send_message2(m_chatRoom, msg, on_chat_message_state_changed, this);

//    linphone_chat_room_destroy(m_chatRoom);

    return true;
}



