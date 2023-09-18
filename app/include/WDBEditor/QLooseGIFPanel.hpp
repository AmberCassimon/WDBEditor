/*
 * @author amber
 * @date 15/09/2023
 */

#pragma once

#include <QLabel>
#include <QModelIndex>
#include <QPushButton>
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

		public slots:
			auto ExportButtonClicked(bool checked) -> void;

		private:
			QVBoxLayout* root_layout {nullptr};

			QLabel* img_label {nullptr};
			QPushButton* export_button {nullptr};

			auto PrepareLabel() -> QLabel*;

			auto PrepareExportButton() -> QPushButton*;
	};
} // namespace WDBEditor