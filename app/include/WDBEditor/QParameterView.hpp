/*
 * @author amber
 * @date 04/09/2023
 */

#pragma once

#include <optional>

#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QModelIndex>
#include <QWidget>

#include <libWDB/BinaryTreeNode.hpp>
#include <libWDB/WorldDatabaseNode.hpp>

namespace WDBEditor
{
	class QParameterView final: public QWidget
	{
			//NOLINTNEXTLINE
			Q_OBJECT;

		public:
			QParameterView();

			explicit QParameterView(QWidget* parent);

			~QParameterView() final;

		public slots:
			void ObjectChanged(const QModelIndex& current_index, const QModelIndex& previous_index);

			void GroupTitleChanged(const QString& text);

			void SubItemTitleChanged(const QString& text);

			void PresenterTitleChanged(const QString& text);

			void UnknownPresenterBytesChanged(const QString& text);

		signals:
			void ModelChanged();

		private:
			auto PrepareFormLayout() -> QFormLayout*;

			auto ResetParameters() -> void;

			auto ResetGroupParameters(const libWDB::Group& group) -> void;

			auto ResetSubGroupParameters(const libWDB::SubGroup& subgroup) -> void;

			auto ResetSubItemParameters(const libWDB::SubItem& subitem) -> void;

			QFormLayout* form;

			std::optional<libWDB::BinaryTreeNode<libWDB::WorldDatabaseNode>*> current_selection;
	};
} // namespace WDBEditor