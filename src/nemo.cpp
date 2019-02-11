#include <algorithm>
#include <iostream>
#include "color.h"
#include "utils.h"
#include "nemo.h"
#include "term.h"
#include <tuple>

void Nemo::menu() const
{
	do
	{
		show_menu();
	} while (choose_menu_option());
}

void Nemo::test() const
{
	File file;
	Vec lines;

	if (!open_wordlist(file)) return;
	if (handle_empty_file(file)) return;
	load_file(lines, file);
	if (!choose_test_mode(lines)) return;
	clear_screen();
	run_test(lines);
}

void Nemo::create_new_wordlist() const
{
	String path = get_path();
	if (is_file_exist(path))
	{
		std::cout <<
			"Such file already exists.\n"
			"Do you want to overwrite it? (y/n): ";

		if (tolower(get_character()) == 'n') return;
	}

	std::string line;
	File file(path, std::ios_base::out);

	clear_screen();
	while (std::getline(std::cin, line))
	{
		remove_extra_spaces(line);
		if (!line.empty())
			file << line << '\n';
	}
	std::cin.clear();
}

void Nemo::show_wordlist() const
{
	File file;
	if (!open_wordlist(file)) return;
	clear_screen();
	show_file_content(file);
	std::cin.get();
}

void Nemo::modify_wordlist() const
{
	Vec vec;
	File file;

	if (!open_wordlist(file)) return;
	load_file(vec, file);

	do
	{
		clear_screen();
		show_vector_content(vec);
		show_modify_wordlist_menu();
	} while (choose_modify_mode(vec));

	file.open(std::ios_base::out | std::ios_base::trunc);
	for (const auto& v : vec) file << v << '\n';
}

void Nemo::delete_wordlist() const
{
	File file;
	if (!open_wordlist(file)) return;
	String path = file.get_path();
	file.close();

	std::cout << "Are you sure you want to remove " << path << "? (y/n): ";
	if (tolower(get_character()) == 'n') return;

	if (remove(path.c_str()))
	{
		std::cout << "This file could not be deleted!\n";
		std::cin.get();
	}
}

void Nemo::show_menu() const
{
	clear_screen();
	std::cout <<
		"NEMO\n\n"
		"[1] Test\n"
		"[2] Create new word list\n"
		"[3] Modify word list\n"
		"[4] Show word list\n"
		"[5] Delete word list\n"
		"[0] Exit\n";
}

bool Nemo::choose_menu_option() const
{
	switch (get_char())
	{
	case '1':
		test();
		break;
	case '2':
		create_new_wordlist();
		break;
	case '3':
		modify_wordlist();
		break;
	case '4':
		show_wordlist();
		break;
	case '5':
		delete_wordlist();
		break;
	case '0':
		return false;
	}
	return true;
}

bool Nemo::open_wordlist(File& f) const
{
	while (!f.open(get_path(), std::ios_base::in))
	{
		show_invalid_path_menu();
		if (get_char() == '0') return false;
	}
	return true;
}

std::string Nemo::get_path() const
{
	String path;

	clear_screen();
	std::cout << "Enter path to the word list: ";
	std::cin >> path;
	std::cin.sync();
	std::cin.get();

	return path;
}

void Nemo::show_invalid_path_menu() const
{
	clear_screen();
	std::cout <<
		"Invalid path\n\n"
		"[1] Try again\n"
		"[0] Back to menu\n";
}

void Nemo::load_file(Vec& vec, File& f) const
{
	while (!f.eof())
	{
		String temp = f.next_line();
		if (!temp.empty()) vec.push_back(temp);
	}
}

bool Nemo::handle_empty_file(File& f) const
{
	if (f.empty())
	{
		std::cout << "Empty\n";
		std::cin.get();
		return true;
	}
	return false;
}

bool Nemo::choose_test_mode(Vec& vec) const
{
	clear_screen();
	std::cout <<
		"[1] In sequence\n"
		"[2] Randomly\n"
		"[3] Return\n";

	for (;;)
	{
		switch (get_char())
		{
		case '3': return false;
		case '2': std::random_shuffle(vec.begin(), vec.end());
		case '1': return true;
		}
	}
}

void Nemo::correct_answer() const
{
	std::cout << color::green << "Correct answer!\n\n" << color::reset;
}

void Nemo::wrong_answer(const Vec& vec) const
{
	std::cout << color::red << "Wrong answer!\n" << color::reset <<
		"Should be: ";

	if (vec.size())
	{
		std::cout << color::green << vec[0] << color::reset;
		for (auto v = ++vec.begin(); v != vec.end(); ++v)
		{
			std::cout << " or " << color::green << *v <<
				color::reset;
		}
	}

	std::cout << "\n\n";
}

void Nemo::run_test(Vec& vec) const
{
	unsigned points{ 0 }, max_points{ 0 };
	String text, answer;
	Vec ans;

	for (const auto& v : vec)
	{
		std::tie(text, ans) = split_to_text_and_answers(v);
		for (auto& a : ans) unescape_character(a, '-');
		unescape_character(text, '-');

		std::cout << text << " - ";
		if (!std::getline(std::cin, answer))
		{
			std::cout << "\n\n";
			std::cin.clear();
			break;
		}

		if (std::find(ans.begin(), ans.end(), answer) != ans.end() ||
			(answer.empty() && ans.empty()))
		{
			correct_answer();
			++points;
		}
		else wrong_answer(ans);

		++max_points;
	}

	std::cout << "Result: " << points * 100 / max_points << "%\n";
	std::cin.get();
}

std::string Nemo::do_line_formatting(String&& line) const
{
	if (line.size())
	{
		size_t i = find_first_nonescaped(line, '-');
		if (i < line.length() - 1) line.insert(i + 1, " ");
		if (i < line.length()) line.insert(i, " ");

		i = line.find(';');
		while (i != std::string::npos)
		{
			if (i < line.length() - 1) line.insert(i + 1, " ");
			i = line.find(';', i + 2);
		}
	}
	return line;
}

std::string Nemo::do_line_formatting(const String& line) const
{
	String str = line;
	return do_line_formatting(std::move(str));
}

Nemo::Pair Nemo::split_to_text_and_answers(const String& s) const
{
	size_t i = find_first_nonescaped(s, '-');
	if (i < s.length())
	{
		return std::make_pair<String, Vec>(s.substr(0, i),
			split(s.substr(i + 1), ';'));
	}
	return std::make_pair<String, Vec>(String(s), Vec{});
}

void Nemo::remove_extra_spaces(String & s) const
{
	s = strip(std::move(s));
	for (size_t i = 0; i < s.size(); ++i)
	{
		if (i && is_space(s[i]) && is_space(s[i - 1])) s.erase(i, 1);
		if (s[i] == '-' || s[i] == ';')
		{
			while (i && is_space(s[i - 1])) s.erase(--i, 1);
			while (i + 1 < s.size() && is_space(s[i + 1]))
				s.erase(i + 1, 1);
		}
	}
}

void Nemo::show_vector_content(const Vec& v) const
{
	if (!v.size()) std::cout << "Empty\n";

	for (size_t i = 0; i < v.size(); ++i)
	{
		String line = do_line_formatting(v[i]);
		unescape_character(line, '-');
		std::cout << i + 1 << ". " << line << '\n';
	}
}

void Nemo::show_file_content(File& f) const
{
	Vec vec;
	load_file(vec, f);
	show_vector_content(vec);
}

char Nemo::get_character() const
{
	char ch;
	std::cin >> ch;
	std::cin.sync();
	std::cin.get();
	return ch;
}

void Nemo::show_modify_wordlist_menu() const
{
	std::cout << "\n"
		"[1] Modify lines\n"
		"[2] Erase lines\n"
		"[3] Add lines\n"
		"[0] Return\n\n";
}

bool Nemo::choose_modify_mode(Vec& v) const
{
	switch (get_char())
	{
	case '1':
		modify_lines(v);
		break;
	case '2':
		erase_lines(v);
		break;
	case '3':
		add_lines(v);
		break;
	case '0':
		return false;
	}
	return true;
}

void Nemo::modify_lines(Vec& vec) const
{
	String line;
	size_t begin, end;
	get_range(begin, end, vec);
	for (auto i = begin; i < end; ++i)
	{
		std::cout << i + 1 << ". ";
		if (!std::getline(std::cin, line)) return;
		remove_extra_spaces(line);
		vec[i] = line;
	}
}

void Nemo::erase_lines(Vec& vec) const
{
	size_t begin, end;
	get_range(begin, end, vec);
	vec.erase(vec.begin() + begin, vec.begin() + end);
}

void Nemo::add_lines(Vec& vec) const
{
	clear_screen();
	for (const auto& v : vec)
		std::cout << do_line_formatting(v) << '\n';

	String line;
	while (std::getline(std::cin, line))
	{
		remove_extra_spaces(line);
		if (!line.empty()) vec.push_back(line);
	}
	std::cin.clear();
}

void Nemo::get_range(size_t& begin, size_t& end, const Vec& v) const
{
	String range;
	std::cout << "Enter range: ";
	getline(std::cin, range);

	auto i = range.find('-');
	String beg_str = strip(range.substr(0, i));
	String end_str = strip(range.substr(i + 1));

	begin = (beg_str.empty()) ? 0 : string_to_size_t(beg_str);
	end = (end_str.empty()) ? v.size() : string_to_size_t(end_str);

	if (begin) --begin;
	if (end && end < v.size()) --end;
	if (end > v.size()) end = v.size();
	if (begin > end) begin = end;
}
