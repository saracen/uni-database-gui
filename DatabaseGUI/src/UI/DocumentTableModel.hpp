#ifndef DOCUMENT_TABLE_MODEL_H
#define DOCUMENT_TABLE_MODEL_H

#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <string>

#include <QAbstractTableModel>
#include <QDateTime>
#include "Database/Document.hpp"

/**
 * The DocumentTableModel represents a Document as a row
 * inside of Qt's table view.
 */
class DocumentTableModel : public QAbstractTableModel
{
    Q_OBJECT

private:
	// Names for each column
	enum Columns {
		Id,
		Title,
		Authors,
		Published
	};

public:
    DocumentTableModel(const std::vector<const Database::Document> documents, QObject *parent) : QAbstractTableModel(parent), documents(documents)
	{
	}

    int rowCount(const QModelIndex &parent = QModelIndex()) const
	{
		// The row count is always the number of documents we have stored
		return documents.size();
	}

    int columnCount(const QModelIndex &parent = QModelIndex()) const
	{
		// Column count is fixed to 4 (Id, Title, Author(s), Published)
		return 4;
	}

	/**
	 * Document returns the document found at a table's row index
	 */
	const Database::Document Document(const QModelIndex &index) const
	{
		return documents[index.row()];
	}

	/**
	 * Return data for the requested display role.
	 */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
	{
		if (role == Qt::DisplayRole)
		{
			int row = index.row();
			int col = index.column();

			// Return displayed information based upon the column
			switch (col)
			{
			case Columns::Id:
				return QVariant(documents[row].Id());

			case Columns::Title:
				return QString::fromStdString((documents[row].Title()));

			case Columns::Authors:
				{
					QStringList list;
					for (auto &author : documents[row].Authors()) {
						list.push_back(QString::fromStdString(author));
					}
					return list.join(", ");
				}

			case Columns::Published:
				return QDateTime::fromTime_t(documents[row].Published()).date().toString(Qt::DateFormat::DefaultLocaleShortDate);
			}
		}
		return QVariant();
	}

	/**
	 * Return header data for the requested display role.
	 */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (role == Qt::DisplayRole)
		{
			if (orientation == Qt::Horizontal) {
				// Display header names for each column
				switch (section)
				{
				case Columns::Id:        return QString("Id");
				case Columns::Title:     return QString("Title");
				case Columns::Authors:   return QString("Author(s)");
				case Columns::Published: return QString("Published");
				}
			}
		}
		return QVariant();
	}
	
	void sort(int column, Qt::SortOrder order = Qt::AscendingOrder)
	{
		// An array of sort functions, indexed by column
		std::function<bool(const Database::Document, const Database::Document)> func[] = {
			/* Columns::Id */
			[&](const Database::Document a, const Database::Document b){
				return (order == Qt::AscendingOrder) ? a.Id() < b.Id() : a.Id() > b.Id();
			},

			/* Columns::Title */
			[&](const Database::Document a, const Database::Document b){
				return (order == Qt::AscendingOrder) ? a.Title() < b.Title() : a.Title() > b.Title();
			},

			/* Columns::Authors */
			[&](const Database::Document a, const Database::Document b){
				return (order == Qt::AscendingOrder) ? a.Authors() < b.Authors() : a.Authors() > b.Authors();
			},

			/* Columns::Published */
			[&](const Database::Document a, const Database::Document b){
				return (order == Qt::AscendingOrder) ? a.Published() < b.Published() : a.Published() > b.Published();
			},
		};

		// Sort using sort functions (func[column])
		std::sort(documents.begin(), documents.end(), func[column]);

		// Tell view that the data has changed
		QModelIndex topLeft = createIndex(0, 0);
		QModelIndex bottomRight = createIndex(documents.size() - 1, 2);
		emit dataChanged(topLeft, bottomRight);
	}

private:
	std::vector<const Database::Document> documents;
};

#endif