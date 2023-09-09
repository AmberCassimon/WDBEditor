/*
 * @author amber
 * @date 30/08/2023
 */

#pragma once

#include <optional>

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QSplitter>
#include <QTreeView>

#include "WDBEditor/QLooseGIFView.hpp"
#include "WDBEditor/QStructureView.hpp"
#include "WDBEditor/QWorldDatabase.hpp"

namespace WDBEditor
{
	class MainWindow final : public QMainWindow
	{
			// NOLINTNEXTLINE
			Q_OBJECT;

		public:
			explicit MainWindow(QWidget* parent);

			~MainWindow() final;

		private:
			constexpr static QSize INITIAL_SIZE = QSize(800, 600);

			// Model
			bool dirty {false};
			std::optional<std::string> filename {std::nullopt};
			QWorldDatabase* qwdb {nullptr};

			// Actions for menu items
			QAction* open_file_act {nullptr};
			QAction* save_file_act {nullptr};
			QAction* save_as_file_act {nullptr};
			QAction* show_license_act {nullptr};
			QAction* show_version_act {nullptr};

			// Menus
			QMenu* file_menu {nullptr};
			QMenu* about_menu {nullptr};

			// Main UI
			QTabWidget* tab_widget;

			QStructureView* structure_view {nullptr};
			QLooseGIFView* loose_gif_view {nullptr};

			// Setup for UI
			// Prepare Actions
			auto PrepareOpenAction() -> QAction*;

			auto PrepareSaveAction() -> QAction*;

			auto PrepareSaveAsAction() -> QAction*;

			auto PrepareShowLicenseAction() -> QAction*;

			auto PrepareShowVersionAction() -> QAction*;

			// Prepare Menu
			auto PrepareFileMenu() -> QMenu*;

			auto PrepareAboutMenu() -> QMenu*;

			// Prepare Main UI
			auto PrepareTabWidget() -> QTabWidget*;

			auto PrepareStructureView() -> QStructureView*;

			auto PrepareLooseGIFView() -> QLooseGIFView*;

			// Updating the UI
			auto UpdateWindowTitle() -> void;

			// Methods invoked when menu items are pressed
		private slots:
			void Open();

			void Save();

			void SaveAs();

			void ShowLicense();

			void ShowVersion();

			void ModelChanged();
	};
} // namespace WDBEditor