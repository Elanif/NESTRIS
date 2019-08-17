#include"TextFormatter.hpp"
#include<SFML/System/String.hpp>
#include<iostream>

// if (t1 of type T and t2 of typeT)&&(t1<=t2)&&(tm=(t1+t2)/2.) => (t1<=tm<=t2)&&((abs(t1-tm)<=min_dist)||(abs(t2-tm)<=mid_dist))
// ternary_function:
//	=0: if t is the right value
//	>0: if t is too big
//	<0: if it's too low
template<typename T>
T dicotomic_search(T const left_boundary, T const right_boundary, int(*ternary_function(T const& t)), std::size_t max_it=0, T const& min_dist=0) {
	if (max_it == 0) max_it = std::numeric_limits<std::size_t>::max;
	if (min_dist == 0) min_dist = std::numeric_limits<T>::lowest;
	else min_dist /= 2.;
	T middle_boundary = (left_boundary + middle_boundary) / 2.;
	std::size_t curr_it=0;
	class test {
	private:int i;
	};
	test a;
	T curr_dist = right_boundary - left_boundary;
	while (curr_it < max_it && curr_dist <= min_dist) {
		middle_boundary = (left_boundary + middle_boundary) / 2.;
		int where = ternary_function(middle_boundary);
		if (where == 0) return middle_boundary;
		if (where < 0) {
			if (abs(left_boundary - middle_boundary) <= min_dist) {
				middle_boundary += min_dist;
				if (abs(left_boundary - middle_boundary) <= min_dist) {
					return right_boundary;
				}
			}	
			left_boundary = middle_boundary;
		}
		else {
			if (abs(right_boundary - middle_boundary) <= min_dist) {
				middle_boundary -= min_dist;
				if (abs(right_boundary - middle_boundary) <= min_dist) {
					return left_boundary;
				}
			}
			right_boundary = middle_boundary;
		}
	}
	return middle_boundary;
}
//
//template<typename CharType>
//bool TextFormatterOld<CharType>::fitString()
//{
//	sf::Text fitting_text;
//	fitting_text.setCharacterSize(m_font_size);
//	fitting_text.setColor(m_text_color);
//	fitting_text.setFillColor(m_fill_color);
//	if (m_font)	fitting_text.setFont;
//
//	m_string_vector.clear();
//
//	if (boundaries.width <= 0) {
//		m_string_vector.push_back(m_string);
//		return true;
//	}
//
//	for (std::size_t str_it = 0; str_it < m_string.length(); ++str_it) {
//		decltype(m_string) substr = "";
//		std::size_t prev_it = str_it;
//		for (std::size_t sub_str_it = 0; str_it+sub_str_it<m_string.length(); ++sub_str_it) {
//			substr += m_string[str_it+sub_str_it];
//			fitting_text.setString(substr);
//			if (fitting_text.getGlobalBounds().width > boundaries.width) {
//				if (m_string.length() == 0) return false;
//				m_string.erase(m_string.length() - 1, 1);
//			}
//		}
//		if (prev_it == str_it) return false;
//		m_string_vector.push_back(substr);
//	}
//	return true;
//}

template<typename CharType>
sf::Text TextFormatter<CharType>::getFormattedText(std::string const& m_string, unsigned int const& character_size, sf::Vector2f const& boundaries, bool const& bold)
{
	sf::Text text_result("", m_font);
	text_result.setCharacterSize(character_size);
	if (bold) text_result.setStyle(sf::Text::Bold);
	std::string formatted_string = "";
	if (boundaries.x <= 0 || boundaries.y <= 0) {
		text_result.setString(formatted_string);

		return text_result;
	}

	float total_text_height = 0;
	float total_text_width = 0;

	for (std::size_t str_it = 0; str_it < m_string.length();) {

		float text_width = 0;
		float text_height = m_font.getLineSpacing(character_size);
		std::size_t prev_it = str_it;

		if (total_text_height + text_height > boundaries.y) {
			text_result.setString(formatted_string);
			return text_result;
		}

		for (; str_it< m_string.length();) {
			
			if (m_string[str_it] == '\r') {
				str_it++;
				if (str_it < m_string.length() && m_string[str_it] == '\n')
					str_it++;
				break;
			}
			if (m_string[str_it] == '\n') {
				str_it++;
				break;
			}

			sf::Vector2f char_size = { 0,0 };

			text_width += m_font.getGlyph(m_string[str_it], character_size, bold).advance;
			//getKerning doesn't work
			//if (str_it > prev_it) text_width += m_font->getKerning(formatted_string[formatted_string.length() - 1], m_string[str_it], character_size);
			if (str_it > prev_it) {
				text_width += m_font.getKerning(m_string[prev_it], m_string[str_it], character_size);
			}

			if (text_width > boundaries.x) {

				if (str_it == prev_it) {
					text_result.setString(formatted_string);
					return text_result;
				}
				break;
			}
			else {
				formatted_string += m_string[str_it];

				++str_it;
			}
		}
		
		if (str_it < m_string.length()) {
			formatted_string += ntris::newline;
		}

		if (text_width > total_text_width) total_text_width = text_width;
		total_text_height += text_height;
	}

	last_formatted_size = { total_text_width,total_text_height };
	text_result.setString(formatted_string);

	return text_result;
}

template <typename CharType>
sf::Vector2f TextFormatter<CharType>::getLastFormattedSize() const {
	return last_formatted_size;
}

template class TextFormatter<char>;