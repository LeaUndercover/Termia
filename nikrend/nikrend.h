#ifndef NIKREND_H_INCLUDED
#define NIKREND_H_INCLUDED

#include "../Game/GameObject.h" //TODO: needs to be changed to CORE as this branch is pre that
#include "nikrend_datatypes.h"
#include <ncurses.h>
#include <string>
#include <tuple>
#include <vector>

using namespace geom;

namespace nikrend {
    enum mode {
        m_standard,
        m_2d,
        m_3d,
    };
} // namespace nikrend

class renderer {
  private:
    // datatypes
    struct term_size {
        unsigned int x;
        unsigned int y;
    };
    enum type {
        reg,
        hold
    };

    // internal persistents
    nikrend::mode terminal_mode;
    term_size terminal_size;
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

    // internal functions
    term_size get_terminal_size();
    vec2 project_v(vec3 v);
    vec2 world_to_screen(vec2 v);
    void draw_line(vec2 p1, vec2 p2, const char *ch);
    bounding_box get_bounding_box(std::vector<vec2> points);
    void rasterize_tri(std::vector<vec2> tri_points);
    void rasterize_projected_face(std::vector<vec2> points);
    float calc_min_z(int vert_y_offset);
    void draw_object(nikrend::object object);
    float get_note_width();
    void draw_lane_dividers_2d();
    void draw_lane_dividers_3d();
    void draw_hit_banner();
    void draw_ur();

  public:
    // constructor
    renderer(const nikrend::mode mode): terminal_mode(mode), terminal_size(get_terminal_size()) {};

    // datatypes
    enum alignment {
        left,
        center,
        right,
    };

    // functions
    void set_terminal_mode(const nikrend::mode mode);
    void set_terminal_size(std::pair<unsigned int, unsigned int> terminal_size); // x, y

    //! call each frame -> add notes before calling anything else
    void add_note_to_render_buffer(const GameObject &note);
    void draw_banner(std::string string);
    void draw_hit(int lane, float unstable_rate, signed int hit_value);
    void draw_sideinfo(int score, float average_ur, int combo, int hits, float od);
    void draw();

    void write_aligned_line(std::string str, int y, alignment alignment); // can be used to draw debug stuff: e.g fps

    // destructor
    ~renderer() {
        set_terminal_mode(nikrend::m_standard);
    }
};

#endif
