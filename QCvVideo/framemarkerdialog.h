#ifndef FRAMEMARKERDIALOG_H
#define FRAMEMARKERDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QList>

namespace Ui {
	class FrameMarkerDialog;
}

class FrameMarkerDialog : public QDialog
{
	Q_OBJECT

	public:
		explicit FrameMarkerDialog(const QList<int> list, QWidget *parent = 0);
		~FrameMarkerDialog();

	private:
		void setupModelView(const QList<int> list);

	private:
		Ui::FrameMarkerDialog *ui;

		QStandardItemModel *m_model;
};

#endif // FRAMEMARKERDIALOG_H
