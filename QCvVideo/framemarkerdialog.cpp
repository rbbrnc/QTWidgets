#include "framemarkerdialog.h"
#include "ui_framemarkerdialog.h"

FrameMarkerDialog::FrameMarkerDialog(const QList<int> list, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FrameMarkerDialog)
{
	ui->setupUi(this);
	setupModelView(list);
}

FrameMarkerDialog::~FrameMarkerDialog()
{
	delete ui;
}

void FrameMarkerDialog::setupModelView(const QList<int> list)
{
	m_model = new QStandardItemModel(0, 2, this);

	m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Frame"));
//	m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Time"));
//	m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("Comment"));

	ui->tableView->setModel(m_model);
	ui->tableView->setSortingEnabled(true);

	for (int i = 0; i < list.size(); i++) {
		m_model->insertRow(i);
		m_model->setData(m_model->index(i, 0), list.at(i));
//		m_model->setData(m_model->index(i, 1), list.at(i));
	}
}
