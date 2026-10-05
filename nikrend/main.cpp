#include "nikrend.h"
#include <algorithm>
#include <bits/stdc++.h>
#include <chrono>
#include <iostream>
#include <math.h>
#include <ncurses.h>
#include <omp.h>
#include <string>
#include <vector>

using namespace geom;

#define REL_PLAYFIELD_START 0.3f // x distance from left to right
#define REL_PLAYFIELD_END 0.7f
#define PLAYFIELD_VERT_START 3 // starts on line 4 (because everything counts first line as line 0)
#define PLAYFIELD_VERT_END 4   // ends 3 lines before bottom

// FUCK I HARDCODED CONSTANTS INTO THE LAYOUT OF CERTAIN ELEMENTS I NEED TO FIX
//
#define NOTE_WIDTH_FACTOR 10.0f
#define PLAYFIELD_LENGTH 40.0f
#define NOTE_HEIGHT_OFFSET 0.0f

namespace term {
    struct size {
        unsigned int x;
        unsigned int y;
    };
    // currently unused
    enum color {
        black,
        red,
        green,
        orange,
        blue,
        purple,
        cyan,
        white,
    };
    struct character {
        const char *ch;
        color col;
    };
} // namespace term

enum alignment {
    left,
    center,
    right,
};

// persistents
term::size terminal_size;
std::vector<nikrend::object> to_render;
nikrend::mode terminal_mode = nikrend::m_standard;
float ur_history[4] = {0, 0, 0, 0};
vec3 camera_pos = {
    0,
    5, // 10
    0,
};
rotation camera_rot = {
    -0.5, //-0.5
    0,
    0,
};
signed int last_hit;

void nikrend::set_terminal_mode(nikrend::mode mode) {
    switch (mode) {
        case nikrend::m_standard:
            scrollok(stdscr, TRUE);
            curs_set(1);
            endwin();
            echo();
            break;
        case nikrend::m_2d:
            initscr();
            scrollok(stdscr, FALSE);
            curs_set(0);
            noecho();
            break;
        case nikrend::m_3d:
            initscr();
            scrollok(stdscr, FALSE);
            curs_set(0);
            noecho();
            break;
    }
    terminal_mode = mode;
}

std::string float_to_string(float value, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    return oss.str();
}

term::size get_terminal_size() {
    term::size terminal_size;
    getmaxyx(stdscr, terminal_size.y, terminal_size.x);
    return terminal_size;
}

void set_terminal_size(std::pair<unsigned int, unsigned int> size) {
    terminal_size = {
        size.first,  // x
        size.second, // y
    };
}

vec3 rotate_v(vec3 v, char axis, float angle, vec3 rotp) {
    // coordtransform center to rotp
    v.x -= rotp.x;
    v.y -= rotp.y;
    v.z -= rotp.z;
    vec3 ret_v = v;
    switch (axis) {
        case 'x': // zy rotation
            ret_v.z = v.z * cos(angle) - v.y * sin(angle);
            ret_v.y = v.z * sin(angle) + v.y * cos(angle);
            break;
        case 'y': // xz rotation
            ret_v.x = v.x * cos(angle) - v.z * sin(angle);
            ret_v.z = v.x * sin(angle) + v.z * cos(angle);
            break;
        case 'z': // xy rotation
            ret_v.x = v.x * cos(angle) - v.y * sin(angle);
            ret_v.y = v.x * sin(angle) + v.y * cos(angle);
            break;
        default:
            perror("invalid axis");
            exit(1);
    }
    // undo coordtransform
    ret_v.x += rotp.x;
    ret_v.y += rotp.y;
    ret_v.z += rotp.z;
    return ret_v;
}

vec2 project_v(vec3 v) {
    v = rotate_v(v, 'x', camera_rot.x, camera_pos);
    v = rotate_v(v, 'y', camera_rot.y, camera_pos);
    v = rotate_v(v, 'z', camera_rot.z, camera_pos);
    vec2 return_vec = {
        (v.x + camera_pos.x) / v.z,
        (v.y + camera_pos.y) / v.z,
    };
    return return_vec;
}

vec2 world_to_screen(vec2 v) {
    vec2 return_v = {
        (v.x + 1) / 2 * terminal_size.x,
        (v.y + 1) / 2 * terminal_size.y,
    };
    return return_v;
}

void write_aligned_line(std::string str, int y, alignment alignment) {
    const char *cstr = str.c_str();
    if (alignment == left)
        mvaddstr(y, 0, cstr);
    if (alignment == center)
        mvaddstr(y, std::round((terminal_size.x - str.length()) / 2) + 1, cstr);
    // think + 1 is needed to align because my renderer is aligned one to the right :sob:
    if (alignment == right)
        mvaddstr(y, terminal_size.x - 1 - str.length(), cstr);
}

// assumes input is in 2d terminal relative coords already & points with z behind cam are pre culled
// (add check for p1 & p2 z less than 0 in calling func)
void draw_line(vec2 p1, vec2 p2, const char *ch) {
    float dx = fabs(p1.x - p2.x);
    float dy = fabs(p1.y - p2.y);

    float steps = (dx > dy) ? dx : dy;
    float step = 1 / steps; // no divide by 0 guard :p

    for (float t = 0; t < 1; t += step) {
        vec2 line_pos = {
            std::round(p1.x + t * (p2.x - p1.x)),
            std::round(p1.y + t * (p2.y - p1.y)),
        };

        // this check only matters for 3d objects so it doesnt segfault when they go off camera
        if (line_pos.x < terminal_size.x - 1 && line_pos.y < terminal_size.y && line_pos.x >= 0 && line_pos.y >= 0)
            mvaddstr(line_pos.y, line_pos.x, ch);
    }
}

bounding_box get_bounding_box(vec2 points[3]) {
    bounding_box return_box;
    vec2 vmin;
    vec2 vmax;

    std::vector<float> xbuf;
    std::vector<float> ybuf;
    for (int p = 0; p < 3; p++) {
        xbuf.push_back(points[p].x);
        ybuf.push_back(points[p].y);
    }

    vmin.x = *std::min_element(xbuf.begin(), xbuf.end());
    if (vmin.x < 0)
        vmin.x = 0;

    vmin.y = *std::min_element(ybuf.begin(), ybuf.end());
    if (vmin.y < PLAYFIELD_VERT_START || vmin.y > terminal_size.y - PLAYFIELD_VERT_END)
        vmin.y = PLAYFIELD_VERT_START;

    vmax.x = *std::max_element(xbuf.begin(), xbuf.end());
    if (vmax.x >= terminal_size.x)
        vmax.x = terminal_size.x - 1;

    vmax.y = *std::max_element(ybuf.begin(), ybuf.end());
    if (vmax.y >= terminal_size.y - PLAYFIELD_VERT_END || vmax.y < PLAYFIELD_VERT_START)
        vmax.y = terminal_size.y - 1 - PLAYFIELD_VERT_END;

    // if (terminal_mode == nikrend::m_3d)
    //     vmin.x++;

    return_box.vmin = vmin;
    return_box.vmax = vmax;
    return return_box;
}

//+ if ccw, - if cw, 0 if collinear
float get_signed_tri_area(vec2 p1, vec2 p2, vec2 p3) {
    return 0.5 * ((p2.y - p1.y) * (p2.x + p1.x) + (p3.y - p2.y) * (p3.x + p2.x) + (p1.y - p3.y) * (p1.x + p3.x));
}

float edgefunc_p(vec2 p, vec2 v1, vec2 v2) { return (v2.x - v1.x) * (p.y - v1.y) - (v2.y - v1.y) * (p.x - v1.x); }

// assumes points are screen space
void rasterize_tri(vec2 tri_points[3]) {
    // rounding needed to match to line stuff
    vec2 p1 = {
        std::round(tri_points[0].x),
        std::round(tri_points[0].y),
    };
    vec2 p2 = {
        std::round(tri_points[1].x),
        std::round(tri_points[1].y),
    };
    vec2 p3 = {
        std::round(tri_points[2].x),
        std::round(tri_points[2].y),
    };

    tri_points[0] = p1;
    tri_points[1] = p2;
    tri_points[2] = p3;
    bounding_box bb = get_bounding_box(tri_points);

    // I DONT KNOW
    int magic_number;
    if (terminal_mode == nikrend::m_3d) {
        magic_number = 2;
    } else {
        magic_number = 1;
    }

    int bbminx = std::floor(bb.vmin.x);
    int bbmaxx = std::ceil(bb.vmax.x) - 1; // idk why this is required fskfsnkjf
    int bbminy = std::floor(bb.vmin.y);
    int bbmaxy = std::ceil(bb.vmax.y) + magic_number; // idk why im dumb sob

    float total_area = get_signed_tri_area(p1, p2, p3);
    bool is_clockwise = (total_area > 0) ? FALSE : TRUE;

    // assumes points with z < 0 are pre culled before input to function!!
    for (int x = bbminx; x <= bbmaxx; x++) {
        for (int y = bbminy; y <= bbmaxy; y++) {
            vec2 plocal = {
                (float)x,
                (float)y,
            };
            float alpha = edgefunc_p(plocal, p2, p3);
            float beta = edgefunc_p(plocal, p3, p1);
            float gamma = edgefunc_p(plocal, p1, p2);

            if (is_clockwise == TRUE) {
                if (alpha > 0 || beta > 0 || gamma > 0)
                    continue;
            }

            else if (alpha < 0 || beta < 0 || gamma < 0)
                continue;

            // TODO: this guard is no longer needed since i switched away from array but i still need to figure out what
            // was causing the segfaults
            // if (x < 0 || y < 0 || x >= (int)terminal_size.x || y >= (int)terminal_size.y) {
            //     // i fucked something up before so bandaid fix
            //     continue;
            // }
            mvaddstr(y, x, "#");
        }
    }
}

// assumes quad :p
void rasterize_projected_face(std::vector<vec2> points) {
    std::vector<vec2> tris;

    if (terminal_mode == nikrend::m_3d) {
        points[0].x++;
        points[1].x++;
    }

    if (points.size() > 3) {
        vec2 tri1[3] = {
            points[0],
            points[1],
            points[2],
        };
        vec2 tri2[3] = {
            points[0],
            points[2],
            points[3],
        };
        rasterize_tri(tri1);
        rasterize_tri(tri2);
    }

    else {
        vec2 tri[3] = {
            points[0],
            points[1],
            points[2],
        };
        rasterize_tri(tri);
    }
}

float calc_min_z(int vert_y_offset) {
    float target_y = terminal_size.y - vert_y_offset + 1;
    // BAD! - need to smarter
    float search_divider = 5; // set this as close as humanly possible to the actual number to reduce cycles
    vec3 cull_vert{
        0,
        NOTE_HEIGHT_OFFSET,
        PLAYFIELD_LENGTH,
    };
    vec2 cull_proj = world_to_screen(project_v(cull_vert));
    // something is wrong it should equal 5 but doesnt
    for (float z = PLAYFIELD_LENGTH / search_divider; cull_proj.y <= target_y; z -= 0.1) {
        cull_vert.z = z;
        cull_proj = world_to_screen(project_v(cull_vert));
    }
    return cull_vert.z;
}

void draw_object(nikrend::object object) {
    for (size_t face = 0; face < object.faces.size(); face++) {
        bool should_cull = false;
        for (size_t pindex = 0; pindex < object.faces[face].pindex.size(); pindex++) {
            float min_z = calc_min_z(PLAYFIELD_VERT_END);
            if (object.verts[object.faces[face].pindex[pindex] - 1].pos.z < min_z) {
                object.verts[object.faces[face].pindex[pindex] - 1].pos.z = min_z;
                should_cull = true;
                break;
            }
        }

        if (should_cull == true)
            continue;

        std::vector<vec2> projected_face;
        projected_face.clear();
        for (size_t pindex = 0; pindex < object.faces[face].pindex.size(); pindex++) {
            vec2 cur_point = world_to_screen(project_v(object.verts[object.faces[face].pindex[pindex] - 1].pos));

            projected_face.push_back(cur_point);
        }

        rasterize_projected_face(projected_face);
    }
}
float get_note_width() {
    float note_width = ((REL_PLAYFIELD_END - REL_PLAYFIELD_START) * static_cast<float>(terminal_size.x)) / 4.0f;
    return note_width;
}

void nikrend::add_note_to_render_buffer(nikrend::note note) {
    float note_width = get_note_width();
    float note_lane = static_cast<float>(note.lane);
    vec2 ts = {
        static_cast<float>(terminal_size.x),
        static_cast<float>(terminal_size.y),
    };

    if (terminal_mode == nikrend::m_3d || terminal_mode == nikrend::m_standard) {
        nikrend::object append_object;

        float width_factor = NOTE_WIDTH_FACTOR;
        float note_width_3d = ((REL_PLAYFIELD_END - REL_PLAYFIELD_START) * width_factor / 4.0f);
        float min_z = calc_min_z(PLAYFIELD_VERT_END);

        if (note.type == nikrend::type::reg) {
            float note_z_pos = (1 - cbrt(note.start_pos)) * (min_z + PLAYFIELD_LENGTH);

            point n_left;
            n_left.pos = {
                (note_lane - 3) * note_width_3d,
                NOTE_HEIGHT_OFFSET,
                note_z_pos,
            };
            point n_right;
            n_right.pos = {
                (note_lane - 2) * note_width_3d,
                NOTE_HEIGHT_OFFSET,
                note_z_pos,
            };

            if (note_z_pos < min_z) {
                n_left.pos.z = min_z;
                n_right.pos.z = min_z;
            }

            vec2 proj_left = world_to_screen(project_v(n_left.pos));
            proj_left.x++;
            vec2 proj_right = world_to_screen(project_v(n_right.pos));

            draw_line(proj_left, proj_right, "X");
        }

        else {
            float note_z_startpos = (1 - cbrt(note.start_pos)) * (min_z + PLAYFIELD_LENGTH);
            if (note.start_pos > 1)
                note_z_startpos = 0;
            float note_z_endpos = (1 - (cbrt(note.start_pos - note.length))) * (min_z + PLAYFIELD_LENGTH);
            if ((note.start_pos - note.length) < 0)
                note_z_endpos = PLAYFIELD_LENGTH;

            point n_left_start;
            n_left_start.pos = {
                (note_lane - 3) * note_width_3d,
                NOTE_HEIGHT_OFFSET,
                note_z_startpos,
            };
            point n_right_start;
            n_right_start.pos = {
                (note_lane - 2) * note_width_3d,
                NOTE_HEIGHT_OFFSET,
                note_z_startpos,
            };
            point n_left_end;
            n_left_end.pos = {
                (note_lane - 3) * note_width_3d,
                NOTE_HEIGHT_OFFSET,
                note_z_endpos,
            };
            point n_right_end;
            n_right_end.pos = {
                (note_lane - 2) * note_width_3d,
                NOTE_HEIGHT_OFFSET,
                note_z_endpos,
            };

            if (n_left_start.pos.z < min_z) {
                n_left_start.pos.z = min_z;
                n_right_start.pos.z = min_z;
            }

            append_object.verts.insert(append_object.verts.end(),
                                       {n_left_start, n_left_end, n_right_end, n_right_start});

            face append_face;
            append_face.pindex.insert(append_face.pindex.end(), {1, 2, 3, 4});
            append_object.faces.push_back(append_face);
            draw_object(append_object);

            vec2 proj_left = world_to_screen(project_v(n_left_start.pos));
            proj_left.x++;
            vec2 proj_right = world_to_screen(project_v(n_right_start.pos));

            if (note.start_pos <= 1)
                draw_line(proj_left, proj_right, "X");
        }
    }

    else if (terminal_mode == nikrend::m_2d) {
        float note_y_startpos =
            PLAYFIELD_VERT_START + note.start_pos * (ts.y - PLAYFIELD_VERT_START - PLAYFIELD_VERT_END);

        if (note.type == nikrend::type::reg) {
            if (note.start_pos <= 1) {
                vec2 p1{
                    REL_PLAYFIELD_START * ts.x + note_width * (note_lane - 1) + 1,
                    note_y_startpos,
                };
                vec2 p2{
                    REL_PLAYFIELD_START * ts.x + note_width * note_lane,
                    note_y_startpos,
                };

                draw_line(p1, p2, "X");
            }
        }

        else {
            float note_draw_length = note.length * (ts.y - PLAYFIELD_VERT_START - PLAYFIELD_VERT_END);
            float note_y_endpos = PLAYFIELD_VERT_START;

            if (note_y_startpos - note_draw_length > PLAYFIELD_VERT_START)
                note_y_endpos = note_y_startpos - note_draw_length;
            else
                note_y_endpos = PLAYFIELD_VERT_START;

            if (note_y_endpos > terminal_size.y - PLAYFIELD_VERT_END)
                note_y_endpos = terminal_size.y - PLAYFIELD_VERT_END;

            if (note.start_pos > 1)
                note_y_startpos = terminal_size.y - PLAYFIELD_VERT_END;

            vec2 left_start{
                REL_PLAYFIELD_START * ts.x + note_width * (note_lane - 1) + 1,
                note_y_startpos,
            };
            vec2 right_start{
                (REL_PLAYFIELD_START * ts.x + note_width * note_lane),
                note_y_startpos,
            };
            vec2 left_end{
                REL_PLAYFIELD_START * ts.x + note_width * (note_lane - 1) + 1,
                note_y_endpos,
            };
            vec2 right_end{
                REL_PLAYFIELD_START * ts.x + note_width * note_lane,
                note_y_endpos,
            };

            std::vector<vec2> hold_points = {left_start, left_end, right_end, right_start};
            rasterize_projected_face(hold_points);

            if (note.start_pos <= 1)
                draw_line(left_start, right_start, "X");
        }
    }
}

void draw_lane_dividers_2d() {
    float note_width = get_note_width();
    vec2 p1;
    p1.y = (float)PLAYFIELD_VERT_START;
    vec2 p2;
    p2.y = terminal_size.y - (float)PLAYFIELD_VERT_END + 0.5; // why does adding 1 add two???????

    for (int divider_index = 0; divider_index <= 4; divider_index++) {
        int x_pos = std::round(REL_PLAYFIELD_START * (float)terminal_size.x + divider_index * note_width);
        p1.x = x_pos;
        p2.x = x_pos;
        draw_line(p1, p2, "|");
    }
}

void draw_lane_dividers_3d() {
    float width_factor = NOTE_WIDTH_FACTOR;
    float note_width_3d = ((REL_PLAYFIELD_END - REL_PLAYFIELD_START) * width_factor / 4.0f);

    float min_z = calc_min_z(PLAYFIELD_VERT_END);

    vec3 p_start = {
        0,
        0,
        PLAYFIELD_LENGTH,
    };
    vec3 p_end = {
        0,
        NOTE_HEIGHT_OFFSET,
        min_z,
    };
    for (int divider = 1; divider < 6; divider++) {
        p_start.x = (divider - 3) * note_width_3d;
        p_end.x = (divider - 3) * note_width_3d;
        draw_line(world_to_screen(project_v(p_start)), world_to_screen((project_v(p_end))), "*");
    }
}

void nikrend::draw_banner(std::string string) {
    write_aligned_line(string, 1, center);

    vec2 p1 = {
        0,
        0,
    };
    vec2 p2 = {
        (float)terminal_size.x - 1,
        0,
    };
    draw_line(p1, p2, "-");
    p1.y = 2;
    p2.y = 2;
    draw_line(p1, p2, "-");
}

void nikrend::draw_hit(int lane, float unstable_rate, signed int hit_value) {
    float note_width = get_note_width();

    const char *ch;
    if (hit_value == 0)
        ch = ".";
    else if (hit_value < 0)
        ch = "-";
    else
        ch = "^";

    if (terminal_mode == nikrend::m_2d) {
        float y_pos = (float)terminal_size.y - PLAYFIELD_VERT_END + 1;
        vec2 p1 = {
            REL_PLAYFIELD_START * (float)terminal_size.x + (lane - 1) * note_width + 1,
            y_pos,
        };
        vec2 p2 = {
            REL_PLAYFIELD_START * (float)terminal_size.x + lane * (note_width),
            y_pos,
        };

        draw_line(p1, p2, ch);
    }

    else {
        float note_width_3d = ((REL_PLAYFIELD_END - REL_PLAYFIELD_START) * NOTE_WIDTH_FACTOR / 4.0f);
        float min_z = calc_min_z(PLAYFIELD_VERT_END);

        vec3 p_start = {
            0,
            NOTE_HEIGHT_OFFSET,
            min_z,
        };
        vec3 p_end = {
            0,
            NOTE_HEIGHT_OFFSET,
            min_z,
        };

        p_start.x = (lane - 3) * note_width_3d;
        p_end.x = (lane - 2) * note_width_3d;

        vec2 proj_p_start = world_to_screen(project_v(p_start));
        vec2 proj_p_end = world_to_screen(project_v(p_end));

        proj_p_start.x += 1;
        proj_p_start.y += 1;
        proj_p_end.x -= 1;
        proj_p_end.y += 1;

        draw_line(proj_p_start, proj_p_end, ch);
    }

    ur_history[lane - 1] = unstable_rate;
    last_hit = hit_value;
}

void draw_hit_banner() {
    std::string hit_text;
    switch (last_hit) {
        case 320:
            hit_text = " Perfect ";
            break;
        case 300:
            hit_text = " 300 ";
            break;
        case 200:
            hit_text = " 200 ";
            break;
        case 100:
            hit_text = " 100 ";
            break;
        case 50:
            hit_text = " 50 ";
            break;
        case 0:
            hit_text = " Miss ";
            break;
        default:
            hit_text = "";
    }
    write_aligned_line(hit_text, terminal_size.y / 2, center);
}

void draw_ur() {
    if (terminal_mode == nikrend::m_2d) {
        float note_width = get_note_width();
        for (int i = 0; i < 4; i++) {
            // HACK ALERT!!!!!!
            std::string str = std::to_string((int)ur_history[i]);
            const char *ur = str.c_str();
            int urlen = strlen(ur);

            mvaddstr(terminal_size.y - PLAYFIELD_VERT_END + 2,
                     (REL_PLAYFIELD_START * (float)terminal_size.x + (i + 1) * note_width + 1) - urlen - 1, ur);
        }
    }

    else {
        float note_width_3d = ((REL_PLAYFIELD_END - REL_PLAYFIELD_START) * NOTE_WIDTH_FACTOR / 4.0f);
        float z_min = calc_min_z(PLAYFIELD_VERT_END);

        vec3 p = {
            0,
            0,
            z_min,
        };
        vec2 p_proj;

        for (int i = 0; i < 4; i++) {
            std::string str = std::to_string((int)ur_history[i]);
            const char *ur = str.c_str();
            int urlen = strlen(ur);

            p.x = (i - 1) * note_width_3d;
            p_proj = world_to_screen((project_v(p)));
            mvaddstr(terminal_size.y - PLAYFIELD_VERT_END + 3, p_proj.x - urlen, ur);
        }
    }
}

void nikrend::draw_sideinfo(int score, float average_ur, int combo, int hits, float od) {
    std::string scorestr = "Score: ";
    scorestr.append(std::to_string(score));
    std::string urstr = "Avg MS: ";
    urstr.append(float_to_string(average_ur, 2));
    std::string combostr = "Combo: ";
    combostr.append(std::to_string(combo));
    std::string hitsstr = "Hits: ";
    hitsstr.append(std::to_string(hits));
    std::string odstr = "OD: ";
    odstr.append(float_to_string(od, 1));

    write_aligned_line(scorestr, 3, right);
    write_aligned_line(urstr, 4, right);
    write_aligned_line(combostr, 5, right);
    write_aligned_line(hitsstr, 6, right);
    write_aligned_line(odstr, 7, right);
}

void render_to_fb() {
    for (size_t object = 0; object < to_render.size(); object++) {
        draw_object(to_render[object]);
    }
}

void nikrend::draw() {
    if (terminal_mode == nikrend::m_3d || terminal_mode == nikrend::m_standard) {
        //     render_to_fb();
        //     to_render.clear();
        draw_lane_dividers_3d();
    }

    else {
        draw_lane_dividers_2d();
    }

    draw_ur();
    draw_hit_banner();

    refresh();
    erase();
}

int main() {
    nikrend::set_terminal_mode(nikrend::m_3d);
    terminal_size = get_terminal_size();
    set_terminal_size({terminal_size.x, terminal_size.y});
    // for debug this is required so that terminal size can be set
    // nikrend::set_terminal_mode(nikrend::mode::m_standard);

    nikrend::note testnote;
    testnote.lane = 3;
    testnote.start_pos = 0;
    testnote.type = nikrend::type::reg;

    nikrend::note testnote2;
    testnote2.lane = 2;
    testnote2.length = 0.3;
    testnote2.type = nikrend::type::hold;
    bool render = true;
    int cycle = 0;
    double fps;
    std::chrono::time_point<std::chrono::steady_clock> last_tick;
    while (render) {
        auto now = std::chrono::steady_clock::now();
        double delta_time_ms = std::chrono::duration<double, std::milli>(now - last_tick).count();
        fps = 1000 / delta_time_ms;
        last_tick = std::chrono::steady_clock::now();
        write_aligned_line(std::to_string(fps), 4, left);

        if (testnote2.start_pos < 1.5) {
            testnote2.start_pos += 0.00001;
        }
        if (testnote.start_pos < 1)
            testnote.start_pos += 0.00001;
        nikrend::add_note_to_render_buffer(testnote);
        nikrend::add_note_to_render_buffer(testnote2);
        nikrend::draw_hit(3, 16, 320);
        nikrend::draw_banner("hewwo :3");
        nikrend::draw_sideinfo(69420, 37.42, 67, 444, 9.5f);
        nikrend::draw();
    }
}
