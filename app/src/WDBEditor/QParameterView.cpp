/*
 * @author amber
 * @date 04/09/2023
 */

#include "WDBEditor/QParameterView.hpp"

#include <iomanip>
#include <sstream>

namespace WDBEditor
{
	QParameterView::QParameterView(): QParameterView(nullptr) {}

	QParameterView::QParameterView(QWidget* parent):
		QWidget(parent), form(this->PrepareFormLayout()), current_selection(std::nullopt)
	{
		this->ResetParameters();
	}

	QParameterView::~QParameterView() { delete this->form; }

	void QParameterView::ObjectChanged(const QModelIndex& current_index, const QModelIndex& previous_index) {
		if (QModelIndex() == current_index)
		{
			this->current_selection = std::nullopt;
			return;
		}

		libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>* bt_node =
			reinterpret_cast<libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>*>(current_index.internalPointer());

		if (nullptr == bt_node)
		{
			this->current_selection = std::nullopt;
			return;
		}

		this->current_selection = std::make_optional<libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>*>(bt_node);

		this->ResetParameters();
	}

	auto QParameterView::PrepareFormLayout() -> QFormLayout*
	{
		QFormLayout* form_layout = new QFormLayout(this);

		return form_layout;
	}

	auto QParameterView::ResetParameters() -> void {
		// Remove all existing rows
		// We need to count in reverse, to ensure we always have valid indices
		// Example: Say we have 2 rows
		// First iteration removes row 0
		// Now only 1 row is left at index 0
		// But our second iteration uses index 1
		// So it won't remove anything
		// This does not occur if we iterate in reverse
		for (int i = this->form->rowCount() - 1; 0 <= i; --i)
		{
			this->form->removeRow(i);
		}

		// If nothing is currently selected, we don't add any new rows
		if (!this->current_selection.has_value())
		{
			return;
		}

		// Add new rows
		switch(this->current_selection.value()->Data().Type())
		{
			case libWDB::NodeType::Group: {
				const libWDB::Group group = this->current_selection.value()->Data().GetGroup().value();

				this->ResetGroupParameters(group);
				return;
			}
			case libWDB::NodeType::SubGroup: {
				const libWDB::SubGroup subgroup = this->current_selection.value()->Data().GetSubGroup().value();

				this->ResetSubGroupParameters(subgroup);
				return;
			}
			case libWDB::NodeType::SubItem: {
				const libWDB::SubItem subitem = this->current_selection.value()->Data().GetSubItem().value();

				this->ResetSubItemParameters(subitem);
				return;
			}
		}
	}

	auto QParameterView::ResetGroupParameters(const libWDB::Group& group) -> void {
		const QString label = QString("Group Title:");
		const QString value = QString::fromStdString(group.title);

		this->form->addRow(new QLabel(label), new QLineEdit(value));
	}

	auto QParameterView::ResetSubGroupParameters(const libWDB::SubGroup& subgroup) -> void {
		// Subgroups have no parameters, as far as I can tell...
	}

	auto QParameterView::ResetSubItemParameters(const libWDB::SubItem& subitem) -> void {
		const QString title_label = QString("SubItem Title:");
		const QString title_value = QString::fromStdString(subitem.title);

		this->form->addRow(new QLabel(title_label), new QLineEdit(title_value));

		if (subitem.extra_data.has_value())
		{
			const QString presenter_title_label = QString("Presenter Title:");
			const QString presenter_title_value = QString::fromStdString(subitem.extra_data->presenter_title);

			this->form->addRow(new QLabel(presenter_title_label), new QLineEdit(presenter_title_value));

			// 37 Unknown Bytes
			std::stringstream hex_stream;
			hex_stream << std::uppercase << std::hex << std::setw(2) << std::setfill('0');

			for (std::size_t i = 0; i < subitem.extra_data.value().unknown.size(); ++i)
			{
				// unsigned char gets treated like char, to avoid that, cast to int
				hex_stream << static_cast<int>(subitem.extra_data.value().unknown[i]) << ' ' << std::setw(2);
			}

			const QString unknown_label = QString("Unknown:");
			const QString unknown_value = QString::fromStdString(hex_stream.str());

			this->form->addRow(new QLabel(unknown_label), new QLineEdit(unknown_value));
		}
	}
} // namespace WDBEditor