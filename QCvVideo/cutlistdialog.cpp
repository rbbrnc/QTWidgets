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
	ui->tableView->setSortingEnabled(true);

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
		//qDebug() << "Remove row:" << row;
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
		ui->logTextEdit->appendPlainText(QString("Error opening \'%1\'").arg(fileName));
		return;
	}

	QHash<int, int> cutList;
	int commentIdx;
	bool startOk;
	bool endOk;
	int startMark;
	int endMark;

	int i = -1;
	QTextStream in(&file);
	while (!in.atEnd()) {
		i++;
		QString line = in.readLine();

		// Trim whitespaces
		line = line.simplified();

		// Skip empty line
		if (line.isEmpty()) {
			continue;
		}

		// Remove comments '#'
		commentIdx = line.indexOf('#', 0);
		if (commentIdx == 0) {
			// Ignore full-line comment
			continue;
		} else if (commentIdx != -1) {
			// Remove comment from commentIdx to EOL
			line.truncate(commentIdx);
		}

		QStringList sl = line.split(';');
		if (sl.count() != 2) {
			ui->logTextEdit->appendPlainText(QString("Line %1 -- Invalid line \'%2\'").arg(i).arg(line));
			continue;
		}

		startMark = sl.at(0).toInt(&startOk);
		endMark   = sl.at(1).toInt(&endOk);

		if ((!startOk) || (!endOk)) {
			ui->logTextEdit->appendPlainText(QString("Line %1 -- Invalid data \'%2\'").arg(i).arg(line));
		} else {
			cutList.insert(startMark, endMark);
			cutList.insert(endMark, startMark);
		}
	}

	file.close();

	if (m_model) {
		delete m_model;
	}
	setupCutListModelView(cutList);

	//qDebug() << __PRETTY_FUNCTION__ << cutList;
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
//		qDebug() << QString("[%1] SAVE:").arg(row) << start << "," << end;
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
