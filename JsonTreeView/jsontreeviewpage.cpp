#include "jsontreeviewpage.h"
#include "ui_jsontreeviewpage.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QDebug>

#include "jsontreemodel.h"

JsonTreeViewPage::JsonTreeViewPage(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::JsonTreeViewPage),
	m_model(0)
{
	ui->setupUi(this);
}

JsonTreeViewPage::~JsonTreeViewPage()
{
	if (m_model) {
		delete m_model;
		m_model = 0;
	}

	delete ui;
}

void JsonTreeViewPage::setJsonData(const QByteArray &data)
{
    if (m_model) {
        delete m_model;
        m_model = 0;
    }

    QJsonParseError err;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError) {
        qWarning() << __PRETTY_FUNCTION__ << err.errorString() << "at offset" << err.offset;
        return;
    }

    m_model = new JsonTreeModel(&jsonDoc);
    ui->treeView->setUniformRowHeights(true);
    ui->treeView->setModel(m_model);
}

void JsonTreeViewPage::setFile(const QString &file)
{
	if (m_model) {
		delete m_model;
		m_model = 0;
	}

    // Load only regular files.
	QFileInfo fi(file);
	if (!fi.isFile()) {
		return;
	}

	QFile loadFile(file);
    if (!loadFile.open(QIODevice::ReadOnly)) {
		qWarning() << "Couldn't open" << file;
        return;
    }

    QByteArray fileData = loadFile.readAll();

	QJsonParseError err;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData, &err);
	if (err.error != QJsonParseError::NoError) {
		qWarning() << __PRETTY_FUNCTION__ << err.errorString() << "at offset" << err.offset;
		return;
	}

	m_model = new JsonTreeModel(&jsonDoc);
	ui->treeView->setUniformRowHeights(true);
	ui->treeView->setModel(m_model);
}
