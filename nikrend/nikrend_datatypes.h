#ifndef NIKREND_DATATYPES_INCLUDED
#define NIKREND_DATATYPES_INCLUDED

#include <string>
#include <tuple>
#include <vector>

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
    class object {
      public:
        std::vector<geom::point> verts;
        std::vector<geom::face> faces;
    };
} // namespace nikrend

#endif
