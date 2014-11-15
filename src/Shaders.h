// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/*
 * Copyright (C) 2014 Canonical Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Brandon Schaefer <brandontschaefer@gmail.com>
 */

char const VERTEX_SHADER_SRC[] =
"attribute vec4 v_position;  \n"
"attribute vec4 a_color;     \n"
"uniform mat4 u_mvp;         \n"
"varying vec4 v_color;       \n"
"                            \n"
"void main()                 \n"
"{                           \n"
"  v_color = a_color;        \n"
"  gl_Position = u_mvp * v_position; \n"
"}                           \n";

char const FRAGMENT_SHADER_SRC[] =
"#ifdef GL_ES              \n"
"precision mediump float;  \n"
"#endif                    \n"
"                          \n"
"varying vec4 v_color;     \n"
"                          \n"
"void main()               \n"
"{                         \n"
"  gl_FragColor = v_color; \n"
"}                         \n";
