/*
 * @author amber
 * @date 15/09/2023
 */

#include "WDBEditor/QLooseGIFPanel.hpp"

#include <QDebug>

#include "libWDB/GIFImage.hpp"

namespace WDBEditor
{
	QLooseGIFPanel::QLooseGIFPanel(): QLooseGIFPanel(nullptr) {}

	QLooseGIFPanel::QLooseGIFPanel(QWidget* parent):
		QWidget(parent), root_layout(new QVBoxLayout(this)), label(this->PrepareLabel())
	{
	}

	auto QLooseGIFPanel::SelectionChanged(const QModelIndex& current_index, const QModelIndex& previous_index) -> void
	{
		libWDB::GIFImage* gifImg = reinterpret_cast<libWDB::GIFImage*>(current_index.internalPointer());

		QImage newImage = QImage(gifImg->width, gifImg->height, QImage::Format::Format_RGB888);
		newImage.setColorCount(static_cast<int>(gifImg->colors.size()));

		qDebug() << "colorCount" << newImage.colorCount();

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

		this->label->setPixmap(QPixmap::fromImage(newImage));

		qDebug() << "Selected " << gifImg->name.c_str();
	}

	auto QLooseGIFPanel::PrepareLabel() -> QLabel*
	{
		QLabel* newLabel = new QLabel();
		QImage newImage = QImage(1, 1, QImage::Format::Format_RGB888);

		newLabel->setPixmap(QPixmap::fromImage(newImage));

		this->root_layout->addWidget(newLabel);

		return newLabel;
	}
} // namespace WDBEditor