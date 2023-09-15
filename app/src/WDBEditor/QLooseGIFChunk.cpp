/*
 * @author amber
 * @date 10/09/2023
 */

#include "WDBEditor/QLooseGIFChunk.hpp"

#include <array>

namespace WDBEditor
{
	QLooseGIFChunk::QLooseGIFChunk(): gif_chunk(libWDB::GIFChunk {}) {}

	auto QLooseGIFChunk::SetModel(libWDB::GIFChunk&& gif_chunk) -> void {
		// Only perform the row removal if we actually have rows
		if (0 < this->rowCount(QModelIndex()))
		{
			this->beginRemoveRows(QModelIndex(), 0, this->rowCount(QModelIndex()) - 1);
			this->endRemoveRows();
		}

		this->gif_chunk = std::move(gif_chunk);

		this->beginInsertRows(QModelIndex(), 0, this->rowCount(QModelIndex()) - 1);
		this->endInsertRows();
	}

	auto QLooseGIFChunk::headerData(int section, Qt::Orientation orientation, int role) const -> QVariant
	{
		if ((Qt::Horizontal == orientation) && (Qt::DisplayRole == role))
		{
			std::array<QString, 5> sections {"Filename", "Width", "Height", "Palette Size", ""};

			if (sections.size() <= section)
			{
				return QVariant();
			}

			return sections.at(section);
		}

		return QAbstractItemModel::headerData(section, orientation, role);
	}

	auto QLooseGIFChunk::rowCount(const QModelIndex& parent) const -> int
	{
		// Parent was invalid, get group count
		if (QModelIndex() == parent)
		{
			return static_cast<int>(this->gif_chunk.images.size());
		}

		return 0;
	}

	auto QLooseGIFChunk::columnCount(const QModelIndex& parent) const -> int { return 5; }

	auto QLooseGIFChunk::index(int row, int column, const QModelIndex& parent) const -> QModelIndex
	{
		if (this->rowCount(parent) <= row)
		{
			return QModelIndex();
		}

		return this->createIndex(row, column, static_cast<void*>(const_cast<libWDB::GIFImage*>(&(this->gif_chunk.images.at(row)))));
	}

	auto QLooseGIFChunk::parent(const QModelIndex& child) const -> QModelIndex { return QModelIndex(); }

	auto QLooseGIFChunk::data(const QModelIndex& index, int role) const -> QVariant {
		if ((Qt::DisplayRole != role) && (Qt::TextAlignmentRole != role))
		{
			return QVariant();
		}

		if (this->rowCount(index.parent()) <= index.row())
		{
			return QVariant();
		}

		if (this->columnCount(index.parent()) <= index.column())
		{
			return QVariant();
		}

		const libWDB::GIFImage& img = this->gif_chunk.images.at(index.row());

		switch (index.column())
		{
			case 0:
				if (Qt::DisplayRole == role)
				{
					return QString::fromStdString(img.name);
				}
				else if (Qt::TextAlignmentRole == role)
				{
					return Qt::AlignLeft;
				}

			case 1:
				if (Qt::DisplayRole == role)
				{
					return QString::fromStdString(std::to_string(img.width));
				}
				else if (Qt::TextAlignmentRole == role)
				{
					return Qt::AlignRight;
				}

			case 2:
				if (Qt::DisplayRole == role)
				{
					return QString::fromStdString(std::to_string(img.height));
				}
				else if (Qt::TextAlignmentRole == role)
				{
					return Qt::AlignRight;
				}

			case 3:
				if (Qt::DisplayRole == role)
				{
					return QString::fromStdString(std::to_string(img.colors.size()));
				}
				else if (Qt::TextAlignmentRole == role)
				{
					return Qt::AlignRight;
				}

			default:
				return QVariant();
		}
	}
} // namespace WDBEditor