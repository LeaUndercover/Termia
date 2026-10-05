#ifndef NIKREND_H_INCLUDED
#define NIKREND_H_INCLUDED

#include <string>
#include <tuple>
#include <vector>

// this is in here so that i can declare object format easily incase we want to
// draw anything other than notes, like maybe some fancy 3d main menu :3
// otherwise could be moved out of header along with class object
namespace geom {
    struct vec3 {
        float x;
        float y;
        float z;
    };
    struct vec2 {
        float x;
        float y;
    };
    struct rotation {
        float x;
        float y;
        float z;
    };
    class point {
      public:
        geom::vec3 pos;
    };
    class face {
      public:
        std::vector<int> pindex;
    };
    class bounding_box {
      public:
        geom::vec2 vmin;
        geom::vec2 vmax;
    };
} // namespace geom

namespace nikrend {
    // can stylize notes and holdes differently
    enum type {
        reg,
        hold
    };
    // for drawing notes
    class note {
      public:
        nikrend::type type;
        int lane;        // 1 - 4
        float start_pos; // float 0 <-> 1; 0 = note appears at top, 1 = note reached hit line;
        float length;    // for holds, same float 0 <-> 1 system as position
    };
    // for directly drawing 3d objects
    class object {
      public:
        std::vector<geom::point> verts;
        std::vector<geom::face> faces;
    };
    // renderer modes
    enum mode {
        m_standard,
        m_2d,
        m_3d,
    };
    // enable/disable ncurses terminal stuff -> std to revert terminal to normal
    void set_terminal_mode(nikrend::mode mode);

    // pass terminal size to renderer
    void set_terminal_size(std::pair<unsigned int, unsigned int> terminal_size); // x, y

    void draw(); // call screen update once everything else has been set

    // use this to pass notes into the renderer -> look at the struct for details
    void add_note_to_render_buffer(nikrend::note note);

    // call this on click
    void draw_hit(int lane, float unstable_rate, signed int hit_value);
    // lane 1-4; pass in ur; hit value equates to 300/50/0, negative means click but no note

    // draw the top banner thingy
    void draw_banner(std::string string);

    // draw the stuffies at the side
    void draw_sideinfo(int score, float average_ur, int combo, int hits, float od);
} // namespace nikrend

#endif
