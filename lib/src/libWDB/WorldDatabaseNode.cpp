/*
 * @author amber
 * @date 02/09/2023
 */

#include "libWDB/WorldDatabaseNode.hpp"

#include <stdexcept>

namespace libWDB
{
	WorldDatabaseNode::WorldDatabaseNode(const Group& group): node_data(group) {}

	WorldDatabaseNode::WorldDatabaseNode(const SubGroup& sub_group): node_data(sub_group) {}

	WorldDatabaseNode::WorldDatabaseNode(const SubItem& sub_item): node_data(sub_item) {}

	auto libWDB::WorldDatabaseNode::Type() const -> NodeType {
		if (std::holds_alternative<Group>(this->node_data))
		{
			return NodeType::Group;
		}
		else if (std::holds_alternative<SubGroup>(this->node_data))
		{
			return NodeType::SubGroup;
		}
		else if (std::holds_alternative<SubItem>(this->node_data))
		{
			return NodeType::SubItem;
		}
		else
		{
			throw std::runtime_error("WorldDatabaseNode holds invalid data!");
		}
	}

	auto WorldDatabaseNode::GetGroup() const -> std::optional<Group> {
		if (!std::holds_alternative<Group>(this->node_data))
		{
			return std::nullopt;
		}

		return std::make_optional<Group>(std::get<Group>(this->node_data));
	}

	auto WorldDatabaseNode::GetSubGroup() const -> std::optional<SubGroup> {
		if (!std::holds_alternative<SubGroup>(this->node_data))
		{
			return std::nullopt;
		}

		return std::make_optional<SubGroup>(std::get<SubGroup>(this->node_data));
	}

	auto WorldDatabaseNode::GetSubItem() const -> std::optional<SubItem> {
		if (!std::holds_alternative<SubItem>(this->node_data))
		{
			return std::nullopt;
		}

		return std::make_optional<SubItem>(std::get<SubItem>(this->node_data));
	}
} // namespace libWDB
