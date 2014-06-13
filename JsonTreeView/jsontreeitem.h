#ifndef JSON_TREE_ITEM_H
#define JSON_TREE_ITEM_H

#include <QList>
#include <QVariant>

class JsonTreeItem
{
	public:
		JsonTreeItem(const QList<QVariant> &data, JsonTreeItem *parent = 0);
		~JsonTreeItem();

		void appendChild(JsonTreeItem *child);

		JsonTreeItem *child(int row);
		int childCount() const;
		int columnCount() const;
		QVariant data(int column) const;
		int row() const;
		JsonTreeItem *parent();

	private:
		QList<JsonTreeItem *> childItems;
		QList<QVariant> itemData;
		JsonTreeItem *parentItem;
};
#endif
