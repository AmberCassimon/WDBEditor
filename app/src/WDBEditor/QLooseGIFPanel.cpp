/*
 * @author amber
 * @date 15/09/2023
 */

#include "WDBEditor/QLooseGIFPanel.hpp"

#include <QDebug>
#include <QFileDialog>

#include "libWDB/GIFImage.hpp"
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
		libWDB::GIFImage* gifImg = reinterpret_cast<libWDB::GIFImage*>(current_index.internalPointer());

		QImage newImage = QImage(gifImg->width, gifImg->height, QImage::Format::Format_Indexed8);
		newImage.setColorCount(static_cast<int>(gifImg->colors.size()));

		for (std::size_t color_idx = 0; color_idx < gifImg->colors.size(); ++color_idx)
		{
			newImage.setColor(
				static_cast<int>(color_idx),
				qRgb(gifImg->colors.at(color_idx).r, gifImg->colors.at(color_idx).g, gifImg->colors.at(color_idx).b)
			);
		}

		for (std::uint32_t row = 0; row < gifImg->height; ++row)
		{
			for (std::uint32_t col = 0; col < gifImg->width; ++col)
			{
				const std::size_t idx = (row * gifImg->width) + col;

				newImage.setPixel(col, row, gifImg->color_index.at(idx));
			}
		}

		this->img_label->setPixmap(QPixmap::fromImage(newImage));
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

		const QString filename = QFileDialog::getSaveFileName(
			this,
			"Save .GIF File",
			"C:\\Users\\amber\\G.GIF"
		);

		// Open the file
		QFile file (filename);
		file.open(QIODevice::WriteOnly);

		this->img_label->pixmap()->save(&file, "BMP");

		file.close();
	}
} // namespace WDBEditor