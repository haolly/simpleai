#pragma once

#include <QAbstractItemModel>
#include <SimpleAI.h>

namespace ai {
namespace debug {

class BehaviourTreeModelItem;

class BehaviourTreeModel: public QAbstractItemModel {
Q_OBJECT
private:
	BehaviourTreeModelItem *_rootItem;
public:
	explicit BehaviourTreeModel(QObject *parent = nullptr);
	~BehaviourTreeModel();

	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex &child) const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	void setRootNode(AIStateNode* node);
};

}
}
