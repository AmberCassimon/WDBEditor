/*
 * @author amber
 * @date 15/09/2023
 */

#pragma once

#include <QLabel>
#include <QModelIndex>
#include <QVBoxLayout>
#include <QWidget>

namespace WDBEditor
{
	class QLooseGIFPanel: public QWidget
	{
		public:
			QLooseGIFPanel();

			explicit QLooseGIFPanel(QWidget* parent);

			auto SelectionChanged(const QModelIndex& current_index, const QModelIndex& previous_index) -> void;

		private:
			QVBoxLayout* root_layout {nullptr};

			QLabel* label {nullptr};

			auto PrepareLabel() -> QLabel*;
	};
} // namespace WDBEditor