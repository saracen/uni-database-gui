#ifndef __DOCUMENT_HPP__
#define __DOCUMENT_HPP__

#include <vector>
#include <string>
#include <ctime>

namespace Database
{

/**
 * The document class represents a research document and contains
 * a unique id, an array of authors, a title, document body and a
 * a date that the article was published.
 */
class Document
{
public:
	Document(unsigned int id, std::string mainAuthor, std::string title, std::string body, std::time_t published = std::time(nullptr)) :
		id(id), title(title), body(body), published(published) {
		authors.push_back(mainAuthor);
	}

	~Document(void) {
	}

	/**
	 * Return document unique id
	 */
	const unsigned int Id(void) const {
		return id;
	}

	/**
	 * Set document unique id
	 */
	void SetId(unsigned int id) {
		this->id = id;
	}

	/**
	 * Return document authors (const protected)
	 */
	const std::vector<std::string> &Authors(void) const {
		return authors;
	}

	/**
	 * Return document authors' array for manipulation
	 */
	std::vector<std::string> &Authors(void) {
		return authors;
	}

	/**
	 * Return document title
	 */
	const std::string &Title(void) const {
		return title;
	}

	/**
	 * Set document title
	 */
	void SetTitle(std::string title) {
		this->title = title;
	}

	/**
	 * Return document body
	 */
	const std::string &Body(void) const {
		return body;
	}

	/**
	 * Set document body
	 */
	void SetBody(std::string body) {
		this->body = body;
	}

	/**
	 * Returned document published date
	 */
	const std::time_t &Published(void) const {
		return published;
	}

	/**
	 * Set document published date
	 */
	void SetPublished(std::time_t published) {
		this->published = published;
	}

private:
	unsigned int id;

	std::vector<std::string> authors;
	std::string title;
	std::string body;
	std::time_t published;
};

};

#endif;
