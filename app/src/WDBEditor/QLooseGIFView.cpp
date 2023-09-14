/*
 * @author amber
 * @date 09/09/2023
 */

#include "WDBEditor/QLooseGIFView.hpp"

namespace WDBEditor
{
	QLooseGIFView::QLooseGIFView(QWidget* parent):
		QWidget(parent),
		root_layout(new QHBoxLayout(this)),
		h_splitter(this->PrepareHSplitter()),
		gif_list(this->PrepareGIFList())
	{
		this->SetModel(new QLooseGIFChunk());

		this->h_splitter->setSizes({INT_MAX});
	}

	auto QLooseGIFView::SetModel(QAbstractItemModel* model) -> void {
		this->gif_list->setModel(model);
	}

	auto QLooseGIFView::PrepareHSplitter() -> QSplitter*
	{
		QSplitter* splitter = new QSplitter();
		splitter->setChildrenCollapsible(false);

		this->root_layout->addWidget(splitter);

		return splitter;
	}

	auto QLooseGIFView::PrepareGIFList() -> QTreeView*
	{
		QTreeView* list_view = new QTreeView();

		this->h_splitter->addWidget(list_view);

		return list_view;
	}
} // namespace WDBEditor