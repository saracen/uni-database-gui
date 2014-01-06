#ifndef AUTHOR_WIDGET_H
#define AUTHOR_WIDGET_H

#include <QtCore/QStringListModel>

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListView>

/**
 * AuthorWidget is a widget with input fields
 * for modifing a list of authors
 */
class AuthorWidget : public QWidget
{
	Q_OBJECT

public:
	AuthorWidget(QStringListModel *authorList, QWidget *parent = 0) : QWidget(parent), listModel(authorList)
	{
		// Create author names list
		list = new QListView(this);
		list->setModel(listModel);

		// Create add button
		addButton = new QPushButton("+", this);
		addButton->setMaximumWidth(30);

		// Create delete button
		delButton = new QPushButton("-", this);
		delButton->setDisabled(true);
		delButton->setMaximumWidth(30);

		// Create layout and arrange list and buttons
		layout = new QGridLayout(this);
		layout->setContentsMargins(0, 0, 0, 0);
		layout->addWidget(list, 0, 0, 2, 1);
		layout->addWidget(addButton, 0, 1, 1, 1);
		layout->addWidget(delButton, 1, 1, 1, 1);

		// Connect clicks and selection events
		connect(addButton, SIGNAL(clicked()), this, SLOT(HandleAddButton()));
		connect(delButton, SIGNAL(clicked()), this, SLOT(HandleDelButton()));
		connect(list->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(HandleSelectionChange(const QModelIndex&, const QModelIndex&)));

		// Name objects
		addButton->setObjectName("add_button");
		delButton->setObjectName("del_button");
	}

private slots:
	void HandleAddButton()
	{
		// Insert new blank row, select and go into edit mode
		listModel->insertRow(0);
		auto index = listModel->index(0, 0);
		list->setCurrentIndex(index);
		list->edit(index);
	}

	void HandleDelButton()
	{
		// Delete currently selected row
		auto selected = list->selectionModel()->currentIndex();
		listModel->removeRow(selected.row());
	}

	void HandleSelectionChange(const QModelIndex& current, const QModelIndex& previous)
	{
		// Enable/disable the delete button depending on whether a row is selected
		if (current.row() >= 0) {
			delButton->setDisabled(false);
		} else {
			delButton->setDisabled(true);
		}
	}

private:
	QGridLayout *layout;
	QListView   *list;
	QPushButton *addButton;
	QPushButton *delButton;

	QStringListModel *listModel;
};

#endif