#ifndef CUTLISTDIALOG_H
#define CUTLISTDIALOG_H

#include <QDialog>
#include <QHash>
#include <QStandardItemModel>

namespace Ui {
	class CutListDialog;
}

class CutListDialog : public QDialog
{
	Q_OBJECT

	public:

		explicit CutListDialog(QHash<int, int> cutList, int endFrame, QWidget *parent = 0);
		~CutListDialog();

		QHash<int, int> list() const;

	private:

	private slots:
		void on_addButton_clicked();
		void on_removeButton_clicked();
		void on_startSpinBox_valueChanged(int value);

	private:
		Ui::CutListDialog *ui;
		QHash<int, int> m_cutList;
		int m_endFrame;

		QStandardItemModel *m_model;
};

#endif
