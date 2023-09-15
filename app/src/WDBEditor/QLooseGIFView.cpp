/*
 * @author amber
 * @date 09/09/2023
 */

#include "WDBEditor/QLooseGIFView.hpp"

#include <QHeaderView>

namespace WDBEditor
{
	QLooseGIFView::QLooseGIFView(QWidget* parent):
		QWidget(parent),
		root_layout(new QHBoxLayout(this)),
		h_splitter(this->PrepareHSplitter()),
		gif_list(this->PrepareGIFList()),
		gif_panel(this->PrepareGIFPanel())
	{
		this->SetModel(new QLooseGIFChunk());

		this->h_splitter->setSizes({INT_MAX});

		connect(
			this->gif_list->selectionModel(),
			&QItemSelectionModel::currentRowChanged,
			this->gif_panel,
			&QLooseGIFPanel::SelectionChanged
		);
	}

	auto QLooseGIFView::SetModel(QAbstractItemModel* model) -> void {
		this->gif_list->setModel(model);

		connect(
			this->gif_list->selectionModel(),
			&QItemSelectionModel::currentRowChanged,
			this->gif_panel,
			&QLooseGIFPanel::SelectionChanged
		);
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
		QTreeView* tree_view = new QTreeView();
		tree_view->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
		tree_view->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
		tree_view->setAlternatingRowColors(true);
		tree_view->setItemsExpandable(false);
		tree_view->setIndentation(0);
		tree_view->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

		this->h_splitter->addWidget(tree_view);

		return tree_view;
	}

	auto QLooseGIFView::PrepareGIFPanel() -> QLooseGIFPanel* {
		QLooseGIFPanel* panel = new QLooseGIFPanel();

		this->h_splitter->addWidget(panel);

		return panel;
	}
} // namespace WDBEditor