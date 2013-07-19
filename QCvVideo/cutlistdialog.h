#ifndef CUTLISTDIALOG_H
#define CUTLISTDIALOG_H

#include <QDialog>
#include <QHash>
#include <QStandardItemModel>
#include <QAbstractButton>

namespace Ui {
	class CutListDialog;
}

class CutListDialog : public QDialog
{
	Q_OBJECT

	public:

		CutListDialog(const QHash<int, int> cutList, int endFrame, QWidget *parent = 0);
		~CutListDialog();

		QHash<int, int> list() const;

	private:
		void setupCutListModelView(const QHash<int, int> cutList);

	private slots:
		void on_addButton_clicked();
		void on_removeButton_clicked();

		void loadCutList();
		void saveCutList();

		void on_dialogButtonBox_clicked(QAbstractButton *button);

	private:
		Ui::CutListDialog *ui;
		int m_endFrame;

		QStandardItemModel *m_model;
};

#endif
