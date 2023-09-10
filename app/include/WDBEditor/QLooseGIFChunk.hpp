/*
 * @author amber
 * @date 10/09/2023
 */

#pragma once

#include <QAbstractItemModel>

#include "libWDB/GIFChunk.hpp"


namespace WDBEditor
{
	class QLooseGIFChunk: public QAbstractItemModel
	{
		public:
			QLooseGIFChunk();

			auto SetModel(libWDB::GIFChunk&& gif_chunk) -> void;

			[[nodiscard]] auto headerData(int section, Qt::Orientation orientation, int role) const -> QVariant override;

			[[nodiscard]] auto rowCount(const QModelIndex& parent) const -> int override;

			[[nodiscard]] auto columnCount(const QModelIndex& parent) const -> int override;

			[[nodiscard]] auto index(int row, int column, const QModelIndex& parent) const -> QModelIndex override;

			[[nodiscard]] auto parent(const QModelIndex& child) const -> QModelIndex override;

			[[nodiscard]] auto data(const QModelIndex& index, int role) const -> QVariant override;

		private:
			libWDB::GIFChunk gif_chunk;
	};
} // namespace WDBEditor