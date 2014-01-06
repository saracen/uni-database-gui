#ifndef __RESEARCH_DOCUMENT_REPOSITORY_HPP__
#define __RESEARCH_DOCUMENT_REPOSITORY_HPP__

#include <vector>
#include <map>
#include <algorithm>

#include "Repository.hpp"
#include "Document.hpp"

namespace Database
{

/**
 * The ResearchDocumentRepository implements, using the Repository pattern,
 * methods for the retrival, storage and indexing of the Document class.
 */
class ResearchDocumentRepository : public Repository<Document> {
public:

	/**
	 * The add method takes a document by reference, but creates
	 * a copy of it for storage. The pointer to the copied document
	 * is then stored in the indexes (indexing title and id) to prevent
	 * even more copies.
	 *
	 * This method returns true on success, false on failure.
	 */
	bool Add(const Document &document) {
		// Ensure that this document id doesn't already exist
		if (id_idx.find(document.Id()) != id_idx.end()) {
			return false;
		}

		// Store
		storage.push_back(document);

		// Grab pointer to our stored document
		Document *doc = &storage.back();

		// Index by Id and Title
		id_idx[document.Id()] = doc;
		title_idx.insert( pair_string(document.Title(), doc) );

		// Index authors
		for (auto &author : document.Authors()) {
			author_idx.insert( pair_string(author, doc) );
		}

		return true;
	}

	/**
	 * The remove method takes a document by reference and uses
	 * the documents unique id to remove all indexes the database
	 * contains in relation to the document, and then the copy of the
	 * document itself.
	 *
	 * This method returns true if the document was found and removed,
	 * false if otherwise.
	 */
	bool Remove(const Document &document) {
		for (auto it = storage.begin(); it != storage.end(); ++it) {
			if ((*it).Id() == document.Id()) {
				// Remove indexes
				id_idx.erase(id_idx.find(document.Id()));
				multimapErase<multimap_string>(author_idx, document.Id());
				multimapErase<multimap_string>(title_idx, document.Id());

				// Remove item
				storage.erase(it);
				
				return true;
			}
		}
		return false;
	}

	/**
	 * FindOneById finds a single document by its
	 * unique id, or else returns null.
	 */
	const Document* FindOneById(unsigned int id) {
		auto found = id_idx.find(id);
		if (found == id_idx.end()) {
			return nullptr;
		}

		return found->second;
	}

	/**
	 * FindAll returns a copy of all elements currently
	 * stored by the database.
	 */
	const std::vector<const Document> FindAll() {
		std::vector<const Document> results;
		for (auto &d : storage) {
			results.push_back(d);
		}
		return results;
	}

	/**
	 * FindManyByAuthor returns all documents by the requested author.
	 */
	const std::vector<const Document*> FindManyByAuthor(std::string author) {
		return multimapFind<multimap_string>(author_idx, author);
	}

	/**
	 * FindManyByTitle returns all documents by the requested title.
	 */
	const std::vector<const Document*> FindManyByTitle(std::string title) {
		return multimapFind<multimap_string>(title_idx, title);
	}
private:
	// Multimap Erase helper method. Erases elements by value, rather than key.
	template <class T>
	void multimapErase(T& multimap, unsigned int id) {
		for (auto it = multimap.begin(); it != multimap.end();) {
			// Increment iterator here to allow us to erase while iterating.
			auto erase_it = it++;

			if (erase_it->second->Id() == id) {
				// Erase element
				multimap.erase(erase_it);
			}
		}
	}

	// Multimap Find helper method. Creates an std::vector of results.
	template <class T>
	const std::vector<const Document*> multimapFind(T& multimap, std::string key) {
		std::vector<const Document*> results;

		// Create a range iterator for our specific key
		auto range = multimap.equal_range(key);
		for (auto it = range.first; it != range.second; ++it) {
			// Add result to vector
			results.push_back(it->second);
		}
		return results;
	}

private:
	std::map<const unsigned int, const Document*> id_idx;     // Primary index
	std::multimap<std::string, const Document*>   author_idx; // Author index
	std::multimap<std::string, const Document*>   title_idx;  // Title index

	// Create types for common used, long named types
	typedef std::multimap<std::string, const Document*> multimap_string;
	typedef std::pair<std::string, const Document*>     pair_string;
};

};

#endif