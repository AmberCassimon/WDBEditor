#include <QApplication>

#include "WDBEditor/MainWindow.hpp"

auto main(int argc, char* argv[]) -> int
{
	const QApplication app (argc, argv);

	WDBEditor::MainWindow window (nullptr);

	window.show();

    return app.exec();
}
