#ifndef JSON_TREE_MODEL_H
#define JSON_TREE_MODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class QJsonDocument;
class JsonTreeItem;

class JsonTreeModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
		JsonTreeModel(QJsonDocument *doc, QObject *parent = 0);
		~JsonTreeModel();

		QVariant data(const QModelIndex &index, int role) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &index) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

	private:
		void setupModelData(JsonTreeItem *parent);
        void addObjectNode(const QJsonObject &obj, const QString &key, JsonTreeItem *parent);
        void addArrayNode(const QJsonArray &array, const QString &key, JsonTreeItem *parent);
        void addValueNode(const QJsonValue &val, const QString &key, JsonTreeItem *parent);

	private:
		QJsonDocument *m_doc;

		JsonTreeItem *m_rootItem;
};

#endif
