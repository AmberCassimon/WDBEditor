/*
 * @author amber
 * @date 15/09/2023
 */

#pragma once

#include <optional>

#include <QLabel>
#include <QModelIndex>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "libWDB/GIFImage.hpp"

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
			std::optional<libWDB::GIFImage*> selected_image {std::nullopt};

			QVBoxLayout* root_layout {nullptr};

			QLabel* img_label {nullptr};
			QPushButton* export_button {nullptr};

			auto PrepareLabel() -> QLabel*;

			auto PrepareExportButton() -> QPushButton*;
	};
} // namespace WDBEditor