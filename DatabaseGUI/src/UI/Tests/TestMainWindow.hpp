#ifndef TEST_MAIN_WINDOW_H
#define TEST_MAIN_WINDOW_H

#include <QTest>
#include "UI/MainWindow.hpp"

/**
 * Unit test for TestMainWindow
 */
class TestMainWindow: public QObject
{
    Q_OBJECT

private slots:
    void testMainWindow()
	{
		// Setup initial state for test
		Database::ResearchDocumentRepository dr;

		Database::Document documents[] = {
		  Database::Document(0, "Edwin Dusty",     "A Title",                "Document Text"),
		  Database::Document(1, "Jarrod Otis",     "A Slightly Large Title", "Document Text"),
		  Database::Document(2, "Harland Raymond", "A Non-Unique Title",     "Document Text"),
		};
	
		for (auto &doc : documents) {
			dr.Add(doc);
		}

		MainWindow mainWindow(dr);

		// Wait for window to display
		mainWindow.show();
		QTest::qWaitForWindowActive(&mainWindow);

		// Find controls
		auto delButton = mainWindow.findChild<QToolButton*>("del_button");
		auto table     = mainWindow.findChild<QTableView*>("table");
		auto text      = mainWindow.findChild<QTextEdit*>("text");

		// Emulate deleting first document, selecting second and deleting
		// followed by selecting the third
		QTest::mouseClick(delButton, Qt::MouseButton::LeftButton);
		QTest::keyClick(table, Qt::Key::Key_Tab);
		QTest::mouseClick(delButton, Qt::MouseButton::LeftButton);
		QTest::keyClick(table, Qt::Key::Key_Tab);

		// Compare output with expected
		QCOMPARE(table->model()->rowCount(), 1);
		QCOMPARE(text->toPlainText(), QString("A Non-Unique Title\nDocument Text"));
	}
};

#endif