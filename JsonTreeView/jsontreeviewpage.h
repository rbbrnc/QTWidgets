#ifndef JSON_TREE_VIEW_PAGE_H
#define JSON_TREE_VIEW_PAGE_H

#include <QWidget>

class JsonTreeModel;

namespace Ui {
	class JsonTreeViewPage;
}

class JsonTreeViewPage : public QWidget
{
	Q_OBJECT

	public:
		explicit JsonTreeViewPage(QWidget *parent = 0);
		~JsonTreeViewPage();

		void setFile(const QString &file);
        void setJsonData(const QByteArray &data);

	private:
		Ui::JsonTreeViewPage *ui;

		JsonTreeModel *m_model;
};
#endif
