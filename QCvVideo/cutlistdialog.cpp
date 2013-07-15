#include "cutlistdialog.h"
#include "ui_cutlistdialog.h"

#include <QDebug>

CutListDialog::CutListDialog(QHash<int, int> cutList, int endFrame, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CutListDialog),
	m_cutList(cutList),
	m_endFrame(endFrame)
{
	ui->setupUi(this);
	ui->startSpinBox->setMinimum(0);
	ui->startSpinBox->setMaximum(m_endFrame);
	ui->endSpinBox->setMaximum(m_endFrame);
//	ui->cutListWidget->setSortingEnabled(true);
	ui->cutListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
	qDebug() << "end frame:" << endFrame;

/*
	for (int i = 0; i < m_cutList.count(); i++) {
		ui->cutListWidget->addItem(QString("%1 ; %2")
			.arg(m_cutList.value(i))
			.arg(m_cutList.key(i)));
	}
*/

	QHash<int, int>::const_iterator i = m_cutList.constBegin();
	while (i != m_cutList.constEnd()) {
		ui->cutListWidget->addItem(QString("%1 ; %2").arg(i.key()).arg(i.value()));
		++i;
	}
}

CutListDialog::~CutListDialog()
{
	delete ui;
}

QHash<int, int> CutListDialog::list() const
{
	return m_cutList;
}

void CutListDialog::on_startSpinBox_valueChanged(int value)
{
	if (value < ui->endSpinBox->value()) {
		return;
	}

	ui->endSpinBox->setValue(value);
	ui->endSpinBox->setMinimum(value);
}

void CutListDialog::on_addButton_clicked()
{
	int start = ui->startSpinBox->value();
	int end   = ui->endSpinBox->value();

//	if (m_cutList.contains(start)) {
//	} else {
//	}

	m_cutList.insert(start, end);
	ui->cutListWidget->addItem(QString("%1 ; %2").arg(start).arg(end));

}

void CutListDialog::on_removeButton_clicked()
{
	QList<QListWidgetItem *> sel = ui->cutListWidget->selectedItems();
	for (int i = 0; i < sel.size(); i++) {
		QStringList s = sel.at(i)->text().split(';');
		m_cutList.remove(s.at(0).toInt());
	}
	qDeleteAll(sel);
}

