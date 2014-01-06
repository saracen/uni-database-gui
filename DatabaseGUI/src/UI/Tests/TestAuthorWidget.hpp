#ifndef TEST_AUTHOR_WIDGET_H
#define TEST_AUTHOR_WIDGET_H

#include <QTest>
#include "UI/AuthorWidget.hpp"

/**
 * Unit test for AuthorWidget
 */
class TestAuthorWidget: public QObject
{
    Q_OBJECT

private slots:
    void testAuthorWidget()
	{
		// Setup initial state for test
		auto authors = QStringList();
		authors.push_back("Name #1");

		auto authorModel = new QStringListModel(authors);
		AuthorWidget authorWidget(authorModel);

		// Wait for widget to display
		authorWidget.show();
		QTest::qWaitForWindowActive(&authorWidget);

		// Find add and delete buttons
		auto addButton = authorWidget.findChild<QPushButton*>("add_button");
		auto delButton = authorWidget.findChild<QPushButton*>("del_button");

		// Emulate clicking add button, submitting a new author,
		// tabbing to the item and clicking the delete button
		QTest::mouseClick(addButton, Qt::MouseButton::LeftButton);
		QTest::keyClicks(nullptr, "hello");
		QTest::keyClick(static_cast<QWidget*>(nullptr), Qt::Key_Tab);
		QTest::mouseClick(delButton, Qt::MouseButton::LeftButton);

		// Compare output with expected
		QCOMPARE(authorModel->stringList().size(), 1);
		QCOMPARE(authorModel->stringList().at(0), QString("hello"));
	}
};

#endif