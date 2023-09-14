/*
 * @author amber
 * @date 09/09/2023
 */

#pragma once

#include <QHBoxLayout>
#include <QListView>
#include <QSplitter>
#include <QTreeView>
#include <QWidget>

#include "WDBEditor/QLooseGIFChunk.hpp"

namespace WDBEditor
{
	class QLooseGIFView final: public QWidget
	{
			// NOLINTNEXTLINE
			Q_OBJECT;

		public:
			explicit QLooseGIFView(QWidget* parent);

			auto SetModel(QAbstractItemModel* model) -> void;

		private:
			QHBoxLayout* root_layout {nullptr};

			QSplitter* h_splitter {nullptr};

			QTreeView* gif_list {nullptr};

			auto PrepareHSplitter() -> QSplitter*;

			auto PrepareGIFList() -> QTreeView*;
	};
} // namespace WDBEditor