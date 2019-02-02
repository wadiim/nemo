#ifndef NEMO_H
#define NEMO_H

#include <utility>
#include <string>
#include <vector>
#include "file.h"

class Nemo
{
	typedef std::string String;
	typedef std::vector<String> Vec;
	typedef std::pair<String, Vec> Pair;

public:
	void menu() const;
	void test() const;
	void create_new_wordlist() const;
	void show_wordlist() const;
	void modify_wordlist() const;
	void delete_wordlist() const;

private:
	void show_menu() const;
	String get_path() const;
	bool choose_menu_option() const;
	bool open_wordlist(File& f) const;
	void show_invalid_path_menu() const;
	void correct_answer() const;
	void wrong_answer(const Vec& vec) const;
	bool choose_test_mode(Vec& vec) const;
	void load_file(Vec& vec, File& f) const;
	bool handle_empty_file(File& f) const;
	void run_test(Vec& vec) const;
	String do_line_formatting(String&& line) const;
	String do_line_formatting(const String& line) const;
	Pair split_to_text_and_answers(const String& s) const;
	void get_range(size_t& b, size_t& e, const Vec& v) const;
	void remove_extra_spaces(String& s) const;
	void show_vector_content(const Vec& v) const;
	void show_file_content(File& f) const;
	void show_modify_wordlist_menu() const;
	bool choose_modify_mode(Vec& v) const;
	void modify_lines(Vec& vec) const;
	void erase_lines(Vec& vec) const;
	void add_lines(Vec& vec) const;
	char get_character() const;
};

#endif
