/*
 * @author amber
 * @date 09/09/2023
 */

#include "WDBEditor/QStructureView.hpp"

#include <QHeaderView>

namespace WDBEditor
{
	QStructureView::QStructureView(QWorldDatabase* qwdb): QStructureView(qwdb, nullptr) {}

	QStructureView::QStructureView(QWorldDatabase* qwdb, QWidget* parent):
		QWidget(parent),
		layout(new QHBoxLayout(this)),
		h_splitter(this->PrepareHSplitter()),
		tree_view(this->PrepareTreeView()),
		parameter_view(this->PrepareParameterView())
	{
		this->SetModel(qwdb);

		connect(
			this->tree_view->selectionModel(),
			&QItemSelectionModel::currentRowChanged,
			this->parameter_view,
			&QParameterView::ObjectChanged
		);

		connect(
			this->parameter_view,
			&QParameterView::ModelChanged,
			this,
			&QStructureView::_ModelChanged
		);

		this->h_splitter->setSizes({INT_MAX, INT_MAX});
	}

	QStructureView::~QStructureView() {
		// Delete Main UI
		// Deleting splitter also deletes tree view
		delete this->h_splitter;
	}

	auto QStructureView::SetModel(QAbstractItemModel* model) -> void {
		this->tree_view->setModel(model);

		connect(
			this->tree_view->selectionModel(),
			&QItemSelectionModel::currentRowChanged,
			this->parameter_view,
			&QParameterView::ObjectChanged
		);
	}

	auto QStructureView::PrepareHSplitter() -> QSplitter* {
		QSplitter* splitter = new QSplitter(Qt::Orientation::Horizontal);
		splitter->setChildrenCollapsible(false);

		this->layout->addWidget(splitter);

		return splitter;
	}

	auto QStructureView::PrepareTreeView() -> QTreeView* {
		QTreeView* treeView = new QTreeView(this->h_splitter);
		treeView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
		treeView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
		treeView->setAlternatingRowColors(true);
		treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

		this->h_splitter->addWidget(treeView);

		return treeView;
	}

	auto QStructureView::PrepareParameterView() -> QParameterView* {
		QParameterView* param_view = new QParameterView();

		this->h_splitter->addWidget(param_view);

		return param_view;
	}

	void QStructureView::_ModelChanged() {
		emit ModelChanged();
	}
} // namespace WDBEditor