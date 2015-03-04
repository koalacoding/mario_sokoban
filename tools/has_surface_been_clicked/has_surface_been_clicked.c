#include "../../structures.h"

int has_surface_been_clicked(Surface surface, int mouse_pointer_x, int mouse_pointer_y) {
    if ((mouse_pointer_x >= surface.x && mouse_pointer_y >= surface.y)
        && (mouse_pointer_x <= surface.x + surface.width
            && mouse_pointer_y <= surface.y + surface.height)) {
        return 1;
    }

    return 0;
}
