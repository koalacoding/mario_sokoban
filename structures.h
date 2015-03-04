// Information about which square is Mario in.
typedef struct MarioSquarePosition MarioSquarePosition;
struct MarioSquarePosition {
    int x;
    int y;
};

// Surface contains x, y, width and height informations of a surface.
typedef struct Surface Surface;
struct Surface {
    int x;
    int y;
    int width;
    int height;
};
