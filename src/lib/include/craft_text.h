/**
 * libcraft
 * Copyright (C) 2015 David Jolly
 * ----------------------
 *
 * libcraft is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcraft is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CRAFT_TEXT_H_
#define CRAFT_TEXT_H_

namespace CRAFT {

	namespace COMPONENT {

		typedef uint32_t craft_font;

		typedef class _craft_text {

			public:

				~_craft_text(void);

				static _craft_text *acquire(void);

				craft_font add_face(
					__in const std::string &path,
					__in size_t size
					);

				void clear(void);

				bool contains_face_reference(
					__in const craft_font &id
					);

				size_t decrement_face_reference(
					__in const craft_font &id
					);

				size_t face_count(void);

				std::string face_path(
					__in const craft_font &id
					);

				size_t face_reference(
					__in const craft_font &id
					);

				size_t face_size(
					__in const craft_font &id
					);

				size_t increment_face_reference(
					__in const craft_font &id
					);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				void render(
					__in const glm::mat4 &mvp
					);

				void set_face_size(
					__in const craft_font &id,
					__in size_t size
					);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

			protected:

				_craft_text(void);

				_craft_text(
					__in const _craft_text &other
					);

				_craft_text &operator=(
					__in const _craft_text &other
					);

				static void _delete(void);

				std::map<craft_font, std::pair<std::pair<FT_Face, std::pair<std::string, size_t>>, size_t>>::iterator find_face(
					__in const craft_font &id
					);

				FT_GlyphSlot load_glyph(
					__in const craft_font &id,
					__in FT_ULong character,
					__in_opt FT_Int32 flag = FT_LOAD_RENDER
					);

				craft_font reserve_id(void);

				void retire_id(
					__in const craft_font &id
					);

				void setup(void);

				void teardown(void);

				std::map<craft_font, std::pair<std::pair<FT_Face, std::pair<std::string, size_t>>, size_t>> m_face_map;				

				bool m_initialized;

				static _craft_text *m_instance;

				FT_Library m_library;

				craft_font m_next_id;

				std::set<craft_font> m_surplus_id;

				GLuint m_texture;

				GLuint m_vertex_buffer;
		} craft_text;
	}
}

#endif // CRAFT_TEXT_H_
