#ifndef DOCUMENT_DIALOG_H
#define DOCUMENT_DIALOG_H

#include <QtCore/QStringListModel>

#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QDateTimeEdit>

#include "AuthorWidget.hpp"
#include "Database/Document.hpp"

/**
 * DocumentDialog is a dialog with input fields
 * for modifing a Database::Document
 */
class DocumentDialog : public QDialog
{
   Q_OBJECT

public:
	DocumentDialog(Database::Document& document, QWidget *parent = 0) : QDialog(parent), document(document)
	{
		// Copy authors to QList
		QStringList list;
		for (auto &author : document.Authors())
			list.push_back(QString::fromStdString(author));

		// Create author list model
		authors = new QStringListModel(list, this);

		// Setup window
		setWindowTitle(QStringLiteral("Document"));
		setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
		resize(400, 400);

		// Create layouts
		mainLayout = new QVBoxLayout(this);
		formLayout = new QFormLayout();

		// Create input fields
		okCancel = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
		editTitle = new QLineEdit(QString::fromStdString(document.Title()), this);
		editBody = new QTextEdit(QString::fromStdString(document.Body()), this);
		authorWidget = new AuthorWidget(authors, this);
		editPublished = new QDateEdit(QDateTime::fromTime_t(document.Published()).date());

		// Placeholder text
		editTitle->setPlaceholderText("New Title");
		editBody->setPlaceholderText("Document Body");

		// We want editBody to vertically expand when the window is resized
		QSizePolicy sp(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
		sp.setVerticalStretch(1);
		editBody->setSizePolicy(sp);

		// Create form rows
		formLayout->addRow(tr("Title"), editTitle);
		formLayout->addRow(tr("Authors"), authorWidget);
		formLayout->addRow(tr("Body"), editBody);
		formLayout->addRow(tr("Published"), editPublished);

		// Add form layout and ok/cancel buttons
		mainLayout->addLayout(formLayout);
		mainLayout->addWidget(okCancel);

		// Connect button clicks and finished event
		connect(okCancel, SIGNAL(accepted()), this, SLOT(accept()));
		connect(okCancel, SIGNAL(rejected()), this, SLOT(reject()));
		connect(this, SIGNAL(finished(int)), this, SLOT(finished(int)));

		// Name objects
		editTitle->setObjectName("edit_title");
		editBody->setObjectName("edit_body");
		okCancel->setObjectName("ok_cancel");
	}

private slots:
	void finished(int r)
	{
		// If user submitted changes, copy form values over to document object
		if (r == QDialog::Accepted) {
			document.SetTitle(editTitle->text().toStdString());
			document.SetBody(editBody->toPlainText().toStdString());

			document.Authors().clear();
			for (auto &author : authors->stringList()) {
				document.Authors().push_back(author.toStdString());
			}

			document.SetPublished(editPublished->dateTime().toTime_t());
		}
	}

private:
	QVBoxLayout *mainLayout;
	QFormLayout *formLayout;

	QLineEdit     *editTitle;
	QTextEdit     *editBody;
	AuthorWidget  *authorWidget;
	QDateEdit     *editPublished;

	QDialogButtonBox *okCancel;

	Database::Document &document;
	QStringListModel *authors;
};

#endif