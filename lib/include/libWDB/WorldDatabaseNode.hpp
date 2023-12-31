/*
 * @author amber
 * @date 02/09/2023
 */

#pragma once

#include <variant>

#include "libWDB/Group.hpp"
#include "libWDB/NodeType.hpp"
#include "libWDB/SubGroup.hpp"
#include "libWDB/SubItem.hpp"


namespace libWDB
{
	class WorldDatabaseNode
	{
		public:
			WorldDatabaseNode() = delete;

			explicit WorldDatabaseNode(const Group& group);

			explicit WorldDatabaseNode(const SubGroup& sub_group);

			explicit WorldDatabaseNode(const SubItem& sub_item);

			[[nodiscard]] auto Type() const -> NodeType;

			[[nodiscard]] auto GetGroup() const -> std::optional<std::reference_wrapper<const Group>>;

			[[nodiscard]] auto GetGroup() -> std::optional<std::reference_wrapper<Group>>;

			[[nodiscard]] auto GetSubGroup() const -> std::optional<std::reference_wrapper<const SubGroup>>;

			[[nodiscard]] auto GetSubGroup() -> std::optional<std::reference_wrapper<SubGroup>>;

			[[nodiscard]] auto GetSubItem() const -> std::optional<std::reference_wrapper<const SubItem>>;

			[[nodiscard]] auto GetSubItem() -> std::optional<std::reference_wrapper<SubItem>>;

		private:
			std::variant<Group, SubGroup, SubItem> node_data;
	};
} // namespace libWDB