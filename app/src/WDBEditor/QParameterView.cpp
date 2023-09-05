/*
 * @author amber
 * @date 04/09/2023
 */

#include "WDBEditor/QParameterView.hpp"

#include <charconv>
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

	auto QParameterView::GroupTitleChanged(const QString& text) -> void {
		// We don't care what's in the field, we have nothing selected
		if (!this->current_selection.has_value())
		{
			return;
		}

		// The current selection is not a group
		if (libWDB::NodeType::Group != this->current_selection.value()->Data().Type())
		{
			return;
		}

		libWDB::Group& group = this->current_selection.value()->Data().GetGroup()->get();
		group.title = text.toStdString();

		emit ModelChanged();
	}

	void QParameterView::SubItemTitleChanged(const QString& text) {
		// We don't care what's in the field, we have nothing selected
		if (!this->current_selection.has_value())
		{
			return;
		}

		// The current selection is not a group
		if (libWDB::NodeType::SubItem != this->current_selection.value()->Data().Type())
		{
			return;
		}

		libWDB::SubItem& subitem = this->current_selection.value()->Data().GetSubItem()->get();
		subitem.title = text.toStdString();

		emit ModelChanged();
	}

	void QParameterView::PresenterTitleChanged(const QString& text) {
		// We don't care what's in the field, we have nothing selected
		if (!this->current_selection.has_value())
		{
			return;
		}

		// The current selection is not a group
		if (libWDB::NodeType::SubItem != this->current_selection.value()->Data().Type())
		{
			return;
		}

		libWDB::SubItem& subitem = this->current_selection.value()->Data().GetSubItem()->get();

		if (!subitem.extra_data.has_value())
		{
			return;
		}

		libWDB::SubItemPresenterData& presenter_data = subitem.extra_data.value();
		presenter_data.presenter_title = text.toStdString();

		emit ModelChanged();
	}

	void QParameterView::UnknownPresenterBytesChanged(const QString& text) {
		// We don't care what's in the field, we have nothing selected
		if (!this->current_selection.has_value())
		{
			return;
		}

		// The current selection is not a group
		if (libWDB::NodeType::SubItem != this->current_selection.value()->Data().Type())
		{
			return;
		}

		libWDB::SubItem& subitem = this->current_selection.value()->Data().GetSubItem()->get();

		if (!subitem.extra_data.has_value())
		{
			return;
		}

		// First, parse the string and check that all the bytes are there
		std::stringstream sstream {text.toStdString()};
		std::string line;

		std::vector<unsigned char> byte_vals {};
		byte_vals.reserve(37);	// Reserve, but don't resize, so we can check the actual size

		while (std::getline(sstream, line, ' '))
		{
			const char* c_str = line.c_str();
			const char* c_str_end = c_str + line.length();

			unsigned char byte = 0;

			const std::from_chars_result res = std::from_chars(c_str, c_str_end, byte, 16);

			// Something went wrong in parsing
			if (std::errc{} != res.ec)
			{
				return;
			}

			byte_vals.push_back(byte);
		}

		// We didn't get all bytes
		if (37 != byte_vals.size())
		{
			return;
		}

		std::copy(byte_vals.begin(), byte_vals.end(), subitem.extra_data.value().unknown.begin());

		emit ModelChanged();
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

		QLineEdit* title_edit = new QLineEdit(QString::fromStdString(group.title));

		connect(
			title_edit,
			&QLineEdit::textChanged,
			this,
			&QParameterView::GroupTitleChanged
		);

		this->form->addRow(new QLabel(label), title_edit);
	}

	auto QParameterView::ResetSubGroupParameters(const libWDB::SubGroup& subgroup) -> void {
		// Subgroups have no parameters, as far as I can tell...
	}

	auto QParameterView::ResetSubItemParameters(const libWDB::SubItem& subitem) -> void {
		const QString title_label = QString("SubItem Title:");

		QLineEdit* title_edit = new QLineEdit(QString::fromStdString(subitem.title));

		connect(
			title_edit,
			&QLineEdit::textChanged,
			this,
			&QParameterView::SubItemTitleChanged
		);

		this->form->addRow(new QLabel(title_label), title_edit);

		if (subitem.extra_data.has_value())
		{
			const QString presenter_title_label = QString("Presenter Title:");

			QLineEdit* presenter_title_edit = new QLineEdit(QString::fromStdString(subitem.extra_data->presenter_title));

			connect(
				presenter_title_edit,
				&QLineEdit::textChanged,
				this,
				&QParameterView::PresenterTitleChanged
			);

			this->form->addRow(new QLabel(presenter_title_label), presenter_title_edit);

			// 37 Unknown Bytes
			std::stringstream hex_stream;
			std::stringstream mask_stream;
			hex_stream << std::uppercase << std::hex << std::setw(2) << std::setfill('0');

			for (std::size_t i = 0; i < subitem.extra_data.value().unknown.size(); ++i)
			{
				// unsigned char gets treated like char, to avoid that, cast to int
				hex_stream << static_cast<int>(subitem.extra_data.value().unknown[i]);
				mask_stream << "HH";

				if ((i + 1) < subitem.extra_data.value().unknown.size())
				{
					hex_stream << ' ' << std::setw(2);
					mask_stream << ' ';
				}
			}

			mask_stream << ';';

			const QString unknown_label = QString("Unknown:");
			const QString unknown_value = QString::fromStdString(hex_stream.str());

			QLineEdit* unknown_editor = new QLineEdit(unknown_value);
			unknown_editor->setMaxLength((subitem.extra_data.value().unknown.size() * 2) + (subitem.extra_data.value().unknown.size() - 1));
			unknown_editor->setInputMask(QString::fromStdString(mask_stream.str()));

			connect(
				unknown_editor,
				&QLineEdit::textChanged,
				this,
				&QParameterView::UnknownPresenterBytesChanged
			);

			this->form->addRow(new QLabel(unknown_label), unknown_editor);
		}
	}
} // namespace WDBEditor