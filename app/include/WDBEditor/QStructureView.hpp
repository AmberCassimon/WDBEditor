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
			QStructureView(QWorldDatabase* qwdb);

			explicit QStructureView(QWorldDatabase* qwdb, QWidget* parent);

			~QStructureView();

			auto SetModel(QAbstractItemModel* model) -> void;

		signals:
				void ModelChanged();

		private:
			// Main UI
			QHBoxLayout* layout;

			QSplitter* h_splitter;

			// Tree View
			QTreeView* tree_view {nullptr};

			QParameterView* parameter_view {nullptr};

			// Prepare UI Elements
			auto PrepareHSplitter() -> QSplitter*;

			auto PrepareTreeView(QWorldDatabase* qwdb) -> QTreeView*;

			auto PrepareParameterView() -> QParameterView*;

		private slots:
			void _ModelChanged();
	};
} // namespace WDBEditor