/*
 * @author amber
 * @date 02/09/2023
 */

#include "WDBEditor/PresenterParams.hpp"

namespace WDBEditor
{
	PresenterParams::~PresenterParams()
	{
		delete this->layout;
		delete this->presenter_title_edit;
	}
} // namespace WDBEditor