/*
 * @author amber
 * @date 09/09/2023
 */

#include "WDBEditor/QStructureView.hpp"

#include <QHeaderView>

namespace WDBEditor
{
	QStructureView::QStructureView(): QStructureView(nullptr) {}

	QStructureView::QStructureView(QWidget* parent):
		QWidget(parent),
		layout(new QHBoxLayout(parent)),
		h_splitter(this->PrepareHSplitter()),
		wdb_model(new QWorldDatabase()),
		tree_view(this->PrepareTreeView()),
		parameter_view(this->PrepareParameterView())
	{
		this->tree_view->setModel(this->wdb_model);

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

		delete this->wdb_model;
	}

	auto QStructureView::SetModel(libWDB::WorldDatabase&& wdb) -> void {
		this->wdb_model->SetModel(std::move(wdb));
	}

	auto QStructureView::GetModel() const -> const libWDB::WorldDatabase& {
		return this->wdb_model->GetModel();
	}

	auto QStructureView::PrepareHSplitter() -> QSplitter* {
		QSplitter* splitter = new QSplitter(Qt::Orientation::Horizontal);
		splitter->setChildrenCollapsible(false);

		this->layout->addWidget(splitter);

		return splitter;
	}

	auto QStructureView::PrepareTreeView() -> QTreeView* {
		QTreeView* treeView = new QTreeView(this->h_splitter);
		treeView->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
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