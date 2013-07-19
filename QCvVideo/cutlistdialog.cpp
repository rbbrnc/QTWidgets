#include "cutlistdialog.h"
#include "ui_cutlistdialog.h"

#include <QFileDialog>
#include <QDebug>

CutListDialog::CutListDialog(const QHash<int, int> cutList, int endFrame, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CutListDialog),
	m_endFrame(endFrame),
	m_model(0)
{
	ui->setupUi(this);
	setupCutListModelView(cutList);
}

CutListDialog::~CutListDialog()
{
	if (m_model) {
		delete m_model;
	}
	delete ui;
}

void CutListDialog::setupCutListModelView(QHash<int, int> cutList)
{
	m_model = new QStandardItemModel(0, 2, this);

	m_model->setHeaderData(0, Qt::Horizontal, QObject::tr("Start Frame"));
	m_model->setHeaderData(1, Qt::Horizontal, QObject::tr("End Frame"));

	ui->tableView->setModel(m_model);

	int idx = 0;
	QHashIterator<int, int> i(cutList);
	while (i.hasNext()) {
		i.next();
		int start = i.key();
		int end   = i.value();

		if (start > end) {
			// Do not insert backward seeks
			continue;
		}

		m_model->insertRow(idx);
		m_model->setData(m_model->index(idx, 0), i.key());
		m_model->setData(m_model->index(idx, 1), i.value());
		idx++;
	}
}

QHash<int, int> CutListDialog::list() const
{
	QHash<int, int> cutList;

	int start;
	int end;

	for (int row = 0; row < m_model->rowCount(); row++) {
		start = m_model->item(row, 0)->data(Qt::DisplayRole).toInt();
		end   = m_model->item(row, 1)->data(Qt::DisplayRole).toInt();

		cutList.insert(start, end);
		cutList.insert(end, start);
	}
	return cutList;
}

void CutListDialog::on_addButton_clicked()
{
	int idx = m_model->rowCount();
	m_model->insertRow(idx);
	m_model->setData(m_model->index(idx, 0), 0 /*startFrame*/);
	m_model->setData(m_model->index(idx, 1), m_endFrame);
}

void CutListDialog::on_removeButton_clicked()
{
	QModelIndexList sel = ui->tableView->selectionModel()->selectedRows();
	qSort(sel);
	for (int i = sel.count() - 1; i >= 0; i--) {
		int row = sel.at(i).row();
		qDebug() << "Remove row:" << row;
		m_model->removeRow(row);
	}
}

void CutListDialog::loadCutList()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Load Cut List"),
				QDir::currentPath(), tr("Cut List (*.csv *.txt *.cl)"));
	if (fileName.isEmpty()) {
		return;
	}

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Open Error!";
		return;
	}

	QHash<int, int> cutList;

	int i = -1;
	QTextStream in(&file);
	while (!in.atEnd()) {
		i++;
		QString line = in.readLine();
		QStringList sl = line.split(';');
		if (sl.count() != 2) {
			qDebug() << QString("Invalid line [%1]: %2").arg(i).arg(line);
			continue;
		}
		cutList.insert(sl.at(0).toInt(), sl.at(1).toInt());
		cutList.insert(sl.at(1).toInt(), sl.at(0).toInt());
	}

	file.close();

	if (m_model) {
		delete m_model;
	}
	setupCutListModelView(cutList);

	qDebug() << __PRETTY_FUNCTION__ << cutList;
}

void CutListDialog::saveCutList()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Cut List"),
		 QString("cutlist.csv"));

	if (fileName.isEmpty()) {
		return;
	}

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return;

	QTextStream out(&file);

	int start;
	int end;

	for (int row = 0; row < m_model->rowCount(); row++) {
		start = m_model->item(row, 0)->data(Qt::DisplayRole).toInt();
		end   = m_model->item(row, 1)->data(Qt::DisplayRole).toInt();

		out << start << ';' << end << "\n";
		qDebug() << QString("[%1] SAVE:").arg(row) << start << "," << end;
	}

	file.close();
}

void CutListDialog::on_dialogButtonBox_clicked(QAbstractButton *button)
{
	switch (ui->dialogButtonBox->standardButton(button)) {
	case QDialogButtonBox::Save:
		saveCutList();
		break;
	case QDialogButtonBox::Open:
		loadCutList();
		break;
	case QDialogButtonBox::Ok:
		QDialog::accept();
		break;
	case QDialogButtonBox::Cancel:
		QDialog::reject();
		break;
	default:
		break;
	}
}
