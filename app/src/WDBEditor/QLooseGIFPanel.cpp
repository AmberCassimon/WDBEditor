/*
 * @author amber
 * @date 15/09/2023
 */

#include "WDBEditor/QLooseGIFPanel.hpp"

#include <QDebug>
#include <QFileDialog>

#include "WDBEditor/Util.hpp"

namespace WDBEditor
{
	QLooseGIFPanel::QLooseGIFPanel(): QLooseGIFPanel(nullptr) {}

	QLooseGIFPanel::QLooseGIFPanel(QWidget* parent):
		QWidget(parent),
		root_layout(new QVBoxLayout(this)),
		img_label(this->PrepareLabel()),
		export_button(this->PrepareExportButton())
	{
		this->root_layout->setAlignment(Qt::AlignCenter);

		connect(this->export_button, &QPushButton::clicked, this, &QLooseGIFPanel::ExportButtonClicked);
	}

	auto QLooseGIFPanel::SelectionChanged(const QModelIndex& current_index, const QModelIndex& previous_index) -> void
	{
		this->selected_image = std::make_optional(reinterpret_cast<libWDB::GIFImage*>(current_index.internalPointer()));

		if (this->selected_image.has_value())
		{
			libWDB::GIFImage* img = this->selected_image.value();

			QImage newImage = QImage(img->width, img->height, QImage::Format::Format_Indexed8);
			newImage.setColorCount(static_cast<int>(img->colors.size()));

			for (std::size_t color_idx = 0; color_idx < img->colors.size(); ++color_idx)
			{
				newImage.setColor(
					static_cast<int>(color_idx),
					qRgb(img->colors.at(color_idx).r, img->colors.at(color_idx).g, img->colors.at(color_idx).b)
				);
			}

			for (std::uint32_t row = 0; row < img->height; ++row)
			{
				for (std::uint32_t col = 0; col < img->width; ++col)
				{
					const std::size_t idx = (row * img->width) + col;

					newImage.setPixel(col, row, img->color_index.at(idx));
				}
			}

			this->img_label->setPixmap(QPixmap::fromImage(newImage));
		}
	}

	auto QLooseGIFPanel::PrepareLabel() -> QLabel*
	{
		QLabel* newLabel = new QLabel();
		newLabel->setMinimumSize(128, 128); // Biggest bitmaps in the native game have this size
											// Since its a minimum, it can still grow if users add larger bitmaps
		newLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		this->root_layout->addWidget(newLabel);

		return newLabel;
	}

	auto QLooseGIFPanel::PrepareExportButton() -> QPushButton*
	{
		QPushButton* exportButton = new QPushButton("Export");

		this->root_layout->addWidget(exportButton);

		return exportButton;
	}

	auto QLooseGIFPanel::ExportButtonClicked(bool checked) -> void
	{
		// No pixmap is set, don't save!
		if (nullptr == this->img_label->pixmap())
		{
			qDebug() << "No pixmap present!";
			return;
		}

		std::string output_filename = HomeDirectory();

		if (this->selected_image.has_value())
		{
			output_filename += ("/" + this->selected_image.value()->name);
		}
		else
		{
			output_filename += "/Image.BMP";
		}

		qDebug() << QString::fromStdString(output_filename);

		const QString filename = QFileDialog::getSaveFileName(
			this,
			"Save Image File",
			QString::fromStdString(output_filename)
		);

		// If the user closes the window, we get an empty string
		if (0 < filename.length())
		{
			// Open the file
			QFile file (filename);
			file.open(QIODevice::WriteOnly);

			this->img_label->pixmap()->save(&file, "BMP");

			file.close();
		}
	}
} // namespace WDBEditor