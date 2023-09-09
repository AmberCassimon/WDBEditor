/*
 * @author amber
 * @date 09/09/2023
 */

#include "WDBEditor/QLooseGIFView.hpp"

namespace WDBEditor
{
	QLooseGIFView::QLooseGIFView(QWidget* parent):
		QWidget(parent), root_layout(new QHBoxLayout(this)), h_splitter(this->PrepareHSplitter()), gif_list()
	{
	}

	auto QLooseGIFView::PrepareHSplitter() -> QSplitter*
	{
		QSplitter* splitter = new QSplitter();
		splitter->setChildrenCollapsible(false);

		this->root_layout->addWidget(splitter);

		return splitter;
	}

	auto QLooseGIFView::PrepareGIFList() -> QListView* {
		QListView* list_view = new QListView();

		this->h_splitter->addWidget(list_view);

		return list_view;
	}
} // namespace WDBEditor