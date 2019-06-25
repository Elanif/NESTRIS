#include"TextFormatter.hpp"
#include<SFML/System/String.hpp>

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
//bool TextFormatter<CharType>::fitString()
//{
//	sf::Text fitting_text;
//	fitting_text.setCharacterSize(m_font_size);
//	fitting_text.setColor(m_text_color);
//	fitting_text.setFillColor(m_fill_color);
//	if (m_font)	fitting_text.setFont;
//
//	m_string_vector.clear();
//
//	if (m_boundaries.width <= 0) {
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
//			if (fitting_text.getGlobalBounds().width > m_boundaries.width) {
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
bool TextFormatter<CharType>::calculated = false;

template<typename CharType>
bool TextFormatter<CharType>::fitString()
{
	if (!m_font) return false;
	if (!calculated) calc_font_sizes(m_font);
	//todo height boundary
	float text_size_multiplier = m_character_size / glb::info_window_character_size;

	if (m_boundaries.x <= 0 && m_boundaries.y<=0) {
		m_formatted_string = m_string;
		m_elaborated = true;
		return true;
	}

	float text_height = 0;

	for (std::size_t str_it = 0; str_it < m_string.length();) {

		float text_width = 0;
		float current_text_height = 0;
		std::size_t prev_it = str_it;

		for (; str_it< m_string.length();) {

			sf::Vector2f const& char_size = character_size_map[m_string[str_it]];
			text_width += char_size.x;

			if (m_boundaries.x>0 && text_width > m_boundaries.x) {

				if (str_it==prev_it) return false;
				
				break;
			}
			else {

				if (char_size.y > current_text_height) current_text_height = char_size.y;

				if (m_boundaries.y > 0 && (text_height + current_text_height > m_boundaries.y)) return false;

				m_formatted_string += m_string[str_it];

				++str_it;
			}
		}

		text_height += current_text_height;

		if (str_it < m_string.length()) m_formatted_string += "\n\r";
	}
	m_elaborated = true;
	return true;
}

template<typename CharType>
bool TextFormatter<CharType>::fitStringApprox()
{
	if (m_boundaries.x <= 0) {
		m_formatted_string = m_string;
		m_elaborated = true;
		return true;
	}
	m_formatted_string = m_string;
	std::size_t char_width = m_boundaries.x / m_character_size*2.;
	if (char_width == 0) {
		m_elaborated = false;
		return false;
	}
	for (std::size_t str_it = 0; str_it < m_string.length();) {
		std::size_t prev_it = str_it;
		std::size_t sub_str_it = 0;
		if (str_it+char_width<m_string.length()) 
			m_formatted_string.insert(str_it + char_width, "\n\r");
		str_it += char_width + 2;
	}
	m_elaborated = true;
	return true;
}


template <typename CharType>
sf::Text TextFormatter<CharType>::getFormattedText() const {
	sf::Text t;
	t.setCharacterSize(m_character_size);
	t.setFillColor(m_fill_color);
	if (m_font) t.setFont(*m_font);
	t.setString(m_formatted_string);
	t.setPosition(m_position);
	return t;
}

template <typename CharType>
float TextFormatter<CharType>::max_character_height = 0;

template <typename CharType>
std::unordered_map<string_character, sf::Vector2f> TextFormatter<CharType>::character_size_map;

template <typename CharType>
void TextFormatter<CharType>::calc_font_sizes(sf::Font const* m_font) {
	if (!m_font) {
		calculated = false;
		return; 
	}
	max_character_height = 0;
	sf::Text t;
	t.setFont(*m_font);
	t.setCharacterSize(glb::info_window_character_size);
	for (string_character _char = (string_character)(std::size_t{ 32 }); _char <= (string_character)(std::size_t{ 126 }); ++_char) {
		t.setString(_char);
		sf::Vector2f char_bounds = { t.getGlobalBounds().width, t.getGlobalBounds().height };
		character_size_map[_char] = char_bounds;
		if (char_bounds.y > max_character_height) max_character_height = char_bounds.y;
	}
	calculated = true;
}

template class TextFormatter<char>;