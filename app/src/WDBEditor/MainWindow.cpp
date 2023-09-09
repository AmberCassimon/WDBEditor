/*
 * @author amber
 * @date 30/08/2023
 */

#include "WDBEditor/MainWindow.hpp"

#include <cstdio>
#include <sstream>

#include <QDebug>
#include <QHeaderView>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QMenuBar>
#include <QMessageBox>
#include <QString>

#include "WDBEditor/QWorldDatabase.hpp"
#include "WDBEditor/Util.hpp"

#include "libWDB/Parser.hpp"
#include "libWDB/Serializer.hpp"
#include "libWDB/WDBParseException.hpp"

namespace WDBEditor
{
	MainWindow::MainWindow(QWidget* parent):
		QMainWindow(parent),
		open_file_act(this->PrepareOpenAction()),
		save_file_act(this->PrepareSaveAction()),
		save_as_file_act(this->PrepareSaveAsAction()),
		show_license_act(this->PrepareShowLicenseAction()),
		show_version_act(this->PrepareShowVersionAction()),
		file_menu(this->PrepareFileMenu()),
		about_menu(this->PrepareAboutMenu()),
		h_splitter(this->PrepareHSplitter()),
		wdb_model(new QWorldDatabase()),
		tree_view(this->PrepareTreeView()),
		parameter_view(this->PrepareParameterView())
	{
		this->resize(MainWindow::INITIAL_SIZE);

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
			&MainWindow::ModelChanged
		);

		this->h_splitter->setSizes({INT_MAX, INT_MAX});

		this->UpdateWindowTitle();
	}

	MainWindow::~MainWindow()
	{
		// File Menu
		delete this->file_menu;
		delete this->about_menu;

		delete this->open_file_act;
		delete this->save_file_act;
		delete this->save_as_file_act;
		delete this->show_license_act;
		delete this->show_version_act;

		// Delete Main UI
		// Deleting splitter also deletes tree view
		delete this->h_splitter;

		delete this->wdb_model;

		// Underlying model data
		this->dirty = false;
		this->filename.reset();
	}

	auto MainWindow::PrepareOpenAction() -> QAction*
	{
		QAction* openAction = new QAction("&Open", this);

		openAction->setShortcut(QKeySequence::Open);
		openAction->setStatusTip("Open a .wdb file");

		connect(openAction, &QAction::triggered, this, &MainWindow::Open);

		return openAction;
	}

	auto MainWindow::PrepareSaveAction() -> QAction*
	{
		QAction* openAction = new QAction("&Save", this);

		openAction->setShortcut(QKeySequence::Save);
		openAction->setStatusTip("Save the currently open .wdb file");

		connect(openAction, &QAction::triggered, this, &MainWindow::Save);

		return openAction;
	}
	auto MainWindow::PrepareSaveAsAction() -> QAction*
	{
		QAction* openAction = new QAction("&Save As", this);

		openAction->setShortcut(QKeySequence::SaveAs);
		openAction->setStatusTip("Save the currently open .wdb file under a custom name");

		connect(openAction, &QAction::triggered, this, &MainWindow::SaveAs);

		return openAction;
	}

	auto MainWindow::PrepareShowLicenseAction() -> QAction*
	{
		QAction* showLicenseAction = new QAction("&License", this);

		showLicenseAction->setStatusTip("Show the software license");

		connect(showLicenseAction, &QAction::triggered, this, &MainWindow::ShowLicense);

		return showLicenseAction;
	}

	auto MainWindow::PrepareShowVersionAction() -> QAction* {
		QAction* showVersionAction = new QAction("&Version", this);

		showVersionAction->setStatusTip("Show the software version");

		connect(showVersionAction, &QAction::triggered, this, &MainWindow::ShowVersion);

		return showVersionAction;
	}

	auto MainWindow::PrepareFileMenu() -> QMenu*
	{
		QMenu* fileMenu = this->menuBar()->addMenu("&File");

		fileMenu->addAction(this->open_file_act);
		fileMenu->addAction(this->save_file_act);
		fileMenu->addAction(this->save_as_file_act);

		return fileMenu;
	}

	auto MainWindow::PrepareAboutMenu() -> QMenu*
	{
		QMenu* aboutMenu = this->menuBar()->addMenu("&About");

		aboutMenu->addAction(this->show_license_act);
		aboutMenu->addAction(this->show_version_act);

		return aboutMenu;
	}

	auto MainWindow::PrepareHSplitter() -> QSplitter*
	{
		QSplitter* splitter = new QSplitter(Qt::Orientation::Horizontal);
		splitter->setChildrenCollapsible(false);

		this->setCentralWidget(splitter);

		return splitter;
	}

	auto MainWindow::PrepareTreeView() -> QTreeView*
	{
		QTreeView* treeView = new QTreeView(this->h_splitter);
		treeView->setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
		treeView->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
		treeView->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
		treeView->setAlternatingRowColors(true);
		treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

		this->h_splitter->addWidget(treeView);

		return treeView;
	}

	auto MainWindow::PrepareParameterView() -> QParameterView* {
		QParameterView* param_view = new QParameterView();

		this->h_splitter->addWidget(param_view);

		return param_view;
	}

	auto MainWindow::UpdateWindowTitle() -> void
	{
		const QByteArray title("WDBEditor");
		QByteArray filename;
		QByteArray dirty;

		if (this->filename.has_value())
		{
			filename = QByteArray(" - ") + QByteArray::fromStdString(this->filename.value());
		}

		if (this->dirty)
		{
			dirty = QByteArray("*");
		}

		this->setWindowTitle(QString::fromUtf8(title + filename + dirty));
	}

	void MainWindow::Open()
	{
		const QString filename = QFileDialog::getOpenFileName(
			this, "Open .wdb file", QString::fromUtf8(HomeDirectory().c_str()), ".wdb Files (*.wdb)"
		);

		const QByteArray utf8_bytes = filename.toUtf8();

		FILE* fileptr;

#if defined(__STDC_WANT_LIB_EXT1__)
		const errno_t open_error = fopen_s(&fileptr, utf8_bytes.data(), "rb");
#else
		fileptr = fopen(utf8_bytes.data(), "rb");
		const error_t open_error = nullptr == fileptr;
#endif

		// Check that we actually managed to open the file
		if (0 != open_error)
		{
			return;
		}

		try
		{
			std::optional<libWDB::WorldDatabase> wdb_opt = libWDB::ParseWDB(fileptr);

			if (wdb_opt.has_value())
			{
				this->filename = std::string {utf8_bytes.data()};
				this->dirty = false;

				this->UpdateWindowTitle();

				libWDB::WorldDatabase wdb = std::move(wdb_opt.value());
				this->wdb_model->SetModel(std::move(wdb));
			}
		} catch (libWDB::WDBParseException& wpe)
		{
			QMessageBox dialog;
			dialog.setText("Failed to parse .wdb file \"" + filename + "\": " + wpe.what());
			dialog.exec();
			return;
		}

		// What are we going to do if an error occurs during closing?
		// Not much we can recover from, realistically?
		const int _ = fclose(fileptr);
	}

	void MainWindow::Save()
	{
		// No file was specified!
		if (!this->filename.has_value())
		{
			QMessageBox dialog;
			dialog.setText("No .wdb file was open!");
			dialog.exec();
			return;
		}

		FILE* fileptr;

#if defined(__STDC_WANT_LIB_EXT1__)
		const errno_t open_error = fopen_s(&fileptr, this->filename.value().c_str(), "wb");
#else
		fileptr = fopen(this->filename.value().c_str(), "rb");
		const error_t open_error = nullptr == fileptr;
#endif

		// Check that we actually managed to open the file
		if (0 != open_error)
		{
			return;
		}

		libWDB::Save(
			this->wdb_model->GetModel(),
			fileptr
		);

		// What are we going to do if an error occurs during closing?
		// Not much we can recover from, realistically?
		const int _ = fclose(fileptr);

		// Changes were written to disk, reset dirty flag
		this->dirty = false;

		this->UpdateWindowTitle();
	}

	void MainWindow::SaveAs()
	{
		const QString filename = QFileDialog::getSaveFileName(
			this, "Save .wdb file", QString::fromUtf8(HomeDirectory().c_str()), ".wdb Files (*.wdb)"
		);

		const QByteArray utf8_bytes = filename.toUtf8();

		FILE* fileptr;

#if defined(__STDC_WANT_LIB_EXT1__)
		const errno_t open_error = fopen_s(&fileptr, utf8_bytes.data(), "wb");
#else
		fileptr = fopen(utf8_bytes.data(), "rb");
		const error_t open_error = nullptr == fileptr;
#endif

		// Check that we actually managed to open the file
		if (0 != open_error)
		{
			return;
		}

		libWDB::Save(
			this->wdb_model->GetModel(),
			fileptr
		);

		// What are we going to do if an error occurs during closing?
		// Not much we can recover from, realistically?
		const int _ = fclose(fileptr);

		// Current filename is set to the last saved filename
		this->filename = utf8_bytes.toStdString();

		// Changes were written to disk, reset dirty flag
		this->dirty = false;

		this->UpdateWindowTitle();
	}

	void MainWindow::ShowLicense() {
		QString license_text (
			"WDBEditor  Copyright (C) 2023  Amber Cassimon<br>"
			"This program is free software: you can redistribute it and/or modify<br>"
			"it under the terms of the GNU General Public License as published by<br>"
			"the Free Software Foundation, either version 3 of the License, or<br>"
			"(at your option) any later version.<br>"
			"<br>"
			"This program is distributed in the hope that it will be useful,<br>"
			"but WITHOUT ANY WARRANTY; without even the implied warranty of<br>"
			"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br>"
			"GNU General Public License for more details.<br>"
			"<br>"
			"You should have received a copy of the GNU General Public License<br>"
			"along with this program.  If not, see <a href=\"https://www.gnu.org/licenses/\">https://www.gnu.org/licenses/</a>."
		);

		QMessageBox license_dialog (
			QMessageBox::Icon::Information,
			QString("License"),
			license_text
		);

		license_dialog.setTextFormat(Qt::RichText);
		license_dialog.setTextInteractionFlags(Qt::TextBrowserInteraction);

		license_dialog.exec();
	}

	void MainWindow::ShowVersion() {
		QString version_string = QString::fromStdString(
			"WDBEditor Version: " + std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION) + "." + std::to_string(PATCH_VERSION) + "\n" +
			"Build Date: " + BUILD_DATE + "\n" +
			"Qt " + std::to_string(QT_VERSION_MAJOR) + "." + std::to_string(QT_VERSION_MINOR) + "." + std::to_string(QT_VERSION_PATCH)
		);

		QMessageBox version_dialog (
			QMessageBox::Icon::Information,
			QString("Version"),
			version_string
		);

		version_dialog.setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByMouse);

		version_dialog.exec();
	}
	void MainWindow::ModelChanged() {
		this->dirty = true;

		this->UpdateWindowTitle();
	}
} // namespace WDBEditor