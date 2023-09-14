/*
 * @author amber
 * @date 02/09/2023
 */

#pragma once

#include <iterator>
#include <optional>
#include <vector>

namespace libWDB
{
	template <typename T>
	class BinaryTreeNode
	{
		public:
			BinaryTreeNode() = delete;

			explicit BinaryTreeNode(T&& data);

			BinaryTreeNode(const BinaryTreeNode<T>& other) = delete;

			BinaryTreeNode(BinaryTreeNode<T>&& other) noexcept;

			auto operator=(const BinaryTreeNode<T>& other) -> BinaryTreeNode<T>& = delete;

			auto operator=(BinaryTreeNode<T>&& other) noexcept -> BinaryTreeNode<T>&;

			~BinaryTreeNode();

			[[nodiscard]] auto HasParent() const noexcept -> bool;

			[[nodiscard]] auto Parent() noexcept -> std::optional<BinaryTreeNode<T>*>;

			[[nodiscard]] auto Parent() const noexcept -> std::optional<const BinaryTreeNode<T>*>;

			auto SetParent(BinaryTreeNode<T>* parent) -> void;

			[[nodiscard]] auto HasSiblings() const noexcept -> bool;

			[[nodiscard]] auto Siblings() -> std::vector<BinaryTreeNode<T>*>;

			auto AddSibling(T&& new_sibling_data) -> BinaryTreeNode<T>*;

			auto AddSibling(BinaryTreeNode<T>* sibling) -> void;

			[[nodiscard]] auto HasChildren() const noexcept -> bool;

			[[nodiscard]] auto Children() const -> std::vector<BinaryTreeNode<T>*>;

			auto AddChild(T&& new_child_data) -> BinaryTreeNode<T>*;

			[[nodiscard]] auto Data() noexcept -> T&;

			[[nodiscard]] auto Data() const noexcept -> const T&;

		private:
			T data;

			std::optional<BinaryTreeNode<T>*> parent {std::nullopt};
			std::optional<BinaryTreeNode<T>*> next_sibling {std::nullopt};
			std::optional<BinaryTreeNode<T>*> first_child {std::nullopt};
	};

	template <typename T>
	BinaryTreeNode<T>::BinaryTreeNode(T&& data): data(std::move(data))
	{
	}

	template <typename T>
	BinaryTreeNode<T>::BinaryTreeNode(BinaryTreeNode<T>&& other) noexcept:
		parent(other.parent), next_sibling(other.next_sibling), first_child(other.first_child)
	{
		other.parent = std::nullopt;
		other.next_sibling = std::nullopt;
		other.first_child = std::nullopt;
	}

	template <typename T>
	auto BinaryTreeNode<T>::operator=(BinaryTreeNode<T>&& other) noexcept -> BinaryTreeNode<T>&
	{
		this->parent = std::move(other.parent);
		this->next_sibling = std::move(other.next_sibling);
		this->first_child = std::move(other.first_child);

		other.parent = std::nullopt;
		other.next_sibling = std::nullopt;
		other.first_child = std::nullopt;

		return *this;
	}

	template <typename T>
	BinaryTreeNode<T>::~BinaryTreeNode()
	{
		if (this->first_child.has_value())
		{
			delete this->first_child.value();
		}

		if (this->next_sibling.has_value())
		{
			delete this->next_sibling.value();
		}
	}

	template <typename T>
	auto BinaryTreeNode<T>::HasParent() const noexcept -> bool
	{
		return this->parent.has_value();
	}

	template <typename T>
	auto BinaryTreeNode<T>::Parent() noexcept -> std::optional<BinaryTreeNode<T>*>
	{
		return this->parent;
	}

	template <typename T>
	auto BinaryTreeNode<T>::Parent() const noexcept -> std::optional<const BinaryTreeNode<T>*>
	{
		return this->parent;
	}

	template <typename T>
	auto BinaryTreeNode<T>::SetParent(BinaryTreeNode<T>* parent) -> void
	{
		this->parent = std::make_optional<BinaryTreeNode<T>*>(parent);
	}

	template <typename T>
	auto BinaryTreeNode<T>::HasSiblings() const noexcept -> bool
	{
		return this->next_sibling.has_value();
	}

	template <typename T>
	auto BinaryTreeNode<T>::Siblings() -> std::vector<BinaryTreeNode<T>*>
	{
		if (!this->next_sibling.has_value())
		{
			return std::vector<BinaryTreeNode<T>*>();
		}

		const std::vector<BinaryTreeNode<T>*> second_order_siblings = this->next_sibling.value()->Siblings();

		std::vector<BinaryTreeNode<T>*> siblings {this->next_sibling.value()};
		siblings.reserve(second_order_siblings.size() + 1);

		std::copy(std::begin(second_order_siblings), std::end(second_order_siblings), std::back_inserter(siblings));

		return siblings;
	}

	template <typename T>
	auto BinaryTreeNode<T>::AddSibling(T&& new_sibling_data) -> BinaryTreeNode<T>*
	{
		if (this->next_sibling.has_value())
		{
			return this->next_sibling.value()->AddSibling(std::move(new_sibling_data));
		}

		BinaryTreeNode<T>* bt_node = new BinaryTreeNode<T>(std::move(new_sibling_data));

		if (this->parent.has_value())
		{
			bt_node->SetParent(this->parent.value());
		}

		this->next_sibling = std::make_optional<BinaryTreeNode*>(bt_node);

		return bt_node;
	}

	template <typename T>
	auto BinaryTreeNode<T>::AddSibling(BinaryTreeNode<T>* sibling) -> void
	{
		if (this->next_sibling.has_value())
		{
			return this->next_sibling.value()->AddSibling(sibling);
		}

		if (this->parent.has_value())
		{
			sibling->SetParent(this->parent.value());
		}

		this->next_sibling = std::make_optional<BinaryTreeNode*>(sibling);
	}

	template <typename T>
	auto BinaryTreeNode<T>::HasChildren() const noexcept -> bool
	{
		return this->first_child.has_value();
	}

	template <typename T>
	auto BinaryTreeNode<T>::Children() const -> std::vector<BinaryTreeNode<T>*>
	{
		if (!this->first_child.has_value())
		{
			return std::vector<BinaryTreeNode<T>*>();
		}

		const std::vector<BinaryTreeNode<T>*> first_child_siblings = this->first_child.value()->Siblings();

		std::vector<BinaryTreeNode<T>*> children {this->first_child.value()};
		children.reserve(first_child_siblings.size() + 1);

		std::copy(std::begin(first_child_siblings), std::end(first_child_siblings), std::back_inserter(children));

		return children;
	}

	template <typename T>
	auto BinaryTreeNode<T>::AddChild(T&& new_child_data) -> BinaryTreeNode<T>*
	{
		if (this->first_child.has_value())
		{
			return this->first_child.value()->AddSibling(std::move(new_child_data));
		}

		BinaryTreeNode<T>* bt_node = new BinaryTreeNode<T>(std::move(new_child_data));
		bt_node->SetParent(this);

		this->first_child = std::make_optional<BinaryTreeNode*>(bt_node);

		return bt_node;
	}

	template <typename T>
	auto BinaryTreeNode<T>::Data() noexcept -> T&
	{
		return this->data;
	}

	template <typename T>
	auto BinaryTreeNode<T>::Data() const noexcept -> const T&
	{
		return this->data;
	}
} // namespace libWDB