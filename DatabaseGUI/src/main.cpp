#include <QApplication>
#include <QDebug>

#include "UI/MainWindow.hpp"
#include "Database/ResearchDocumentRepository.hpp"

void QtUnitTests(int, char *[]);
void DatabaseTests();

int main(int argc, char *argv[])
{
	Database::ResearchDocumentRepository dr;

	// Setup default documents
	Database::Document documents[] = {
	  Database::Document(0, "Edwin Dusty",     "A Title",                "Document Text"),
	  Database::Document(1, "Jarrod Otis",     "A Slightly Large Title", "Document Text"),
	  Database::Document(2, "Harland Raymond", "A Non-Unique Title",     "Document Text"),
	  Database::Document(3, "Eldred Wilson",   "A Non-Unique Title",     "Document Text"),
	  Database::Document(4, "Andrew Bishop",   "A Non-Unique Title",     "Document Text"),
	  Database::Document(5, "Andrew Bishop",   "A SHOUTY TITLE",         "Document Text"),
	  Database::Document(6, "Harland Raymond", "A Title: The Sequel",    "Document Text"),
	  Database::Document(7, "Jarrod Otis",     "A Title: The Prequel",   "Document Text")
	};
	
	for (auto &doc : documents) {
		dr.Add(doc);
	}

	// GUI
    QApplication a(argc, argv);

	// Testing
#ifndef NDEBUG
	QtUnitTests(argc, argv);
	DatabaseTests();
#endif

	typedef QList<QMap<QString, QString>> Data;
	auto dataList = static_cast<Data*>(nullptr);

    MainWindow w(dr);
    w.show();

    return a.exec();
}