/*
 * @author amber
 * @date 31/08/2023
 */

#include <utility>

#include "WDBEditor/QWorldDatabase.hpp"

namespace WDBEditor
{
	QWorldDatabase::QWorldDatabase(): model() {}

	auto QWorldDatabase::SetModel(libWDB::WorldDatabase&& wdb) -> void
	{
		// Only perform the row removal if we actually have rows
		if (0 < this->rowCount(QModelIndex()))
		{
			this->beginRemoveRows(QModelIndex(), 0, this->rowCount(QModelIndex()) - 1);
			this->endRemoveRows();
		}

		this->model = std::move(wdb);

		this->beginInsertRows(QModelIndex(), 0, this->rowCount(QModelIndex()) - 1);
		this->endInsertRows();
	}

	auto QWorldDatabase::GetModel() const -> const libWDB::WorldDatabase& { return this->model; }

	auto QWorldDatabase::headerData(int section, Qt::Orientation orientation, int role) const -> QVariant
	{
		if ((Qt::Horizontal == orientation) && (Qt::DisplayRole == role))
		{
			std::array<QString, 3> sections {"Title", "Offset", "Size"};

			if (sections.size() <= section)
			{
				return QVariant();
			}

			return sections.at(section);
		}
		return QAbstractItemModel::headerData(section, orientation, role);
	}

	auto QWorldDatabase::rowCount(const QModelIndex& parent) const -> int
	{
		// Parent was invalid, get group count
		if (QModelIndex() == parent)
		{
			return static_cast<int>(this->model.Groups().size());
		}

		const libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>* parent_ptr =
			reinterpret_cast<libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>*>(parent.internalPointer());

		return static_cast<int>(parent_ptr->Children().size());
	}

	auto QWorldDatabase::columnCount(const QModelIndex& parent) const -> int { return 3; }

	auto QWorldDatabase::index(int row, int column, const QModelIndex& parent) const -> QModelIndex
	{
		if (column < 0)
		{
			return QModelIndex();
		}

		if (row < 0)
		{
			return QModelIndex();
		}

		// Parent was valid
		if (QModelIndex() != parent)
		{
			const libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>* parent_ptr =
				reinterpret_cast<libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>*>(parent.internalPointer());
			std::vector<libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>*> children = parent_ptr->Children();

			if (children.size() <= row)
			{
				return QModelIndex();
			}

			return this->createIndex(row, column, children.at(row));
		}

		// Requested Item was a group
		const std::vector<const libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>*> group_nodes = this->model.Groups();

		if (group_nodes.size() <= row)
		{
			return QModelIndex();
		}

		return this->createIndex(
			row, column, const_cast<libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>*>(group_nodes.at(row))
		);
	}

	auto QWorldDatabase::parent(const QModelIndex& child) const -> QModelIndex
	{
		if (QModelIndex() == child)
		{
			return QModelIndex();
		}

		const libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>* child_ptr =
			reinterpret_cast<libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>*>(child.internalPointer());

		if (child_ptr->HasParent())
		{
			// Get the parent
			const libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>* parent_ptr = child_ptr->Parent().value();

			// Next, we need to figure out the row of the parent
			int row = 0;

			// If the parent pointer has a parent, we can simply iterate over the children until we find this one
			// Otherwise, the parent must be at the root level, so we get the tree root, and run through the siblings
			if (parent_ptr->HasParent())
			{
				for (const libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>* sibling_ptr :
					 parent_ptr->Parent().value()->Children())
				{
					++row;
					if (parent_ptr == sibling_ptr)
					{
						break;
					}
				}
			}
			else
			{
				for (const libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>* group_ptr : this->model.Groups())
				{
					++row;
					if (parent_ptr == group_ptr)
					{
						break;
					}
				}
			}

			return this->createIndex(
				/* Column must be 0 here, since regardless of the column that is clicked, the parent is always in the first (tree) column */
				row, 0, const_cast<libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>*>(parent_ptr)
			);
		}
		else
		{
			return QModelIndex();
		}
	}

	auto QWorldDatabase::data(const QModelIndex& index, int role) const -> QVariant
	{
		if (Qt::DisplayRole != role)
		{
			return QVariant();
		}

		// Get the node
		const auto* bt_node =
			static_cast<const libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>*>(index.internalPointer());

		switch (bt_node->Data().Type())
		{
			case libWDB::NodeType::Group: {
				if (0 == index.column())
				{
					return QString::fromStdString(bt_node->Data().GetGroup().value().title);
				}
				else
				{
					return QVariant();
				}
			}
			case libWDB::NodeType::SubGroup: {
				if (0 == index.column())
				{
					return QString::fromStdString(std::to_string(index.row()));
				}
				else
				{
					return QVariant();
				}
			}
			case libWDB::NodeType::SubItem: {
				switch(index.column())
				{
					case 0: {
						return QString::fromStdString(bt_node->Data().GetSubItem().value().title);
					}

					case 1: {
						return QString::fromStdString(std::to_string(bt_node->Data().GetSubItem().value().offset));
					}

					case 2: {
						return QString::fromStdString(std::to_string(bt_node->Data().GetSubItem().value().size));
					}

					default:
						return QVariant();
				}

			}
		}
	}
} // namespace WDBEditor