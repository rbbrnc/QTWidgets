//
// JSON Tree Model
//
// TODO:
//	Rendere il modello editabile;
//	Metodo save per salvare su file le modifiche (edit)
//

#include <QtGui>
#include <QJsonDocument>
#include <QDebug>

#include "jsontreeitem.h"
#include "jsontreemodel.h"

JsonTreeModel::JsonTreeModel(QJsonDocument *doc, QObject *parent)
	: QAbstractItemModel(parent),
	  m_doc(doc)
{
	QList<QVariant> rootData;
	rootData << "Key" << "Value";

	m_rootItem = new JsonTreeItem(rootData);
	setupModelData(m_rootItem);
}

JsonTreeModel::~JsonTreeModel()
{
	delete m_rootItem;
}

int JsonTreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid()) {
		return static_cast<JsonTreeItem *>(parent.internalPointer())->columnCount();
	}

	return m_rootItem->columnCount();
}

QVariant JsonTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	if (role == Qt::DisplayRole) {
		JsonTreeItem *item = static_cast<JsonTreeItem*>(index.internalPointer());
		return item->data(index.column());
	}

	return QVariant();
}

Qt::ItemFlags JsonTreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return 0;
	}

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant JsonTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		return m_rootItem->data(section);
	}

	return QVariant();
}

QModelIndex JsonTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent)) {
		return QModelIndex();
	}

	JsonTreeItem *parentItem;

	if (!parent.isValid()) {
		parentItem = m_rootItem;
	} else {
		parentItem = static_cast<JsonTreeItem*>(parent.internalPointer());
	}

	JsonTreeItem *childItem = parentItem->child(row);
	if (childItem) {
		return createIndex(row, column, childItem);
	}

	return QModelIndex();
}

QModelIndex JsonTreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid()) {
		return QModelIndex();
	}

	JsonTreeItem *childItem = static_cast<JsonTreeItem*>(index.internalPointer());
	JsonTreeItem *parentItem = childItem->parent();

	if (parentItem == m_rootItem) {
		return QModelIndex();
	}

	return createIndex(parentItem->row(), 0, parentItem);
}

int JsonTreeModel::rowCount(const QModelIndex &parent) const
{
	JsonTreeItem *parentItem;
	if (parent.column() > 0) {
		return 0;
	}

	if (!parent.isValid()) {
		parentItem = m_rootItem;
	} else {
		parentItem = static_cast<JsonTreeItem*>(parent.internalPointer());
	}

	return parentItem->childCount();
}

void JsonTreeModel::addValueNode(const QJsonValue &val, const QString &key, JsonTreeItem *parent)
{
    QList<QVariant> columnData;
    columnData << key << "";

    switch (val.type()) {
    case QJsonValue::Null:
        qDebug() << key << "is null";
        break;
    case QJsonValue::Bool:
        qDebug() << key << "is bool";
        columnData[1] = val.toBool();
        break;
    case QJsonValue::Double:
        qDebug() << key << "is double";
        columnData[1] = val.toDouble();
        break;
    case QJsonValue::String:
        qDebug() << key << "is string";
        columnData[1] = val.toString();
        break;
    case QJsonValue::Array:
        qDebug() << key << "is array";
        addArrayNode(val.toArray(), key, parent);
        return;
    case QJsonValue::Object:
        addObjectNode(val.toObject(), key, parent);
        return;
    case QJsonValue::Undefined:
    default:
        qDebug() << key << "is undefined";
        return;
    }

    JsonTreeItem *node = new JsonTreeItem(columnData, parent);
    parent->appendChild(node);
}

void JsonTreeModel::addObjectNode(const QJsonObject &obj, const QString &key, JsonTreeItem *parent)
{
    // Add Object Root Node
    QList<QVariant> columnData;
    columnData << key << "";

    JsonTreeItem *node = new JsonTreeItem(columnData, parent);
    parent->appendChild(node);

    // Add Object Children
    QStringList keys = obj.keys();

    for (int i = 0; i < keys.size(); i++) {
        addValueNode(obj[keys.at(i)], keys.at(i), node);
    }
}

void JsonTreeModel::addArrayNode(const QJsonArray &array, const QString &key, JsonTreeItem *parent)
{
    // Add Array Root Node
    QList<QVariant> columnData;
    columnData << key << "";

    JsonTreeItem *node = new JsonTreeItem(columnData, parent);
    parent->appendChild(node);

    for (int i = 0; i < array.size(); i++) {
        QString nodeName = QString("%1[%2]").arg(key).arg(i);
        addValueNode(array.at(i), nodeName, node);
    }
}

// Fill model with data
void JsonTreeModel::setupModelData(JsonTreeItem *parent)
{
    QJsonObject obj = m_doc->object();
    QStringList keys = obj.keys();

	for (int i = 0; i < keys.size(); i++) {
        addValueNode(obj[keys.at(i)], keys.at(i), parent);
	}
}
