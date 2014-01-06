#include <iostream>
#include <functional>

#include <QDebug>

#include "UI/Tests/TestAuthorWidget.hpp"
#include "UI/Tests/TestDocumentDialog.hpp"
#include "UI/Tests/TestMainWindow.hpp"

#include "Database/ResearchDocumentRepository.hpp"

/**
 * Run unit tests for the GUI application
 */
void QtUnitTests(int argc, char *argv[])
{
	TestAuthorWidget test1;
	QTest::qExec(&test1, argc, argv);

	TestDocumentDialog test2;
	QTest::qExec(&test2, argc, argv);

	TestMainWindow test3;
	QTest::qExec(&test3, argc, argv);
}

/**
 * Run database tests
 */
void DatabaseTests()
{
	// Store name of test and lambda function to be called
	// when executing the text.
	struct {
		std::string name;
		std::function<bool()> func;
	} tests[] = {
		// Positive tests
		{ 
			"Positive Test: Adding document to database",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc(0, "a", "b", "c");
				return dr.Add(doc) &&
				       dr.FindOneById(0) != nullptr &&
				       dr.FindAll().size() == 1;
			}
		},
		{ 
			"Positive Test: Adding multiple documents to database",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc1(0, "a", "b", "c");
				Database::Document doc2(1, "a", "b", "c");
				return dr.Add(doc1) &&
				       dr.Add(doc2) &&
				       dr.FindOneById(0) != nullptr &&
					   dr.FindOneById(1) != nullptr &&
				       dr.FindAll().size() == 2;
			}
		},
		{
			"Positive Test: Deleting document from database",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc(0, "a", "b", "c");
				return dr.Add(doc) &&
				       dr.Remove(doc) &&
			           dr.FindOneById(0) == nullptr &&
				       dr.FindAll().size() == 0;
			}
		},
		{
			"Positive Test: Retrieval of document",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc(0, "a", "b", "c");
				return dr.Add(doc) &&
			           dr.FindOneById(0) != nullptr &&
				       dr.FindAll().size() == 1;
			}
		},
		{
			"Positive Test: Retrieval of document by author",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc(0, "a", "b", "c");
				return dr.Add(doc) &&
			           dr.FindManyByAuthor("a").size() == 1 &&
				       dr.FindAll().size() == 1;
			}
		},
		{
			"Positive Test: Retrieval of document by title",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc(0, "a", "b", "c");
				return dr.Add(doc) &&
			           dr.FindManyByTitle("b").size() == 1 &&
				       dr.FindAll().size() == 1;
			}
		},
		{
			"Positive Test: Retrieval of documents by author",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc1(0, "a", "b", "c");
				Database::Document doc2(1, "a", "b", "c");
				Database::Document doc3(3, "a", "b", "c");
				return dr.Add(doc1) &&
					   dr.Add(doc2) &&
					   dr.Add(doc3) &&
			           dr.FindManyByAuthor("a").size() == 3 &&
				       dr.FindAll().size() == 3;
			}
		},
		{
			"Positive Test: Retrieval of documents by title",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc1(0, "a", "b", "c");
				Database::Document doc2(1, "a", "b", "c");
				Database::Document doc3(3, "a", "b", "c");
				return dr.Add(doc1) &&
					   dr.Add(doc2) &&
					   dr.Add(doc3) &&
			           dr.FindManyByTitle("b").size() == 3 &&
				       dr.FindAll().size() == 3;
			}
		},
		{
			"Positive Test: Retrieval of documents by iterator",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc1(0, "a", "b", "c");
				Database::Document doc2(1, "a", "b", "c");
				Database::Document doc3(3, "a", "b", "c");

				bool success = dr.Add(doc1) && dr.Add(doc2) && dr.Add(doc3);
				int count = 0;
				for(auto it = dr.Begin(); it != dr.End(); ++it) {
					count++;
				}
				return success && count == 3 && dr.FindAll().size() == 3;
			}
		},
		// Negative tests
		{
			"Negative Test: Adding multiple documents with same ID",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc1(0, "a", "b", "c");
				Database::Document doc2(0, "a", "b", "c");
				return dr.Add(doc1) &&
				       dr.Add(doc2) == false &&
				       dr.FindOneById(0) != nullptr &&
				       dr.FindAll().size() == 1;
			}
		},
		{
			"Negative Test: Retrieval of non-existent document",
			[&] {
				Database::ResearchDocumentRepository dr;
				return dr.FindOneById(0) == nullptr &&
				       dr.FindAll().size() == 0;
			}
		},
		{
			"Negative Test: Retrieval by non-existent author",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc1(0, "a", "b", "c");
				return dr.Add(doc1) &&
				       dr.FindManyByAuthor("aa").size() == 0 &&
				       dr.FindAll().size() == 1;
			}
		},
		{
			"Negative Test: Retrieval by non-existent author that previously existed",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc1(0, "a", "b", "c");
				Database::Document doc2(1, "aa", "b", "c");
				return dr.Add(doc1) &&
					   dr.Add(doc2) &&
					   dr.Remove(doc1) &&
				       dr.FindManyByAuthor("a").size() == 0 &&
				       dr.FindAll().size() == 1;
			}
		},
		{
			"Negative Test: Retrieval by non-existent title",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc1(0, "a", "b", "c");
				return dr.Add(doc1) &&
				       dr.FindManyByTitle("bb").size() == 0 &&
				       dr.FindAll().size() == 1;
			}
		},
		{
			"Negative Test: Retrieval by non-existent title that previously existed",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc1(0, "a", "b", "c");
				Database::Document doc2(1, "a", "bb", "c");
				return dr.Add(doc1) &&
					   dr.Add(doc2) &&
					   dr.Remove(doc1) &&
				       dr.FindManyByTitle("b").size() == 0 &&
				       dr.FindAll().size() == 1;
			}
		},
		{
			"Negative Test: Removal of non-existent document",
			[&] {
				Database::ResearchDocumentRepository dr;
				Database::Document doc(0, "a", "b", "c");
				return !dr.Remove(doc);
			}
		},

	};

	// Iterate over and execute tests
	for (auto test : tests) {
#ifdef QT_TESTLIB_LIB
		qDebug() << QString::fromStdString(test.name) << "\n\t" << "Passed: " << test.func() << "\n";
#else
		std::cout << test.name << "\n\t" << "Passed: " << test.func() << "\n\n";
#endif
	}
}