/*
 * @author amber
 * @date 31/08/2023
 */

#pragma once

#include <list>
#include <optional>
#include <string>

#include <QAbstractItemModel>

#include "libWDB/WorldDatabase.hpp"

namespace WDBEditor
{
	class QWorldDatabase : public QAbstractItemModel
	{
		public:
			QWorldDatabase();

			auto SetModel(libWDB::WorldDatabase&& wdb) -> void;

			[[nodiscard]] auto GetModel() const -> const libWDB::WorldDatabase&;

			// Inherited from QAbstractItemModel
			[[nodiscard]] auto headerData(int section, Qt::Orientation orientation, int role) const
				-> QVariant override;

			[[nodiscard]] auto rowCount(const QModelIndex& parent) const -> int override;

			[[nodiscard]] auto columnCount(const QModelIndex& parent) const -> int override;

			[[nodiscard]] auto index(int row, int column, const QModelIndex& parent) const -> QModelIndex override;

			[[nodiscard]] auto parent(const QModelIndex& child) const -> QModelIndex override;

			[[nodiscard]] auto data(const QModelIndex& index, int role) const -> QVariant override;

		private:
			libWDB::WorldDatabase model;
	};

} // namespace WDBEditor