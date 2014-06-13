/*
    JSON Tree Item

    A container for items of data supplied by the JSON Tree Model.
*/

#include <QStringList>

#include "jsontreeitem.h"

JsonTreeItem::JsonTreeItem(const QList<QVariant> &data, JsonTreeItem *parent)
{
	parentItem = parent;
	itemData = data;
}

JsonTreeItem::~JsonTreeItem()
{
	qDeleteAll(childItems);
}

void JsonTreeItem::appendChild(JsonTreeItem *item)
{
	childItems.append(item);
}

JsonTreeItem *JsonTreeItem::child(int row)
{
	return childItems.value(row);
}

int JsonTreeItem::childCount() const
{
	return childItems.count();
}

int JsonTreeItem::columnCount() const
{
	return itemData.count();
}

QVariant JsonTreeItem::data(int column) const
{
	return itemData.value(column);
}

JsonTreeItem *JsonTreeItem::parent()
{
	return parentItem;
}

int JsonTreeItem::row() const
{
	if (parentItem) {
		return parentItem->childItems.indexOf(const_cast<JsonTreeItem *>(this));
	}

	return 0;
}
