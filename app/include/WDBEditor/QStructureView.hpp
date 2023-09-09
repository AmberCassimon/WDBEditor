/*
 * @author amber
 * @date 09/09/2023
 */

#pragma once

#include <QSplitter>
#include <QTreeView>
#include <QWidget>

#include "WDBEditor/QParameterView.hpp"
#include "WDBEditor/QWorldDatabase.hpp"

namespace WDBEditor
{
	class QStructureView final: public QWidget
	{
			// NOLINTNEXTLINE
			Q_OBJECT;

		public:
			QStructureView();

			explicit QStructureView(QWidget* parent);

			~QStructureView();

			auto SetModel(libWDB::WorldDatabase&& wdb) -> void;

			auto GetModel() const -> const libWDB::WorldDatabase&;

		signals:
				void ModelChanged();

		private:
			// Main UI
			QHBoxLayout* layout;

			QSplitter* h_splitter;

			// Tree View
			QWorldDatabase* wdb_model {nullptr};
			QTreeView* tree_view {nullptr};

			QParameterView* parameter_view {nullptr};

			// Prepare UI Elements
			auto PrepareHSplitter() -> QSplitter*;

			auto PrepareTreeView() -> QTreeView*;

			auto PrepareParameterView() -> QParameterView*;

		private slots:
			void _ModelChanged();
	};
} // namespace WDBEditor