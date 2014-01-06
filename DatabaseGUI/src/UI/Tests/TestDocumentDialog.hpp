#ifndef TEST_DOCUMENT_DIALOG_H
#define TEST_DOCUMENT_DIALOG_H

#include <QTest>
#include "UI/DocumentDialog.hpp"

/**
 * Unit test for TestDocumentDialog
 */
class TestDocumentDialog: public QObject
{
    Q_OBJECT

private slots:
    void testDocumentDialog()
	{
		// Setup initial state for test
		Database::Document doc(0, "New Author", "", "");

		DocumentDialog documentDialog(doc);

		// Wait for widget to display
		documentDialog.show();
		QTest::qWaitForWindowActive(&documentDialog);

		// Find input elements and ok/cancel buttons
		auto editTitle = documentDialog.findChild<QLineEdit*>("edit_title");
		auto editBody  = documentDialog.findChild<QTextEdit*>("edit_body");
		auto okCancel  = documentDialog.findChild<QDialogButtonBox*>("ok_cancel");

		// Emulate typing in a title and document body, followed by clicking the OK button
		QTest::keyClicks(editTitle, "A title of a research document");
		QTest::keyClicks(editBody, "Body test of a research document");
		QTest::mouseClick(okCancel->button(QDialogButtonBox::Ok), Qt::MouseButton::LeftButton);

		// Compare output with expected
		QCOMPARE(static_cast<QDialog::DialogCode>(documentDialog.result()), QDialog::Accepted);
		QCOMPARE(doc.Id(), 0u);
		QCOMPARE(QString::fromStdString(doc.Title()), QString("A title of a research document"));
		QCOMPARE(QString::fromStdString(doc.Body()), QString("Body test of a research document"));
	}
};

#endif