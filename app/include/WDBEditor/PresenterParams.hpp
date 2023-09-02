/*
 * @author amber
 * @date 02/09/2023
 */

#pragma once

#include <QFormLayout>
#include <QLineEdit>

namespace WDBEditor
{
	struct PresenterParams
	{
			PresenterParams() = default;

			~PresenterParams();

			QFormLayout* layout {nullptr};

			QLineEdit* presenter_title_edit {nullptr};
	};
} // namespace WDBEditor
