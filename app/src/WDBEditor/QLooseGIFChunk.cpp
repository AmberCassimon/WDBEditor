/*
 * @author amber
 * @date 10/09/2023
 */

#include "WDBEditor/QLooseGIFChunk.hpp"

#include <array>

namespace WDBEditor
{
	QLooseGIFChunk::QLooseGIFChunk(): gif_chunk(libWDB::GIFChunk {}) {}

	auto QLooseGIFChunk::SetModel(libWDB::GIFChunk&& gif_chunk) -> void { this->gif_chunk = std::move(gif_chunk); }

	auto QLooseGIFChunk::headerData(int section, Qt::Orientation orientation, int role) const -> QVariant
	{
		if ((Qt::Horizontal == orientation) && (Qt::DisplayRole == role))
		{
			std::array<QString, 4> sections {"Filename", "Width", "Height", ""};

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
		return static_cast<int>(this->gif_chunk.images.size());
	}

	auto QLooseGIFChunk::columnCount(const QModelIndex& parent) const -> int { return 4; }

	auto QLooseGIFChunk::index(int row, int column, const QModelIndex& parent) const -> QModelIndex
	{
		return this->createIndex(row, column);
	}

	auto QLooseGIFChunk::parent(const QModelIndex& child) const -> QModelIndex { return QModelIndex(); }

	auto QLooseGIFChunk::data(const QModelIndex& index, int role) const -> QVariant {
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
				return QString::fromStdString(img.name);

			case 1:
				return QString::fromStdString(std::to_string(img.width));

			case 2:
				return QString::fromStdString(std::to_string(img.height));

			default:
				return QVariant();
		}
	}
} // namespace WDBEditor