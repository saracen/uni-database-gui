#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTableView>

#include "DocumentDialog.hpp"
#include "DocumentTableModel.hpp"

#include "Database/ResearchDocumentRepository.hpp"

/**
 * MainWindow is the applications main window, containing
 * a table with database results and an Add, Delete and Edit
 * button to manipulate database contents
 */
class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
    MainWindow(Database::ResearchDocumentRepository &dr, QWidget *parent = 0) : QMainWindow(parent), dr(dr), tableModel(nullptr), counter(8)
	{
		// Set basic window properties
		setWindowTitle("Database Frontend");
		setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
		resize(600, 600);

		// Toolbar add button
		toolButtonAdd = new QToolButton(this);
		toolButtonAdd->setText("Add");

		// Toolbar delete button
		toolButtonDel = new QToolButton(this);
		toolButtonDel->setText("Delete");
		toolButtonDel->setEnabled(false);

		// Toolbar edit button
		toolButtonEdit = new QToolButton(this);
		toolButtonEdit->setText("Edit");
		toolButtonEdit->setEnabled(false);

		// Create toolbar and add buttons
		toolbar = new QToolBar(this);
		toolbar->setFloatable(false);
		toolbar->setMovable(false);
		toolbar->addWidget(toolButtonAdd);
		toolbar->addWidget(toolButtonDel);
		toolbar->addWidget(toolButtonEdit);
		addToolBar(Qt::TopToolBarArea, toolbar);

		// Create table
		table = new QTableView(this);
		table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
		table->setSelectionBehavior(QAbstractItemView::SelectRows);
		table->verticalHeader()->setVisible(false);
		table->horizontalHeader()->setStretchLastSection(true);
		table->setSortingEnabled(true);
		table->setAlternatingRowColors(true);
		table->sortByColumn(0, Qt::SortOrder::AscendingOrder);

		// Create text view
		text = new QTextEdit(this);
		text->setReadOnly(true);

		// Create split view
		splitter = new QSplitter(this);
		splitter->addWidget(table);
		splitter->addWidget(text);

		// Create central widget
		setCentralWidget(new QWidget(this));

		// Create layout, adding table and text view
		layout = new QGridLayout(centralWidget());
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(splitter);

		// Connect button clicks
		connect(toolButtonAdd, SIGNAL(clicked()), this, SLOT(HandleAddButton()));
		connect(toolButtonDel, SIGNAL(clicked()), this, SLOT(HandleDelButton()));
		connect(toolButtonEdit, SIGNAL(clicked()), this, SLOT(HandleEditButton()));

		// Name objects
		toolButtonDel->setObjectName("del_button");
		table->setObjectName("table");
		text->setObjectName("text");

		// Load table model
		Load();

		// Resize columns for initial load
		table->resizeColumnsToContents();

		// Select first row
		table->setFocus();
		table->selectRow(0);
	}

private:
	void Load()
	{
		// Delete pointer to older tableModel if it exists
		if (tableModel != nullptr)
			delete tableModel;

		// Create new document table model
		tableModel = new DocumentTableModel(dr.FindAll(), this);
		table->setModel(tableModel);

		// Sort based upon table settings
		table->sortByColumn(table->horizontalHeader()->sortIndicatorSection(), table->horizontalHeader()->sortIndicatorOrder());

		// Connect selection changes
		connect(table->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(HandleSelectionChange(const QModelIndex&, const QModelIndex&)));
	}

private slots:
	void HandleAddButton()
	{
		// Create a document with placeholder values. Increment id counter.
		Database::Document doc(counter++, "New Author", "", "");

		// Display document dialog. If accepted, add new document and reload data.
		DocumentDialog dialog(doc, this);
		if (dialog.exec() == QDialog::Accepted) {
			dr.Add(doc);
			Load();
		}
	}

	void HandleDelButton()
	{
		auto selected = table->selectionModel()->selectedRows();
		if (selected.size() > 0) {
			// If a row is selected, delete it.
			dr.Remove(tableModel->Document(selected.at(0)));

			// Disable delete & edit button
			toolButtonDel->setDisabled(true);
			toolButtonEdit->setDisabled(true);

			// Clear text from view
			text->clear();

			// Reload data
			Load();
		}
	}

	void HandleEditButton()
	{
		auto selected = table->selectionModel()->selectedRows();
		if (selected.size() > 0) {
			// Get selected document
			auto doc = tableModel->Document(selected.at(0));

			// Remove the document
			dr.Remove(doc);

			// Create dialog, populated with document
			DocumentDialog dialog(doc, this);
			if (dialog.exec() == QDialog::Accepted) {
				// Re-add the document with changes
				dr.Add(doc);

				// Reload data
				Load();

				// Reselect item
				table->selectionModel()->setCurrentIndex(tableModel->index(selected.at(0).row(), 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
			}
		}
	}

	void HandleSelectionChange(const QModelIndex& current, const QModelIndex& previous)
	{
		if (current.row() >= 0) {
			// A row has been selected, so enable delete & edit button
			toolButtonDel->setDisabled(false);
			toolButtonEdit->setDisabled(false);

			// Update current text view to selected document
			auto doc = tableModel->Document(current);
			text->setHtml(QString("<h1>%1</h1><pre>%3</pre>").arg(QString::fromStdString(doc.Title())).arg(QString::fromStdString(doc.Body())));
		} else {
			// No row selected, disable delete & edit button
			toolButtonDel->setDisabled(true);
			toolButtonEdit->setDisabled(true);
		}
	}

private:
	int counter;
	Database::ResearchDocumentRepository &dr;

	QGridLayout *layout;
	QSplitter   *splitter;

	QToolBar    *toolbar;
	QToolButton *toolButtonAdd;
	QToolButton *toolButtonDel;
	QToolButton *toolButtonEdit;
	QTableView  *table;
	QTextEdit   *text;

	DocumentTableModel *tableModel;
};

#endif