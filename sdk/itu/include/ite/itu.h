/** @file
 * ITE UI Library.
 *
 * @author Jim Tan
 * @version 1.0
 * @date 2013
 * @copyright ITE Tech. Inc. All Rights Reserved.
 */
/** @defgroup itu ITE UI Library
 *  @{
 */
#ifndef ITE_ITU_H
#define ITE_ITU_H

#include "ite/itc.h"
#include "ite/ith.h"
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include "gfx/gfx.h"

#ifdef __cplusplus
extern "C"
{
#endif

// Definitions
#define ITU_COMMAND_SIZE            8       ///< Maximum size of command count.
#define ITU_ICON_LISTBOX_TYPE_COUNT 16      ///< Maximum icon type count of icon list box.
#define ITU_EXTENSIONS_SIZE         32      ///< Maximum size of extensions in ITUMediaFileListBox, in bytes.
#define ITU_MEDIA_FILE_MAX_COUNT    999     ///< Maximum file count of media file list box.
#define ITU_DRAG_DISTANCE           22      ///< Distance to start dragging, in pixels.
#define ITU_EFFECT_STEP_COUNT       10      ///< Default effect steps.
#define ITU_FREETYPE_MAX_SBIT_CACHE 256     ///< The font size to use graph bitmap cache (< 128 use sbit, or use graph cache)
#define ITU_VARIABLE_SIZE           8       ///< Maximum size of variable count.
#define ITU_GIF_BLOCK_BITS          4       ///< The block bits align for GIF decode block.


#ifndef CFG_ITU_LITE_MODE
#define ITU_WIDGET_CHILD_MAX        1000    ///< Maximum count of children.
#define ITU_STRINGSET_SIZE          32      ///< Maximum number of strings in ITUStringSet.
#define ITU_ACTIONS_SIZE            15      ///< Maximum number of actions in input type of widgets.
#define ITU_ACTION_PARAM_SIZE       64      ///< Maximum size of parameter in ITUAction, in bytes.
#define ITU_ACTION_QUEUE_SIZE       64      ///< Maximum size of action queue.
#define ITU_WIDGET_NAME_SIZE        64      ///< Maximum size of name of Widget, in bytes.
#define ITU_PATH_MAX                512     ///< Maximum size of path, in bytes.
#define ITU_FREETYPE_MAX_FONTS      16      ///< Maximum count of loaded fonts.
#define ITU_GIF_MAX_FRAME           120     ///< The maximum frame count for GIF support (memory limit issue)
#else
#define ITU_WIDGET_CHILD_MAX        256     ///< Maximum count of children.
#define ITU_STRINGSET_SIZE          32      ///< Maximum number of strings in ITUStringSet.
#define ITU_ACTIONS_SIZE            4       ///< Maximum number of actions in input type of widgets.
#define ITU_ACTION_PARAM_SIZE       64      ///< Maximum size of parameter in ITUAction, in bytes.
#define ITU_ACTION_QUEUE_SIZE       16      ///< Maximum size of action queue.
#define ITU_WIDGET_NAME_SIZE        64      ///< Maximum size of name of Widget, in bytes.
#define ITU_PATH_MAX                128     ///< Maximum size of path, in bytes.
#define ITU_FREETYPE_MAX_FONTS      2       ///< Maximum count of loaded fonts.
#define ITU_GIF_MAX_FRAME           20      ///< The maximum frame count for GIF support (memory limit issue)
#endif


#define ITU_LITE_MODE    0x2     ///< Use ITU LITE mode

#ifndef _WIN32
#if (((CFG_CHIP_FAMILY == 970) || (CFG_CHIP_FAMILY == 9860)|| (CFG_CHIP_FAMILY == 9830)) && defined(CFG_ITU_FT_CACHE_ENABLE))
#define ITU_ENABLE_DEFER_DESTROY
#endif
#endif

/**
 * Rectangle definition
 */
typedef struct
{
    int x;      ///< X coordinate, in pixels
    int y;      ///< Y coordinate, in pixels
    int width;  ///< Rectangle width, in pixels
    int height; ///< Rectangle height, in pixels
} ITURectangle;

/**
 * Color definition
 */
typedef struct
{
    uint8_t alpha;  ///< Alpha value
    uint8_t red;    ///< Red value
    uint8_t green;  ///< Green value
    uint8_t blue;   ///< Blue value
} ITUColor;

/**
 * Sets color value.
 *
 * @param color the color to set to.
 * @param a the alpha value.
 * @param r the red value.
 * @param g the green value.
 * @param b the blue value.
 */
static inline void ituSetColor(ITUColor* color, uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    color->alpha  = a;
    color->red    = r;
    color->green  = g;
    color->blue   = b;
}

/**
 * Copies copy value.
 *
 * @param dest the destination to copy to.
 * @param src the source to copy.
 */
static inline void ituCopyColor(ITUColor* dest, ITUColor* src)
{
    dest->alpha  = src->alpha;
    dest->red    = src->red;
    dest->green  = src->green;
    dest->blue   = src->blue;
}

/**
 * Point definition
 */
typedef struct
{
    int x;  ///< X coordinate, in pixels
    int y;  ///< Y coordinate, in pixels
} ITUPoint;

/** @defgroup itu_renderer Renderers
 *  @{
 */
/**
 * Pixel format definition
 */
typedef enum
{
    ITU_RGB565      = 0, ///< RGB565 format
    ITU_ARGB1555    = 1, ///< ARGB1555 format
    ITU_ARGB4444    = 2, ///< ARGB4444 format
    ITU_ARGB8888    = 3, ///< ARGB8888 format
    ITU_MONO        = 4, ///< 1-bpp format
    ITU_RGB565A8    = 5, ///< RGB565A8 format
    ITU_A8          = 6, ///< A8 format
    ITU_MASK_A8     = 7  ///< Mask A8 format
} ITUPixelFormat;

/**
 * Glyph format definition
 */
typedef enum
{
    ITU_GLYPH_1BPP, ///< 1 bit-per-pixel
    ITU_GLYPH_4BPP, ///< 4 bit-per-pixel
    ITU_GLYPH_8BPP  ///< 8 bit-per-pixel
} ITUGlyphFormat;

typedef enum
{
    ITU_MASK_A_8,           // 0x00
    ITU_MASK_A_4,           // 0x01
    ITU_MASK_A_2,           // 0x02
    ITU_MASK_A_1,           // 0x03
    ITU_MASK_UNKNOWN,       // Some Error!
} ITUMaskFormat;

/**
 * Rotation definition
 */
typedef enum
{
    ITU_ROT_0,      ///< No rotate
    ITU_ROT_90,     ///< 90 degree rotate
    ITU_ROT_180,    ///< 180 degree rotate
    ITU_ROT_270     ///< 270 degree rotate
} ITURotation;

/**
 * Gradient mode definition
 */
typedef enum
{
    ITU_GF_NONE,                ///< No gradient.
    ITU_GF_HORIZONTAL,          ///< Specifies a gradient from left to right.
    ITU_GF_VERTICAL,            ///< Specifies a gradient from top to bottom.
    ITU_GF_FORWARD_DIAGONAL,    ///< Specifies a gradient from upper left to lower right.
    ITU_GF_BACKWARD_DIAGONAL    ///< Specifies a gradient from upper right to lower left.
} ITUGradientMode;

typedef enum
{
    ITU_TILE_FILL,          // 0
    ITU_TILE_PAD,           // 1
    ITU_TILE_REPEAT,        // 2
    ITU_TILE_REFLECT,       // 3
    ITU_TILE_UNKNOWN,       // Error
} ITUTileMode;

typedef enum
{
    ITU_QUALITY_BETTER,     // 0
    ITU_QUALITY_FASTER      // 1
} ITUQualityMode;

/**
 * PageFlow type definition.
 */
typedef enum
{
    ITU_PAGEFLOW_FOLD,      ///< Fold effect
    ITU_PAGEFLOW_FLIP,      ///< Flip effect
    ITU_PAGEFLOW_FLIP2,     ///< Flip 2 pieces effect
    ITU_PAGEFLOW_FOLD2,     ///< Fold 2 effect, no clipping destWidth
    ITU_PAGEFLOW_QUADRANGLE ///< Quadrangle
} ITUPageFlowType;

/**
* Transform type definition.
*/
typedef enum
{
    ITU_TRANSFORM_NONE = 0,        ///< No transform
    ITU_TRANSFORM_TURN_LEFT = 1,   ///< Turn to left side
    ITU_TRANSFORM_TURN_TOP = 2,    ///< Turn to top side
    ITU_TRANSFORM_TURN_RIGHT = 3,  ///< Turn to right side
    ITU_TRANSFORM_TURN_BOTTOM = 4  ///< Turn to bottom side

} ITUTransformType;

// Surface
#define ITU_STATIC      0x1     ///< This surface won't be deleted.
#define ITU_CLIPPING    0x2     ///< This surface is clipping.
#define ITU_COMPRESSED  0x4     ///< This surface is compressed.
#define ITU_JPEG        0x8     ///< This surface is JPEG image.
#define ITU_DITHER      0x10    ///< This surface need to be dithered.
#define ITU_SPEEDY      0x20    ///< This surface is compressed by Speedy algorithm.

/**
 * Surface definition.
 */
typedef struct ITUSurface
{
    int                  width;     ///< Width in pixels
    int                  height;    ///< Height in pixels
    int                  pitch;     ///< Pitch in bytes
    ITUPixelFormat       format;    ///< Format
    unsigned int         flags;     ///< Flags; can be union of ITU_STATIC/ITU_CLIPPING/ITU_COMPRESSED/ITU_JPEG
    ITUColor             fgColor;   ///< Foreground color
    ITURectangle         clipping;  ///< Clipping region
    uint32_t             addr;      ///< Surface buffer address
    unsigned int         size;      ///< Surface data size
    uint8_t              *lockAddr; ///< Locked buffer address for read/write
    unsigned int         lockSize;  ///< Locked buffer size for read/write
    struct ITUSurfaceTag *parent;   ///< Compressed parent surface
} ITUSurface;

typedef struct
{
    int32_t         width;
    int32_t         height;
    uint32_t        pitch;
    ITUMaskFormat   format;
    uint8_t*        imageData;
    uint32_t        imageDataLength;
    bool            imageDataOwner;
} ITUMaskSurface;

typedef struct ITUSurface ITUSurface;

/**
 * Decompresses the surface.
 *
 * @param surf the surface to decompress.
 * @return the decompressed surface.
 */
ITUSurface* ituSurfaceDecompress(ITUSurface* surf);

/**
 * Releases the surface.
 *
 * @param surf the surface to release.
 */
void ituSurfaceRelease(ITUSurface* surf);

/** @defgroup itu_renderer_lcd LCD Display
 *  @{
 */
/**
 * Initializes LCD display.
 */
void ituLcdInit(void);

/**
 * Exits LCD display.
 */
void ituLcdExit(void);

/** @} */ // end of itu_renderer_lcd

/** @defgroup itu_renderer_sw SW Renderer
 *  @{
 */
/**
 * Initializes software renderer.
 */
void ituSWInit(void);

/**
 * Exits software renderer.
 */
void ituSWExit(void);

/** @} */ // end of itu_renderer_sw

/** @defgroup itu_renderer_m2d M2D Renderer
 *  @{
 */
/**
 * Initializes M2D renderer.
 */
void ituM2dInit(void);

/**
 * Exits M2D renderer.
 */
void ituM2dExit(void);

/** @} */ // end of itu_renderer_m2d

/** @defgroup itu_renderer_framefunc Frame Function Renderer
 *  @{
 */
/**
 * Initializes frame-function renderer and display.
 */
void ituFrameFuncInit(void);

/**
 * Exits frame-function renderer and display.
 */
void ituFrameFuncExit(void);
int ituFrameFuncGetSurfWidth();
int ituFrameFuncGetSurfHeight();
void ituFrameFuncColorFill(int rect_x, int rect_y, int rect_w, int rect_h, int alpha, int red, int green, int blue);
void ituFrameFuncDrawStr(int font_x, int font_y, int font_w, int font_h, char *string);
void ituFrameFuncDrawJpeg(int gui_x, int gui_y, int gui_w, int gui_h, char *jpg_data, int jpg_size);
void ituFrameFuncFlip(int gui_x, int gui_y, int gui_w, int gui_h);


/**
 * Draw video frame to RGB565 surface. 
 */
void ituDrawVideoSurface(ITUSurface* dest, int startX, int startY, int width, int height);

int ituDrawVideoSurface_ex(ITUSurface* dest, int startX, int startY, int width, int height);
/** @} */ // end of itu_renderer_framefunc

/** @defgroup itu_renderer_stnlcd STN LCD
 *  @{
 */
/**
 * Initializes STN LCD renderer and display.
 */
void ituStnLcdInit(void);

/**
 * Exits STN LCD renderer and display.
 */
void ituStnLcdExit(void);

/** @} */ // end of itu_renderer_stnlcd

// Renderer implementation callback functions
/**
 * Gets display surface.
 *
 * @return the display surface.
 */
extern ITUSurface* (*ituGetDisplaySurface)(void);

/**
 * Creates surface.
 *
 * @param w the surface width, in pixels.
 * @param h the surface height, in pixels.
 * @param pitch the surface pitch, in bytes.
 * @param format the surface format.
 * @param bitmap surface bitmap data address, null for empty surface.
 * @param flags can be ITU_STATIC or 0.
 * @return the STN LCD display surface. null if failed
 */
extern ITUSurface* (*ituCreateSurface)(int w, int h, int pitch, ITUPixelFormat format, const uint8_t* bitmap, unsigned int flags);

/**
 * Destroies surface.
 *
 * @param surf the surface to destroy.
 */
extern void (*ituDestroySurface)(ITUSurface* surf);

extern ITUMaskSurface* (*ituCreateMaskSurface)(int w, int h, int pitch, ITUMaskFormat format, uint8_t* buffer, uint32_t bufferLength);

extern void (*ituDestroyMaskSurface)(ITUMaskSurface* surf);

extern void (*ituSetMaskSurface)(ITUSurface* surface, ITUMaskSurface* maskSurface, bool enable);

/**
 * Locks surface for software rendering.
 *
 * @param surf the surface to lock.
 * @param x the locking x coordinate.
 * @param y the locking y coordinate.
 * @param w the locking width.
 * @param h the locking height.
 * @return the locking buffer address.
 */
extern uint8_t* (*ituLockSurface)(ITUSurface* surf, int x, int y, int w, int h);

/**
 * Unlocks surface.
 *
 * @param surf the surface to unlock.
 */
extern void (*ituUnlockSurface)(ITUSurface* surf);

/**
 * Sets rotation.
 *
 * @param rot the rotation.
 */
extern void (*ituSetRotation)(ITURotation rot);


/**
 * Draws a font glyph to specified surface.
 *
 * @param surf the surface to draw.
 * @param x the x coordinate, in pixels.
 * @param y the y coordinate, in pixels.
 * @param format the glyph format.
 * @param bitmap the glyph bitmap.
 * @param w the glyph width, in pixels.
 * @param h the glyph height, in pixels.
 */
extern void (*ituDrawGlyph)(ITUSurface* surf, int x, int y, ITUGlyphFormat format, const uint8_t* bitmap, int w, int h);

/**
 * Bitblts source surface to destination surface.
 *
 * @param dest the destination surface to draw to.
 * @param dx the x coordinate of destination surface, in pixels.
 * @param dy the y coordinate of destination surface, in pixels.
 * @param w the width to bitblt, in pixels.
 * @param h the height to bitblt, in pixels.
 * @param src the source surface to draw.
 * @param sx the x coordinate of source surface, in pixels.
 * @param sy the y coordinate of source surface, in pixels.
 */
extern void (*ituBitBlt)(ITUSurface* dest, int dx, int dy, int w, int h, ITUSurface* src, int sx, int sy);

/**
 * Stretchblts source surface to destination surface.
 *
 * @param dest the destination surface to draw to.
 * @param dx the x coordinate of destination surface, in pixels.
 * @param dy the y coordinate of destination surface, in pixels.
 * @param dw the destination width to stretchblt, in pixels.
 * @param dh the destination height to stretchblt, in pixels.
 * @param src the source surface to draw.
 * @param sx the x coordinate of source surface, in pixels.
 * @param sy the y coordinate of source surface, in pixels.
 * @param sw the source width to stretchblt, in pixels.
 * @param sh the source height to stretchblt, in pixels.
 */
extern void (*ituStretchBlt)(ITUSurface* dest, int dx, int dy, int dw, int dh, ITUSurface* src, int sx, int sy, int sw, int sh);

/**
 * AlphaBlending source surface to destination surface.
 *
 * @param dest the destination surface to draw to.
 * @param dx the x coordinate of destination surface, in pixels.
 * @param dy the y coordinate of destination surface, in pixels.
 * @param w the width to alphablend, in pixels.
 * @param h the height to alphablend, in pixels.
 * @param src the source surface to draw.
 * @param sx the x coordinate of source surface, in pixels.
 * @param sy the y coordinate of source surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending.
 */
extern void (*ituAlphaBlend)(ITUSurface* dest, int dx, int dy, int w, int h, ITUSurface* src, int sx, int sy, uint8_t alpha);

/**
 * Fills specified color to specified surface.
 *
 * @param surf the surface to draw to.
 * @param dx the x coordinate of surface, in pixels.
 * @param dy the y coordinate of surface, in pixels.
 * @param w the width to fill, in pixels.
 * @param h the height to fill, in pixels.
 * @param color the color value to draw.
 */
extern void (*ituColorFill)(ITUSurface* surf, int x, int y, int w, int h, ITUColor* color);

extern void (*ituColorFillBlend)(ITUSurface* surf, int x, int y, int w, int h, ITUColor* color, bool enableAlpha, bool enableConstantAlpha, uint8_t constantAlphaValue);
/**
 * Gradient fills specified colors and mode to specified surface.
 *
 * @param surf the surface to draw to.
 * @param dx the x coordinate of surface, in pixels.
 * @param dy the y coordinate of surface, in pixels.
 * @param w the width to fill, in pixels.
 * @param h the height to fill, in pixels.
 * @param startColor the start color value to draw.
 * @param endColor the end color value to draw.
 * @param mode the gradinet fill mode.
 */
extern void (*ituGradientFill)(ITUSurface* surf, int x, int y, int w, int h, ITUColor* startColor, ITUColor* endColor, ITUGradientMode mode);

extern void (*ituGradientFillBlend)(ITUSurface* surf, int x, int y, int w, int h, ITUColor* startColor, ITUColor* endColor, ITUGradientMode mode, bool enableAlpha, bool enableConstantAlpha, uint8_t constantAlphaValue);

/**
 * Rotates source surface to destination surface.
 *
 * @param dest the destination surface to draw to.
 * @param dx the x coordinate of destination surface to draw the center of source surface, in pixels.
 * @param dy the y coordinate of destination surface to draw the center of source surface, in pixels.
 * @param src the source surface to draw.
 * @param cx the x coordinate of the center of rotation.
 * @param cy the y coordinate of the center of rotation.
 * @param angle the angle to rotate in degrees.
 * @param scaleX the x coordinate of scale value.
 * @param scaleY the y coordinate of scale value.
 */
extern void (*ituRotate)(ITUSurface* dest, int dx, int dy, ITUSurface* src, int cx, int cy, float angle, float scaleX, float scaleY);

extern void (*ituTransform)(ITUSurface* dest, int dx, int dy, int dw, int dh, ITUSurface* src, int sx, int sy, int sw, int sh, int cx, int cy, float scaleWidth, float scaleHeight, float angle, ITUTileMode tilemode, bool enableAlpha, bool enableConstantAlpha, uint8_t constantAlphaValue);

/**
 * Flips the specified surface.
 *
 * @param surf the surface to flip.
 */
extern void (*ituFlip)(ITUSurface* surf);

extern void (*ituProjection)(ITUSurface* dest, int dx, int dy, int dw, int dh, ITUSurface* src, int sx, int sy, int sw, int sh, float scaleWidth, float scaleHeight, float degree, float FOV, float pivotX, bool enableAlpha, bool enableConstantAlpha, uint8_t constantAlphaValue);

/**
* Draws a line on the specified surface.
*
* @param surf the surface to draw.
* @param fromX the x coordinate of start point, in pixels.
* @param fromY the y coordinate of start point, in pixels.
* @param toX the x coordinate of end point, in pixels.
* @param toY the y coordinate of end point, in pixels.
* @param lineColor the color of line.
* @param lineWidth the width of line, in pixels.
*/
extern void (*ituDrawLine)(ITUSurface* surf, int32_t fromX, int32_t fromY, int32_t toX, int32_t toY, ITUColor* lineColor, int32_t lineWidth);

extern void (*ituDrawCurve)(ITUSurface* surf, ITUPoint* point1, ITUPoint* point2, ITUPoint* point3, ITUPoint* point4, ITUColor* lineColor, int32_t lineWidth);

/**
 * Transformblits source surface to destination surface.
 *
 * @param dest the destination surface to draw to.
 * @param dx the x coordinate of destination surface, in pixels.
 * @param dy the y coordinate of destination surface, in pixels.
 * @param src the source surface to draw.
 * @param sx the x coordinate of source surface, in pixels.
 * @param sy the y coordinate of source surface, in pixels.
 * @param sw the source width to transformblt, in pixels.
 * @param sh the source height to transformblt, in pixels.
 * @param x0 the x coordinate of point 0 on source surface, in pixels.
 * @param y0 the y coordinate of point 0 of source surface, in pixels.
 * @param x1 the x coordinate of point 1 on source surface, in pixels.
 * @param y1 the y coordinate of point 1 of source surface, in pixels.
 * @param x2 the x coordinate of point 2 on source surface, in pixels.
 * @param y2 the y coordinate of point 2 of source surface, in pixels.
 * @param x3 the x coordinate of point 3 on source surface, in pixels.
 * @param y3 the y coordinate of point 3 of source surface, in pixels.
 */
extern void (*ituTransformBlt)(ITUSurface* dest, int dx, int dy, ITUSurface* src, int sx, int sy, int sw, int sh, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3, bool bInverse, ITUPageFlowType type, ITUTransformType transformType);

/**
* Reflected source surface to destination surface.
*
* @param dest the destination surface to draw to.
* @param dx the x coordinate of destination surface, in pixels.
* @param dy the y coordinate of destination surface, in pixels.
* @param src the source surface to draw.
*/
extern void(*ituReflected)(ITUSurface* dest, int dx, int dy, ITUSurface* src, int sx, int sy, int reflectedWidth, int reflectedHeight, ITUSurface* masksurf);

/**
* Sync the content of LCD framebuffers.
*
* @param points the points of rectangles to sync.
* @param count the count of rectangles.
*/
extern void(*ituFlipSync)(int* points, int count);

/**
 * Sets clipping region to surface.
 *
 * @param surf the surface to set to.
 * @param x the clipping x coordinate.
 * @param y the clipping y coordinate.
 * @param w the clipping width.
 * @param h the clipping height.
 */
void ituSurfaceSetClipping(ITUSurface* surf, int x, int y, int w, int h);

/** @} */ // end of itu_renderer

/** @defgroup itu_jpeg JPEG
 *  @{
 */

/**
 * Loads JPEG data to surface.
 *
 * @param width surface width. Could be 0.
 * @param height surface height. Could be 0.
 * @param data the JPEG data.
 * @param size the JPEG data size.
 * @param flags Can be 0 or ITU_FIT_TO_RECT or ITU_CUT_BY_RECT.
 * @return the loaded surface.
 */
ITUSurface* ituJpegLoad(int width, int height, uint8_t* data, int size, unsigned int flags);

/**
* Loads JPEG with alpha data to surface.
*
* @param width surface width.
* @param height surface height.
* @param alpha the alpha data.
* @param data the JPEG data.
* @param size the JPEG data size.
* @return the loaded surface.
*/
ITUSurface* ituJpegAlphaLoad(int width, int height, uint8_t* alpha, uint8_t* data, int size);

/**
 * Loads JPEG file to surface.
 *
 * @param width surface width. Could be 0.
 * @param height surface height. Could be 0.
 * @param filepath the JPEG file path.
 * @param flags Can be 0 or ITU_FIT_TO_RECT or ITU_CUT_BY_RECT.
 * @return the loaded surface.
 */
ITUSurface *ituJpegLoadFile(int width, int height, char* filepath, unsigned int flags);

/**
* Saves surface to JPEG file.
*
* @param surf The surface to save.
* @param filepath the JPEG file path.
*/
void ituJpegSaveFile(ITUSurface* surf, char* filepath);

/**
 * Loads PNG data to surface.
 *
 * @param width surface width. Could be 0.
 * @param height surface height. Could be 0.
 * @param data the PNG data.
 * @param size the PNG data size.
 * @return the loaded surface.
 */
ITUSurface *ituPngLoad(int width, int height, uint8_t *data, int size);

/**
 * Loads PNG file to surface.
 *
 * @param width surface width. Could be 0.
 * @param height surface height. Could be 0.
 * @param filepath the PNG file path.
 * @return the loaded surface.
 */
ITUSurface *ituPngLoadFile(int width, int height, char* filepath);

/**
* Saves surface to PNG file.
*
* @param surf The surface to save.
* @param filepath the PNG file path.
*/
void ituPngSaveFile(ITUSurface* surf, char* filepath);

/** @} */ // end of itu_jpeg

/** @defgroup itu_freetype FreeType
 *  @{
 */
/**
 * Initializes FreeType module.
 *
 * @return 0 if success. non-zero value if failed.
 */
int ituFtInit(void);

/**
 * Exits FreeType module.
 */
void ituFtExit(void);

/**
 * Loads font file to specified index.
 *
 * @param index the index to load to. Must small than ITU_FREETYPE_MAX_FONTS.
 * @param filename the font file to load.
 * @param format the glyph format to convert to.
 * @return 0 if success. non-zero value if failed.
 */
int ituFtLoadFont(int index, char* filename, ITUGlyphFormat format);

/**
* Loads font array to specified index.
*
* @param index the index to load to. Must small than ITU_FREETYPE_MAX_FONTS.
* @param array The array buffer to load.
* @param size The array size.
* @param format the glyph format to convert to.
* @return 0 if success. non-zero value if failed.
*/
int ituFtLoadFontArray(int index, uint8_t* array, int size, ITUGlyphFormat format);

/**
 * Sets current font to use.
 *
 * @param index the index of loaded font to use.
 */
void ituFtSetCurrentFont(int index);

/**
 * Sets the size of current font.
 *
 * @param width the font width.
 * @param height the font height.
 */
void ituFtSetFontSize(int width, int height);

/**
 * Gets the dimension of text on current font.
 *
 * @param text the text to get dimension.
 * @param width the pointer to retrieved width.
 * @param height the pointer to retrieved height.
 */
void ituFtGetTextDimension(const char* text, int* width, int* height);

/**
 * Draws text to the specified surface.
 *
 * @param surf the surface to draw to.
 * @param x the x coordinate of surface, in pixels.
 * @param y the y coordinate of surface, in pixels.
 * @param text the text to draw.
 * @return 0 if success. non-zero value if failed.
 */
int ituFtDrawText(ITUSurface* surf, int x, int y, const char* text);

/**
 * Gets the width and size of specified character.
 *
 * @param text the text of character.
 * @param width the width of character to get.
 * @return the size of character, in bytes.
 */
int ituFtGetCharWidth(const char* text, int* width);

/**
 * Draws a character to the specified surface.
 *
 * @param surf the surface to draw to.
 * @param x the x coordinate of surface, in pixels.
 * @param y the y coordinate of surface, in pixels.
 * @param text the text to draw.
 * @return 0 if success. non-zero value if failed.
 */
int ituFtDrawChar(ITUSurface* surf, int x, int y, const char* text);

/**
 * Resets font cache to release memory resource.
 */
void ituFtResetCache(void);

/**
* Is the character valid or not.
*
* @param text the text of character.
* @return is the character valid or not.
*/
bool ituFtIsCharValid(const char* text);

// Style definitions
#define ITU_FT_STYLE_DEFAULT	0x0       ///< Default style
#define ITU_FT_STYLE_BOLD 		0x1       ///< Bold style
#define ITU_FT_STYLE_ARABIC		0x2       ///< Arabic style
#define ITU_FT_STYLE_HEBREW		0x4       ///< Hebrew style

/**
* Sets the style of current font.
*
* @param style the font style.
*/
void ituFtSetFontStyle(unsigned int style);

/**
* Sets the style value of current font.
*
* @param style the font style to set.
* @param value the style value.
*/
void ituFtSetFontStyleValue(unsigned int style, int value);

int ituFtFreeDestroyData(void* pData);

/** @} */ // end of itu_freetype

/** @defgroup itu_action Actions
 *  @{
 */

/**
 * Event definition
 */
typedef enum
{
    ITU_EVENT_TIMER                 = 0,    ///< Timer event, should be issued every 33ms in main loop
    ITU_EVENT_PRESS                 = 1,    ///< Press event, equals union of ITU_EVENT_KEYDOWN and ITU_EVENT_MOUSEDOWN
    ITU_EVENT_LAYOUT                = 2,    ///< Notify widgets should relayout itself
    ITU_EVENT_LANGUAGE              = 3,    ///< Notify widgets to change its lanauage
    ITU_EVENT_KEYDOWN               = 4,    ///< Notify the key-down event to all visible widgets
    ITU_EVENT_KEYUP                 = 5,    ///< Notify the key-up event to all visible widgets
    ITU_EVENT_MOUSEDOWN             = 6,    ///< Notify the mouse-down event to all visible widgets
    ITU_EVENT_MOUSEUP               = 7,    ///< Notify the mouse-up event to all visible widgets
    ITU_EVENT_MOUSEDOUBLECLICK      = 8,    ///< Notify the mouse-double-click event to all visible widgets
    ITU_EVENT_MOUSEMOVE             = 9,    ///< Notify the mouse-move event to all visible widgets
    ITU_EVENT_TOUCHSLIDELEFT        = 10,   ///< Notify the slide to left event of touch panel to all visible widgets
    ITU_EVENT_TOUCHSLIDEUP          = 11,   ///< Notify the slide to left event of touch panel to all visible widgets
    ITU_EVENT_TOUCHSLIDERIGHT       = 12,   ///< Notify the slide to left event of touch panel to all visible widgets
    ITU_EVENT_TOUCHSLIDEDOWN        = 13,   ///< Notify the slide to left event of touch panel to all visible widgets
    ITU_EVENT_LOAD                  = 14,   ///< Notify the widget to load data
    ITU_EVENT_RELEASE               = 15,   ///< Notify the widgets to release surface
    ITU_EVENT_ENTER                 = 16,   ///< Notify the layer is entered.
    ITU_EVENT_LEAVE                 = 17,   ///< Notify the layer is leaved.
    ITU_EVENT_SELECT                = 18,   ///< Notify the item of list box is selected.
    ITU_EVENT_DELAY                 = 19,   ///< Notify the delayed event.
    ITU_EVENT_CHANGED               = 20,   ///< Notify the changed event.
    ITU_EVENT_MOUSELONGPRESS        = 21,   ///< Notify the mouse-long-press event to all visible widgets
    ITU_EVENT_SYNC                  = 22,   ///< Notify the sync event to binded widgets
    ITU_EVENT_MOUSEDOWN_OUTSIDE     = 23,   ///< mouse-down event on pressed outside this widget
    ITU_EVENT_MOUSEUP_OUTSIDE       = 24,   ///< mouse-up event on pressed outside this widget
    ITU_EVENT_DELAY0                = 25,   ///< Notify the delayed #0 event.
    ITU_EVENT_DELAY1                = 26,   ///< Notify the delayed #1 event.
    ITU_EVENT_DELAY2                = 27,   ///< Notify the delayed #2 event.
    ITU_EVENT_DELAY3                = 28,   ///< Notify the delayed #3 event.
    ITU_EVENT_DELAY4                = 29,   ///< Notify the delayed #4 event.
    ITU_EVENT_DELAY5                = 30,   ///< Notify the delayed #5 event.
    ITU_EVENT_DELAY6                = 31,   ///< Notify the delayed #6 event.
    ITU_EVENT_DELAY7                = 32,   ///< Notify the delayed #7 event.
    ITU_EVENT_STOPPED               = 33,   ///< Notify the stopped event.
    ITU_EVENT_TOUCHPINCH            = 34,   ///< Notify the pinch event of touch panel to all visible widgets
    ITU_EVENT_LOAD_EXTERNAL         = 35,   ///< Notify the widget to load external data
    ITU_EVENT_LOAD_IMAGE            = 36,   ///< Notify the widget to load external image
    ITU_EVENT_DRAGGING              = 37,   ///< Notify the widget is dragging

    ITU_EVENT_CUSTOM                = 100   ///< Notify the custom event
} ITUEvent;

struct ITUWidgetTag;

/**
 * Action type definition.
 */
typedef enum
{
    ITU_ACTION_NONE,			    ///< Do nothing.
    ITU_ACTION_SHOW,			    ///< Shows the widget. Parameter can be integer of ITUEffectType.
    ITU_ACTION_HIDE,			    ///< Hides the widget. Parameter can be integer of ITUEffectType.
    ITU_ACTION_FOCUS,			    ///< Focuses the widget.
    ITU_ACTION_PREV,			    ///< Goto previous.
    ITU_ACTION_NEXT,			    ///< Goto next.
    ITU_ACTION_BACK,			    ///< Back command.
    ITU_ACTION_PLAY,			    ///< Play command.
    ITU_ACTION_STOP,			    ///< Stop command.
    ITU_ACTION_GOTO,			    ///< Goto indicated page. Need parameter as integer.
    ITU_ACTION_LANGUAGE,		    ///< Change language to indicated language. Need parameter as integer. Empty target to change all the widgets.
    ITU_ACTION_INPUT,			    ///< Input parameter to target widget.
    ITU_ACTION_BIND,			    ///< Bind target widget as input target.
    ITU_ACTION_CLEAR,			    ///< Clear command.
    ITU_ACTION_ENABLE,			    ///< Enable command.
    ITU_ACTION_DISABLE,			    ///< Disable command.
    ITU_ACTION_CHECK,			    ///< Check command.
    ITU_ACTION_UNCHECK,			    ///< Uncheck command.
    ITU_ACTION_FUNCTION,		    ///< Execute action function command.
    ITU_ACTION_RELOAD,			    ///< Reload command.
    ITU_ACTION_DODELAY0,		    ///< Execute delay #0 action commands.
    ITU_ACTION_DODELAY1,		    ///< Execute delay #1 action commands.
    ITU_ACTION_DODELAY2,		    ///< Execute delay #2 action commands.
    ITU_ACTION_DODELAY3,		    ///< Execute delay #3 action commands.
    ITU_ACTION_DODELAY4,		    ///< Execute delay #4 action commands.
    ITU_ACTION_DODELAY5,		    ///< Execute delay #5 action commands.
    ITU_ACTION_DODELAY6,		    ///< Execute delay #6 action commands.
    ITU_ACTION_DODELAY7,		    ///< Execute delay #7 action commands.
    ITU_ACTION_SET0,			    ///< Sets to global variable #0.
    ITU_ACTION_SET1,			    ///< Sets to global variable #1.
    ITU_ACTION_SET2,			    ///< Sets to global variable #2.
    ITU_ACTION_SET3,			    ///< Sets to global variable #3.
    ITU_ACTION_SET4,			    ///< Sets to global variable #4.
    ITU_ACTION_SET5,			    ///< Sets to global variable #5.
    ITU_ACTION_SET6,			    ///< Sets to global variable #6.
    ITU_ACTION_SET7,			    ///< Sets to global variable #7.
	ITU_ACTION_LOAD_EXTERNAL,       ///< Load external data.
	ITU_ACTION_RELEASE_EXTERNAL,    ///< Release external data.
    ITU_ACTION_LOAD_FONT            ///< Load font.
} ITUActionType;

/**
 * Action definition for input type widgets to do pre-defined actions.
 */
typedef struct
{
    ITUActionType action;               ///< Action type.
    ITUEvent ev;                        ///< What event to trigger this action.
    char target[ITU_WIDGET_NAME_SIZE];  ///< Target widget/function name to do the action.
    char param[ITU_ACTION_PARAM_SIZE];  ///< Action parameter.
    void* cachedTarget;                 ///< Cached target widget/function pointer to do the action.
} ITUAction;

/**
 * Queued actions definition to execute.
 */
typedef struct
{
    struct ITUWidgetTag* widget;    ///< Source widget
    ITUAction* action;              ///< Action to execute.
    int delay;                      ///< Delay value.
} ITUActionExecution;

/**
 * Executes the actions.
 *
 * @param widget The widget to do the actions.
 * @param actions The actions to be executed.
 * @param ev Current event.
 * @param arg Current event argument.
 * @return true if there is any action will be executed. false if none.
 */
bool ituExecActions(struct ITUWidgetTag* widget, ITUAction* actions, ITUEvent ev, int arg);

/**
 * Flushes the action queue.
 *
 * @param actionQueue The queued actions to execute.
 * @param queueLen The length of queued actions.
 * @return true if there is any action will be executed. false if none.
 */
bool ituFlushActionQueue(ITUActionExecution actionQueue[ITU_ACTION_QUEUE_SIZE], int* queueLen);

/**
 * Executes the delay queue.
 *
 * @param delayQueue The queued actions to execute.
 */
void ituExecDelayQueue(ITUActionExecution delayQueue[ITU_ACTION_QUEUE_SIZE]);

/**
 * Has the event of action or not.
 *
 * @param widget The widget to get the actions.
 * @param actions The actions to be determined.
 * @param ev The event.
 * @return true if there is any action matched the event. false if none.
 */
bool ituActionHasEvent(struct ITUWidgetTag* widget, ITUAction* actions, ITUEvent ev);

/** @} */ // end of itu_action

/** @defgroup itu_effect Effects
 *  @{
 */
/**
 * Effect type definition.
 */
typedef enum
{
    ITU_EFFECT_NONE                 = 0,    ///< No effect
    ITU_EFFECT_FADE                 = 1,    ///< Fade in/Fade out
    ITU_EFFECT_SCROLL_LEFT          = 2,    ///< Scroll to left side
    ITU_EFFECT_SCROLL_UP            = 3,    ///< Scroll to up side
    ITU_EFFECT_SCROLL_RIGHT         = 4,    ///< Scroll to right side
    ITU_EFFECT_SCROLL_DOWN          = 5,    ///< Scroll to down side
    ITU_EFFECT_SCROLL_LEFT_FADE     = 6,    ///< Scroll to left side with fade
    ITU_EFFECT_SCROLL_UP_FADE       = 7,    ///< Scroll to up side with fade
    ITU_EFFECT_SCROLL_RIGHT_FADE    = 8,    ///< Scroll to right side with fade
    ITU_EFFECT_SCROLL_DOWN_FADE     = 9,    ///< Scroll to down side with fade
    ITU_EFFECT_SCALE                = 10,   ///< Scale
    ITU_EFFECT_SCALE_FADE           = 11,   ///< Scale with fade
    ITU_EFFECT_WIPE_LEFT            = 12,   ///< Wipe to left side
    ITU_EFFECT_WIPE_UP              = 13,   ///< Wipe to up side
    ITU_EFFECT_WIPE_RIGHT           = 14,   ///< Wipe to right side
    ITU_EFFECT_WIPE_DOWN            = 15,   ///< Wipe to down side

    ITU_EFFECT_MAX_COUNT                    ///< Maximum count
} ITUEffectType;

/**
 * Effect definition.
 */
typedef struct ITUEffectTag
{
    int totalStep;  ///< Total steps to do the effect
    int currStep;   ///< Current step on effecting
    bool playing;   ///< Is playing or not
    
    /**
     * Exits the effect.
     *
     * @param effect the effect to exit.
     */
    void (*Exit)(struct ITUEffectTag* effect);

    /**
     * Updates the effect.
     *
     * @param effect the effect to update.
     * @param widget the widget to apply the effect.
     */
    void (*Update)(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

    /**
     * Starts the effect.
     *
     * @param effect the effect to start.
     * @param widget the widget to apply the effect.
     */
    void (*Start)(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

    /**
     * Stops the effect.
     *
     * @param effect the effect to stop.
     * @param widget the widget to apply the effect.
     */
    void (*Stop)(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);
} ITUEffect;

/**
 * Initializes the effect.
 *
 * @param effect the effect to initialize.
 */
void ituEffectInit(ITUEffect* effect);

/**
 * The base implementation of effect updating.
 *
 * @param effect the effect to update.
 * @param widget the widget to apply the effect.
 */
void ituEffectUpdateImpl(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

/**
 * The base implementation of effect starting.
 *
 * @param effect the effect to start.
 * @param widget the widget to apply the effect.
 */
void ituEffectStartImpl(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

/**
 * The base implementation of effect stoping.
 *
 * @param effect the effect to stop.
 * @param widget the widget to apply the effect.
 */
void ituEffectStopImpl(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

// Macros to easily use ITUEffect structure
/**
 * Exits the effect.
 *
 * @param effect the effect to exit.
 */
#define ituEffectExit(effect)                       ((ITUEffect*)(effect))->Exit((ITUEffect*)(effect))

/**
 * Sets Exit callback function of effect.
 *
 * @param effect Pointer referring to the effect.
 * @param ex The callback function to set.
 */
#define ituEffectSetExit(effect, ex)                (((ITUEffect*)(effect))->Exit = (ex))

/**
 * Updates the effect.
 *
 * @param effect the effect to update.
 * @param widget the widget to apply the effect.
 */
#define ituEffectUpdate(effect, widget)             ((ITUEffect*)(effect))->Update((ITUEffect*)(effect), (widget))

/**
 * Sets Update callback function of effect.
 *
 * @param effect Pointer referring to the effect.
 * @param update The callback function to set.
 */
#define ituEffectSetUpdate(effect, update)          (((ITUEffect*)(effect))->Update = (update))

/**
 * Starts the effect.
 *
 * @param effect the effect to start.
 * @param widget the widget to apply the effect.
 */
#define ituEffectStart(effect, widget)              ((ITUEffect*)(effect))->Start((ITUEffect*)(effect), (widget))

/**
 * Sets Start callback function of effect.
 *
 * @param effect Pointer referring to the effect.
 * @param start The callback function to set.
 */
#define ituEffectSetStart(effect, start)            (((ITUEffect*)(effect))->Start = (start))

/**
 * Stops the effect.
 *
 * @param effect the effect to stop.
 * @param widget the widget to apply the effect.
 */
#define ituEffectStop(effect, widget)               ((ITUEffect*)(effect))->Stop((ITUEffect*)(effect), (widget))

/**
 * Sets Stop callback function of effect.
 *
 * @param effect Pointer referring to the effect.
 * @param stop The callback function to set.
 */
#define ituEffectSetStop(effect, stop)              (((ITUEffect*)(effect))->Stop = (stop))

/**
 * Sets total steps of effect.
 *
 * @param effect Pointer referring to the effect.
 * @param step The total steps to set.
 */
#define ituEffectSetTotalStep(effect, step)         (((ITUEffect*)(effect))->totalStep = (step))

/**
 * Is the effect playing or not.
 *
 * @param effect Pointer referring to the effect.
 * @return stop the effect playing or not.
 */
#define ituEffectIsPlaying(effect)                  (((ITUEffect*)(effect))->playing)

/** @defgroup itu_effect_fade Fade Effect
 *  @{
 */
/**
 * Fade in/out effect definition.
 */
typedef struct
{
    ITUEffect effect;   ///< The base effect
    bool fadeIn;        ///< Is on fade in mode or not
    uint8_t orgAlpha;   ///< Keep the orginal alpha value of widget
} ITUFadeEffect;

/**
 * Initializes the fade effect.
 *
 * @param fe the fade effect to initialize.
 * @param fadeIn true for fade in effect, false for fade out effect.
 */
void ituFadeEffectInit(ITUFadeEffect* fe, bool fadeIn);

/** @} */ // end of itu_effect_fade

/** @defgroup itu_effect_scroll Scroll Effect
 *  @{
 */
/**
 * Scroll effect type definition.
 */
typedef enum
{
    ITU_SCROLL_IN_LEFT,     ///< Scroll in to left side
    ITU_SCROLL_IN_UP,       ///< Scroll in to up side
    ITU_SCROLL_IN_RIGHT,    ///< Scroll in to right side
    ITU_SCROLL_IN_DOWN,     ///< Scroll in to down side
    ITU_SCROLL_OUT_LEFT,    ///< Scroll out to left side
    ITU_SCROLL_OUT_UP,      ///< Scroll out to up side
    ITU_SCROLL_OUT_RIGHT,   ///< Scroll out to right side
    ITU_SCROLL_OUT_DOWN     ///< Scroll out to down side
} ITUScrollType;

/**
 * Scroll effect definition.
 */
typedef struct
{
    ITUEffect effect;   ///< Base effect definition
    ITUScrollType type; ///< Scroll type
    int orgX;           ///< Origional x coordinate of widget, in pixels.
    int orgY;           ///< Origional y coordinate of widget, in pixels.
} ITUScrollEffect;

/**
 * Initializes the scroll effect.
 *
 * @param se the scroll effect to initialize.
 * @param type the scroll type.
 */
void ituScrollEffectInit(ITUScrollEffect* se, ITUScrollType type);

/**
 * Updates the scroll effect.
 *
 * @param effect the scroll effect to update.
 * @param widget the widget to apply the effect.
 */
void ituScrollEffectUpdate(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

/**
 * Starts the scroll effect.
 *
 * @param effect the scroll effect to start.
 * @param widget the widget to apply the effect.
 */
void ituScrollEffectStart(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

/**
 * Stops the scroll effect.
 *
 * @param effect the scroll effect to stop.
 * @param widget the widget to apply the effect.
 */
void ituScrollEffectStop(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

/** @} */ // end of itu_effect_scroll

/** @defgroup itu_effect_scrollfade Scroll Fade Effect
 *  @{
 */
/**
 * Scroll fade effect definition.
 */
typedef struct
{
    ITUScrollEffect effect; ///< Base effect definition
    bool fadeIn;            ///< Is on fade in mode or not
    uint8_t orgAlpha;       ///< Keep the orginal alpha value of widget
} ITUScrollFadeEffect;

/**
 * Initializes the scroll fade effect.
 *
 * @param sfe the scroll fade effect to initialize.
 * @param type the scroll type.
 */
void ituScrollFadeEffectInit(ITUScrollFadeEffect* sfe, ITUScrollType type, bool fadeIn);

/** @} */ // end of itu_effect_scrollfade

/** @defgroup itu_effect_scale Scale Effect
 *  @{
 */
/**
 * Scale effect definition.
 */
typedef struct
{
    ITUEffect effect;   ///< Base effect definition
    bool enlarge;       ///< Is on enlarge mode or not
    int orgX;           ///< Origional x coordinate of widget, in pixels.
    int orgY;           ///< Origional y coordinate of widget, in pixels.
    int orgWidth;       ///< Origional width of widget, in pixels.
    int orgHeight;      ///< Origional height of widget, in pixels.    
} ITUScaleEffect;

/**
 * Initializes the scale effect.
 *
 * @param se the scale effect to initialize.
 * @param enlarge enlarge or not.
 */
void ituScaleEffectInit(ITUScaleEffect* se, bool enlarge);

/**
 * Updates the scale effect.
 *
 * @param effect the scale effect to update.
 * @param widget the widget to apply the effect.
 */
void ituScaleEffectUpdate(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

/**
 * Starts the scale effect.
 *
 * @param effect the scale effect to start.
 * @param widget the widget to apply the effect.
 */
void ituScaleEffectStart(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

/**
 * Stops the scale effect.
 *
 * @param effect the scale effect to stop.
 * @param widget the widget to apply the effect.
 */
void ituScaleEffectStop(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

/** @} */ // end of itu_effect_scale

/** @defgroup itu_effect_scalefade Scale Fade Effect
 *  @{
 */
/**
 * Scale fade effect definition.
 */
typedef struct
{
    ITUScaleEffect effect; ///< Base effect definition
    bool fadeIn;            ///< Is on fade in mode or not
    uint8_t orgAlpha;       ///< Keep the orginal alpha value of widget
} ITUScaleFadeEffect;

/**
 * Initializes the scale fade effect.
 *
 * @param sfe the scale fade effect to initialize.
 * @param type the scale type.
 */
void ituScaleFadeEffectInit(ITUScaleFadeEffect* sfe, bool fadeIn);

/** @} */ // end of itu_effect_scalefade

/** @defgroup itu_effect_wipe Wipe Effect
 *  @{
 */
/**
 * Wipe effect type definition.
 */
typedef enum
{
    ITU_WIPE_IN_LEFT,     ///< Wipe in to left side
    ITU_WIPE_IN_UP,       ///< Wipe in to up side
    ITU_WIPE_IN_RIGHT,    ///< Wipe in to right side
    ITU_WIPE_IN_DOWN,     ///< Wipe in to down side
    ITU_WIPE_OUT_LEFT,    ///< Wipe out to left side
    ITU_WIPE_OUT_UP,      ///< Wipe out to up side
    ITU_WIPE_OUT_RIGHT,   ///< Wipe out to right side
    ITU_WIPE_OUT_DOWN     ///< Wipe out to down side
} ITUWipeType;

/**
 * Wipe effect definition.
 */
typedef struct
{
    ITUEffect effect;   ///< Base effect definition
    ITUWipeType type; ///< Wipe type
} ITUWipeEffect;

/**
 * Initializes the wipe effect.
 *
 * @param se the wipe effect to initialize.
 * @param type the wipe type.
 */
void ituWipeEffectInit(ITUWipeEffect* se, ITUWipeType type);

/**
 * Updates the wipe effect.
 *
 * @param effect the wipe effect to update.
 * @param widget the widget to apply the effect.
 */
void ituWipeEffectUpdate(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

/**
 * Starts the wipe effect.
 *
 * @param effect the wipe effect to start.
 * @param widget the widget to apply the effect.
 */
void ituWipeEffectStart(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

/**
 * Stops the wipe effect.
 *
 * @param effect the wipe effect to stop.
 * @param widget the widget to apply the effect.
 */
void ituWipeEffectStop(struct ITUEffectTag* effect, struct ITUWidgetTag* widget);

/** @} */ // end of itu_effect_wipe

/** @} */ // end of itu_effect

/** @defgroup itu_widget Widgets
 *  @{
 */

#define ITU_TAPSTOP             0x1         ///< This widget accepts Tab key to stop.
#define ITU_ENABLED             0x2         ///< This widget is enabled for input.
#define ITU_STRETCH             0x4         ///< This widget will use stretch blt to draw it's image.
#define ITU_RESPONSE_TO_UP_KEY  0x8         ///< This widget responses to up key intead of down key.
#define ITU_PROGRESS            0x10        ///< This widget shows the progress animation.
#define ITU_DRAGGABLE           0x20        ///< This widget is draggable.
#define ITU_DRAGGING            0x40        ///< This widget is dragging.
#define ITU_UNDRAGGING          0x80        ///< This widget is un-dragging.
#define ITU_BOUNCING            0x100       ///< This widget is bouncing.
#define ITU_HAS_LONG_PRESS      0x200       ///< This widget has long-press event to action.
#define ITU_PREDRAW             0x400       ///< Predraw this layer to speedup drawing on the first time.
#define ITU_LOADING             0x800       ///< This widget is loading.
#define ITU_LOADED              0x1000      ///< This widget is loaded.
#define ITU_ALWAYS_VISIBLE      0x2000      ///< This widget is always visible.
#define ITU_EXTERNAL            0x4000      ///< This widget is in external resources.
#define ITU_LONG_PRESSING       0x8000      ///< This widget has long-pressing.
#define ITU_EXTERNAL_IMAGE      0x10000     ///< This widget has external image.
#define ITU_TOUCHABLE           0x20000     ///< This widget support touch input.
#define ITU_CLIP_DISABLED       0x40000     ///< This widget is clip disabled.
#define ITU_DYNAMIC             0x80000     ///< This widget is dynamic allocated.
#define ITU_TRANSFER_ALPHA      0x100000    ///< This widget will transfer self alpha value to children.
#define ITU_FIT_TO_RECT         0x200000    ///< The image will fit to rectangle of this widget.
#define ITU_CUT_BY_RECT         0x400000    ///< The image will cut by rectangle of this widget.
#define ITU_LONG_DRAG           0x800000    ///< The widget has been dragging sufficient distance.
#define ITU_CHILD_LONG_DRAGGING 0x1000000   ///< The child of this widget is under long dragging mode.

/**
 * Widget layout definition
 */
typedef enum
{
    ITU_LAYOUT_DEFAULT          = 0,    ///< Default layout
    ITU_LAYOUT_UP               = 0,    ///< Layout to up-side
    ITU_LAYOUT_VERTICAL         = 0,    ///< Layout to vertical
    ITU_LAYOUT_DOWN             = 1,    ///< Layout to down side
    ITU_LAYOUT_LEFT             = 2,    ///< Layout to left side
    ITU_LAYOUT_TOP_LEFT         = 2,    ///< Layout to top-left side
    ITU_LAYOUT_HORIZONTAL       = 2,    ///< Layout to horizontal
    ITU_LAYOUT_RIGHT            = 3,    ///< Layout to right side
    ITU_LAYOUT_TOP_RIGHT        = 3,    ///< Layout to top-right side
    ITU_LAYOUT_CENTER           = 4,    ///< Layout to center
    ITU_LAYOUT_TOP_CENTER       = 4,    ///< Layout to top-center
    ITU_LAYOUT_MIDDLE_LEFT      = 5,    ///< Layout to middle-left side
    ITU_LAYOUT_MIDDLE_RIGHT     = 6,    ///< Layout to middle-right side
    ITU_LAYOUT_MIDDLE_CENTER    = 7,    ///< Layout to middle-center
    ITU_LAYOUT_BOTTOM_LEFT      = 8,    ///< Layout to bottom-left side
    ITU_LAYOUT_BOTTOM_RIGHT     = 9,    ///< Layout to bottom-right side
    ITU_LAYOUT_BOTTOM_CENTER    = 10    ///< Layout to bottom-center
} ITULayout;

/**
 * Widget state definition
 */
typedef enum
{
    ITU_STATE_NORMAL,   ///< The widget is in normal state
    ITU_STATE_SHOWING,  ///< The widget is showing
    ITU_STATE_HIDING,   ///< The widget is hiding

    ITU_STATE_COUNT     ///< Total state count
} ITUState;

/**
 * Widget type definition
 */
typedef enum
{
    ITU_WIDGET,                 ///< Base widget
    ITU_LAYER,                  ///< Layer
    ITU_ICON,                   ///< Icon
    ITU_BACKGROUND,             ///< Background
    ITU_TEXT,                   ///< Text
    ITU_TEXTBOX,                ///< Text box
    ITU_BORDERWINDOW,           ///< Border window
    ITU_BUTTON,                 ///< Button
    ITU_CHECKBOX,               ///< Check box
    ITU_RADIOBOX,               ///< Radio box
    ITU_SCROLLTEXT,             ///< Scroll text
    ITU_FLOWWINDOW,             ///< Flow window
    ITU_LISTBOX,                ///< List box
    ITU_FILELISTBOX,            ///< File list box
    ITU_SPRITE,                 ///< Sprite
    ITU_PROGRESSBAR,            ///< Progress bar
    ITU_KEYBOARD,               ///< Keyboard
    ITU_SPRITEBUTTON,           ///< Sprite button
    ITU_TRACKBAR,               ///< Track bar
    ITU_DIGITALCLOCK,           ///< Digital Clock
    ITU_ANALOGCLOCK,            ///< Analog Clock
    ITU_CALENDAR,               ///< Calendar
    ITU_ICONLISTBOX,            ///< Icon list box
    ITU_MEDIAFILELISTBOX,       ///< Media file list box
    ITU_CIRCLEPROGRESSBAR,      ///< Progress bar
    ITU_SCROLLBAR,              ///< Scroll bar
    ITU_ANIMATION,              ///< Animation
    ITU_WHEEL,                  ///< Wheel
    ITU_COVERFLOW,              ///< Cover flow
    ITU_POPUPBUTTON,            ///< Popup button
    ITU_SCROLLLISTBOX,          ///< Scroll list box
    ITU_SCROLLMEDIAFILELISTBOX, ///< Scroll media file list box
    ITU_METER,                  ///< Meter
    ITU_SCROLLICONLISTBOX,      ///< Scroll icon list box
    ITU_VIDEO,                  ///< Video
    ITU_COLORPICKER,            ///< Color picker
    ITU_IMAGECOVERFLOW,         ///< Image cover flow
    ITU_BACKGROUNDBUTTON,       ///< Background button
    ITU_RIPPLEBACKGROUND,       ///< Ripple background
    ITU_CURVE,                  ///< Curve
    ITU_TABLELISTBOX,           ///< Table list box
    ITU_POPUPRADIOBOX,          ///< Popup radio box
    ITU_LANGUAGESPRITE,         ///< Language sprite
    ITU_PAGEFLOW,               ///< Page flow
    ITU_SHADOW,                 ///< Shadow
    ITU_CONTAINER,              ///< Container
	ITU_TABLEICONLISTBOX,       ///< Table icon list box
    ITU_AUDIO,                  ///< Audio
    ITU_SLIDESHOW,              ///< Slideshow
    ITU_DRAGICON,               ///< Draggable Icon
    ITU_BLUR,                   ///< Blur
    ITU_SCALECOVERFLOW,         ///< Scale cover flow
    ITU_WHEELBACKGROUND,        ///< Wheel background
    ITU_DRAWPEN,                ///< Draw Pen
    ITU_WAVEBACKGROUND,         ///< Wave background
	ITU_STEPWHEEL,              ///< Step Wheel
    ITU_CLIPPER,                ///< Clipper
    ITU_SIMPLEANIMATION,        ///< Simple animation
	ITU_STOPANYWHERE,           ///< StopAnywhere container
    ITU_OSCILLOSCOPE,           ///< Oscilloscope
	ITU_HWHEEL,                 ///< HWheel
    ITU_TABLEBAR,               ///< Table bar
    ITU_TABLEGRID,              ///< Table grid
    ITU_COMBOTABLE,             ///< Combo Table
    ITU_CUSTOM = 100            ///< Custom widget
} ITUWidgetType;

/**
 * Widget definition
 */
typedef struct ITUWidgetTag
{
    ITCTree tree;                           ///< Tree node
    ITUWidgetType type;                     ///< Widget type
    char name[ITU_WIDGET_NAME_SIZE];        ///< Widget name
    unsigned int flags;                     ///< Flags. Can be ITU_TAPSTOP
    uint8_t visible;                        ///< Is visible or not
    uint8_t active;                         ///< Is focused or not
    uint8_t dirty;                          ///< Is ready to draw or not
    uint8_t alpha;                          ///< The alpha value
    int tabIndex;                           ///< The index of tab to focus
    ITURectangle rect;                      ///< The position and dimension of this widget
	ITURectangle org_rect;                  ///< The orginal position and dimension of this widget
    ITUColor color;                         ///< The color used on different type of widgets
    ITURectangle bound;                     ///< The boundary of this widget for clipping
    ITUState state;                         ///< The state of this widget
	ITCTree* pObj;                          ///< The temporary obj container.
    int angle;                              ///< The rotate angle.
    int showDelay;                          ///< Delay frames to show
    int hideDelay;                          ///< Delay frames to hide; -1 indicates not to hide automatically
    ITUTransformType transformType;         ///< The transform type
    int transformX;                         ///< The transform value on X coordinate
    int transformY;                         ///< The transform value on Y coordinate
    int effectSteps;                        ///< Total effect steps
	
    ITUEffectType effects[ITU_STATE_COUNT]; ///< The effect used on every state
    ITUEffect* effect;                      ///< Current active effect

    /**
     * Exits the widget.
     *
     * @param widget The widget to exit.
     */
    void (*Exit)(struct ITUWidgetTag* widget);

    /**
     * Clones this widget.
     *
     * @param widget The widget to clone.
     * @param cloned Retrieved cloned widget.
     * @return true if clone is success, false otherwise.
     */
    bool (*Clone)(struct ITUWidgetTag* widget, struct ITUWidgetTag** cloned);
    
    /**
     * Updates the widget by specified event.
     *
     * @param widget The widget to update.
     * @param ev The event to notify.
     * @param arg1 The event related argument #1.
     * @param arg2 The event related argument #2.
     * @param arg3 The event related argument #3.
     * @return true if the widget is modified and need to be redraw, false if no need to be redraw.
     */
    bool (*Update)(struct ITUWidgetTag* widget, ITUEvent ev, int arg1, int arg2, int arg3);
    
    /**
     * Draws the widget to the specified surface.
     *
     * @param widget The widget to draw.
     * @param dest The surface to draw to.
     * @param x The x coordinate of destination surface, in pixels.
     * @param y The y coordinate of destination surface, in pixels.
     * @param alpha the alpha value to do the constant alphablending to the surface.
     */
    void (*Draw)(struct ITUWidgetTag* widget, ITUSurface* dest, int x, int y, uint8_t alpha);
    
    /**
     * Do the specified action. This is triggered by other widget's event.
     *
     * @param widget The widget to do the action.
     * @param action The action to do.
     * @param param The parameter of action.
     */
    void (*OnAction)(struct ITUWidgetTag* widget, ITUActionType action, char* param);

    /**
     * Called when this widget is pressed. This is triggered by ITU_EVENT_KEYUP/ITU_EVENT_MOUSEUP/ITU_EVENT_PRESS events.
     *
     * @param widget The pressed widget.
     * @param ev The triggered event.
     * @param arg1 The key or mouse code.
     * @param arg2 The x coordinate of mouse if is pressed by mouse.
     * @param arg3 The y coordinate of mouse if is pressed by mouse.
     */
    bool (*OnPress)(struct ITUWidgetTag* widget, ITUEvent ev, int arg1, int arg2, int arg3);
    
    void* customData;   ///< Custom data for any use.
} ITUWidget;

/**
 * Initializes the widget.
 *
 * @param widget The widget to initialize.
 */
void ituWidgetInit(ITUWidget* widget);

/**
 * Loads the widget. This is called by scene manager.
 *
 * @param widget The widget to load.
 * @param base The address in the scene file buffer.
 */
void ituWidgetLoad(ITUWidget* widget, uint32_t base);

/**
 * Exits the widget. This is the implementation of base widget.
 *
 * @param widget The widget to exit.
 */
void ituWidgetExitImpl(ITUWidget* widget);

/**
 * Clones a widget.
 *
 * @param widget The widget to clone.
 * @param cloned Retrieved cloned widget.
 * @return true if clone is success, false otherwise.
 */
bool ituWidgetCloneImpl(ITUWidget* widget, ITUWidget** cloned);

/**
 * Adds a child widget this this widget. This is the implementation of base widget.
 *
 * @param widget The widget to add to.
 * @param child The child widget to add.
 */
void ituWidgetAddImpl(ITUWidget* widget, ITUWidget* child);

/**
 * Updates the widget by specified event. This is the implementation of base widget.
 *
 * @param widget The widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituWidgetUpdateImpl(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the widget to the specified surface. This is the implementation of base widget.
 *
 * @param widget The widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituWidgetDrawImpl(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event. This is the implementation of base widget.
 *
 * @param widget The widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituWidgetOnActionImpl(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Called when this widget is pressed. This is triggered by ITU_EVENT_KEYUP/ITU_EVENT_MOUSEUP/ITU_EVENT_PRESS events.
 *  This is the implementation of base widget.
 *
 * @param widget The pressed widget.
 * @param ev The triggered event.
 * @param arg1 The key or mouse code.
 * @param arg2 The x coordinate of mouse if is pressed by mouse.
 * @param arg3 The y coordinate of mouse if is pressed by mouse.
 */
bool ituWidgetOnPressImpl(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Sets name of the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param name The name to set.
 */
void ituWidgetSetNameImpl(ITUWidget* widget, const char* name);

/**
 * Sets visibility of the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param visible true for visible, false for invisible.
 */
void ituWidgetSetVisibleImpl(ITUWidget* widget, bool visible);

/**
* Is the specified widget visible or not. This is the implementation of base widget.
*
* @param widget Pointer referring to the widget.
* @return true if the widget visible, false if not.
*/
bool ituWidgetIsVisibleImpl(ITUWidget* widget);

/**
 * Sets focused or unfocused of the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param active true for focused, false for unfocused.
 */
void ituWidgetSetActiveImpl(ITUWidget* widget, bool active);

/**
 * Sets alpha value of the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param alpha The alpha value to set.
 */
void ituWidgetSetAlphaImpl(ITUWidget* widget, uint8_t alpha);

/**
 * Sets color value of the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param alpha The alpha value.
 * @param red The red value.
 * @param green The green value.
 * @param blue The blue value.
 */
void ituWidgetSetColorImpl(ITUWidget* widget, uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);

/**
 * Sets position of the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param x The x coordinate of widget.
 * @param y The y coordinate of widget.
 */
void ituWidgetSetPositionImpl(ITUWidget* widget, int x, int y);

/**
 * Sets x coordinate of the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param x The x coordinate of widget.
 */
void ituWidgetSetXImpl(ITUWidget* widget, int x);

/**
 * Sets y coordinate of the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param y The y coordinate of widget.
 */
void ituWidgetSetYImpl(ITUWidget* widget, int y);

/**
 * Sets width of the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param width The width of widget.
 */
void ituWidgetSetWidthImpl(ITUWidget* widget, int width);

/**
 * Sets height of the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param height The height of widget.
 */
void ituWidgetSetHeightImpl(ITUWidget* widget, int height);

/**
 * Sets width and height of the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param width The width of widget.
 * @param height The height of widget.
 */
void ituWidgetSetDimensionImpl(ITUWidget* widget, int width, int height);

/**
 * Sets boundary of the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param x The x coordinate of boundary.
 * @param y The y coordinate of boundary.
 * @param width The width of boundary.
 * @param height The height of boundary.
 */
void ituWidgetSetBoundImpl(ITUWidget* widget, int x, int y, int width, int height);

/**
 * Is the specified position inside the widget or not. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param x The x coordinate of position.
 * @param y The y coordinate of position.
 * @return true if the position inside the widget, false if not.
 */
bool ituWidgetIsInsideImpl(ITUWidget* widget, int x, int y);

/**
 * Sets the clipping of surface by the boundary of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param dest The surface to set the clipping rectange.
 * @param x The x coordinate of widget on the surface.
 * @param y The y coordinate of widget on the surface.
 * @param prevClip The previous clipping of surface to backup.
 */
void ituWidgetSetClipping(ITUWidget* widget, ITUSurface* dest, int x, int y, ITURectangle* prevClip);

/**
 * Is the widget overlap the clipping or not.
 *
 * @param widget Pointer referring to the widget.
 * @param dest The surface owns the clipping rectange.
 * @param x The x coordinate of position.
 * @param y The y coordinate of position.
 * @return true if the widget overlap with clipping, false if not.
 */
bool ituWidgetIsOverlapClipping(ITUWidget* widget, ITUSurface* dest, int x, int y);

/**
 * Gets global position of widget. This is the implementation of base widget.
 *
 * @param widget the widget to get.
 * @param x Retrieved X coordinate , in pixels.
 * @param y Retrieved Y coordinate, in pixels.
 */
void ituWidgetGetGlobalPositionImpl(ITUWidget* widget, int* x, int* y);

/**
 * Shows the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param effect The effect type.
 * @param step The step count.
 */
void ituWidgetShowImpl(ITUWidget* widget, ITUEffectType effect, int step);

/**
 * Hides the widget. This is the implementation of base widget.
 *
 * @param widget Pointer referring to the widget.
 * @param effect The effect type.
 * @param step The step count.
 */
void ituWidgetHideImpl(ITUWidget* widget, ITUEffectType effect, int step);

/**
* Moves the widget to the first child of parent widget. This is the implementation of base widget.
*
* @param widget Pointer referring to the widget.
*/
void ituWidgetToTopImpl(ITUWidget* widget);

/**
* Moves the widget to the last child of parent widget. This is the implementation of base widget.
*
* @param widget Pointer referring to the widget.
*/
void ituWidgetToBottomImpl(ITUWidget* widget);

/**
* Moves the widget to the last sibling widget. This is the implementation of base widget.
*
* @param widget Pointer referring to the widget.
*/
void ituWidgetMoveUpImpl(ITUWidget* widget);

/**
* Moves the widget to the next sibling widget. This is the implementation of base widget.
*
* @param widget Pointer referring to the widget.
*/
void ituWidgetMoveDownImpl(ITUWidget* widget);

// Macros to easily use ITUWidget structure
/**
 * Exits the widget.
 *
 * @param widget The widget to exit.
 */
#define ituWidgetExit(widget)                               ((ITUWidget*)(widget))->Exit((ITUWidget*)(widget))

/**
 * Sets Exit callback function of widget.
 *
 * @param widget Pointer referring to the widget.
 * @param ex The callback function to set.
 */
#define ituWidgetSetExit(widget, ex)                        (((ITUWidget*)(widget))->Exit = (ex))

/**
 * Clones this widget.
 *
 * @param widget The widget to clone.
 * @param cloned Retrieved cloned widget.
 * @return true if clone is success, false otherwise.
 */
#define ituWidgetClone(widget, cloned)                      ((ITUWidget*)(widget))->Clone((ITUWidget*)(widget), (ITUWidget**)(cloned))

/**
 * Adds a child widget this this widget.
 *
 * @param widget The widget to add to.
 * @param child The child widget to add.
 */
#define ituWidgetAdd(widget, child)                         ituWidgetAddImpl((ITUWidget*)(widget), (ITUWidget*)(child))

/**
 * Sets Clone callback function of widget.
 *
 * @param widget Pointer referring to the widget.
 * @param add The callback function to set.
 */
#define ituWidgetSetClone(widget, clone)                    (((ITUWidget*)(widget))->Clone = (clone))

/**
 * Updates the widget by specified event.
 *
 * @param widget The widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the widget is modified and need to be redraw, false if no need to be redraw.
 */
#define ituWidgetUpdate(widget, ev, arg1, arg2, arg3)       ((ITUWidget*)(widget))->Update((ITUWidget*)(widget), (ev), (arg1), (arg2), (arg3))

/**
 * Sets Update callback function of widget.
 *
 * @param widget Pointer referring to the widget.
 * @param update The callback function to set.
 */
#define ituWidgetSetUpdate(widget, update)                  (((ITUWidget*)(widget))->Update = (update))

/**
 * Draws the widget to the specified surface.
 *
 * @param widget The widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
#define ituWidgetDraw(widget, dest, x, y, alpha)            ((ITUWidget*)(widget))->Draw((ITUWidget*)(widget), (dest), (x), (y), (alpha))

/**
 * Sets Draw callback function of widget.
 *
 * @param widget Pointer referring to the widget.
 * @param draw The callback function to set.
 */
#define ituWidgetSetDraw(widget, draw)                      (((ITUWidget*)(widget))->Draw = (draw))

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
#define ituWidgetOnAction(widget, action, param)            ((ITUWidget*)(widget))->OnAction((ITUWidget*)(widget), (action), (param))

/**
 * Sets OnAction callback function of widget.
 *
 * @param widget Pointer referring to the widget.
 * @param onaction The callback function to set.
 */
#define ituWidgetSetOnAction(widget, onaction)              (((ITUWidget*)(widget))->OnAction = (onaction))

/**
 * Called when this widget is pressed. This is triggered by ITU_EVENT_KEYUP/ITU_EVENT_MOUSEUP/ITU_EVENT_PRESS events.
 *
 * @param widget The pressed widget.
 * @param ev The triggered event.
 * @param arg1 The key or mouse code.
 * @param arg2 The x coordinate of mouse if is pressed by mouse.
 * @param arg3 The y coordinate of mouse if is pressed by mouse.
 */
#define ituWidgetOnPress(widget, ev, arg1, arg2, arg3)      ((ITUWidget*)(widget))->OnPress((ITUWidget*)(widget), (ev), (arg1), (arg2), (arg3))

/**
 * Sets OnPress callback function of widget.
 *
 * @param widget Pointer referring to the widget.
 * @param onpress The callback function to set.
 */
#define ituWidgetSetOnPress(widget, onpress)                (((ITUWidget*)(widget))->OnPress = (onpress))

/**
 * Sets type of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param t The type to set.
 */
#define ituWidgetSetType(widget, t)                         (((ITUWidget*)(widget))->type = (t))

/**
 * Sets name of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param name The name to set.
 */
#define ituWidgetSetName(widget, name)                      ituWidgetSetNameImpl((ITUWidget*)(widget), (name))

/**
 * Sets visibility of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param visible true for visible, false for invisible.
 */
#define ituWidgetSetVisible(widget, visible)                ituWidgetSetVisibleImpl((ITUWidget*)(widget), (visible))

/**
 * Is the widget visible or not.
 *
 * @param widget Pointer referring to the widget.
 * @return true for visible, false for invisible.
 */
#define ituWidgetIsVisible(widget)                          ituWidgetIsVisibleImpl((ITUWidget*)(widget))

/**
 * Sets focused or unfocused of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param active true for focused, false for unfocused.
 */
#define ituWidgetSetActive(widget, active)                  ituWidgetSetActiveImpl((ITUWidget*)(widget), (active))

/**
 * Is the widget focused or not.
 *
 * @param widget Pointer referring to the widget.
 * @return true for focused, false for unfocused.
 */
#define ituWidgetIsActive(widget)                           (((ITUWidget*)(widget))->active)

/**
 * Sets the widget to need to be redraw.
 *
 * @param widget Pointer referring to the widget.
 * @param d true for need to be redraw, false for don't need to be redraw.
 */
#define ituWidgetSetDirty(widget, d)                        (((ITUWidget*)(widget))->dirty = (d))

/**
 * Sets alpha value of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param alpha The alpha value to set.
 */
#define ituWidgetSetAlpha(widget, alpha)                    ituWidgetSetAlphaImpl((ITUWidget*)(widget), (alpha))

/**
 * Sets color value of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param alpha The alpha value.
 * @param red The red value.
 * @param green The green value.
 * @param blue The blue value.
 */
#define ituWidgetSetColor(widget, alpha, red, green, blue)  ituWidgetSetColorImpl((ITUWidget*)(widget), (alpha), (red), (green), (blue))

/**
 * Sets position of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param x The x coordinate of widget.
 * @param y The y coordinate of widget.
 */
#define ituWidgetSetPosition(widget, x, y)                  ituWidgetSetPositionImpl((ITUWidget*)(widget), (x), (y))

/**
 * Sets x coordinate of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param x The x coordinate of widget.
 */
#define ituWidgetSetX(widget, x)                            ituWidgetSetXImpl((ITUWidget*)(widget), (x))

/**
 * Sets y coordinate of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param y The y coordinate of widget.
 */
#define ituWidgetSetY(widget, y)                            ituWidgetSetYImpl((ITUWidget*)(widget), (y))

/**
 * Gets x coordinate of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @return The x coordinate of widget.
 */
#define ituWidgetGetX(widget)                               (((ITUWidget*)(widget))->rect.x)

/**
 * Gets y coordinate of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @return The y coordinate of widget.
 */
#define ituWidgetGetY(widget)                               (((ITUWidget*)(widget))->rect.y)

/**
 * Sets width of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param width The width of widget.
 */
#define ituWidgetSetWidth(widget, width)                    ituWidgetSetWidthImpl((ITUWidget*)(widget), (width))

/**
 * Sets height of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param height The height of widget.
 */
#define ituWidgetSetHeight(widget, height)                  ituWidgetSetHeightImpl((ITUWidget*)(widget), (height))

/**
 * Sets width and height of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param width The width of widget.
 * @param height The height of widget.
 */
#define ituWidgetSetDimension(widget, width, height)        ituWidgetSetDimensionImpl((ITUWidget*)(widget), (width), (height))

/**
 * Gets width of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @return The width of widget.
 */
#define ituWidgetGetWidth(widget)                           (((ITUWidget*)(widget))->rect.width)

/**
 * Gets height of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @return The height of widget.
 */
#define ituWidgetGetHeight(widget)                          (((ITUWidget*)(widget))->rect.height)

/**
 * Sets boundary of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param x The x coordinate of boundary.
 * @param y The y coordinate of boundary.
 * @param width The width of boundary.
 * @param height The height of boundary.
 */
#define ituWidgetSetBound(widget, x, y, width, height)      ituWidgetSetBoundImpl((ITUWidget*)(widget), (x), (y), (width), (height))

/**
 * Is the specified position inside the widget or not.
 *
 * @param widget Pointer referring to the widget.
 * @param x The x coordinate of position.
 * @param y The y coordinate of position.
 * @return true if the position inside the widget, false if not.
 */
#define ituWidgetIsInside(widget, x, y)                     ituWidgetIsInsideImpl((ITUWidget*)(widget), (x), (y))

/**
 * Is the effect playing or not.
 *
 * @param widget Pointer referring to the widget.
 * @return true if the effect is playing, false if not.
 */
#define ituWidgetIsEffecting(widget)                        (((ITUWidget*)(widget))->effect != NULL)

/**
 * Sets effect to the specified of state of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param state The state of widget to play the effect.
 * @param effect The effect to set.
 */
#define ituWidgetSetEffect(widget, state, effect)           (((ITUWidget*)(widget))->effects[(state)] = (effect))

/**
 * Gets the custom data of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @return The custom data.
 */
#define ituWidgetGetCustomData(widget)                      (((ITUWidget*)(widget))->customData)

/**
 * Sets the custom data of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param data The custom data.
 */
#define ituWidgetSetCustomData(widget, data)                (((ITUWidget*)(widget))->customData = (void*)(data))

/**
 * Gets the parent widget of the widget.
 *
 * @param widget Pointer referring to the widget.
 * @return The parent widget. NULL if not exist.
 */
#define ituWidgetGetParent(widget)                          ((ITUWidget*)(((ITUWidget*)(widget))->tree.parent))

/**
 * Gets global position of widget.
 *
 * @param widget the widget to get.
 * @param x Retrieved X coordinate , in pixels.
 * @param y Retrieved Y coordinate, in pixels.
 */
#define ituWidgetGetGlobalPosition(widget, x, y)            ituWidgetGetGlobalPositionImpl((ITUWidget*)(widget), (x), (y))

/**
 * Enables the input of the widget.
 *
 * @param widget Pointer referring to the widget.
 */
#define ituWidgetEnable(widget)                             (((ITUWidget*)(widget))->flags |= ITU_ENABLED)

/**
 * Disables the input of the widget.
 *
 * @param widget Pointer referring to the widget.
 */
#define ituWidgetDisable(widget)                             (((ITUWidget*)(widget))->flags &= ~ITU_ENABLED)

/**
 * Is the widget enabled or not.
 *
 * @param widget Pointer referring to the widget.
 */
#define ituWidgetIsEnabled(widget)                           ((((ITUWidget*)(widget))->flags) & ITU_ENABLED)

/**
 * Shows the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param effect The effect type.
 * @param step The step count.
 */
#define ituWidgetShow(widget, effect, step)                 ituWidgetShowImpl((ITUWidget*)(widget), (effect), (step))

/**
 * Hides the widget.
 *
 * @param widget Pointer referring to the widget.
 * @param effect The effect type.
 * @param step The step count.
 */
#define ituWidgetHide(widget, effect, step)                 ituWidgetHideImpl((ITUWidget*)(widget), (effect), (step))

/**
* Moves the widget to the first child of parent widget.
*
* @param widget The widget to move.
*/
#define ituWidgetToTop(widget)                              ituWidgetToTopImpl((ITUWidget*)(widget))

/**
* Moves the widget to the last child of parent widget.
*
* @param widget The widget to move.
*/
#define ituWidgetToBottom(widget)                           ituWidgetToBottomImpl((ITUWidget*)(widget))

/**
* Moves the widget to the last sibling widget.
*
* @param widget The widget to move.
*/
#define ituWidgetMoveUp(widget)                              ituWidgetMoveUpImpl((ITUWidget*)(widget))

/**
* Moves the widget to the next sibling widget.
*
* @param widget The widget to move.
*/
#define ituWidgetMoveDown(widget)                           ituWidgetMoveDownImpl((ITUWidget*)(widget))

/** @defgroup itu_widget_layer Layer
 *  @{
 */

#define ITU_LAYER_IS_HIDED 0x2 ///< Layer is hided.

/**
 * Layer widget definition. This is used for layer container.
 */
typedef struct ITULayerTag
{
    ITUWidget widget;                                   ///< Base widget definition.
    int hideDelay;                                      ///< Delay frames to hide
    uint8_t* buffer;                                    ///< Loaded file buffer
	int layerID;                                        ///< The id mark of layer.(internal usage)
    ITUAction actions[ITU_ACTIONS_SIZE];                ///< Actions for events to trigger
} ITULayer;

/**
 * Initializes the layer widget.
 *
 * @param layer The layer widget to initialize.
 */
void ituLayerInit(ITULayer* layer);

/**
 * Loads the layer widget. This is called by scene manager.
 *
 * @param layer The layer widget to load.
 * @param base The address in the scene file buffer.
 */
void ituLayerLoad(ITULayer* layer, uint32_t base);

/**
 * Exits the layer widget.
 *
 * @param widget The layer widget to exit.
 */
void ituLayerExit(ITUWidget* widget);

/**
 * Clones the layer widget.
 *
 * @param widget The layer widget to clone.
 * @param cloned Retrieved cloned layer widget.
 * @return true if clone is success, false otherwise.
 */
bool ituLayerClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the layer widget by specified event.
 *
 * @param widget The layer widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the layer widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituLayerUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Draws the layer widget to the specified surface.
*
* @param widget The layer widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituLayerDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The layer widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituLayerOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Goto the specified layer. It will set the specified layer visible and set other layers invisible.
 *
 * @param layer the layer to goto.
 */
void ituLayerGoto(ITULayer* layer);

/**
 * Enables all other layers.
 *
 * @param layer the layer not to enable.
 */
void ituLayerEnableOthers(ITULayer* layer);

/**
 * Disables all other layers.
 *
 * @param layer the layer not to disable.
 */
void ituLayerDisableOthers(ITULayer* layer);

/**
* Loads external module.
*
* @param layer the layer to load external module.
*/
void ituLayerLoadExternal(ITULayer* layer);

/**
* Releases external module.
*
* @param layer the layer to release external module.
*/
void ituLayerReleaseExternal(ITULayer* layer);

/**
* Loads font.
*
* @param layer the layer to load font.
*/
void ituLayerLoadFont(ITULayer* layer);

/**
 * Loads external surface.
 *
 * @param layer the layer to load.
 * @param offset the surface offset.
 */
ITUSurface* ituLayerLoadExternalSurface(ITULayer* layer, uint32_t offset);

/**
* Releases external surface.
*
* @param layer the layer to release.
*/
void ituLayerReleaseExternalSurface(ITULayer* layer);

/** @} */ // end of itu_widget_layer

/** @defgroup itu_widget_icon Icon
 *  @{
 */
/**
 * Icon widget definition. This is used for drawing bitmap icon.
 */

typedef struct ITUIconTag
{
    ITUWidget widget;       ///< Base widget definition.
    ITUSurface* staticSurf; ///< The static surface.
    ITUSurface* surf;       ///< The icon surface.
    ITUSurface* loadedSurf; ///< The loaded icon surface.
    char* filePath;         ///< Path to load image file.
	int use_rotate_mask;    ///< Use rotate mask or not.
	int start_rm_angle;     ///< The start angle for rotate mask.
	int end_rm_angle;       ///< The end angle for rotate mask.
} ITUIcon;

/**
 * Initializes the icon widget.
 *
 * @param icon The icon widget to initialize.
 */
void ituIconInit(ITUIcon* icon);

/**
 * Loads the icon widget. This is called by scene manager.
 *
 * @param icon The icon widget to load.
 * @param base The address in the scene file buffer.
 */
void ituIconLoad(ITUIcon* icon, uint32_t base);

/**
 * Exits the icon widget.
 *
 * @param widget The icon widget to exit.
 */
void ituIconExit(ITUWidget* widget);

/**
 * Clones the icon widget.
 *
 * @param widget The icon widget to clone.
 * @param cloned Retrieved cloned icon widget.
 * @return true if clone is success, false otherwise.
 */
bool ituIconClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the icon widget by specified event.
 *
 * @param widget The icon widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the icon widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituIconUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the icon widget to the specified surface.
 *
 * @param widget The icon widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituIconDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Loads JPEG data to the icon widget.
 *
 * @param icon The icon widget to load.
 * @param data The JPEG data.
 * @param size The JPEG data size.
 * @return true if loaded success, false otherwise.
 */
bool ituIconLoadJpegData(ITUIcon* icon, uint8_t* data, int size);

/**
 * Loads JPEG file to the icon widget.
 *
 * @param icon The icon widget to load.
 * @param filepath The JPEG file path.
 */
void ituIconLoadJpegFile(ITUIcon* icon, char* filepath);

/**
 * Loads JPEG file to the icon widget synchronously.
 *
 * @param icon The icon widget to load.
 * @param filepath The JPEG file path.
 */
void ituIconLoadJpegFileSync(ITUIcon* icon, char* filepath);

/**
 * Loads PNG data to the icon widget.
 *
 * @param icon The icon widget to load.
 * @param data The PNG data.
 * @param size The PNG data size.
 * @return true if loaded success, false otherwise.
 */
bool ituIconLoadPngData(ITUIcon* icon, uint8_t* data, int size);

/**
 * Loads PNG file to the icon widget.
 *
 * @param icon The icon widget to load.
 * @param filepath The PNG file path.
 */
void ituIconLoadPngFile(ITUIcon* icon, char* filepath);

/**
 * Loads PNG file to the icon widget synchronously.
 *
 * @param icon The icon widget to load.
 * @param filepath The PNG file path.
 */
void ituIconLoadPngFileSync(ITUIcon* icon, char* filepath);

/**
 * Loads static data to the icon widget.
 *
 * @param icon The icon widget to load.
 */
void ituIconLoadStaticData(ITUIcon* icon);

/**
 * Releases surface of icon widget.
 *
 * @param icon The icon widget to release.
 */
void ituIconReleaseSurface(ITUIcon* icon);

/**
 * Links surface of icon widget.
 *
 * @param icon The icon widget to link to.
 * @param src The source icon widget to be linked.
 */
void ituIconLinkSurface(ITUIcon* icon, ITUIcon* src);

/**
* Set rotate mask for the icon widget.
*
* @param icon The icon widget to set rotate mask.
* @param enable Set the rotate mask enable or not.
*/
void ituIconRmSetEnable(ITUIcon* icon, bool enable);

/**
* Get status of the rotate mask of the icon widget.
*
* @param icon The icon widget to get status.
* @return true if rotate mask is active now, false otherwise.
*/
bool ituIconRmIsEnable(ITUIcon* icon);

/**
* Set angle range for the rotate mask of the icon widget.
*
* @param icon The icon widget to set.
* @param start_angle Set the start angle of rotate mask.
* @param end_angle Set the end angle of rotate mask.
* @return true if setting success, false otherwise.
*/
bool ituIconRmSetAngle(ITUIcon* icon, int start_angle, int end_angle);

/** @} */ // end of itu_widget_icon

/** @defgroup itu_widget_background Background
 *  @{
 */
/**
 * Background widget definition. This is used for drawing background with bitmap or pure color.
 */
typedef struct
{
    ITUIcon icon;                   ///< Base icon widget definition.
    ITUGradientMode graidentMode;   ///< Gradient mode
    ITUColor graidentColor;         ///< Gradient end color
    int orgWidth;                   ///< Original width for scale animation
    int orgHeight;                  ///< Original height for scale animation
} ITUBackground;

/**
 * Initializes the background widget.
 *
 * @param bg The background widget to initialize.
 */
void ituBackgroundInit(ITUBackground* bg);

/**
 * Loads the background widget. This is called by scene manager.
 *
 * @param bg The background widget to load.
 * @param base The address in the scene file buffer.
 */
void ituBackgroundLoad(ITUBackground* bg, uint32_t base);

/**
 * Clones the background widget.
 *
 * @param widget The background widget to clone.
 * @param cloned Retrieved cloned background widget.
 * @return true if clone is success, false otherwise.
 */
bool ituBackgroundClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Draws the background widget to the specified surface.
 *
 * @param widget The background widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituBackgroundDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/** @} */ // end of itu_widget_background

/**
 * String set for multi-language widgets like Text widgets.
 */
typedef struct
{
    int count;                          ///< Total count of strings
    int totalSize;                      ///< Total size of this string set
    char* strings[ITU_STRINGSET_SIZE];  ///< String array
} ITUStringSet;

/**
 * Loads the string set.
 *
 * @param stringSet The string set to load.
 * @param base The address in the scene file buffer.
 */
void ituStringSetLoad(ITUStringSet* stringSet, uint32_t base);

/** @defgroup itu_widget_text Text
 *  @{
 */
#define ITU_TEXT_BOLD    0x1     ///< Bold type.
#define ITU_TEXT_ARABIC  0x2     ///< Arabic text enabled.
#define ITU_TEXT_HEBREW  0x4     ///< Hebrew text enabled.

/**
 * Text widget definition. This is used for drawing label.
 */
typedef struct
{
    ITUWidget widget;               ///< Base widget definition.
    ITUColor bgColor;               ///< Background color.
    int fontHeight;                 ///< Font height.
	int fontWidth;                  ///< Font width.
	int org_fontHeight;             ///< Orginal font height.
	int org_fontWidth;              ///< Orginal font width.
    ITULayout layout;               ///< Text layout.
    char* string;                   ///< String of the text.
    int lang;                       ///< Current language.
    int fontIndex;                  ///< Font index.
	unsigned int textFlags;         ///< Flags for the text type.
	int boldSize;                   ///< Bold size. Unit is pixel.
    unsigned int arabicIndices;     ///< Arabic text indices.
    unsigned int hebrewIndices;     ///< Hebrew text indices.
    int letterSpacing;              ///< Letter spacing
    ITUStringSet* stringSet;        ///< The string set definition.
} ITUText;

/**
 * Initializes the text widget.
 *
 * @param text The text widget to initialize.
 */
void ituTextInit(ITUText* text);

/**
 * Loads the text widget. This is called by scene manager.
 *
 * @param text The text widget to load.
 * @param base The address in the scene file buffer.
 */
void ituTextLoad(ITUText* text, uint32_t base);

/**
 * Clones the text widget.
 *
 * @param widget The text widget to clone.
 * @param cloned Retrieved cloned text widget.
 * @return true if clone is success, false otherwise.
 */
bool ituTextClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the text widget by specified event.
 *
 * @param widget The text widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the text widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituTextUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the text widget to the specified surface.
 *
 * @param widget The text widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituTextDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The text widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituTextOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Sets font width of the text widget.
 *
 * @param text Pointer referring to the text widget.
 * @param width The font width to set.
 */
void ituTextSetFontWidth(ITUText* text, int width);

/**
 * Sets font height of the text widget.
 *
 * @param text Pointer referring to the text widget.
 * @param height The font height to set.
 */
void ituTextSetFontHeight(ITUText* text, int height);

/**
 * Sets font size of the text widget.
 *
 * @param text Pointer referring to the text widget.
 * @param size The font size to set.
 */
void ituTextSetFontSize(ITUText* text, int size);

/**
 * Sets language of the text widget.
 *
 * @param text Pointer referring to the text widget.
 * @param lang The language to set.
 */
void ituTextSetLanguage(ITUText* text, int lang);

/**
 * Sets string of the text widget. This is the implementation of text widget.
 *
 * @param text Pointer referring to the text widget.
 * @param string The string to set.
 */
void ituTextSetStringImpl(ITUText* text, char* string);

/**
 * Gets string of the text widget. This is the implementation of text widget.
 *
 * @param text Pointer referring to the text widget.
 * @return The string.
 */
char* ituTextGetStringImpl(ITUText* text);

/**
 * Sets background color of the text widget.
 *
 * @param text Pointer referring to the text widget.
 * @param alpha The alpha value.
 * @param red The red value.
 * @param green The green value.
 * @param blue The blue value.
 */
void ituTextSetBackColor(ITUText* text, uint8_t alpha, uint8_t red, uint8_t green, uint8_t blue);

/**
 * Resizes the text widget to fit the length of text.
 *
 * @param text Pointer referring to the text widget.
 */
void ituTextResize(ITUText* text);

/**
 * Sets string of the text widget.
 *
 * @param text Pointer referring to the text widget.
 * @param string The string to set.
 */
#define ituTextSetString(text, string)  ituTextSetStringImpl((ITUText*)(text), (char*)(string))

/**
 * Gets string of the text widget.
 *
 * @param text Pointer referring to the text widget.
 * @return The string.
 */
#define ituTextGetString(text)  ituTextGetStringImpl((ITUText*)(text))

/** @} */ // end of itu_widget_text

/** @defgroup itu_widget_textbox TextBox
 *  @{
 */
#define ITU_TEXTBOX_PASSWORD    0x1     ///< This is a password text box.
#define ITU_TEXTBOX_CURSOR      0x2     ///< Shows cursor on text box.
#define ITU_TEXTBOX_UPPER       0x4     ///< Auto convert input character to to upper character.
#define ITU_TEXTBOX_LOWER       0x8     ///< Auto convert input character to to lower character.
#define ITU_TEXTBOX_MULTILINE   0x10    ///< This is a multi-line text box.
#define ITU_TEXTBOX_WORDWRAP    0x20    ///< Word wrap on a multi-line text box.

/**
 * Text box widget definition. This is used for input text from keyboard.
 */
typedef struct
{
    ITUText text;                           ///< Base text widget definition.
    int maxLen;                             ///< Maximum characters to input.
    unsigned int textboxFlags;              ///< Flags for the text box type.
    ITUColor fgColor;                       ///< Inputted Text color
    ITUColor defColor;                      ///< Hint Text color
    ITUColor focusColor;                    ///< Focused border color
    int lineHeight;                         ///< Line height on multi-line text box.
    int cursorIndex;                        ///< Current cursor position on string.
    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger
} ITUTextBox;

/**
 * Initializes the text box widget.
 *
 * @param textbox The text box widget to initialize.
 */
void ituTextBoxInit(ITUTextBox* textbox);

/**
 * Loads the text box widget. This is called by scene manager.
 *
 * @param textbox The text box widget to load.
 * @param base The address in the scene file buffer.
 */
void ituTextBoxLoad(ITUTextBox* textbox, uint32_t base);

/**
 * Clones the text box widget.
 *
 * @param widget The text box widget to clone.
 * @param cloned Retrieved cloned text box widget.
 * @return true if clone is success, false otherwise.
 */
bool ituTextBoxClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the text box widget by specified event.
 *
 * @param widget The text box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the text box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituTextBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the text box widget to the specified surface.
 *
 * @param widget The text box widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituTextBoxDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The text box widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituTextBoxOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Sets string of the text box widget.
 *
 * @param textbox Pointer referring to the text box widget.
 * @param string The string to set.
 */
void ituTextBoxSetString(ITUTextBox* textbox, char* string);

/**
 * Inputs characters to the text box.
 *
 * @param textbox The text box widget to input.
 * @param input The characters to input.
 */
void ituTextBoxInput(ITUTextBox* textbox, char* input);

/**
 * Backs a character of the text box.
 *
 * @param textbox The text box widget to back.
 */
void ituTextBoxBack(ITUTextBox* textbox);

/**
 * Gets string of the text box widget.
 *
 * @param textbox Pointer referring to the text box widget.
 * @return The string.
 */
char* ituTextBoxGetString(ITUTextBox* textbox);

/** @} */ // end of itu_widget_textbox

/** @defgroup itu_widget_borderwindow BorderWindow
 *  @{
 */
/**
 * Border window definition. This is used for layout children widgets to the border of parent widget.
 */
typedef struct
{
    ITUWidget widget;       ///< Base widget definition.
    ITUWidget* widgets[5];  ///< Children widgets for up/down/left/right/center sides.
    int borderSize;         ///< Border size;
} ITUBorderWindow;

/**
 * Initializes the border window.
 *
 * @param bwin The border window to initialize.
 */
void ituBorderWindowInit(ITUBorderWindow* bwin);

/**
 * Loads the border window widget. This is called by scene manager.
 *
 * @param bwin The border window to load.
 * @param base The address in the scene file buffer.
 */
void ituBorderWindowLoad(ITUBorderWindow* bwin, uint32_t base);

/**
 * Clones the border window widget.
 *
 * @param widget The border window widget to clone.
 * @param cloned Retrieved cloned border window widget.
 * @return true if clone is success, false otherwise.
 */
bool ituBorderWindowClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the border window by specified event.
 *
 * @param widget The border window to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the border window is modified and need to be redraw, false if no need to be redraw.
 */
bool ituBorderWindowUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the border window to the specified surface.
 *
 * @param widget The border window to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituBorderWindowDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Adds a child widghet this border window.
 *
 * @param bwin The border window to add to.
 * @param child The child widget to add.
 * @param layout The layout to place child widget.
 */
void ituBorderWindowAdd(ITUBorderWindow* bwin, ITUWidget* child, ITULayout layout);

/** @} */ // end of itu_widget_borderwindow

/** @defgroup itu_widget_button Button
 *  @{
 */
/**
 * Button widget definition
 */
typedef struct
{
    ITUBackground bg;                       ///< Base background widget definition
    ITUBorderWindow bwin;                   ///< Border window to layout text
    ITUText text;                           ///< Text of button
    ITUColor bgColor;                       ///< Background color
    ITUColor focusColor;                    ///< Focused button color
    ITUSurface* staticFocusSurf;            ///< The static focused button surface.
    ITUSurface* focusSurf;                  ///< Focused bitmap surface
    ITUColor pressColor;                    ///< Pressed background color
    ITUSurface* staticPressSurf;            ///< The static pressed button surface.
    ITUSurface* pressSurf;                  ///< Pressed bitmap surface
    int pressed;                            ///< Is pressed or not
    int mouseLongPressDelay;                ///< The delay on issuing mouse long-press event. Unit is depend on ITU_EVENT_TIMER event.
    int mouseLongPressDelayCount;           ///< The delay count to issue mouse long-press event.
    uint32_t mouseUpDelay;                  ///< The delay on issuing mouse-up event. Unit is ms.
    uint32_t mouseUpDelayCount;             ///< The delay count to issue mouse-up event.
	int imagepos;                           ///< The imagepos for image alignment.
    int fsg1;                               ///< Special flag usage
	int fsg2;                               ///< Special flag usage
	int fsg3;                               ///< Special flag usage
	int press_alpha_delay_count;            ///< The count for press alpha delay.(internal usage)
    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger
} ITUButton;

/**
 * Initializes the button widget.
 *
 * @param btn The button widget to initialize.
 */
void ituButtonInit(ITUButton* btn);

/**
 * Loads the button widget. This is called by scene manager.
 *
 * @param btn The button widget to load.
 * @param base The address in the scene file buffer.
 */
void ituButtonLoad(ITUButton* btn, uint32_t base);

/**
 * Exits the button widget.
 *
 * @param widget The button widget to exit.
 */
void ituButtonExit(ITUWidget* widget);

/**
 * Clones the button widget.
 *
 * @param widget The button widget to clone.
 * @param cloned Retrieved cloned button widget.
 * @return true if clone is success, false otherwise.
 */
bool ituButtonClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the button widget by specified event.
 *
 * @param widget The button widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the button widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituButtonUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the button widget to the specified surface.
 *
 * @param widget The button widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituButtonDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The button widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituButtonOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Sets the button pressed or not.
 *
 * @param btn The button to set.
 * @param pressed Is pressed or not.
 */
void ituButtonSetPressed(ITUButton* btn, bool pressed);

/**
 * Is the button pressed or not.
 *
 * @param btn The button.
 * @return Is pressed or not.
 */
#define ituButtonIsPressed(btn) (((ITUButton*)(btn))->pressed)

/**
 * Sets string of the button widget. This is the implementation of button widget.
 *
 * @param btn Pointer referring to the button widget.
 * @param string The string to set.
 */
void ituButtonSetStringImpl(ITUButton* btn, char* string);

/**
 * Sets string of the button widget.
 *
 * @param btn Pointer referring to the button widget.
 * @param string The string to set.
 */
#define ituButtonSetString(btn, string)  ituButtonSetStringImpl((ITUButton*)(btn), (char*)(string))

/**
 * Loads static data to the button widget.
 *
 * @param btn The button widget to load.
 */
void ituButtonLoadStaticData(ITUButton* btn);

/**
 * Releases surface of button widget.
 *
 * @param btn The button widget to release.
 */
void ituButtonReleaseSurface(ITUButton* btn);

/**
* Sets text layout of the button widget. This is the implementation of button widget.
*
* @param btn Pointer referring to the button widget.
* @param layout The text layout to set.
*/
void ituButtonSetTextLayoutImpl(ITUButton* btn, ITULayout layout);

/**
* Sets the button widget to be checkbox status or not
*
* @param btn Pointer referring to the button widget.
* @param ckstat The checkbox status.
*/
//void ituButtonSetCheckStat(ITUButton* btn, bool ckstat);

/**
* Sets text layout of the button widget.
*
* @param btn Pointer referring to the button widget.
* @param layout The text layout to set.
*/
#define ituButtonSetTextLayout(btn, layout)  ituButtonSetTextLayoutImpl((ITUButton*)(btn), (layout))

/**
* Sets the button to receive mouseup action after slide action.
*
* @param btn The button to set.
*/
void ituButtonSetSlideMouseUP(ITUButton* btn);

/** @} */ // end of itu_widget_button

/** @defgroup itu_widget_checkbox CheckBox
 *  @{
 */
/**
 * Check box widget definition
 */
typedef struct
{
    ITUButton btn;                      ///< Base button widget definition
    ITUColor checkedColor;              ///< Checked background color
    ITUColor checkedFontColor;          ///< Checked font color
    ITUColor orgFontColor;              ///< Orginal font color
    ITUSurface* staticCheckedSurf;      ///< Static checked bitmap surface
    ITUSurface* checkedSurf;            ///< Checked bitmap surface
    int checked;                        ///< Is checked or not
    ITUSurface* staticFocusCheckedSurf; ///< Static focused and checked bitmap surface
    ITUSurface* focusCheckedSurf;       ///< Focused and checked bitmap surface
} ITUCheckBox;

/**
 * Initializes the check box widget.
 *
 * @param checkbox The check box widget to initialize.
 */
void ituCheckBoxInit(ITUCheckBox* checkbox);

/**
 * Loads the check box widget. This is called by scene manager.
 *
 * @param checkbox The check box widget to load.
 * @param base The address in the scene file buffer.
 */
void ituCheckBoxLoad(ITUCheckBox* checkbox, uint32_t base);

/**
 * Exits the check box widget.
 *
 * @param widget The check box widget to exit.
 */
void ituCheckBoxExit(ITUWidget* widget);

/**
 * Clones the check box widget.
 *
 * @param widget The check box widget to clone.
 * @param cloned Retrieved cloned check box widget.
 * @return true if clone is success, false otherwise.
 */
bool ituCheckBoxClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the check box widget by specified event.
 *
 * @param widget The check box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the check box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituCheckBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the check box widget to the specified surface.
 *
 * @param widget The check box widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituCheckBoxDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The check box widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituCheckBoxOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Sets the check box widget checked or not.
 *
 * @param checkbox The check box widget to set.
 * @param checked true for checked or false for un-checked.
 */
void ituCheckBoxSetChecked(ITUCheckBox* checkbox, bool checked);

/**
 * Is the check box widget checked or not.
 *
 * @param checkbox The check box widget.
 * @return Is checked or not.
 */
bool ituCheckBoxIsChecked(ITUCheckBox* checkbox);

/**
 * Loads static data to the check box widget.
 *
 * @param checkbox The check box widget to load.
 */
void ituCheckBoxLoadStaticData(ITUCheckBox* checkbox);

/**
 * Releases surface of check box widget.
 *
 * @param checkbox The check box widget to release.
 */
void ituCheckBoxReleaseSurface(ITUCheckBox* checkbox);

/** @} */ // end of itu_widget_checkbox

/** @defgroup itu_widget_radiobox RadioBox
 *  @{
 */
/**
 * Radio box widget definition
 */
typedef struct
{
    ITUCheckBox checkbox;   ///< Base check box widget definition
} ITURadioBox;

/**
 * Initializes the radio box widget.
 *
 * @param radiobox The radio box widget to initialize.
 */
void ituRadioBoxInit(ITURadioBox* radiobox);

/**
 * Loads the radio box widget. This is called by scene manager.
 *
 * @param radiobox The radio box widget to load.
 * @param base The address in the scene file buffer.
 */
void ituRadioBoxLoad(ITURadioBox* radiobox, uint32_t base);

/**
 * Clones the radio box widget.
 *
 * @param widget The radio box widget to clone.
 * @param cloned Retrieved cloned radio box widget.
 * @return true if clone is success, false otherwise.
 */
bool ituRadioBoxClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the radio box widget by specified event.
 *
 * @param widget The radio box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the radio box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituRadioBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the radio box widget to the specified surface.
 *
 * @param widget The radio box widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituRadioBoxDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The radio box widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituRadioBoxOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Sets the radio box widget checked or not.
 *
 * @param radiobox The radio box widget to set.
 * @param checked true for checked or false for un-checked.
 */
void ituRadioBoxSetChecked(ITURadioBox* radiobox, bool checked);

/**
 * Is the radio box widget checked or not.
 *
 * @param radiobox The radio box widget.
 * @return Is checked or not.
 */
bool ituRadioBoxIsChecked(ITURadioBox* radiobox);

/** @} */ // end of itu_widget_radiobox

/** @defgroup itu_widget_scrolltext ScrollText
 *  @{
 */
/**
 * Scroll text widget definition. This is used for drawing scrolling text.
 */
typedef struct
{
    ITUText text;       ///< Base text widget definition.
    int width;          ///< The width range to scroll.
    int scrollDelay;    ///< The delay on scrolling. Unit is depend on ITU_EVENT_TIMER event.
    int stopDelay;      ///< The delay on stop after scrolling. Unit is depend on ITU_EVENT_TIMER event.
    int state;          ///< The scrolling state.
    int delayCount;     ///< The delay count to go to the next scroll state.
    int offsetX;        ///< The scrolling x coordinate, in pixels.
    int offsetWidth;    ///< The width to scroll, in pixels.
	int tmpMark;        ///< The temp mark.(internal usage)
	char* tmpStr;       ///< The temp string.(internal usage)
} ITUScrollText;

/**
 * Initializes the scroll text widget.
 *
 * @param text The scroll text widget to initialize.
 * @param width The scroll text width.
 */
void ituScrollTextInit(ITUScrollText* text, int width);

/**
 * Loads the scroll text widget. This is called by scene manager.
 *
 * @param text The scroll text widget to load.
 * @param base The address in the scene file buffer.
 */
void ituScrollTextLoad(ITUScrollText* text, uint32_t base);

/**
 * Clones the scroll text widget.
 *
 * @param widget The scroll text widget to clone.
 * @param cloned Retrieved cloned scroll text widget.
 * @return true if clone is success, false otherwise.
 */
bool ituScrollTextClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the scroll text widget by specified event.
 *
 * @param widget The scroll text widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the scroll text widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituScrollTextUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the scroll text widget to the specified surface.
 *
 * @param widget The scroll text widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituScrollTextDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Sets the delay values to the scroll text widget.
 *
 * @param text The scroll text widget to set.
 * @param scrollDelay The delay on scrolling. Unit is depend on ITU_EVENT_TIMER event.
 * @param stopDelay The delay on stop after scrolling. Unit is depend on ITU_EVENT_TIMER event.
 */
void ituScrollTextSetDelay(ITUScrollText* text, int scrollDelay, int stopDelay);

/**
 * Starts to scroll the scroll text widget.
 *
 * @param text The scroll text widget to start to scroll.
 */
void ituScrollTextStart(ITUScrollText* text);

/**
 * Stops to scroll the scroll text widget.
 *
 * @param text The scroll text widget to stop scrolling.
 */
void ituScrollTextStop(ITUScrollText* text);

/**
 * Sets string of the scroll text widget.
 *
 * @param text Pointer referring to the scroll text widget.
 * @param string The string to set.
 */
void ituScrollTextSetString(ITUScrollText* text, char* string);

/**
 * Sets the scroll text widget as read.
 *
 * @param text Pointer referring to the scroll text widget.
 */
#define ituScrollTextSetAsRead(text)                            (((ITUWidget*)(text))->flags |= ITU_LOADED)

/**
 * Sets the scroll text widget as unread.
 *
 * @param text Pointer referring to the scroll text widget.
 */
#define ituScrollTextSetAsUnread(text)                          (((ITUWidget*)(text))->flags &= ~ITU_LOADED)

/**
 * Is the scroll text widget read or not.
 *
 * @param text Pointer referring to the scroll text widget.
 */
#define ituScrollTextIsRead(text)                               ((((ITUWidget*)(text))->flags) & ITU_LOADED)

/** @} */ // end of itu_widget_scrolltext

/** @defgroup itu_widget_flowwindow FlowWindow
 *  @{
 */
/**
 * Flow window definition. This is used for layout children widgets as a flow.
 */
typedef struct
{
    ITUWidget widget;   ///< Base widget definition.
    ITULayout layout;   ///< The flow layout. Can be ITU_LAYOUT_UP/ITU_LAYOUT_LEFT/ITU_LAYOUT_DOWN/ITU_LAYOUT_RIGHT.
    int borderSize;     ///< The border size.
} ITUFlowWindow;

/**
 * Initializes the flow window.
 *
 * @param fwin The flow window to initialize.
 * @param layout The flow layout. Can be ITU_LAYOUT_UP/ITU_LAYOUT_LEFT/ITU_LAYOUT_DOWN/ITU_LAYOUT_RIGHT.
 */
void ituFlowWindowInit(ITUFlowWindow* fwin, ITULayout layout);

/**
 * Loads the flow window widget. This is called by scene manager.
 *
 * @param fwin The flow window to load.
 * @param base The address in the scene file buffer.
 */
void ituFlowWindowLoad(ITUFlowWindow* fwin, uint32_t base);

/**
 * Updates the flow window by specified event.
 *
 * @param widget The flow window to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the border window is modified and need to be redraw, false if no need to be redraw.
 */
bool ituFlowWindowUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the flow window to the specified surface.
 *
 * @param widget The flow window to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituFlowWindowDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/** @} */ // end of itu_widget_flowwindow

/** @defgroup itu_widget_listbox ListBox
 *  @{
 */
/**
 * List box widget definition. This is a container of scroll text widgets to draw list of texts.
 */
typedef struct ITUListBoxTag
{
    ITUFlowWindow fwin;                         ///< Base flow window definition.
    ITUColor focusColor;                        ///< Focused background color
    ITUColor focusFontColor;                    ///< Focused font color
    ITUColor orgFontColor;                      ///< Orginal font color
    ITUColor readFontColor;                     ///< Read font color
    int focusIndex;                             ///< The index of focused item

    int pageIndex;                              ///< The current page index which is started from 1.
    char pageIndexName[ITU_WIDGET_NAME_SIZE];   ///< The name of text widget to show the page index.
    ITUText* pageIndexText;                     ///< The text widget to show the page index.

    int pageCount;                              ///< The total page count.
    char pageCountName[ITU_WIDGET_NAME_SIZE];   ///< The name of text widget to show the page count.
    ITUText* pageCountText;                     ///< The text widget to show the page count.

    int scrollDelay;                            ///< The delay on scrolling. Unit is depend on ITU_EVENT_TIMER event.
    int stopDelay;                              ///< The delay on stop after scrolling. Unit is depend on ITU_EVENT_TIMER event.
    int itemCount;                              ///< Item count on current page.

    ITUAction actions[ITU_ACTIONS_SIZE];        ///< Actions for events to trigger

    /**
     * Do loading page. This is called when user goto previous/next page.
     *
     * @param listbox The list box widget to load page.
     * @param pageIndex The page index to load.
     */
    void (*OnLoadPage)(struct ITUListBoxTag* listbox, int pageIndex);
    
    /**
     * Do selection. This is called when user select an item of the list.
     *
     * @param listbox The list box widget to select.
     * @param item The item to select.
     * @param confirm true if user press enter key or double-click the item, false for just select this item.
     */
    void (*OnSelection)(struct ITUListBoxTag* listbox, ITUScrollText* item, bool confirm);

} ITUListBox;

/**
 * Initializes the list box widget.
 *
 * @param listbox The list box widget to initialize.
 * @param width The width of the list box widget.
 */
void ituListBoxInit(ITUListBox* listbox, int width);

/**
 * Loads the list box widget. This is called by scene manager.
 *
 * @param listbox The list box widget to load.
 * @param base The address in the scene file buffer.
 */
void ituListBoxLoad(ITUListBox* listbox, uint32_t base);

/**
 * Updates the list box widget by specified event.
 *
 * @param widget The list box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the list box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituListBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the list box widget to the specified surface.
 *
 * @param widget The list box widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituListBoxDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The list box widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituListBoxOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Selects the specified index of item of the list box widget.
 *
 * @param listbox The list box widget to select.
 * @param index The index of item to select.
 */
void ituListBoxSelect(ITUListBox* listbox, int index);

/**
 * Selects the specified index of item of the list box widget without executing actions.
 *
 * @param listbox The list box widget to select.
 * @param index The index of item to select.
 */
void ituListBoxCheck(ITUListBox* listbox, int index);

/**
 * Reloads the list box widget.
 *
 * @param listbox The list box widget to select.
 */
void ituListBoxReload(ITUListBox* listbox);

/**
 * Gotos the specified index of page of the list box widget.
 *
 * @param listbox The list box widget to goto.
 * @param index The index of page to goto.
 */
void ituListBoxGoto(ITUListBox* listbox, int index);

// Macros to easily use ITUListBox structure
/**
 * Do loading page. This is called when user goto previous/next page.
 *
 * @param listbox The list box widget to load page.
 * @param pageIndex The page index to load.
 * @return The actual count of loaded items.
 */
#define ituListBoxOnLoadPage(listbox, pageIndex)        ((ITUListBox*)(listbox))->OnLoadPage((ITUListBox*)(listbox), (pageIndex))

/**
 * Sets OnLoadPage callback function of list box.
 *
 * @param listbox Pointer referring to the list box.
 * @param onLoadPage The callback function to set.
 */
#define ituListBoxSetOnLoadPage(listbox, onLoadPage)    (((ITUListBox*)(listbox))->OnLoadPage = (onLoadPage))

/**
 * Do selection. This is called when user select an item of the list.
 *
 * @param listbox The list box widget to select.
 * @param item The item to select.
 * @param confirm true if user press enter key or double-click the item, false for just select this item.
 */
#define ituListBoxOnSelection(listbox, item, confirm)   ((ITUListBox*)(listbox))->OnSelection((ITUListBox*)(listbox), (item), (confirm))

/**
 * Sets OnSelection callback function of list box.
 *
 * @param listbox Pointer referring to the list box.
 * @param onSelection The callback function to set.
 */
#define ituListBoxSetOnSelection(listbox, onSelection)  (((ITUListBox*)(listbox))->OnSelection = (onSelection))

void ituListBoxPrevImpl(ITUListBox* listbox);
#define ituListBoxPrev(listbox)                      ituListBoxPrevImpl((ITUListBox*)(listbox))

void ituListBoxNextImpl(ITUListBox* listbox);
#define ituListBoxNext(listbox)                      ituListBoxNextImpl((ITUListBox*)(listbox))

/**
 * Gets the focus item of list box widget.
 *
 * @param listbox The list box widget.
 * @return the focus item. NULL if not focused item.
 */
ITUWidget* ituListBoxGetFocusItem(ITUListBox* listbox);

/**
 * Sets the specified item as read or not.
 *
 * @param listbox The list box widget to set.
 * @param item The item to set.
 * @param read Read or not.
 */
void ituListBoxSetItemRead(ITUListBox* listbox, ITUWidget* item, bool read);

/** @} */ // end of itu_widget_listbox

/** @defgroup itu_widget_filelistbox FileListBox
 *  @{
 */
#define ITU_FILELIST_BUSYING    0x1 ///< The file list box is busying
#define ITU_FILELIST_CREATED    0x2 ///< The file list box is created
#define ITU_FILELIST_RECREATE   0x4 ///< The file list box need to recreate
#define ITU_FILELIST_DESTROYING 0x8 ///< The file list box is destroying

/**
 * File list box widget definition. This is used for drawing list of files.
 */
typedef struct
{
    ITUListBox listbox;         ///< Base list box widget definition.
    char path[ITU_PATH_MAX];    ///< The drive path to list
    unsigned int flistboxFlags; ///< The flags to indicate the status of the file list box.
    int fileCount;              ///< Total file count
    void* rbtree;               ///< Red-black tree to sort file name

} ITUFileListBox;

/**
 * Initializes the file list box widget.
 *
 * @param flistbox The file list box widget to initialize.
 * @param width The width of the file list box widget.
 * @param path The drive path to list.
 */
void ituFileListBoxInit(ITUFileListBox* flistbox, int width, char* path);

/**
 * Loads the file list box widget. This is called by scene manager.
 *
 * @param flistbox The file list box widget to load.
 * @param base The address in the scene file buffer.
 */
void ituFileListBoxLoad(ITUFileListBox* flistbox, uint32_t base);

/**
 * Sets the path to browse.
 *
 * @param flistbox The file list box widget to set.
 * @param path The path to browse.
 */
void ituFileListSetPath(ITUFileListBox* flistbox, char* path);

/**
 * Updates the file list box widget by specified event.
 *
 * @param widget The file list box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the file list box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituFileListBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The file list box widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituFileListBoxOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Do loading page. This is called when user goto previous/next page.
 *
 * @param listbox The file list box widget to load page.
 * @param pageIndex The page index to load.
 */
void ituFileListBoxOnLoadPage(ITUListBox* listbox, int pageIndex);

/**
 * Do selection. This is called when user select an file of the list.
 *
 * @param listbox The file list box widget to select.
 * @param item The file to select.
 * @param confirm true if user press enter key or double-click the file, false for just select this file.
 */
void ituFileListOnSelection(ITUListBox* listbox, ITUScrollText* item, bool confirm);

/** @} */ // end of itu_widget_filelistbox

/** @defgroup itu_widget_sprite Sprite
 *  @{
 */
/**
 * Sprite widget definition. This is used for drawing animation of sprite.
 * It is a container of icon widgets to do the sprite job.
 */
typedef struct ITUSpriteTag
{
    ITUWidget widget;                       ///< Base widget definition.
    int delay;                              ///< The delay on playing. Unit is depend on ITU_EVENT_TIMER event.
    int repeat;                             ///< Is play repeatly or not.
    int playing;                            ///< Is playing or not.
    int childCount;                         ///< Total frames count.
    int frame;                              ///< Current frame.
    int delayCount;                         ///< The delay count to play next frame.
    ITUWidget* child;                       ///< Current frame of widget.
    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

    /**
     * Called when sprite stopped.
     *
     * @param sprite The stopped sprite widget.
     */
    void (*OnStop)(struct ITUSpriteTag* sprite);

} ITUSprite;

/**
 * Initializes the sprite widget.
 *
 * @param sprite The sprite widget to initialize.
 */
void ituSpriteInit(ITUSprite* sprite);

/**
 * Loads the sprite widget. This is called by scene manager.
 *
 * @param sprite The sprite widget to load.
 * @param base The address in the scene file buffer.
 */
void ituSpriteLoad(ITUSprite* sprite, uint32_t base);

/**
 * Clones the sprite widget.
 *
 * @param widget The sprite widget to clone.
 * @param cloned Retrieved sprite widget.
 * @return true if clone is success, false otherwise.
 */
bool ituSpriteClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the sprite widget by specified event.
 *
 * @param widget The sprite widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the sprite widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituSpriteUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the sprite widget to the specified surface.
 *
 * @param widget The sprite widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituSpriteDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The sprite widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituSpriteOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * This is called when the sprite stop playing.
 *
 * @param sprite The sprite widget.
 */
#define ituSpriteOnStop(sprite) ((ITUSprite*)(sprite))->OnStop((ITUSprite*)(sprite))

/**
 * Sets OnStop callback function of sprite.
 *
 * @param sprite Pointer referring to the sprite.
 * @param onStop The callback function to set.
 */
#define ituSpriteSetOnStop(sprite, onStop)    (((ITUSprite*)(sprite))->OnStop = (onStop))

/**
 * Sets the delay value to the sprite widget.
 *
 * @param sprite The sprite widget to set.
 * @param delay The delay on playing. Unit is depend on ITU_EVENT_TIMER event.
 */
void ituSpriteSetDelay(ITUSprite* sprite, int delay);

/**
 * Plays the sprite widget.
 *
 * @param sprite The sprite widget to play.
 * @param frame The specified frame of sprite widget to start playing. -1 indicates playing from current frame.
 */
void ituSpritePlay(ITUSprite* sprite, int frame);

/**
 * Stops the playing sprite widget.
 *
 * @param sprite The playing sprite widget to stop.
 */
void ituSpriteStop(ITUSprite* sprite);

/**
 * Gotos the specified frame of sprite widget.
 *
 * @param sprite The playing sprite widget to goto.
 * @param frame The specified frame of sprite widget to goto.
 */
void ituSpriteGoto(ITUSprite* sprite, int frame);

/** @} */ // end of itu_widget_sprite

/** @defgroup itu_widget_progressbar ProgressBar
 *  @{
 */
/**
 * Progress bar widget definition.
 */
typedef struct
{
    ITUBackground bg;       ///< Base background widget definition.
    ITULayout layout;       ///< Layout. Supports ITU_LAYOUT_VERTICAL and ITU_LAYOUT_HORIZONTAL only.
    int min;                                ///< Minimum value.
    int max;                                ///< Maximum value.
    int step;                               ///< Step value to change.
    int value;                              ///< Current value.
    char valueName[ITU_WIDGET_NAME_SIZE];   ///< The name of text widget to show the value.
    ITUText* valueText;                     ///< The text widget to show the value.
    ITUColor fgColor;                       ///< Color of progress bar.
    ITUSurface* staticBarSurf;              ///< The static progress bar surface.
    ITUSurface* barSurf;                    ///< The progress bar surface.
} ITUProgressBar;

/**
 * Initializes the progress bar widget.
 *
 * @param bar The progress bar widget to initialize.
 * @param layout Layout. Supports ITU_LAYOUT_VERTICAL and ITU_LAYOUT_HORIZONTAL only.
 */
void ituProgressBarInit(ITUProgressBar* bar, ITULayout layout);

/**
 * Loads the progress bar widget. This is called by scene manager.
 *
 * @param bar The progress bar widget to load.
 * @param base The address in the scene file buffer.
 */
void ituProgressBarLoad(ITUProgressBar* bar, uint32_t base);

/**
 * Exits the progress bar widget.
 *
 * @param widget The progress bar widget to exit.
 */
void ituProgressBarExit(ITUWidget* widget);

/**
 * Updates the progress bar widget by specified event.
 *
 * @param widget The progress bar widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the progress bar widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituProgressBarUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the progress bar widget to the specified surface.
 *
 * @param widget The progress bar widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituProgressBarDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The progress bar widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituProgressBarOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Sets the progressing value to the progress bar widget.
 *
 * @param bar The progress bar widget to set.
 * @param value Progressing value.
 */
void ituProgressBarSetValue(ITUProgressBar* bar, int value);

/**
 * Loads static data to the progress bar widget.
 *
 * @param bar The progress bar widget to load.
 */
void ituProgressBarLoadStaticData(ITUProgressBar* bar);

/**
 * Releases surface of progress bar widget.
 *
 * @param bar The progress bar widget to release.
 */
void ituProgressBarReleaseSurface(ITUProgressBar* bar);

/** @} */ // end of itu_widget_progressbar

/** @defgroup itu_widget_keyboard Keyboard
 *  @{
 */
/**
 * Keyboard widget definition. It is a container of button widgets to do the keyboard job.
 */
typedef struct
{
    ITUBackground bg;   ///< Base background widget definition.
    ITUWidget* target;  ///< Text box widget for input.
} ITUKeyboard;

/**
 * Initializes the keyboard widget.
 *
 * @param kb The keyboard widget to initialize.
 */
void ituKeyboardInit(ITUKeyboard* kb);

/**
 * Loads the keyboard widget. This is called by scene manager.
 *
 * @param kb The keyboard widget to load.
 * @param base The address in the scene file buffer.
 */
void ituKeyboardLoad(ITUKeyboard* kb, uint32_t base);

/**
 * Updates the keyboard widget by specified event.
 *
 * @param widget The keyboard widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the keyboard widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituKeyboardUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The keyboard widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituKeyboardOnAction(ITUWidget* widget, ITUActionType action, char* param);

/** @} */ // end of itu_widget_keyboard

/** @defgroup itu_widget_spritebutton SpriteButton
 *  @{
 */
/**
 * Sprite button widget definition
 */
typedef struct
{
    ITUSprite sprite;   ///< Sprite
    int pressed;        ///< Is pressed or not
} ITUSpriteButton;

/**
 * Initializes the sprite button widget.
 *
 * @param sbtn The sprite button widget to initialize.
 */
void ituSpriteButtonInit(ITUSpriteButton* sbtn);

/**
 * Loads the sprite button widget. This is called by scene manager.
 *
 * @param sbtn The sprite button widget to load.
 * @param base The address in the scene file buffer.
 */
void ituSpriteButtonLoad(ITUSpriteButton* sbtn, uint32_t base);

/**
 * Updates the sprite button widget by specified event.
 *
 * @param widget The sprite button widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the sprite button widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituSpriteButtonUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/** @} */ // end of itu_widget_spritebutton

/** @defgroup itu_widget_trackbar TrackBar
 *  @{
 */
/**
 * Track bar widget definition
 */
typedef struct ITUTrackBarTag
{
    ITUBackground bg;                       ///< Base background widget definition
    char trackerName[ITU_WIDGET_NAME_SIZE]; ///< The name of button widget as tracker.
    ITUButton* tracker;                     ///< Tracker widget.
    ITULayout layout;                       ///< Layout. Supports ITU_LAYOUT_VERTICAL and ITU_LAYOUT_HORIZONTAL only.
    int min;                                ///< Minimum value.
    int max;                                ///< Maximum value.
    int step;                               ///< Step value to change.
    int value;                              ///< Current value.
    char valueName[ITU_WIDGET_NAME_SIZE];   ///< The name of text widget to show the value.
    ITUText* valueText;                     ///< The text widget to show the value.
    int gap;                                ///< Gap on the both side.
    char tipName[ITU_WIDGET_NAME_SIZE];     ///< The name of widget to show the tip.
    ITUWidget* tip;                         ///< The widget to show the tip.
    int delay;                              ///< The delay on playing. Unit is depend on ITU_EVENT_TIMER event.
    int delayCount;                         ///< The delay count to play next frame.

    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

    /**
     * Called when the value is changed.
     *
     * @param tbar The track bar.
     * @param value The new value.
     * @param confirm true on mouse-up, false for value is changing.
     */
    void (*OnValueChanged)(struct ITUTrackBarTag* tbar, int value, bool confirm);

} ITUTrackBar;

/**
 * Initializes the track bar widget.
 *
 * @param tbar The track bar widget to initialize.
 * @param layout Layout. Supports ITU_LAYOUT_VERTICAL and ITU_LAYOUT_HORIZONTAL only.
 */
void ituTrackBarInit(ITUTrackBar* tbar, ITULayout layout);

/**
 * Loads the track bar widget. This is called by scene manager.
 *
 * @param tbar The track bar widget to load.
 * @param base The address in the scene file buffer.
 */
void ituTrackBarLoad(ITUTrackBar* tbar, uint32_t base);

/**
 * Updates the track bar widget by specified event.
 *
 * @param widget The track bar widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the track bar widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituTrackBarUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the track bar widget to the specified surface.
 *
 * @param widget The track bar widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituTrackBarDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Called when the value is changed.
 *
 * @param tbar The track bar.
 * @param value The new value.
 * @param confirm true on mouse-up, false for value is changing.
 */
#define ituTrackBarOnValueChanged(tbar, value, confirm)   ((ITUTrackBar*)(tbar))->OnValueChanged((ITUTrackBar*)(tbar), (value), (confirm))

/**
 * Sets OnValueChanged callback function of track bar.
 *
 * @param tbar Pointer referring to the track bar.
 * @param onValueChanged The callback function to set.
 */
#define ituTrackBarSetValueChanged(tbar, onValueChanged)  (((ITUTrackBar*)(tbar))->OnValueChanged = (onValueChanged))

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The track bar widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituTrackBarOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Sets the value to the track bar widget.
 *
 * @param bar The track bar widget to set.
 * @param value Current value.
 */
void ituTrackBarSetValue(ITUTrackBar* bar, int value);

/** @} */ // end of itu_widget_trackbar

/** @defgroup itu_widget_digitalclock DigitalClock
 *  @{
 */

#define ITU_DIGITALCLOCK_12H    0x1    ///< This digital clock is in 12H format.
#define ITU_DIGITALCLOCK_STOP   0x2    ///< This digital clock is stopped.

/**
 * Digital clock widget definition
 */
typedef struct ITUDigitalClockTag
{
    ITUBackground bg;                       ///< Base background widget definition
    unsigned int digitalClockFlags;         ///< Flags for the digital clock.
    char yearName[ITU_WIDGET_NAME_SIZE];    ///< The name of text widget to show the year.
    ITUText* yearText;                      ///< The text widget to show the year.
    int year;                               ///< Current year value.
    char monthName[ITU_WIDGET_NAME_SIZE];   ///< The name of text widget to show the month.
    ITUText* monthText;                     ///< The text widget to show the month.
    int month;                              ///< Current month value.
    char dayName[ITU_WIDGET_NAME_SIZE];     ///< The name of text widget to show the day.
    ITUText* dayText;                       ///< The text widget to show the day.
    int day;                                ///< Current day value.
    char hourName[ITU_WIDGET_NAME_SIZE];    ///< The name of text widget to show the hour.
    ITUText* hourText;                      ///< The text widget to show the hour.
    int hour;                               ///< Current hour value.
    char minuteName[ITU_WIDGET_NAME_SIZE];  ///< The name of text widget to show the minute.
    ITUText* minuteText;                    ///< The text widget to show the minute.
    int minute;                             ///< Current minute value.
    char secondName[ITU_WIDGET_NAME_SIZE];  ///< The name of text widget to show the second.
    ITUText* secondText;                    ///< The text widget to show the second.
    int second;                             ///< Current second value.
    char weekName[ITU_WIDGET_NAME_SIZE];    ///< The name of week sprite to show the week.
    ITUSprite* weekSprite;                  ///< The week sprite to show the week.
    int week;                               ///< Current week value.
    char colonName[ITU_WIDGET_NAME_SIZE];  ///< The name of text widget to show the colon.
    ITUText* colonText;                    ///< The text widget to show the colon.

} ITUDigitalClock;

/**
 * Initializes the digital clock widget.
 *
 * @param dclk The digital clock widget to initialize.
 */
void ituDigitalClockInit(ITUDigitalClock* dclk);

/**
 * Loads the digital clock widget. This is called by scene manager.
 *
 * @param dclk The digital clock widget to load.
 * @param base The address in the scene file buffer.
 */
void ituDigitalClockLoad(ITUDigitalClock* dclk, uint32_t base);

/**
 * Updates the digital clock widget by specified event.
 *
 * @param widget The digital clock widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the digital clock widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituDigitalClockUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/** @} */ // end of itu_widget_digitalclock


/** @defgroup itu_widget_analogclock AnalogClock
 *  @{
 */
/**
 * Analog clock widget definition
 */
typedef struct ITUAnalogClockTag
{
    ITUBackground bg;                       ///< Base background widget definition
    char hourName[ITU_WIDGET_NAME_SIZE];    ///< The name of text widget to show the hour pointer.
    ITUIcon* hourIcon;                      ///< The icon widget to show the hour pointer.
    int hour;                               ///< Current hour value.
    int hourX;                              ///< Center X coordinate of hour pointer
    int hourY;                              ///< Center Y coordinate of hour pointer
    char minuteName[ITU_WIDGET_NAME_SIZE];  ///< The name of text widget to show the minute.
    ITUIcon* minuteIcon;                    ///< The text widget to show the minute.
    int minute;                             ///< Current minute value.
    int minuteX;                            ///< Center X coordinate of minute pointer
    int minuteY;                            ///< Center Y coordinate of minute pointer
    char secondName[ITU_WIDGET_NAME_SIZE];  ///< The name of text widget to show the second.
    ITUIcon* secondIcon;                    ///< The text widget to show the second.
    int second;                             ///< Current second value.
    int secondX;                            ///< Center X coordinate of second pointer
    int secondY;                            ///< Center Y coordinate of second pointer
} ITUAnalogClock;

/**
 * Initializes the analog clock widget.
 *
 * @param aclk The analog clock widget to initialize.
 */
void ituAnalogClockInit(ITUAnalogClock* aclk);

/**
 * Loads the analog clock widget. This is called by scene manager.
 *
 * @param aclk The analog clock widget to load.
 * @param base The address in the scene file buffer.
 */
void ituAnalogClockLoad(ITUAnalogClock* aclk, uint32_t base);

/**
 * Updates the analog clock widget by specified event.
 *
 * @param widget The analog clock widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the analog clock widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituAnalogClockUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the analog clock widget to the specified surface.
 *
 * @param widget The progress bar widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituAnalogClockDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/** @} */ // end of itu_widget_analogclock

/** @defgroup itu_widget_calendar Calendar
 *  @{
 */

/**
 * Calendar type definition
 */
typedef enum
{
    ITU_CAL_AD,     ///< AD year.
    ITU_CAL_LUNAR   ///< Chinese lunar year.
} ITUCalendarType;

/**
 * Calendar widget definition
 */
typedef struct ITUCalendarTag
{
    ITUBackground bg;                       ///< Base background widget definition.
    ITUCalendarType type;                   ///< Calendar type.
    int minYear;                            ///< Minimum year.
    int maxYear;                            ///< Maximum year.
    char sunName[ITU_WIDGET_NAME_SIZE];     ///< The name of listbox widget to show the Sunday of calendar.
    ITUListBox* sunListBox;                 ///< The listbox widget to show the Sunday of calendar.
    char monName[ITU_WIDGET_NAME_SIZE];     ///< The name of listbox widget to show the Monday of calendar.
    ITUListBox* monListBox;                 ///< The listbox widget to show the Monday of calendar.
    char tueName[ITU_WIDGET_NAME_SIZE];     ///< The name of listbox widget to show the Tuesday of calendar.
    ITUListBox* tueListBox;                 ///< The listbox widget to show the Tuesday of calendar.
    char wedName[ITU_WIDGET_NAME_SIZE];     ///< The name of listbox widget to show the Wednesday of calendar.
    ITUListBox* wedListBox;                 ///< The listbox widget to show the Wednesday of calendar.
    char thuName[ITU_WIDGET_NAME_SIZE];     ///< The name of listbox widget to show the Thursday of calendar.
    ITUListBox* thuListBox;                 ///< The listbox widget to show the Thursday of calendar.
    char friName[ITU_WIDGET_NAME_SIZE];     ///< The name of listbox widget to show the Friday of calendar.
    ITUListBox* friListBox;                 ///< The listbox widget to show the Friday of calendar.
    char satName[ITU_WIDGET_NAME_SIZE];     ///< The name of listbox widget to show the Saturday of calendar.
    ITUListBox* satListBox;                 ///< The listbox widget to show the Saturday of calendar.

    int year;                               ///< Selected year.
    char yearName[ITU_WIDGET_NAME_SIZE];    ///< The name of text widget to show the year.
    ITUText* yearText;                      ///< The text widget to show the year.

    int month;                              ///< Selected month.
    char monthName[ITU_WIDGET_NAME_SIZE];   ///< The name of text widget to show the month.
    ITUWidget* monthTarget;                 ///< The widget to show the month.

    int day;                                ///< Selected day.
    char dayName[ITU_WIDGET_NAME_SIZE];     ///< The name of text widget to show the day.
    ITUText* dayText;                       ///< The text widget to show the day.

    int weekDays[7][6];                     ///< Week days.
} ITUCalendar;

/**
 * Initializes the calendar widget.
 *
 * @param cal The calendar widget to initialize.
 */
void ituCalendarInit(ITUCalendar* cal);

/**
 * Loads the calendar widget. This is called by scene manager.
 *
 * @param cal The calendar widget to load.
 * @param base The address in the scene file buffer.
 */
void ituCalendarLoad(ITUCalendar* cal, uint32_t base);

/**
 * Updates the calendar widget by specified event.
 *
 * @param widget The calendar widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the calendar widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituCalendarUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The calendar widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituCalendarOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Gotos the last month.
 *
 * @param cal The calendar widget.
 */
void ituCalendarLastMonth(ITUCalendar* cal);

/**
 * Gotos the next month.
 *
 * @param cal The calendar widget.
 */
void ituCalendarNextMonth(ITUCalendar* cal);

/**
 * Gotos the today.
 *
 * @param cal The calendar widget.
 */
void ituCalendarToday(ITUCalendar* cal);

/**
 * Gotos the specified year and month.
 *
 * @param cal The calendar widget.
 * @param year The year.
 * @param month The month.
 */
void ituCalendarGoto(ITUCalendar* cal, int year, int month);

/** @} */ // end of itu_widget_calendar

/** @defgroup itu_widget_iconlistbox IconListBox
 *  @{
 */

/**
 * IconListBox widget definition
 */
typedef struct ITUIconListBoxTag
{
    ITUListBox listbox;                                             ///< Base listbox widget definition.
    ITUSurface* staticSurfArray[ITU_ICON_LISTBOX_TYPE_COUNT];       ///< The static surfaces.
    ITUSurface* surfArray[ITU_ICON_LISTBOX_TYPE_COUNT];             ///< The icon surfaces.
    ITUSurface* focusStaticSurfArray[ITU_ICON_LISTBOX_TYPE_COUNT];  ///< The focus static surfaces.
    ITUSurface* focusSurfArray[ITU_ICON_LISTBOX_TYPE_COUNT];        ///< The focus icon surfaces.
} ITUIconListBox;

/**
 * Initializes the icon list box widget.
 *
 * @param ilistbox The icon list box widget to initialize.
 * @param width The width of the icon list box widget.
 */
void ituIconListBoxInit(ITUIconListBox* ilistbox, int width);

/**
 * Loads the icon list box widget. This is called by scene manager.
 *
 * @param ilistbox The icon list box widget to load.
 * @param base The address in the scene file buffer.
 */
void ituIconListBoxLoad(ITUIconListBox* ilistbox, uint32_t base);

/**
 * Updates the icon list box widget by specified event.
 *
 * @param widget The icon list box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the icon list box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituIconListBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Loads static data to the icon list box widget.
 *
 * @param ilistbox The icon list box widget to load.
 */
void ituIconListBoxLoadStaticData(ITUIconListBox* ilistbox);

/**
 * Releases surface of icon list box widget.
 *
 * @param ilistbox The icon list box widget to release.
 */
void ituIconListBoxReleaseSurface(ITUIconListBox* ilistbox);

/** @} */ // end of itu_widget_iconlistbox

/** @defgroup itu_widget_mediafilelistbox MediaFileListBox
*  @{
*/
/**
 * Media repeat mode definition
 */
typedef enum
{
    ITU_MEDIA_REPEAT_NONE   = 0, ///< No repeat
    ITU_MEDIA_REPEAT_ONCE   = 1, ///< Repeat once
    ITU_MEDIA_REPEAT_ALL    = 2  ///< Repeat all
} ITUMediaRepeatMode;

/**
* Media file list box widget definition. This is used for drawing list of media files.
*/
typedef struct
{
    ITUListBox listbox;                     ///< Base list box widget definition.
    char path[ITU_PATH_MAX];                ///< The drive path to list
    char extensions[ITU_EXTENSIONS_SIZE];   ///< The extensions of files
    unsigned int mflistboxFlags;            ///< The flags to indicate the status of the media file list box.
    int fileCount;                          ///< Total file count
    void* rbtree;                           ///< Red-black tree to sort file name
    ITUMediaRepeatMode repeatMode;          ///< Repeat mode
    int randomPlay;                         ///< Random play or not
    int playIndex;                          ///< Current play index
    int randomPlayedCount;                  ///< Random played count
    int* randomPlayedArray;                 ///< Records for random play

} ITUMediaFileListBox;

/**
* Initializes the media file list box widget.
*
* @param mflistbox The media file list box widget to initialize.
* @param width The width of the media file list box widget.
* @param path The drive path to list.
*/
void ituMediaFileListBoxInit(ITUMediaFileListBox* mflistbox, int width, char* path);

/**
* Loads the media file list box widget. This is called by scene manager.
*
* @param mflistbox The media file list box widget to load.
* @param base The address in the scene file buffer.
*/
void ituMediaFileListBoxLoad(ITUMediaFileListBox* mflistbox, uint32_t base);

/**
* Sets the path to browse.
*
* @param mflistbox The media file list box widget to set.
* @param path The path to browse.
*/
void ituMediaFileListSetPath(ITUMediaFileListBox* mflistbox, char* path);

/**
* Updates the media file list box widget by specified event.
*
* @param widget The media file list box widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the media file list box widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituMediaFileListBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Do the specified action. This is triggered by other widget's event.
*
* @param widget The media file list box widget to do the action.
* @param action The action to do.
* @param param The parameter of action.
*/
void ituMediaFileListBoxOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* Do loading page. This is called when user goto previous/next page.
*
* @param listbox The media file list box widget to load page.
* @param pageIndex The page index to load.
*/
void ituMediaFileListBoxOnLoadPage(ITUListBox* listbox, int pageIndex);

/**
* Do selection. This is called when user select an file of the list.
*
* @param listbox The media file list box widget to select.
* @param item The file to select.
* @param confirm true if user press enter key or double-click the file, false for just select this file.
*/
void ituMediaFileListOnSelection(ITUListBox* listbox, ITUScrollText* item, bool confirm);

/**
* Plays current selected item.
*
* @param mflistbox The media file list box widget to play.
* @return The chosen item. NULL for no item.
*/
ITUScrollText* ituMediaFileListPlay(ITUMediaFileListBox* mflistbox);

/**
* Plays previous item.
*
* @param mflistbox The media file list box widget to go previous.
* @return The previous item. NULL for no previous item.
*/
ITUScrollText* ituMediaFileListPrev(ITUMediaFileListBox* mflistbox);

/**
* Plays next item.
*
* @param mflistbox The media file list box widget to go next.
* @return The next item. NULL for no next item.
*/
ITUScrollText* ituMediaFileListNext(ITUMediaFileListBox* mflistbox);

/** @} */ // end of itu_widget_mediafilelistbox

/** @defgroup itu_widget_circleprogressbar CircleProgressBar
 *  @{
 */
/**
 * Circle progress bar widget definition.
 */
typedef struct
{
    ITUBackground bg;                           ///< Base background widget definition
    int value;                                  ///< Progressing value.
    char progressName[ITU_WIDGET_NAME_SIZE];    ///< The name of icon widget to show the progress.
    ITUIcon* progressIcon;                      ///< The icon widget to show the progress.    
    char percentName[ITU_WIDGET_NAME_SIZE];     ///< The name of text widget to show the value.
    ITUText* percentText;                       ///< The text widget to show the value.
    int startAngle;                             ///< The angle of start position. Range from 0-360.
    int endAngle;                               ///< The angle of end position. Range from 0-360.
    int maxValue;                               ///< The maximum value.
    int cacheCount;                             ///< The value count to cache.
    ITUSurface* cacheSurf;                      ///< The cached surface for rotation.

} ITUCircleProgressBar;

/**
 * Initializes the circle progress bar widget.
 *
 * @param bar The circle progress bar widget to initialize.
 */
void ituCircleProgressBarInit(ITUCircleProgressBar* bar);

/**
 * Loads the circle progress bar widget. This is called by scene manager.
 *
 * @param bar The circle progress bar widget to load.
 * @param base The address in the scene file buffer.
 */
void ituCircleProgressBarLoad(ITUCircleProgressBar* bar, uint32_t base);

/**
 * Updates the circle progress bar widget by specified event.
 *
 * @param widget The circle progress bar widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the circle progress bar widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituCircleProgressBarUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the circle progress bar widget to the specified surface.
 *
 * @param widget The circle progress bar widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituCircleProgressBarDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The circle progress bar widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituCircleProgressBarOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Sets the progressing value to the circle progress bar widget.
 *
 * @param bar The circle progress bar widget to set.
 * @param value Progressing value. Unit is percentage.
 */
void ituCircleProgressBarSetValue(ITUCircleProgressBar* bar, int value);

/** @} */ // end of itu_widget_circleprogressbar

/** @defgroup itu_widget_scrollbar ScrollBar
 *  @{
 */
/**
 * Scroll bar widget definition.
 */
typedef struct ITUScrollBarTag
{
    ITUBackground bg;       ///< Base background widget definition.
    ITULayout layout;       ///< Layout. Supports ITU_LAYOUT_VERTICAL and ITU_LAYOUT_HORIZONTAL only.
    ITUColor fgColor;       ///< Color of scrolling bar.
    int pos;                ///< Current position of scrolling bar.
    int length;             ///< Total length of scrolling bar.

    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

    /**
     * Called when the position is changed.
     *
     * @param bar The scroll bar.
     * @param pos The new pos.
     * @param confirm true on mouse-up, false for value is changing.
     */
    void (*OnPositionChanged)(struct ITUScrollBarTag* bar, int pos, bool confirm);

} ITUScrollBar;

/**
 * Initializes the scroll bar widget.
 *
 * @param bar The scroll bar widget to initialize.
 */
void ituScrollBarInit(ITUScrollBar* bar);

/**
 * Loads the scroll bar widget. This is called by scene manager.
 *
 * @param bar The scroll bar widget to load.
 * @param base The address in the scene file buffer.
 */
void ituScrollBarLoad(ITUScrollBar* bar, uint32_t base);

/**
 * Updates the scroll bar widget by specified event.
 *
 * @param widget The scroll bar widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the scroll bar widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituScrollBarUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the scroll bar widget to the specified surface.
 *
 * @param widget The scroll bar widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituScrollBarDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The scroll bar widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituScrollBarOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Sets the scrolling length to the scroll bar widget.
 *
 * @param bar The scroll bar widget to set.
 * @param length Scrolling length.
 */
void ituScrollBarSetLength(ITUScrollBar* bar, int length);

/**
 * Sets the scrolling position to the scroll bar widget.
 *
 * @param bar The scroll bar widget to set.
 * @param pos Scrolling position. Must less than length.
 */
void ituScrollBarSetPosition(ITUScrollBar* bar, int pos);

/**
 * Called when the value is changed.
 *
 * @param bar The scroll bar.
 * @param pos The new position.
 * @param confirm true on mouse-up, false for position is changing.
 */
#define ituScrollBarOnPositionChanged(bar, pos, confirm)   ((ITUScrollBar*)(bar))->OnPositionChanged((ITUScrollBar*)(bar), (pos), (confirm))

/**
 * Sets OnPositionChanged callback function of scroll bar.
 *
 * @param tbar Pointer referring to the scroll bar.
 * @param onPositionChanged The callback function to set.
 */
#define ituScrollBarSetPositionChanged(bar, onPositionChanged)  (((ITUScrollBar*)(bar))->OnPositionChanged = (onPositionChanged))

/** @} */ // end of itu_widget_scrollbar

/** @defgroup itu_widget_animation Animation
 *  @{
 */
#define ITU_ANIM_MOVE               0x1     ///< Move animation.
#define ITU_ANIM_ROTATE             0x2     ///< Rotate animation.
#define ITU_ANIM_SCALE              0x4     ///< Scale animation.
#define ITU_ANIM_COLOR              0x8     ///< Color with alphablending animation.
#define ITU_ANIM_CYCLE              0x10    ///< Cycling animation.
#define ITU_ANIM_SCALE_CENTER       0x20    ///< Scale on center point.
#define ITU_ANIM_REVERSE            0x40    ///< Reverse play.
#define ITU_ANIM_TRANSFORM          0x80    ///< Transform animation.
#define ITU_ANIM_EASE_IN            0x100   ///< Cubic ease-in animation.
#define ITU_ANIM_EASE_OUT           0x200   ///< Cubic ease-out animation.
#define ITU_ANIM_MOTION_BLUR        0x400   ///< Mothin blur animation.

/**
 * Animation widget definition. This is used for drawing animation.
 * It is a container of multiple widgets to do the animation job.
 */
typedef struct ITUAnimationTag
{
    ITUWidget widget;                       ///< Base widget definition.
    unsigned int animationFlags;            ///< Flags for the animation type.
    int delay;                              ///< The delay on playing. Unit is depend on ITU_EVENT_TIMER event.
    int totalframe;                         ///< Total frame count of animation between keyframe.
    int repeat;                             ///< Is play repeatly or not.
    int playing;                            ///< Is playing or not.
    int frame;                              ///< Current frame.
    int keyframe;                           ///< Current keyframe.
    int delayCount;                         ///< The delay count to play next frame.
    ITUWidget* child;                       ///< Current frame of widget.
    ITURectangle orgRect;                   ///< Keep the orginal rectangle of widget
    ITUColor orgColor;                      ///< Keep the orginal color of widget    
    int orgAlpha;                           ///< Keep the orginal alpha value of widget
    int orgAngle;                           ///< Keep the orginal angle value of widget
    int orgTransformX;                      ///< Keep the orginal transform x value of widget
    int orgTransformY;                      ///< Keep the orginal transform y value of widget
	int orgFontWidth;                       ///< Keep the orginal width value of the text widget.
	int orgFontHeight;                      ///< Keep the orginal height value of the text widget.
    ITURectangle keyRect;                   ///< Keep the keyframe rectangle of widget
    ITUColor keyColor;                      ///< Keep the keyframe color of widget    
    int keyAlpha;                           ///< Keep the keyframe alpha value of widget
    int keyAngle;                           ///< Keep the keyframe angle value of widget
    int keyTransformX;                      ///< Keep the keyframe transform x value of widget
    int keyTransformY;                      ///< Keep the keyframe transform y value of widget
    int playCount;                          ///< Frame count to play
    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger
    /**
     * Called when animation stopped.
     *
     * @param animation The stopped animation widget.
     */
    void (*OnStop)(struct ITUAnimationTag* animation);

} ITUAnimation;

/**
 * Initializes the animation widget.
 *
 * @param animation The animation widget to initialize.
 */
void ituAnimationInit(ITUAnimation* animation);

/**
 * Loads the animation widget. This is called by scene manager.
 *
 * @param animation The animation widget to load.
 * @param base The address in the scene file buffer.
 */
void ituAnimationLoad(ITUAnimation* animation, uint32_t base);

/**
 * Clones the animation widget.
 *
 * @param widget The animation widget to clone.
 * @param cloned Retrieved animation widget.
 * @return true if clone is success, false otherwise.
 */
bool ituAnimationClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the animation widget by specified event.
 *
 * @param widget The animation widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the animation widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituAnimationUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the animation widget to the specified surface.
 *
 * @param widget The animation widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituAnimationDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The animation widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituAnimationOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * This is called when the animation stop playing.
 *
 * @param animation The animation widget.
 */
#define ituAnimationOnStop(animation) ((ITUAnimation*)(animation))->OnStop((ITUAnimation*)(animation))

/**
 * Sets OnStop callback function of animation.
 *
 * @param animation Pointer referring to the animation.
 * @param onStop The callback function to set.
 */
#define ituAnimationSetOnStop(animation, onStop)    (((ITUAnimation*)(animation))->OnStop = (onStop))

/**
 * Sets the delay value to the animation widget.
 *
 * @param animation The animation widget to set.
 * @param delay The delay on playing. Unit is depend on ITU_EVENT_TIMER event.
 */
void ituAnimationSetDelay(ITUAnimation* animation, int delay);

/**
 * Plays the animation widget.
 *
 * @param animation The animation widget to play.
 * @param keyframe The specified keyframe of animation widget to start playing. -1 indicates playing from current keyframe.
 */
void ituAnimationPlay(ITUAnimation* animation, int keyframe);

/**
 * Stops the playing animation widget.
 *
 * @param animation The playing animation widget to stop.
 */
void ituAnimationStop(ITUAnimation* animation);

/**
 * Gotos the specified keyframe of animation widget.
 *
 * @param animation The playing animation widget to goto.
 * @param keyframe The specified keyframe of animation widget to goto.
 */
void ituAnimationGoto(ITUAnimation* animation, int keyframe);

/**
 * Gotos the specified frame of animation widget.
 *
 * @param animation The playing animation widget to goto.
 * @param frame The specified frame of animation widget to goto.
 */
void ituAnimationGotoFrame(ITUAnimation* animation, int frame);

/**
 * Resets the animation widget.
 *
 * @param animation The animation widget to reset.
 */
void ituAnimationReset(ITUAnimation* animation);

/**
 * Reverse plays the animation widget.
 *
 * @param animation The animation widget to reverse play.
 * @param keyframe The specified keyframe of animation widget to start reverse playing. -1 indicates reverse playing from current keyframe.
 */
void ituAnimationReversePlay(ITUAnimation* animation, int keyframe);

/** @} */ // end of itu_widget_animation

/** @defgroup itu_widget_wheel Wheel
 *  @{
 */
#define ITU_WHEEL_CYCLE_ARR_LIMIT 256
#define ITU_WHEEL_FOCUS_FONT_FIX_POS 6
#define ITU_WHEEL_MAX_STRING_LEN 20
/* Wheel SPEED BASE range: 3 ~ 5 (4) */
#define ITU_WHEEL_STEP_SPEED_BASE 5
/* Wheel Motion Factor 20 40 (60) */
#define ITU_WHEEL_MOTION_FACTOR 20
#define ITU_WHEEL_MOTION_THRESHOLD 40
#define ITU_WHEEL_PROCESS_STAGE1 0.1f
#define ITU_WHEEL_PROCESS_STAGE2 0.2f
/* ITU_WHEEL_MAX_INIT_POWER range: 2.0 ~ 4.5 */
#define ITU_WHEEL_MAX_INIT_POWER 4.5
#define ITU_WHEEL_FOCUSFONT_POS_Y_FIX_DIV ITU_WHEEL_FOCUS_FONT_FIX_POS
#define ITU_WHEEL_SLIDE_INIT_FRAME 3

/**
 * Wheel widget definition.
 */
typedef struct ITUWheelTag
{
    ITUFlowWindow fwin;         ///< Base flow window definition.
    ITUColor focusColor;        ///< Focused text color
	ITUColor normalColor;       ///< Normal text color  
	int tempy;                  ///< temp use for y position
	int shift_one;              ///< self check to use shift not slide
	int sliding;                ///< self check sliding status
	int scal;                   ///< self check scal factor
	int moving_step;            ///< self check moving progress
	int inside;                 ///< self check inside
	int slide_step;             ///< step of each item when sliding
	int slide_itemcount;        ///< slide total item range
	int idle;                   ///< wheel idle state
    int focusIndex;             ///< The index of focused item
    int itemCount;              ///< Item count.
    int totalframe;             ///< Total frame count of animation.
    int inc;                    ///< Increment difference.
    int frame;                  ///< Current frame.
    int touchY;                 ///< Y coordinate of touch point.
    int touchCount;             ///< Count time for touch behavior.
    int fontHeight;             ///< Font height.
    int focusFontHeight;        ///< Font height of focused text.

	int cycle_tor;              ///< cycle mode or not
	int cycle_arr_count;        ///< the variable to calculate active cycle item realtime
	int maxci;                  ///< the variable to log max valid cycle index
	int minci;                  ///< the variable to log min valid cycle index
	int layout_ci;              ///< the variable to calculate layout shift for flowwindow
	int fix_count;              ///< the count to log the drag change item
	int focus_c;                ///< memo focus index for cycle mode.
	int focus_dev;              ///< the variable use for calculate font size animation.

	int fontsquare;             ///< the font square mode.
	int mouseup_change_factor;  ///< the change factor when mouseup
	int drag_change_factor;     ///< the change factor when dragging
	int drag_last_pos;          ///< the variable to log last focus item position
	int slide;                  ///< slide able or not
	int org_totalframe;         ///< use to backup the original totalframe
	int temp1;                  ///< temporary usage
	int temp2;                  ///< temporary usage
	int temp3;                  ///< temporary usage
	int temp4;                  ///< temporary usage
	int temp5;                  ///< temporary usage
	int temp6;                  ///< temporary usage

	int cycle_arr[ITU_WHEEL_CYCLE_ARR_LIMIT];

    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

    /**
     * Called when the value is changed.
     *
     * @param wheel The wheel.
     * @param value The new value.
     */
    void (*OnValueChanged)(struct ITUWheelTag* wheel, char* value);

} ITUWheel;

/**
 * Initializes the wheel widget.
 *
 * @param wheel The wheel widget to initialize.
 */
void ituWheelInit(ITUWheel* wheel);

/**
 * Loads the wheel widget. This is called by scene manager.
 *
 * @param wheel The wheel widget to load.
 * @param base The address in the scene file buffer.
 */
void ituWheelLoad(ITUWheel* wheel, uint32_t base);

/**
 * Updates the wheel widget by specified event.
 *
 * @param widget The wheel widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the wheel widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituWheelUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the wheel widget to the specified surface.
 *
 * @param widget The wheel widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituWheelDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Called when the value is changed.
 *
 * @param wheel The wheel.
 * @param value The new value.
 */
#define ituWheelOnValueChanged(wheel, value)   ((ITUWheel*)(wheel))->OnValueChanged((ITUWheel*)(wheel), (value))

/**
 * Sets OnValueChanged callback function of wheel.
 *
 * @param wheel Pointer referring to the wheel.
 * @param onValueChanged The callback function to set.
 */
#define ituWheelSetValueChanged(wheel, onValueChanged)  (((ITUWheel*)(wheel))->OnValueChanged = (onValueChanged))

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The wheel widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituWheelOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Gotos the previous item of the wheel widget.
 *
 * @param wheel The wheel widget to goto.
 */
void ituWheelPrev(ITUWheel* wheel);

/**
 * Gotos the next item of the wheel widget.
 *
 * @param wheel The wheel widget to goto.
 */
void ituWheelNext(ITUWheel* wheel);

/**
 * Gets the focus item of wheel widget.
 *
 * @param wheel The wheel widget.
 * @return the focus item. NULL if not focused item.
 */
ITUWidget* ituWheelGetFocusItem(ITUWheel* wheel);

/**
 * Gotos the specified index of item of the wheel widget.
 *
 * @param wheel The wheel widget to goto.
 * @param index The index of page to goto.
 */
void ituWheelGoto(ITUWheel* wheel, int index);

/**
* Set the specified scaling factor for the mouse move distance of the wheel widget.
*
* @param wheel The wheel widget.
* @param scal The scal default is 1 and could be >= 1.
*/
void ituWheelScal(ITUWheel* wheel, int scal);

/**
* Check the wheel is undel idle state or not when mouse up.
*
* @param wheel The wheel widget.
* @return true if the wheel is idle, false if the wheel is working.
*/
bool ituWheelCheckIdle(ITUWheel* wheel);

/**
* Get the wheel total item count.
*
* @param wheel The wheel widget.
* @return the itemcount.
*/
int ituWheelItemCount(ITUWheel* wheel);

/**
* Check the wheel is undel idle state or not when mouse up.
*
* @param wheel The wheel widget.
* @param stringarr The string array used to set.
* @param itemcount The new item count.(0 < itemcount <= stringarr_size or ITU_WHEEL_CYCLE_ARR_LIMIT)
* @return true if successful, false for fail.
*/
bool ituWheelSetItemTree(ITUWheel* wheel, char** stringarr, int itemcount);

/**
* Set the wheel slide able or not.
*
* @param wheel The wheel widget.
* @param slidable The slide enable or not.(true for enable, false for disable)
*/
void ituWheelSetSlidable(ITUWheel* wheel, bool slidable);

/** @} */ // end of itu_widget_wheel

/** @defgroup itu_widget_stepwheel StepWheel
*  @{
*/

/**
* StepWheel widget definition.
*/
typedef struct ITUStepWheelTag
{
	ITUFlowWindow fwin;         ///< Base flow window definition.
	ITUColor focusColor;        ///< Focused text color
	ITUColor normalColor;       ///< Normal text color  
	int tempy;                  ///< temp use for y position
	int shift_one;              ///< self check to use shift not slide
	int sliding;                ///< self check sliding status
	int scal;                   ///< self check scal factor
	int moving_step;            ///< self check moving progress
	int inside;                 ///< self check inside
	int slide_step;             ///< step of each item when sliding
	int slide_itemcount;        ///< slide total item range
	int idle;                   ///< wheel idle state
	int focusIndex;             ///< The index of focused item
	int itemCount;              ///< Item count.
	int totalframe;             ///< Total frame count of animation.
	int inc;                    ///< Increment difference.
	int frame;                  ///< Current frame.
	int touchY;                 ///< Y coordinate of touch point.
	int touchCount;             ///< Count time for touch behavior.
	int fontHeight;             ///< Font height.
	int focusFontHeight;        ///< Font height of focused text.
	int stepFontHeight1;         ///< Font height of the step item next to the focus.(stage1)
	int stepFontHeight2;         ///< Font height of the step item next to the focus.(stage2)
	int stepFontHeight3;         ///< Font height of the step item next to the focus.(stage3)
	int cycle_tor;              ///< cycle mode or not
	int cycle_arr_count;        ///< the variable to calculate active cycle item realtime
	int maxci;                  ///< the variable to log max valid cycle index
	int minci;                  ///< the variable to log min valid cycle index
	int layout_ci;              ///< the variable to calculate layout shift for flowwindow
	int fix_count;              ///< the count to log the drag change item
	int focus_c;                ///< memo focus index for cycle mode.
	int focus_dev;              ///< the variable use for calculate font size animation.

	int fontsquare;             ///< the font square mode.
	int mouseup_change_factor;  ///< the change factor when mouseup
	int drag_change_factor;     ///< the change factor when dragging
	int drag_last_pos;          ///< the variable to log last focus item position
	int slide;                  ///< slide able or not
	int org_totalframe;         ///< use to backup the original totalframe
	int temp1;                  ///< temporary usage
	int temp2;                  ///< temporary usage
	int temp3;                  ///< temporary usage
	int temp4;                  ///< temporary usage
	int temp5;                  ///< temporary usage
	int temp6;                  ///< temporary usage

	int cycle_arr[ITU_WHEEL_CYCLE_ARR_LIMIT];

	ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

	/**
	* Called when the value is changed.
	*
	* @param wheel The stepwheel.
	* @param value The new value.
	*/
	void(*OnValueChanged)(struct ITUStepWheelTag* wheel, char* value);

} ITUStepWheel;

/**
* Initializes the stepwheel widget.
*
* @param wheel The stepwheel widget to initialize.
*/
void ituStepWheelInit(ITUStepWheel* wheel);

/**
* Loads the stepwheel widget. This is called by scene manager.
*
* @param wheel The stepwheel widget to load.
* @param base The address in the scene file buffer.
*/
void ituStepWheelLoad(ITUStepWheel* wheel, uint32_t base);

/**
* Updates the stepwheel widget by specified event.
*
* @param widget The stepwheel widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the stepwheel widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituStepWheelUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Draws the stepwheel widget to the specified surface.
*
* @param widget The stepwheel widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituStepWheelDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
* Called when the value is changed.
*
* @param wheel The stepwheel.
* @param value The new value.
*/
#define ituStepWheelOnValueChanged(wheel, value)   ((ITUStepWheel*)(wheel))->OnValueChanged((ITUStepWheel*)(wheel), (value))

/**
* Sets OnValueChanged callback function of stepwheel.
*
* @param wheel Pointer referring to the stepwheel.
* @param onValueChanged The callback function to set.
*/
#define ituStepWheelSetValueChanged(wheel, onValueChanged)  (((ITUStepWheel*)(wheel))->OnValueChanged = (onValueChanged))

/**
* Do the specified action. This is triggered by other widget's event.
*
* @param widget The stepwheel widget to do the action.
* @param action The action to do.
* @param param The parameter of action.
*/
void ituStepWheelOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* Gotos the previous item of the stepwheel widget.
*
* @param wheel The stepwheel widget to goto.
*/
void ituStepWheelPrev(ITUStepWheel* wheel);

/**
* Gotos the next item of the stepwheel widget.
*
* @param wheel The stepwheel widget to goto.
*/
void ituStepWheelNext(ITUStepWheel* wheel);

/**
* Gets the focus item of stepwheel widget.
*
* @param wheel The stepwheel widget.
* @return the focus item. NULL if not focused item.
*/
ITUWidget* ituStepWheelGetFocusItem(ITUStepWheel* wheel);

/**
* Gotos the specified index of item of the stepwheel widget.
*
* @param wheel The stepwheel widget to goto.
* @param index The index of page to goto.
*/
void ituStepWheelGoto(ITUStepWheel* wheel, int index);

/**
* Set the specified scaling factor for the mouse move distance of the stepwheel widget.
*
* @param wheel The stepwheel widget.
* @param scal The scal default is 1 and could be >= 1.
*/
void ituStepWheelScal(ITUStepWheel* wheel, int scal);

/**
* Check the stepwheel is undel idle state or not when mouse up.
*
* @param wheel The stepwheel widget.
* @return true if the stepwheel is idle, false if the stepwheel is working.
*/
bool ituStepWheelCheckIdle(ITUStepWheel* wheel);

/**
* Get the stepwheel total item count.
*
* @param wheel The stepwheel widget.
* @return the itemcount.
*/
int ituStepWheelItemCount(ITUStepWheel* wheel);

/**
* Check the stepwheel is under idle state or not when mouse up.
*
* @param wheel The stepwheel widget.
* @param stringarr The string array used to set.
* @param itemcount The new item count.(0 < itemcount <= stringarr_size or ITU_WHEEL_CYCLE_ARR_LIMIT)
* @return true if successful, false for fail.
*/
bool ituStepWheelSetItemTree(ITUStepWheel* wheel, char** stringarr, int itemcount);

/**
* Set the stepwheel slide able or not.
*
* @param wheel The stepwheel widget.
* @param slidable The slide enable or not.(true for enable, false for disable)
*/
void ituStepWheelSetSlidable(ITUStepWheel* wheel, bool slidable);

/** @} */ // end of itu_widget_stepwheel

/** @defgroup itu_widget_coverflow CoverFlow
 *  @{
 */

#define ITU_COVERFLOW_CYCLE         0x1    ///< This cover flow is in cycle mode.
#define ITU_COVERFLOW_VERTICAL      0x2    ///< This cover flow is in vertical mode.
#define ITU_COVERFLOW_ENABLE_ALL    0x4    ///< Enables all items of this cover flow.
#define ITU_COVERFLOW_SLIDING       0x8    ///< This cover flow is under sliding.
#define ITU_COVERFLOW_DISABLE_MOVE_UNPRESS  0x100   ///< This cover flow won't unpress child under dragging move.
#define COVERFLOW_MAX_PROCARR_SIZE  10     ///< The queue array size for next(prev) action.
#define COVERFLOW_MIN_SFRAME_PERCENT_SPLIT 80  ///< The minimum start frame(percentage) when mouse up under split mode.
/**
 * CoverFlow widget definition.
 */
typedef struct ITUCoverFlowTag
{
	ITUFlowWindow fwin;             ///< Base flow window definition.
	unsigned int coverFlowFlags;    ///< Flags for the cover flow type.
	int focusIndex;                 ///< The index of focused item
	int totalframe;                 ///< Total frame count of animation.
	int inc;                        ///< Increment difference.
	int frame;                      ///< Current frame.
	int touchPos;                   ///< coordinate of touch point.
	int touchCount;                 ///< Count time for touch behavior.
	int bounceRatio;                ///< Bounce ratio.
	int boundaryAlign;              ///< Set to align the boundary or not under non-cycle-mode.
	int slideMaxCount;              ///< Slide max item count under non-cycle-mode.
	int slideCount;                 ///< Slide item progress under non-cycle-mode.
	int overlapsize;                ///< the overlap size
	int procArr[10];                ///< the proc array used to comp inside.
	uint32_t clock;                 ///< time clock log
	int movelog;                    ///< internal move log
	int split;                      ///< split distance for non-cycle mode.
	int slide_diff;                 ///< the slide diff temporary variable
	int mousedown_position;         ///< the variable for mousedown log
	int min_change_dist_factor;     ///< the minimum change factor when dragging (default:2 2~6)
	int init_drag_xy;               ///< the variable for drag init position
	int boundary_touch_memo;        ///< the variable to memo the position when touch boundary
	int org_totalframe;             ///< the variable to log the original totalframe
	int eye_motion;                 ///< the count of eye motion effect (default:0 0~2)
	int boundary_memo1;             ///< the memo for left-top side boundary position.
	int boundary_memo2;             ///< the memo for right-bottom side boundary position.
	int prevnext_trigger;           ///< the force trigger for prev/next when slidemaxcount is 0.
	int temp1;                      ///< for temporary usage
	int temp2;                      ///< for temporary usage
	int temp3;                      ///< for temporary usage
	int temp4;                      ///< for temporary usage
	int temp5;                      ///< for temporary usage
	int temp6;                      ///< for temporary usage

    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

    /**
     * Called when the cover is changed.
     *
     * @param coverflow The coverflow.
     * @param cover The new cover.
     */
    void (*OnCoverChanged)(struct ITUCoverFlowTag* coverflow, ITUWidget* cover);

} ITUCoverFlow;

/**
 * Initializes the coverflow widget.
 *
 * @param coverflow The coverflow widget to initialize.
 * @param layout Layout. Supports ITU_LAYOUT_VERTICAL and ITU_LAYOUT_HORIZONTAL only.
 */
void ituCoverFlowInit(ITUCoverFlow* coverflow, ITULayout layout);

/**
 * Loads the coverflow widget. This is called by scene manager.
 *
 * @param coverflow The coverflow widget to load.
 * @param base The address in the scene file buffer.
 */
void ituCoverFlowLoad(ITUCoverFlow* coverflow, uint32_t base);

/**
* Clones the coverflow widget.
*
* @param widget The coverflow widget to clone.
* @param cloned Retrieved cloned coverflow widget.
* @return true if clone is success, false otherwise.
*/
bool ituCoverFlowClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the coverflow widget by specified event.
 *
 * @param widget The coverflow widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the coverflow widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituCoverFlowUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the coverflow widget to the specified surface.
 *
 * @param widget The coverflow widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituCoverFlowDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The coverflow widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituCoverFlowOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Gotos the specified frame of coverflow widget.
 *
 * @param coverflow The coverflow widget to goto.
 * @param index The specified index of coverflow widget to goto.
 */
void ituCoverFlowGoto(ITUCoverFlow* coverFlow, int index);

/**
 * Gotos the previous item of the coverflow widget.
 *
 * @param coverflow The coverflow widget to goto.
 */
void ituCoverFlowPrev(ITUCoverFlow* coverflow);

/**
 * Gotos the next item of the coverflow widget.
 *
 * @param coverflow The coverflow widget to goto.
 */
void ituCoverFlowNext(ITUCoverFlow* coverflow);

/**
 * Called when the value is changed.
 *
 * @param coverflow The coverflow.
 * @param value The new value.
 */
#define ituCoverFlowOnCoverChanged(coverflow, cover)   ((ITUCoverFlow*)(coverflow))->OnCoverChanged((ITUCoverFlow*)(coverflow), (cover))

/**
 * Sets OnCoverChanged callback function of coverflow.
 *
 * @param coverflow Pointer referring to the coverflow.
 * @param onCoverChanged The callback function to set.
 */
#define ituCoverFlowSetCoverChanged(coverflow, onCoverChanged)  (((ITUCoverFlow*)(coverflow))->OnCoverChanged = (onCoverChanged))

/**
* Get the child count of coverflow widget.
*
* @param coverflow The coverflow widget.
* @return the child count of the coverflow widget.
*/
int CoverFlowGetVisibleChildCount(ITUCoverFlow* coverflow);

/**
* Get the child of coverflow widget.
*
* @param coverflow The coverflow widget.
* @param index The index of child that want to get.
* @return the child widget of the coverflow widget.
*/
ITUWidget* CoverFlowGetVisibleChild(ITUCoverFlow* coverflow, int index);

/**
* Get the index of the child that is the first order on the valid display range of Coverflow.
*
* @param coverflow The coverflow widget.
* @return index The index of child that is the first order on the valid display range of Coverflow. (-1 for cycle mode).
*/
int CoverFlowGetFirstDisplayIndex(ITUCoverFlow* coverflow);

/**
* Get the dragging distance of the Coverflow.
*
* @param coverflow The coverflow widget.
* @return the dragging distance (return 0 when the coverflow is not dragging or dragging back to the origin position).
*/
int CoverFlowGetDraggingDist(ITUCoverFlow* coverflow);

/**
* Check the coverflow widget is under idle or not.
*
* @param coverflow The coverflow widget.
* @return the idle status of the coverflow widget, true is idle, false is not.
*/
bool CoverFlowCheckIdle(ITUCoverFlow* coverflow);

/**
* Disable the unpress action when coverflow under move-dragging.
*
* @param coverflow The coverflow widget to set.
* @param disable True to disable and False to enable.
*/
void ituCoverFlowDisableMoveUnPress(ITUCoverFlow* coverFlow, bool disable);

/** @} */ // end of itu_widget_coverflow

/** @defgroup itu_widget_popupbutton PopupButton
 *  @{
 */
/**
 * Popup button widget definition
 */
typedef struct
{
    ITUButton btn;          ///< Base button widget definition
    int totalframe;         ///< Total frame count of animation.
    int incPercent;         ///< Percentage of increase.
    int alphaPercent;       ///< Percentage of transparent.
	int orgFontSize;
	int textScaleFactor;
    int frame;              ///< Current frame.
    ITURectangle orgRect;   ///< Keep the orginal rectangle of widget
    int orgAlpha;           ///< Keep the orginal alpha value of widget
} ITUPopupButton;

/**
 * Initializes the popup button widget.
 *
 * @param pbtn The popup button widget to initialize.
 */
void ituPopupButtonInit(ITUPopupButton* pbtn);

/**
 * Loads the popup button widget. This is called by scene manager.
 *
 * @param pbtn The popup button widget to load.
 * @param base The address in the scene file buffer.
 */
void ituPopupButtonLoad(ITUPopupButton* pbtn, uint32_t base);

/**
 * Clones the popup button widget.
 *
 * @param widget The popup button widget to clone.
 * @param cloned Retrieved cloned popup button widget.
 * @return true if clone is success, false otherwise.
 */
bool ituPopupButtonClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the popup button widget by specified event.
 *
 * @param widget The popup button widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the popup button widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituPopupButtonUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Do the specified action. This is triggered by other widget's event.
*
* @param widget The popup button widget to do the action.
* @param action The action to do.
* @param param The parameter of action.
*/
void ituPopupButtonOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* Resets the popup button widget.
*
* @param pbtn The popup button widget to reset.
*/
void ituPopupButtonReset(ITUPopupButton* pbtn);

/** @} */ // end of itu_widget_popupbutton

/** @defgroup itu_widget_scrolllistbox ScrollListBox
 *  @{
 */
/**
 * Scroll list box widget definition. This is a container of scroll text widgets to draw list of texts with scroll function.
 */
typedef struct ITUScrollListBoxTag
{
    ITUListBox listbox;         ///< Base list box widget definition.
    int totalframe;             ///< Total frame count of animation.
    int inc;                    ///< Increment difference.
    int frame;                  ///< Current frame.
    int preindex;               ///< Pre-select index.
    int touchY;                 ///< Y coordinate of touch point.
    int touchCount;             ///< Count time for touch behavior.

} ITUScrollListBox;

/**
 * Initializes the scroll list box widget.
 *
 * @param scrolllistbox The list box widget to initialize.
 * @param width The width of the list box widget.
 */
void ituScrollListBoxInit(ITUScrollListBox* scrolllistbox, int width);

/**
 * Loads the scroll list box widget. This is called by scene manager.
 *
 * @param scrolllistbox The list box widget to load.
 * @param base The address in the scene file buffer.
 */
void ituScrollListBoxLoad(ITUScrollListBox* scrolllistbox, uint32_t base);

/**
 * Updates the scroll list box widget by specified event.
 *
 * @param widget The scroll list box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the list box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituScrollListBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The scroll list box widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituScrollListBoxOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Gets the item count of scroll list box widget.
 *
 * @param scrolllistbox The scroll list box widget.
 * @return the item count.
 */
int ituScrollListBoxGetItemCount(ITUScrollListBox* scrolllistbox);

/**
 * Gets the first item of last page of scroll list box widget.
 *
 * @param scrolllistbox The scroll list box widget.
 * @return the first item of last page.
 */
ITCTree* ituScrollListBoxGetLastPageItem(ITUScrollListBox* scrolllistbox);

/**
 * Gets the first item of current page of scroll list box widget.
 *
 * @param scrolllistbox The scroll list box widget.
 * @return the first item of current page.
 */
ITCTree* ituScrollListBoxGetCurrPageItem(ITUScrollListBox* scrolllistbox);

/**
 * Gets the first item of next page of scroll list box widget.
 *
 * @param scrolllistbox The scroll list box widget.
 * @return the first item of next page.
 */
ITCTree* ituScrollListBoxGetNextPageItem(ITUScrollListBox* scrolllistbox);

/** @} */ // end of itu_widget_scrolllistbox

/** @defgroup itu_widget_smflistbox ScrollMediaFileListBox
 *  @{
 */
/**
 * Scroll media file list box widget definition. This is a container of scroll text widgets to draw list of texts with scroll function.
 */
typedef struct ITUScrollMediaFileListBoxTag
{
    ITUMediaFileListBox mflistbox;  ///< Base media file list box widget definition.
    int totalframe;                 ///< Total frame count of animation.
    int inc;                        ///< Increment difference.
    int frame;                      ///< Current frame.
    int preindex;                   ///< Pre-select index.
    int touchY;                     ///< Y coordinate of touch point.
    int touchCount;                 ///< Count time for touch behavior.

} ITUScrollMediaFileListBox;

/**
 * Initializes the scroll media file box widget.
 *
 * @param smflistbox The list box widget to initialize.
 * @param width The width of the list box widget.
 * @param path The drive path to list.
 */
void ituScrollMediaFileListBoxInit(ITUScrollMediaFileListBox* smflistbox, int width, char* path);

/**
 * Loads the scroll media file box widget. This is called by scene manager.
 *
 * @param smflistbox The list box widget to load.
 * @param base The address in the scene file buffer.
 */
void ituScrollMediaFileListBoxLoad(ITUScrollMediaFileListBox* smflistbox, uint32_t base);

/**
 * Updates the scroll media file box widget by specified event.
 *
 * @param widget The list box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the list box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituScrollMediaFileListBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The scroll media file list box widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituScrollMediaFileListBoxOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* Do loading page. This is called when user goto previous/next page.
*
* @param listbox The scroll media file list box widget to load page.
* @param pageIndex The page index to load.
*/
void ituScrollMediaFileListBoxOnLoadPage(ITUListBox* listbox, int pageIndex);

/**
 * Gets the item count of scroll media file box widget.
 *
 * @param smflistbox The scroll media file box widget.
 * @return the item count.
 */
int ituScrollMediaFileListBoxGetItemCount(ITUScrollMediaFileListBox* smflistbox);

/**
 * Gets the first item of last page of scroll media file box widget.
 *
 * @param smflistbox The scroll media file box widget.
 * @return the first item of last page.
 */
ITCTree* ituScrollMediaFileListBoxGetLastPageItem(ITUScrollMediaFileListBox* smflistbox);

/**
 * Gets the first item of current page of scroll media file box widget.
 *
 * @param smflistbox The scroll media file box widget.
 * @return the first item of current page.
 */
ITCTree* ituScrollMediaFileListBoxGetCurrPageItem(ITUScrollMediaFileListBox* smflistbox);

/**
 * Gets the first item of next page of scroll media file box widget.
 *
 * @param smflistbox The scroll media file box widget.
 * @return the first item of next page.
 */
ITCTree* ituScrollMediaFileListBoxGetNextPageItem(ITUScrollMediaFileListBox* smflistbox);

/** @} */ // end of itu_widget_smflistbox

/** @defgroup itu_widget_meter Meter
 *  @{
 */
/**
 * Meter widget definition.
 */
typedef struct
{
	ITUBackground bg;                           ///< Base background widget definition
	int value;                                  ///< Current value. Unit is percentage.
	char pointerName[ITU_WIDGET_NAME_SIZE];     ///< The name of icon widget to show the pointer.
	ITUIcon* pointerIcon;                       ///< The icon widget to show the pointer.    
	int pointerX;                               ///< Center X coordinate of pointer
	int pointerY;                               ///< Center Y coordinate of pointer
	int startAngle;                             ///< The angle of start position. Range from 0-360.
	int endAngle;                               ///< The angle of end position. Range from 0-360.
	int pressed;                                ///< Is pressed or not
	int maxValue;                               ///< Maximum value. Unit is percentage.
	int minRadius;                              ///< Minimum radius. Unit is pixels.
	int delay;                                  ///< The delay on playing. Unit is depend on ITU_EVENT_TIMER event.
	int delayCount;                             ///< The delay count to play next frame.
	int lastvalue;                              ///< The last value of meter.
	int shadowmode;                             ///< The animation shadow mode activing when meter pointer rotate fast.
	int diffvalue;                              ///< The local value to save rotate way.
	int shadow_loop_count;                      ///< The local value to save shadow loop count.
	int shadow_loop_max;                        ///< The maximum loop of shadow step for each value change.
	ITUSurface* cw_shadow[3];                   ///< The static surface to store the clockwise level 1-3 shadow for shadow mode.
	ITUSurface* acw_shadow[3];                  ///< The static surface to store the anti-clockwise level 1-3 shadow for shadow mode.
	ITUSurface* cw_surf[3];                     ///< The surface of clockwise level 1-3.
	ITUSurface* acw_surf[3];                    ///< The surface of anti-clockwise level 1-3.
	ITUAction actions[ITU_ACTIONS_SIZE];        ///< Actions for events to trigger

} ITUMeter;

/**
* Initializes the meter widget.
*
* @param meter The meter widget to initialize.
*/
void ituMeterInit(ITUMeter* meter);

/**
* Loads the meter widget. This is called by scene manager.
*
* @param meter The meter widget to load.
* @param base The address in the scene file buffer.
*/
void ituMeterLoad(ITUMeter* meter, uint32_t base);

/**
* Updates the meter widget by specified event.
*
* @param widget The meter widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the meter widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituMeterUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Draws the meter widget to the specified surface.
*
* @param widget The meter widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituMeterDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
* Sets the current value to the meter widget.
*
* @param meter The meter widget to set.
* @param value Current value. Unit is percentage.
*/
void ituMeterSetValue(ITUMeter* meter, int value);

/**
* Loads static shadow data to the meter widget.
*
* @param meter The Meter widget to load.
*/
void ituMeterLoadStaticData(ITUMeter* meter);

/**
* Set the shadow mode of the meter widget.
*
* @param meter The meter widget to set.
* @param sh_mode Shadow mode 1 is ON mode-1, 2 is ON mode-2, etc, 0 is OFF.
* @return 0 is setting fail or disable shadow mode, > 0 is setting successful and how much valid shadow blur range.
*/
int ituMeterSetShadowMode(ITUMeter* meter, int sh_mode);

/** @} */ // end of itu_widget_meter

/** @defgroup itu_widget_scrolliconlistbox ScrollIconListBox
 *  @{
 */
/**
 * Scroll icon list box widget definition. This is a container of scroll icon widgets to draw list of icons with scroll function.
 */
typedef struct ITUScrollIconListBoxTag
{
    ITUIconListBox listbox; ///< Base list box widget definition.
    int totalframe;         ///< Total frame count of animation.
    int inc;                ///< Increment difference.
    int frame;              ///< Current frame.
    int preindex;           ///< Pre-select index.
    int touchY;             ///< Y coordinate of touch point.
    int touchCount;         ///< Count time for touch behavior.

} ITUScrollIconListBox;

/**
 * Initializes the scroll icon list box widget.
 *
 * @param scrolliconlistbox The list box widget to initialize.
 * @param width The width of the list box widget.
 */
void ituScrollIconListBoxInit(ITUScrollIconListBox* scrolliconlistbox, int width);

/**
 * Loads the scroll icon list box widget. This is called by scene manager.
 *
 * @param scrolliconlistbox The list box widget to load.
 * @param base The address in the scene file buffer.
 */
void ituScrollIconListBoxLoad(ITUScrollIconListBox* scrolliconlistbox, uint32_t base);

/**
 * Updates the scroll icon list box widget by specified event.
 *
 * @param widget The scroll icon list box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the list box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituScrollIconListBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The scroll icon list box widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituScrollIconListBoxOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Gets the item count of scroll list box widget.
 *
 * @param scrolliconlistbox The scroll icon list box widget.
 * @return the item count.
 */
int ituScrollIconListBoxGetItemCount(ITUScrollIconListBox* scrolliconlistbox);

/**
* Sets the item select able or not of scroll icon list box widget.
*
* @param scrolliconlistbox The scroll icon list box widget.
* @param selectable The item select able or not.
*/
void ituScrollIconListBoxSetSelectable(ITUScrollIconListBox* scrolliconlistbox, bool selectable);

/**
 * Gets the first item of last page of scroll list box widget.
 *
 * @param scrolliconlistbox The scroll icon list box widget.
 * @return the first item of last page.
 */
ITCTree* ituScrollIconListBoxGetLastPageItem(ITUScrollIconListBox* scrolliconlistbox);

/**
 * Gets the first item of current page of scroll list box widget.
 *
 * @param scrolliconlistbox The scroll icon list box widget.
 * @return the first item of current page.
 */
ITCTree* ituScrollIconListBoxGetCurrPageItem(ITUScrollIconListBox* scrolliconlistbox);

/**
 * Gets the first item of next page of scroll list box widget.
 *
 * @param scrolliconlistbox The scroll icon list box widget.
 * @return the first item of next page.
 */
ITCTree* ituScrollIconListBoxGetNextPageItem(ITUScrollIconListBox* scrolliconlistbox);

/** @} */ // end of itu_widget_scrolliconlistbox

/** @defgroup itu_widget_video Video
 *  @{
 */
/**
 * Video widget definition. This is used for playing video.
 * It is a container of icon widgets to do the video job.
 */
typedef struct ITUVideoTag
{
    ITUWidget widget;               ///< Base widget definition.
    char filePath[ITU_PATH_MAX];    ///< The video file path
    int repeat;                     ///< Is play repeatly or not.
    int playing;                    ///< Is playing or not.
    int paused;                      ///>Is paused or not
    int volume;                     ///< Volume.

    /**
     * Called when video stopped.
     *
     * @param video The stopped video widget.
     */
    void (*OnStop)(struct ITUVideoTag* video);

} ITUVideo;

/**
 * Initializes the video widget.
 *
 * @param video The video widget to initialize.
 */
void ituVideoInit(ITUVideo* video);

/**
 * Loads the video widget. This is called by scene manager.
 *
 * @param video The video widget to load.
 * @param base The address in the scene file buffer.
 */
void ituVideoLoad(ITUVideo* video, uint32_t base);

/**
 * Updates the video widget by specified event.
 *
 * @param widget The video widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the video widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituVideoUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the video widget to the specified surface.
 *
 * @param widget The video widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituVideoDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The video widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituVideoOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * This is called when the video stop playing.
 *
 * @param video The video widget.
 */
#define ituVideoOnStop(video) ((ITUVideo*)(video))->OnStop((ITUVideo*)(video))

/**
 * Sets OnStop callback function of video.
 *
 * @param video Pointer referring to the video.
 * @param onStop The callback function to set.
 */
#define ituVideoSetOnStop(video, onStop)    (((ITUVideo*)(video))->OnStop = (onStop))

/**
 * Plays the video widget.
 *
 * @param video The video widget to play.
 * @param frame The specified frame of video widget to start playing. -1 indicates playing from current frame.
 */
void ituVideoPlay(ITUVideo* video, int frame);

/**
 * Stops the playing video widget.
 *
 * @param video The playing video widget to stop.
 */
void ituVideoStop(ITUVideo* video);

/**
 * Pause the playing video widget.
 *
 * @param video The playing video widget to pause.
 */
void ituVideoPause(ITUVideo* video);

/**
 * Gotos the specified progress percentage of video widget.
 *
 * @param video The playing video widget to goto.
 * @param percentage The specified progress percentage of video widget to goto.
 */
void ituVideoGoto(ITUVideo* video, int percentage);

/**
 * Gets the playing progress of video widget.
 *
 * @param video The playing video widget to get.
 * @return the percentage of playing progress.
 */
int ituVideoGetPlayingPercentage(ITUVideo* video);

void ituVideoSpeedUpDown(ITUVideo* video, float speed);


/** @} */ // end of itu_widget_video

/** @defgroup itu_widget_colorpicker Color picker
 *  @{
 */

#define ITU_COLORPICKER_PICKING     0x1    ///< This color picker is picking.
#define ITU_COLORPICKER_PICKED      0x2    ///< This color picker is in picked.

/**
 * Color picker widget definition. This is used for picking color.
 */
typedef struct
{
    ITUBackground bg;                       ///< Base background widget definition.
    unsigned int colorPickerFlags;          ///< The flags to indicate the status of the color picker.
    int touchX;                             ///< X coordinate of touch point.
    int touchY;                             ///< Y coordinate of touch point.
    ITUColor pickedColor;                   ///< Picked color

    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

} ITUColorPicker;

/**
 * Initializes the color picker widget.
 *
 * @param cp The color picker widget to initialize.
 */
void ituColorPickerInit(ITUColorPicker* cp);

/**
 * Loads the color picker widget. This is called by scene manager.
 *
 * @param cp The color picker widget to load.
 * @param base The address in the scene file buffer.
 */
void ituColorPickerLoad(ITUColorPicker* cp, uint32_t base);

/**
 * Updates the color picker widget by specified event.
 *
 * @param widget The list box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the list box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituColorPickerUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the color picker widget to the specified surface.
 *
 * @param widget The button widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituColorPickerDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/** @} */ // end of itu_widget_colorpicker

/** @defgroup itu_widget_imagecoverflow ImageCoverFlow
 *  @{
 */

#define ITU_IMAGECOVERFLOW_BUSYING          0x8		///< The image cover flow is busying
#define ITU_IMAGECOVERFLOW_CREATED          0x10	///< The image cover flow is created
#define ITU_IMAGECOVERFLOW_DESTROYING       0x20	///< The image cover flow is destroying
#define ITU_IMAGECOVERFLOW_CREATE_FINISHED  0x40	///< The image cover flow is creating finished
#define ITU_IMAGECOVERFLOW_RELEASING        0x80	///< The image cover flow is releasing
#define ITU_IMAGECOVERFLOW_INITED           0x100	///< The image cover flow is initialized

/**
 * ImageCoverFlow widget definition.
 */
typedef struct ITUImageCoverFlowTag
{
    ITUCoverFlow coverFlow;         ///< Base cover flow definition.
    char path[ITU_PATH_MAX];        ///< The drive path to load images
    int imageIndex;                 ///< The image index to load
    uint8_t* imageData;             ///< The image data
    uint32_t imageSize;             ///< The image size
	int itemCount;					///< Total item count
    char filePath[ITU_PATH_MAX];    ///< The file path of current loaded image

} ITUImageCoverFlow;

/**
 * Initializes the imagecoverflow widget.
 *
 * @param imagecoverflow The imagecoverflow widget to initialize.
 * @param layout Layout. Supports ITU_LAYOUT_VERTICAL and ITU_LAYOUT_HORIZONTAL only.
 * @param path The drive path to load images.
 */
void ituImageCoverFlowInit(ITUImageCoverFlow* imagecoverflow, ITULayout layout, char* path);

/**
 * Loads the imagecoverflow widget. This is called by scene manager.
 *
 * @param imagecoverflow The imagecoverflow widget to load.
 * @param base The address in the scene file buffer.
 */
void ituImageCoverFlowLoad(ITUImageCoverFlow* imagecoverflow, uint32_t base);

/**
 * Exits the imagecoverflow widget.
 *
 * @param widget The imagecoverflow widget to exit.
 */
void ituImageCoverFlowExit(ITUWidget* widget);

/**
 * Updates the imagecoverflow widget by specified event.
 *
 * @param widget The imagecoverflow widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the imagecoverflow widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituImageCoverFlowUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the imagecoverflow widget to the specified surface.
 *
 * @param widget The imagecoverflow widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituImageCoverFlowDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The imagecoverflow widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituImageCoverFlowOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Reloads the imagecoverflow widget.
 *
 * @param imagecoverflow The imagecoverflow widget to reload.
 */
void ituImageCoverFlowReload(ITUImageCoverFlow* imagecoverflow);

/** @} */ // end of itu_widget_imagecoverflow

/** @defgroup itu_widget_backgroundbutton BackgroundButton
 *  @{
 */
/**
 * BackgroundButton widget definition
 */
typedef struct
{
    ITUBackground bg;                       ///< Base background widget definition
	int mouseLogging;                       ///< Mouse logging or not
	int currentX;                           ///< Mouse logging X
	int currentY;                           ///< Mouse logging Y
    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger
} ITUBackgroundButton;

/**
 * Initializes the background button widget.
 *
 * @param bgbtn The background button widget to initialize.
 */
void ituBackgroundButtonInit(ITUBackgroundButton* bgbtn);

/**
 * Loads the background button widget. This is called by scene manager.
 *
 * @param bgbtn The background button widget to load.
 * @param base The address in the scene file buffer.
 */
void ituBackgroundButtonLoad(ITUBackgroundButton* bgbtn, uint32_t base);

/**
 * Updates the background button widget by specified event.
 *
 * @param widget The background button widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the background button widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituBackgroundButtonUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The background button widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituBackgroundButtonOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* get the mouse down location inside the background button
*
* @param widget The background button widget to do the action.
* @param x the x position.
* @param y the y position.
*/
void ituBackgroundButtonMouseDownXY(ITUWidget* widget, int* x, int* y);

/** @} */ // end of itu_widget_backgroundbutton

/** @defgroup itu_widget_ripplebackground RippleBackground
 *  @{
 */

#define ITU_RIPPLEBACKGROUND_DRAGGABLE    0x1    ///< This ripple background is draggable.
#define ITU_RIPPLEBACKGROUND_DROPPING     0x2    ///< This ripple background is dropping.
#define ITU_RIPPLEBACKGROUND_DRAGGING     0x4    ///< This ripple background is dragging.

typedef int16_t ripple_t;

/**
 * Ripple background widget definition.
 */
typedef struct
{
    ITUBackground bg;                       ///< Base background widget definition
    unsigned int rippleBackgroundFlags;     ///< The flags to indicate the status of the ripple background.
    int radius;                             ///< Radius of ripple.
    int range;                              ///< Range of ripple.
    int oldIndex;                           ///< Old index of ripple map.
    int newIndex;                           ///< New index of ripple map.
    ripple_t* rippleMap;                    ///< Ripple map.

} ITURippleBackground;

/**
 * Initializes the ripple background widget.
 *
 * @param rb The ripple background widget to initialize.
 */
void ituRippleBackgroundInit(ITURippleBackground* rb);

/**
 * Loads the ripple background widget. This is called by scene manager.
 *
 * @param rb The ripple background widget to load.
 * @param base The address in the scene file buffer.
 */
void ituRippleBackgroundLoad(ITURippleBackground* rb, uint32_t base);

/**
 * Exits the ripple background widget.
 *
 * @param widget The ripple background widget to exit.
 */
void ituRippleBackgroundExit(ITUWidget* widget);

/**
 * Updates the ripple background widget by specified event.
 *
 * @param widget The ripple background widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the ripple background widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituRippleBackgroundUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the ripple background widget to the specified surface.
 *
 * @param widget The ripple background widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituRippleBackgroundDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/** @} */ // end of itu_widget_ripple background

/** @defgroup itu_widget_curve Curve
 *  @{
 */

#define ITU_CURVE_MAX_POINT_COUNT   32  ///< Maximum point count.
#define ITU_CURVE_LINE_ONLY         0x1 ///< Draw line only
#define ITU_CURVE_STATIC            0x2 ///< Static curve

/**
 * Curve widget definition.
 */
typedef struct
{
    ITUBackground bg;                           ///< Base background widget definition
    unsigned int curveFlags;                    ///< The flags to indicate the status of the curve.
    int pointCount;                             ///< Total count of points
    int xPoints[ITU_CURVE_MAX_POINT_COUNT];     ///< The x coordinate of points
    int yPoints[ITU_CURVE_MAX_POINT_COUNT];     ///< The y coordinate of points
    ITUSurface* cacheSurf;                      ///< The cache surface.
	int* xPP;                                   ///< The malloc x points.(internal usage)
	int* yPP;                                   ///< The malloc y points.(internal usage)
} ITUCurve;

/**
 * Initializes the curve widget.
 *
 * @param curve The curve widget to initialize.
 */
void ituCurveInit(ITUCurve* curve);

/**
 * Loads the curve widget. This is called by scene manager.
 *
 * @param curve The curve widget to load.
 * @param base The address in the scene file buffer.
 */
void ituCurveLoad(ITUCurve* curve, uint32_t base);

/**
 * Exits the curve widget.
 *
 * @param widget The curve widget to exit.
 */
void ituCurveExit(ITUWidget* widget);

/**
 * Updates the curve widget by specified event.
 *
 * @param widget The curve widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the curve widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituCurveUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the curve widget to the specified surface.
 *
 * @param widget The curve widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituCurveDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
* Set the new point count of the curve widget.
*
* @param curve The curve widget to set.
* @param count The new point count.
*/
void ituCurveSetPointCount(ITUCurve* curve, int count);

/**
* Set the curve point array of the curve widget.
*
* @param curve The curve widget to set.
* @param pointX The curve x point array.
* @param pointY The curve y point array.
* @param count The point count.
*/
void ituCurveSetPoint(ITUCurve* curve, int* pointX, int* pointY, int count);

/** @} */ // end of itu_widget_curve

/** @defgroup itu_widget_tablelistbox TableListBox
 *  @{
 */
/**
 * Table list box widget definition. This is a container of table text widgets to draw list of texts with table function.
 */
typedef struct ITUTableListBoxTag
{
    ITUListBox listbox;         ///< Base list box widget definition.
    int totalframe;             ///< Total frame count of animation.
    int inc;                    ///< Increment difference.
    int frame;                  ///< Current frame.
    int preindex;               ///< Pre-select index.
    int touchY;                 ///< Y coordinate of touch point.
    int touchCount;             ///< Count time for touch behavior.
    int touchOffset;            ///< Offset of touch point.
    int slidePage;              ///< Slide page count.

} ITUTableListBox;

/**
 * Initializes the table list box widget.
 *
 * @param tablelistbox The list box widget to initialize.
 * @param width The width of the list box widget.
 */
void ituTableListBoxInit(ITUTableListBox* tablelistbox, int width);

/**
 * Loads the table list box widget. This is called by scene manager.
 *
 * @param tablelistbox The list box widget to load.
 * @param base The address in the scene file buffer.
 */
void ituTableListBoxLoad(ITUTableListBox* tablelistbox, uint32_t base);

/**
 * Updates the table list box widget by specified event.
 *
 * @param widget The table list box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the list box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituTableListBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The table list box widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituTableListBoxOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* Do loading page. This is called when user reload page.
*
* @param listbox The table list box widget to load page.
* @param pageIndex The page index to load.
*/
void ituTableListBoxOnLoadPage(ITUListBox* listbox, int pageIndex);

/**
 * Gets the item count of table list box widget.
 *
 * @param tablelistbox The table list box widget.
 * @return the item count.
 */
int ituTableListBoxGetItemCount(ITUTableListBox* tablelistbox);

/** @} */ // end of itu_widget_tablelistbox

/** @defgroup itu_widget_tableiconlistbox TableIconListBox
 *  @{
 */
/**
 * Table icon list box widget definition. This is a container of table icon widgets to draw list of icons with table function.
 */
typedef struct ITUTableIconListBoxTag
{
    ITUListBox listbox;         ///< Base list box widget definition.
	int totalframe;             ///< Total frame count of animation.
	int inc;                    ///< Increment difference.
	int frame;                  ///< Current frame.
	int preindex;               ///< Pre-select index.
	int touchY;                 ///< Y coordinate of touch point.
	int touchCount;             ///< Count time for touch behavior.
	int touchOffset;            ///< Offset of touch point.
	int slidePage;              ///< Slide page count.
	ITUSurface* staticSurfArray[ITU_ICON_LISTBOX_TYPE_COUNT];       ///< The static surfaces.
	ITUSurface* surfArray[ITU_ICON_LISTBOX_TYPE_COUNT];             ///< The icon surfaces.
	ITUSurface* focusStaticSurfArray[ITU_ICON_LISTBOX_TYPE_COUNT];  ///< The focus static surfaces.
	ITUSurface* focusSurfArray[ITU_ICON_LISTBOX_TYPE_COUNT];        ///< The focus icon surfaces.

} ITUTableIconListBox;

/**
 * Initializes the table icon list box widget.
 *
 * @param tableiconlistbox The list box widget to initialize.
 * @param width The width of the list box widget.
 */
void ituTableIconListBoxInit(ITUTableIconListBox* tableiconlistbox, int width);

/**
 * Loads the table icon list box widget. This is called by scene manager.
 *
 * @param tableiconlistbox The list box widget to load.
 * @param base The address in the scene file buffer.
 */
void ituTableIconListBoxLoad(ITUTableIconListBox* tableiconlistbox, uint32_t base);

/**
 * Updates the table icon list box widget by specified event.
 *
 * @param widget The table icon list box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the list box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituTableIconListBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The table icon list box widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituTableIconListBoxOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* Do loading page. This is called when user reload page.
*
* @param listbox The table icon list box widget to load page.
* @param pageIndex The page index to load.
*/
void ituTableIconListBoxOnLoadPage(ITUListBox* listbox, int pageIndex);

/**
* Loads static data to the table icon list box widget.
*
* @param tableiconlistbox The table icon list box widget to load.
*/
void ituTableIconListBoxLoadStaticData(ITUTableIconListBox* tableiconlistbox);

/**
* Releases surface of table icon list box widget.
*
* @param tableiconlistbox The table icon list box widget to release.
*/
void ituTableIconListBoxReleaseSurface(ITUTableIconListBox* tableiconlistbox);

/**
 * Gets the item count of table icon list box widget.
 *
 * @param tableiconlistbox The table icon list box widget.
 * @return the item count.
 */
int ituTableIconListBoxGetItemCount(ITUTableIconListBox* tableiconlistbox);

/**
 * Sets the icon index of specified item of table icon list box widget.
 *
 * @param tableiconlistbox The table icon list box widget.
 * @param itemIndex The specified item index.
 * @param iconIndex The icon index.
 */
void ituTableIconListBoxSetIconIndex(ITUTableIconListBox* tableiconlistbox, int itemIndex, int iconIndex);

/** @} */ // end of itu_widget_tableiconlistbox

/** @defgroup itu_widget_popupradiobox PopupRadioBox
 *  @{
 */
/**
 * Popup radio box widget definition
 */
typedef struct
{
    ITURadioBox rb;         ///< Base radio box widget definition
    int totalframe;         ///< Total frame count of animation.
    int incPercent;         ///< Percentage of increase.
    int alphaPercent;       ///< Percentage of transparent.
    int frame;              ///< Current frame.
    ITURectangle orgRect;   ///< Keep the orginal rectangle of widget
    int orgAlpha;           ///< Keep the orginal alpha value of widget
} ITUPopupRadioBox;

/**
 * Initializes the popup radio box widget.
 *
 * @param prb The popup radio box widget to initialize.
 */
void ituPopupRadioBoxInit(ITUPopupRadioBox* prb);

/**
 * Loads the popup radio box widget. This is called by scene manager.
 *
 * @param prb The popup radio box widget to load.
 * @param base The address in the scene file buffer.
 */
void ituPopupRadioBoxLoad(ITUPopupRadioBox* prb, uint32_t base);

/**
 * Updates the popup radio box widget by specified event.
 *
 * @param widget The popup radio box widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the popup radio box widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituPopupRadioBoxUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/** @} */ // end of itu_widget_popupradiobox

/** @defgroup itu_widget_langsprite LangSprite
 *  @{
 */
/**
 * Language sprite widget definition
 */
typedef struct
{
    ITUSprite sprite;   ///< Sprite
} ITULanguageSprite;

/**
 * Initializes the language sprite widget.
 *
 * @param ls The language sprite widget to initialize.
 */
void ituLanguageSpriteInit(ITULanguageSprite* ls);

/**
 * Loads the language sprite widget. This is called by scene manager.
 *
 * @param ls The language sprite widget to load.
 * @param base The address in the scene file buffer.
 */
void ituLanguageSpriteLoad(ITULanguageSprite* ls, uint32_t base);

/**
 * Updates the language sprite widget by specified event.
 *
 * @param widget The language sprite widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the language sprite widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituLanguageSpriteUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The sprite widget to do the action. It will bypass to current child widget.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituSpriteOnAction(ITUWidget* widget, ITUActionType action, char* param);

/** @} */ // end of itu_widget_langsprite

/** @defgroup itu_widget_pageflow PageFlow
 *  @{
 */

#define ITU_PAGEFLOW_CYCLE     0x1    ///< This page flow is in cycle mode.
#define ITU_PAGEFLOW_VERTICAL  0x2    ///< This page flow is in vertical mode.

/**
 * PageFlow widget definition.
 */
typedef struct ITUPageFlowTag
{
    ITUWidget widget;       ///< Base widget definition.
    unsigned int pageFlowFlags;    ///< Flags for the page flow type.
    ITUPageFlowType type;   ///< Type
	int axisShift;          ///< The axis shift pixel (default 0 for no shift from center)
	int effectRatio;        ///< The 3D effect ratio (range is 1 ~ 4)
    int focusIndex;         ///< The index of focused item
    int totalframe;         ///< Total frame count of animation.
    int inc;                ///< Increment difference.
    int frame;              ///< Current frame.
    int offset;             ///< Offset.
	int ratioflip2;         ///< The reference point ratio of FLIP2 effect.
	int touchPos;           ///< The position memo.
	int working;            ///< Memo the working status.
	int lastpos;            ///< The last position for special calculation.

    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

    /**
     * Called when the page is changed.
     *
     * @param pageflow The pageflow.
     * @param page The new page.
     */
    void (*OnPageChanged)(struct ITUPageFlowTag* pageflow, ITUWidget* page);
    
} ITUPageFlow;

/**
 * Initializes the pageflow widget.
 *
 * @param pageflow The pageflow widget to initialize.
 */
void ituPageFlowInit(ITUPageFlow* pageflow);

/**
 * Loads the pageflow widget. This is called by scene manager.
 *
 * @param pageflow The pageflow widget to load.
 * @param base The address in the scene file buffer.
 */
void ituPageFlowLoad(ITUPageFlow* pageflow, uint32_t base);

/**
 * Updates the pageflow widget by specified event.
 *
 * @param widget The pageflow widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the pageflow widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituPageFlowUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the pageflow widget to the specified surface.
 *
 * @param widget The pageflow widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituPageFlowDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The pageflow widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituPageFlowOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * Gotos the specified frame of pageflow widget.
 *
 * @param pageflow The pageflow widget to goto.
 * @param index The specified index of pageflow widget to goto.
 */
void ituPageFlowGoto(ITUPageFlow* pageFlow, int index);

/**
 * Gotos the previous item of the pageflow widget.
 *
 * @param pageflow The pageflow widget to goto.
 */
void ituPageFlowPrev(ITUPageFlow* pageflow);

/**
 * Gotos the next item of the pageflow widget.
 *
 * @param pageflow The pageflow widget to goto.
 */
void ituPageFlowNext(ITUPageFlow* pageflow);

/**
* Set the reference point ratio of FLIP2 effect.
*
* @param pageflow The pageflow widget to set.
* @param ratio The ratio of FLIP2 effect. (5 ~ 95, default is 50)
* @return 1 when set successfal, return 0 when fail, or return the current value if you set ratio -1.
*/
int ituPageFlowRatioFlip2(ITUPageFlow* pageFlow, int ratio);

/**
 * Called when the value is changed.
 *
 * @param pageflow The pageflow.
 * @param value The new value.
 */
#define ituPageFlowOnPageChanged(pageflow, page)   ((ITUPageFlow*)(pageflow))->OnPageChanged((ITUPageFlow*)(pageflow), (page))

/**
 * Sets OnPageChanged callback function of pageflow.
 *
 * @param pageflow Pointer referring to the pageflow.
 * @param onPageChanged The callback function to set.
 */
#define ituPageFlowSetPageChanged(pageflow, onPageChanged)  (((ITUPageFlow*)(pageflow))->OnPageChanged = (onPageChanged))

/** @} */ // end of itu_widget_pageflow

/** @defgroup itu_widget_shadow Shadow
 *  @{
 */
/**
 * Shadow widget definition. This is used for shadow container.
 */
typedef struct ITUShadowTag
{
    ITUWidget widget;                       ///< Base widget definition.
    char targetName[ITU_WIDGET_NAME_SIZE];  ///< The name of target widget to show the shadow.
    ITUWidget* target;                      ///< The target widget to show the shadow.
    ITUSurface* maskSurf;                   ///< The mask surface.
} ITUShadow;

/**
 * Initializes the shadow widget.
 *
 * @param shadow The shadow widget to initialize.
 */
void ituShadowInit(ITUShadow* shadow);

/**
 * Loads the shadow widget. This is called by scene manager.
 *
 * @param shadow The shadow widget to load.
 * @param base The address in the scene file buffer.
 */
void ituShadowLoad(ITUShadow* shadow, uint32_t base);

/**
 * Exits the shadow widget.
 *
 * @param widget The shadow widget to exit.
 */
void ituShadowExit(ITUWidget* widget);

/**
 * Clones the shadow widget.
 *
 * @param widget The shadow widget to clone.
 * @param cloned Retrieved cloned shadow widget.
 * @return true if clone is success, false otherwise.
 */
bool ituShadowClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the shadow widget by specified event.
 *
 * @param widget The shadow widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the shadow widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituShadowUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the shadow widget to the specified surface.
 *
 * @param widget The shadow widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituShadowDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/** @} */ // end of itu_widget_shadow

/** @defgroup itu_widget_container Container
 *  @{
 */

#define ITU_CONTAINER_GRID  0x1    ///< This container is in grid layout.

/**
 * Container widget definition.
 */
typedef struct
{
    ITUWidget widget;               ///< Base widget definition
    unsigned int containerFlags;    ///< The flags to indicate the status of the container.
    int gap;                        ///< The gap between children.
    int touchX;                     ///< X-coordinate of touch point.
    int touchY;                     ///< Y-coordinate of touch point.
    int touchCount;                 ///< Count time for touch behavior.
    int offsetX;                    ///< X-coordinate of offset.
    int offsetY;                    ///< Y-coordinate of offset.

} ITUContainer;

/**
 * Initializes the container widget.
 *
 * @param gc The container widget to initialize.
 */
void ituContainerInit(ITUContainer* container);

/**
 * Loads the container widget. This is called by scene manager.
 *
 * @param container The container widget to load.
 * @param base The address in the scene file buffer.
 */
void ituContainerLoad(ITUContainer* container, uint32_t base);

/**
 * Clones the container widget.
 *
 * @param widget The container widget to clone.
 * @param cloned Retrieved cloned container widget.
 * @return true if clone is success, false otherwise.
 */
bool ituContainerClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the container widget by specified event.
 *
 * @param widget The container widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the background widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituContainerUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The layer widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituContainerOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* Starts dragging widget on the container.
*
* @param container The container widget to start dragging.
*/
void ituContainerStartDragging(ITUContainer* container);

/**
* Ends dragging widget on the container.
*
* @param container The container widget to end dragging.
*/
void ituContainerEndDragging(ITUContainer* container);

/** @} */ // end of itu_widget_container

/** @defgroup itu_widget_audio Audio
 *  @{
 */

#define ITU_AUDIO_REPEAT        0x1    ///< This audio is play repeatly.
#define ITU_AUDIO_PLAYING       0x2    ///< This audio is playing.
#define ITU_AUDIO_VOLUME        0x4    ///< This audio is volume enabled.
#define ITU_AUDIO_STOPPING      0x8    ///< This audio is stopping.

/**
 * Audio widget definition. This is used for playing audio.
 */
typedef struct ITUAudioTag
{
    ITUWidget widget;                       ///< Base widget definition.
    unsigned int audioFlags;                ///< Flags for the audio type.
    char filePath[ITU_PATH_MAX];            ///< The audio file path
    int volume;                             ///< The audio volume, range is 0~100
    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

    /**
     * Called when audio stopped.
     *
     * @param audio The stopped audio widget.
     */
    void (*OnStop)(struct ITUAudioTag* audio);

} ITUAudio;

/**
 * Initializes the audio widget.
 *
 * @param audio The audio widget to initialize.
 */
void ituAudioInit(ITUAudio* audio);

/**
 * Loads the audio widget. This is called by scene manager.
 *
 * @param audio The audio widget to load.
 * @param base The address in the scene file buffer.
 */
void ituAudioLoad(ITUAudio* audio, uint32_t base);

/**
 * Updates the audio widget by specified event.
 *
 * @param widget The audio widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the audio widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituAudioUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The audio widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituAudioOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * This is called when the audio stop playing.
 *
 * @param audio The audio widget.
 */
#define ituAudioOnStop(audio) ((ITUAudio*)(audio))->OnStop((ITUAudio*)(audio))

/**
 * Sets OnStop callback function of audio.
 *
 * @param audio Pointer referring to the audio.
 * @param onStop The callback function to set.
 */
#define ituAudioSetOnStop(audio, onStop)    (((ITUAudio*)(audio))->OnStop = (onStop))

/**
 * Plays the audio widget.
 *
 * @param audio The audio widget to play.
 */
void ituAudioPlay(ITUAudio* audio);

/**
 * Stops the playing audio widget.
 *
 * @param audio The playing audio widget to stop.
 */
void ituAudioStop(ITUAudio* audio);

/** @} */ // end of itu_widget_audio

/** @defgroup itu_widget_slideshow Slideshow
 *  @{
 */
/**
 * Slideshow widget definition. This is used for slideshow children widgets.
 * It is a container of icon widgets to do the slideshow job.
 */
typedef struct ITUSlideshowTag
{
    ITUWidget widget;                       ///< Base widget definition.
    int delay;                              ///< The delay on playing. Unit is depend on ITU_EVENT_TIMER event.
    int repeat;                             ///< Is play repeatly or not.
    int playing;                            ///< Is playing or not.
    ITUEffectType effect;                   ///< Event type
    int effectSteps;                        ///< Total effect steps
    int childCount;                         ///< Total frames count.
    int frame;                              ///< Current frame.
    int delayCount;                         ///< The delay count to play next frame.
    ITUWidget* child;                       ///< Current frame of widget.
    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

    /**
     * Called when slideshow stopped.
     *
     * @param slideshow The stopped slideshow widget.
     */
    void (*OnStop)(struct ITUSlideshowTag* slideshow);

} ITUSlideshow;

/**
 * Initializes the slideshow widget.
 *
 * @param slideshow The slideshow widget to initialize.
 */
void ituSlideshowInit(ITUSlideshow* slideshow);

/**
 * Loads the slideshow widget. This is called by scene manager.
 *
 * @param slideshow The slideshow widget to load.
 * @param base The address in the scene file buffer.
 */
void ituSlideshowLoad(ITUSlideshow* slideshow, uint32_t base);

/**
 * Clones the slideshow widget.
 *
 * @param widget The slideshow widget to clone.
 * @param cloned Retrieved slideshow widget.
 * @return true if clone is success, false otherwise.
 */
bool ituSlideshowClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the slideshow widget by specified event.
 *
 * @param widget The slideshow widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the slideshow widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituSlideshowUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The slideshow widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituSlideshowOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * This is called when the slideshow stop playing.
 *
 * @param slideshow The slideshow widget.
 */
#define ituSlideshowOnStop(slideshow) ((ITUSlideshow*)(slideshow))->OnStop((ITUSlideshow*)(slideshow))

/**
 * Sets OnStop callback function of slideshow.
 *
 * @param slideshow Pointer referring to the slideshow.
 * @param onStop The callback function to set.
 */
#define ituSlideshowSetOnStop(slideshow, onStop)    (((ITUSlideshow*)(slideshow))->OnStop = (onStop))

/**
 * Sets the delay value to the slideshow widget.
 *
 * @param slideshow The slideshow widget to set.
 * @param delay The delay on playing. Unit is depend on ITU_EVENT_TIMER event.
 */
void ituSlideshowSetDelay(ITUSlideshow* slideshow, int delay);

/**
 * Plays the slideshow widget.
 *
 * @param slideshow The slideshow widget to play.
 * @param frame The specified frame of slideshow widget to start playing. -1 indicates playing from current frame.
 */
void ituSlideshowPlay(ITUSlideshow* slideshow, int frame);

/**
 * Stops the playing slideshow widget.
 *
 * @param slideshow The playing slideshow widget to stop.
 */
void ituSlideshowStop(ITUSlideshow* slideshow);

/**
 * Gotos the specified frame of slideshow widget.
 *
 * @param slideshow The playing slideshow widget to goto.
 * @param frame The specified frame of slideshow widget to goto.
 */
void ituSlideshowGoto(ITUSlideshow* slideshow, int frame);

/** @} */ // end of itu_widget_slideshow

/** @defgroup itu_widget_dragicon DragIcon
*  @{
*/
/**
* DragIcon widget definition
*/
typedef struct
{
	ITUWidget widget;       ///< Base widget definition.
	ITUSurface* staticIconSurf; ///< The static icon surface.
	ITUSurface* surf;       ///< The icon surface.
	ITUSurface* loadedSurf; ///< The loaded icon surface.
	char* filePath;         ///< Path to load image file.
	ITUSurface* staticBackSurf; ///< The static background surface.
	ITUSurface* bsurf;       ///< The background surface.
	ITUSurface* bloadedSurf; ///< The loaded background surface.
	char* bfilePath;         ///< Path to load background image file.
	int touchX;                 ///< X coordinate of touch point.
	int touchY;                 ///< Y coordinate of touch point.
	int working;              ///< The dragicon is working or not.
	int moveX;               ///< The move X when dragicon working.
	int moveY;               ///< The move Y when dragicon working.
	int frame;               ///< The current frame.
	int totalframe;          ///< The total frame to run when dragicon active.
	int pw;
	int ph;
	int tmp1;
	int tmp2;
	ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger
} ITUDragIcon;

/**
* Initializes the drag icon widget.
*
* @param dragicon The drag icon widget to initialize.
*/
void ituDragIconInit(ITUDragIcon* dragicon);

/**
* Loads the drag icon widget. This is called by scene manager.
*
* @param dragicon The drag icon widget to load.
* @param base The address in the scene file buffer.
*/
void ituDragIconLoad(ITUDragIcon* dragicon, uint32_t base);

/**
* Updates the drag icon widget by specified event.
*
* @param widget The drag icon widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the drag icon widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituDragIconUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* set press function for the drag icon widget by specified event.
*
* @param widget The drag icon widget to run press.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the drag icon widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituDragIconPress(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Do the specified action. This is triggered by other widget's event.
*
* @param widget The drag icon widget to do the action.
* @param action The action to do.
* @param param The parameter of action.
*/
void ituDragIconOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* get the mouse move shift from the center of drag icon widget.
*
* @param widget The drag icon widget to do the action.
* @param shiftX the x shift.
* @param shiftY the y shift.
*/
void ituDragIconMouseShift(ITUWidget* widget, int* shiftX, int* shiftY);

/**
* check the drag icon widget is working or not.
*
* @param widget The drag icon widget to check.
* @return true if the drag icon widget is working, false is not.
*/
bool ituDragIconIsWorking(ITUWidget* widget);

/**
* set the init background image size of drag icon widget under AniFrame mode.
*
* @param widget The drag icon widget to set.
* @param pw the percentage of width  (default is 25 --> that means 25% of original width)
* @param ph the percentage of height (default is 25 --> that means 25% of original height)
*/
void ituDragIconSetAniBSN(ITUWidget* widget, int pw, int ph);

/**
* force the drag icon widget reset
*
* @param widget The drag icon widget to reset.
*/
void ituDragIconReset(ITUWidget* widget);

/** @} */ // end of itu_widget_dragicon

/** @defgroup itu_widget_blur Blur
 *  @{
 */
/**
 * Blur widget definition. This is used for blur container.
 */
typedef struct ITUBlurTag
{
    ITUWidget widget;                       ///< Base widget definition.
    int factor;                             ///< The blur factor.
    int iter;                               ///< The blur iterator.
    ITUSurface* maskSurf;                   ///< The mask surface.
} ITUBlur;

/**
 * Initializes the blur widget.
 *
 * @param blur The blur widget to initialize.
 */
void ituBlurInit(ITUBlur* blur);

/**
 * Loads the blur widget. This is called by scene manager.
 *
 * @param blur The blur widget to load.
 * @param base The address in the scene file buffer.
 */
void ituBlurLoad(ITUBlur* blur, uint32_t base);

/**
 * Exits the blur widget.
 *
 * @param widget The blur widget to exit.
 */
void ituBlurExit(ITUWidget* widget);

/**
 * Clones the blur widget.
 *
 * @param widget The blur widget to clone.
 * @param cloned Retrieved cloned blur widget.
 * @return true if clone is success, false otherwise.
 */
bool ituBlurClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the blur widget by specified event.
 *
 * @param widget The blur widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the blur widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituBlurUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the blur widget to the specified surface.
 *
 * @param widget The blur widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituBlurDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/** @} */ // end of itu_widget_blur

/** @defgroup itu_widget_scalecoverflow ScaleCoverFlow
 *  @{
 */

/**
 * ScaleCoverFlow widget definition.
 */
typedef struct ITUScaleCoverFlowTag
{
    ITUCoverFlow coverFlow;         ///< Base cover flow definition.
    int factor;                     ///< The scale factor
    int itemCount;                  ///< Item count
    int itemWidth;                  ///< Item width
    int itemHeight;                 ///< Item height
    int itemPos;                    ///< Item position
	int offsetx;                    ///< internal use offsetx
	int offsety;                    ///< internal use offsety
	int orgFocusIndex;              ///< internal original focus index log
	int currentFocusIndex;          ///< internal current focus index log
	int draggable;                  ///< draggable
	int concentration;              ///< concentration shift award to focus item
	int layoutMemoX[ITU_WIDGET_CHILD_MAX];   ///< internal layout memo X array
	int layoutMemoY[ITU_WIDGET_CHILD_MAX];   ///< internal layout memo Y array
} ITUScaleCoverFlow;

/**
 * Initializes the scalecoverflow widget.
 *
 * @param scalecoverflow The scalecoverflow widget to initialize.
 * @param layout Layout. Supports ITU_LAYOUT_VERTICAL and ITU_LAYOUT_HORIZONTAL only.
 */
void ituScaleCoverFlowInit(ITUScaleCoverFlow* scalecoverflow, ITULayout layout);

/**
 * Loads the scalecoverflow widget. This is called by scene manager.
 *
 * @param scalecoverflow The scalecoverflow widget to load.
 * @param base The address in the scene file buffer.
 */
void ituScaleCoverFlowLoad(ITUScaleCoverFlow* scalecoverflow, uint32_t base);

/**
 * Updates the scalecoverflow widget by specified event.
 *
 * @param widget The scalecoverflow widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the scalecoverflow widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituScaleCoverFlowUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the scalecoverflow widget to the specified surface.
 *
 * @param widget The scalecoverflow widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituScaleCoverFlowDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
* Do the specified action. This is triggered by other widget's event.
*
* @param widget The scalecoverflow widget to do the action.
* @param action The action to do.
* @param param The parameter of action.
*/
void ituScaleCoverFlowOnAction(ITUWidget* widget, ITUActionType action, char* param);

/** @} */ // end of itu_widget_scalecoverflow

/** @defgroup itu_widget_wheelbackground WheelBackground
 *  @{
 */
/**
 * WheelBackground widget definition.
 */
typedef struct
{
    ITUBackground bg;                           ///< Base background widget definition
    int value;                                  ///< Current value. Unit is degree.
    int pressed;                                ///< Is pressed or not
    int pressedAngle;                           ///< Pressed angle
    int orgValue;                               ///< Origional value. Unit is degree.

    ITUAction actions[ITU_ACTIONS_SIZE];        ///< Actions for events to trigger

} ITUWheelBackground;

/**
 * Initializes the wheel background widget.
 *
 * @param wbg The wheel background widget to initialize.
 */
void ituWheelBackgroundInit(ITUWheelBackground* wbg);

/**
 * Loads the wheel background widget. This is called by scene manager.
 *
 * @param wbg The wheel background widget to load.
 * @param base The address in the scene file buffer.
 */
void ituWheelBackgroundLoad(ITUWheelBackground* wbg, uint32_t base);

/**
 * Updates the wheel background widget by specified event.
 *
 * @param widget The wheel background widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the wheel background widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituWheelBackgroundUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the wheel background widget to the specified surface.
 *
 * @param widget The wheel background widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituWheelBackgroundDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Sets the current value to the wheel background widget.
 *
 * @param wbg The wheel background widget to set.
 * @param value Current value. Unit is percentage.
 */
void ituWheelBackgroundSetValue(ITUWheelBackground* wbg, int value);

/** @} */ // end of itu_widget_wheelbackground

/** @defgroup itu_widget_drawpen DrawPen
*  @{
*/

#define ITU_DRAWPEN_WORKING    0x1    ///< This drawpen is working.
#define ITU_DRAWPEN_CLEAN      0x2    ///< This drawpen would be clean.
#define ITU_DRAWPEN_MOVING     0x4    ///< This drawpen is moving.
#define ITU_DRAWPEN_DRAWING    0x8    ///< This drawpen is drawing.
#define ITU_DRAWPEN_SIM_MODE   0x10   ///< This drawpen is under api mode.
#define ITU_DRAWPEN_INIT_DONE  0x20   ///< This drawpen is initialized.
#define ITU_DRAWPEN_NOT        0x40   ///< This drawpen disable mousedown.

/**
* DrawPen widget definition. This is used for DrawPen running.
*/
typedef struct ITUDrawPenTag
{
	ITUWidget widget;                       ///< Base widget definition.
	ITUSurface* surf;                       ///<  The surface to draw.
	ITUSurface* screensurf;                 ///<  The surface of screen.
	ITUSurface* bitsurf;                    ///<  The bitmap surface used to clear.
	ITUSurface* cursorsurf;                 ///<  The bitmap surface of cursor.
	ITUSurface* pensurf;                    ///<  The bitmap surface of pen.
	unsigned int drawpenFlags;              ///< Flags for the DrawPen type.
	int pressure;                           ///< The pressure of DrawPen. (<=0 is Auto mode).
	int pressure_rawdata;                   ///< The pressure rawdata value of DrawPen.
	int hasmove;                            ///< counter for moved.
	int px0;
	int py0;
	int px1;
	int py1;
	int px2;
	int py2;
	int px3;
	int py3;
	int last_pen_x;                         ///< The last X value of pen.
	int last_pen_y;                         ///< The last Y value of pen.
	int pen_x;                              ///< The X value of pen.
	int pen_y;                              ///< The Y value of pen.
	int usb_status;
	int usb_working;
	int usb_ready_to_rw;
	unsigned int usb_lock;
	unsigned long usb_thread;
	int* rawdata;                           ///< The raw data of pen.
	ITUColor lastcolor;
	ITUColor pencolor;                      ///< The pen color.
	ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

	/**
	* Called when DrawPen stopped.
	*
	* @param drawpen The stopped DrawPen widget.
	*/
	void(*OnStop)(struct ITUDrawPenTag* drawpen);

} ITUDrawPen;

/**
* Initializes the DrawPen widget.
*
* @param drawpen The DrawPen widget to initialize.
*/
void ituDrawPenInit(ITUDrawPen* drawpen);

/**
* Loads the DrawPen widget. This is called by scene manager.
*
* @param drawpen The DrawPen widget to load.
* @param base The address in the scene file buffer.
*/
void ituDrawPenLoad(ITUDrawPen* drawpen, uint32_t base);

/**
* Updates the DrawPen widget by specified event.
*
* @param widget The DrawPen widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the DrawPen widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituDrawPenUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Draws the DrawPen widget to the specified surface.
*
* @param widget The DrawPen widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituDrawPenDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
* Do the specified action. This is triggered by other widget's event.
*
* @param widget The DrawPen widget to do the action.
* @param action The action to do.
* @param param The parameter of action.
*/
void ituDrawPenOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* This is called when the DrawPen stop working.
*
* @param drawpen The DrawPen widget.
*/
#define ituDrawPenOnStop(drawpen) ((ITUDrawPen*)(drawpen))->OnStop((ITUDrawPen*)(drawpen))

/**
* Sets OnStop callback function of DrawPen.
*
* @param drawpen Pointer referring to the DrawPen.
* @param onStop The callback function to set.
*/
#define ituDrawPenSetOnStop(drawpen, onStop)    (((ITUDrawPen*)(drawpen))->OnStop = (onStop))

/**
* Make the DrawPen widget start to work.
*
* @param drawpen The DrawPen widget to work.
*/
void ituDrawPenPlay(ITUDrawPen* drawpen);

/**
* Stops the working DrawPen widget.
*
* @param drawpen The working DrawPen widget to stop.
*/
void ituDrawPenStop(ITUDrawPen* drawpen);

/**
* Dump the surface of DrawPen widget to JPEG file.
*
* @param drawpen The DrawPen widget to stop.
* @param percent The image compress rate.
* @param filename The output JPEG fully filename.
* @return true for dump success. fail for dump fail.
*/
bool ituDrawPenDumpJpeg(ITUDrawPen* drawpen, int percent, char* filename);

/**
* Set the colot for the pen of the DrawPen widget.
*
* @param drawpen The DrawPen widget.
* @param color The color of the pen.
*/
void ituDrawPenSetPenColor(ITUDrawPen* drawpen, ITUColor color);

/**
* Make the DrawPen widget USB protocol start or stop
*
* @param drawpen The DrawPen widget.
* @param usbtask USB protocol start or not. True for start, False for stop.
*/
void ituDrawPenUSBTask(ITUDrawPen* drawpen, bool usbtask);

/**
* Make the DrawPen widget start to get(change) background by usb.
*
* @param drawpen The DrawPen widget.
* @param filename The background filename.
*/
void ituDrawPenUSBGetBackground(ITUDrawPen* drawpen, char* filename);

/**
* Get the usb status of the DrawPen widget.
*
* @param drawpen The DrawPen widget.
* @return the status number.
*/
int ituDrawPenUSBStatus(ITUDrawPen* drawpen);

/**
* Send command ID to USB HOST with the DrawPen widget.
*
* @param drawpen The DrawPen widget.
* @param id The USB command ID.
*/
void ituDrawPenUSBSendID(ITUDrawPen* drawpen, unsigned int id);

/**
* Set the cursor of the DrawPen widget to show or not.
*
* @param drawpen The DrawPen widget.
* @param switchon The switch for cursor. True is Enable cursor and false for not.
*/
void ituDrawPenCursorSwitch(ITUDrawPen* drawpen, bool switchon);

/**
* Set the touch of the DrawPen widget to enable or not.
*
* @param drawpen The DrawPen widget.
* @param touch The switch for touch. True is Enable touch and false for not.
*/
void ituDrawPenTouchSwitch(ITUDrawPen* drawpen, bool touch);

/**
* Get the initialize status of the DrawPen widget.
*
* @param drawpen The DrawPen widget.
* @return True is initialized and False is not.
*/
bool ituDrawPenInitDone(ITUDrawPen* drawpen);

/**
* Get the working status of the DrawPen widget.
*
* @param drawpen The DrawPen widget.
* @return True is working and False is not.
*/
bool ituDrawPenIsWorking(ITUDrawPen* drawpen);

/**
* Make the DrawPen widget to draw point.
*
* @param drawpen The DrawPen widget.
* @param width The pen width.
* @param x The x point.
* @param y The y point.
*/
void ituDrawPenDrawPoint(ITUDrawPen* drawpen, int width, int x, int y);

/**
* Make the DrawPen widget to draw line.
*
* @param drawpen The DrawPen widget.
* @param width The line width.
* @param x1 The x for start point.
* @param y1 The y for start point.
* @param x2 The x for end point.
* @param y2 The y for end point.
*/
void ituDrawPenDrawLine(ITUDrawPen* drawpen, int width, int x1, int y1, int x2, int y2);

/** @} */ // end of itu_widget_drawpen

/** @defgroup itu_widget_wavebackground WaveBackground
*  @{
*/

/**
* Wave background widget definition.
*/
typedef struct
{
    ITUBackground bg;                       ///< Base background widget definition
    int xstep;                              ///< Steps of x-coordinate
    int ystep;                              ///< Steps of y-coordinate
    int ampitude;                           ///< Wave ampitude. Unit is pixels.
    int offsetx;                            ///< X coordinate of offset to start wave. Unit is pixels.
    int offsety;                            ///< Y coordinate of offset to start wave. Unit is pixels.
    int delay;                              ///< The delay on playing. Unit is depend on ITU_EVENT_TIMER event.
    int totalframe;                         ///< Total frame count.
    int frame;                              ///< Current frame.
    int delayCount;                         ///< The delay count to play next frame.
    int xcount;                             ///< The block count of X coordinate for computing.
    int ycount;                             ///< The block count of Y coordinate for computing.
    ITUPoint* waveMap;                      ///< Wave map.

} ITUWaveBackground;

/**
* Initializes the wave background widget.
*
* @param wave The wave background widget to initialize.
*/
void ituWaveBackgroundInit(ITUWaveBackground* wave);

/**
* Loads the wave background widget. This is called by scene manager.
*
* @param wave The wave background widget to load.
* @param base The address in the scene file buffer.
*/
void ituWaveBackgroundLoad(ITUWaveBackground* wave, uint32_t base);

/**
* Exits the wave background widget.
*
* @param widget The wave background widget to exit.
*/
void ituWaveBackgroundExit(ITUWidget* widget);

/**
* Updates the wave background widget by specified event.
*
* @param widget The wave background widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the wave background widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituWaveBackgroundUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Draws the wave background widget to the specified surface.
*
* @param widget The wave background widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituWaveBackgroundDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/** @} */ // end of itu_widget_wavebackground

/** @defgroup itu_widget_clipper Clipper
*  @{
*/
/**
* Clipper widget definition. This is used for drawing label.
*/
typedef struct
{
    ITUWidget widget;           ///< Base widget definition.
    int outside;             	///< Clipping outside or not.
    ITUSurface* maskSurf;       ///< The mask surface.
} ITUClipper;

/**
* Initializes the clipper widget.
*
* @param clipper The clipper widget to initialize.
*/
void ituClipperInit(ITUClipper* clipper);

/**
* Loads the clipper widget. This is called by scene manager.
*
* @param clipper The clipper widget to load.
* @param base The address in the scene file buffer.
*/
void ituClipperLoad(ITUClipper* clipper, uint32_t base);

/**
* Exits the clipper widget.
*
* @param widget The blur widget to exit.
*/
void ituClipperExit(ITUWidget* widget);

/**
* Clones the clipper widget.
*
* @param widget The clipper widget to clone.
* @param cloned Retrieved cloned clipper widget.
* @return true if clone is success, false otherwise.
*/
bool ituClipperClone(ITUWidget* widget, ITUWidget** cloned);

/**
* Updates the clipper widget by specified event.
*
* @param widget The clipper widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the clipper widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituClipperUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Draws the clipper widget to the specified surface.
*
* @param widget The clipper widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituClipperDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
* Post-draws the clipper widget to the specified surface.
*
* @param widget The clipper widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituClipperPostDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/** @} */ // end of itu_widget_clipper

/** @defgroup itu_widget_simpleanimation Simple Animation
*  @{
*/
/**
 * Simple animation widget definition. This is used for drawing sanim.
 * It is a container of 2 widgets to do the simple animation job.
 */
typedef struct ITUSimpleAnimationTag
{
    ITUWidget widget;                       ///< Base widget definition.
    unsigned int animationFlags;            ///< Flags for the simple animation type.
    int delay;                              ///< The delay on playing. Unit is depend on ITU_EVENT_TIMER event.
    int totalframe;                         ///< Total frame count of sanim between keyframe.
    int repeat;                             ///< Is play repeatly or not.
    int playing;                            ///< Is playing or not.
    int frame;                              ///< Current frame.
    int delayCount;                         ///< The delay count to play next frame.
    ITUWidget* child;                       ///< Current frame of widget.
    ITURectangle orgRect;                   ///< Keep the orginal rectangle of widget
    ITUColor orgColor;                      ///< Keep the orginal color of widget    
    int orgAlpha;                           ///< Keep the orginal alpha value of widget
    int orgAngle;                           ///< Keep the orginal angle value of widget
    int orgTransformX;                      ///< Keep the orginal transform x value of widget
    int orgTransformY;                      ///< Keep the orginal transform y value of widget
    ITURectangle keyRect;                   ///< Keep the keyframe rectangle of widget
    ITUColor keyColor;                      ///< Keep the keyframe color of widget    
    int keyAlpha;                           ///< Keep the keyframe alpha value of widget
    int keyAngle;                           ///< Keep the keyframe angle value of widget
    int keyTransformX;                      ///< Keep the keyframe transform x value of widget
    int keyTransformY;                      ///< Keep the keyframe transform y value of widget
    int playCount;                          ///< Frame count to play
    ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger
    /**
     * Called when sanim stopped.
     *
     * @param sanim The stopped simple animation widget.
     */
    void (*OnStop)(struct ITUSimpleAnimationTag* sanim);

} ITUSimpleAnimation;

/**
 * Initializes the simple animation widget.
 *
 * @param sanim The simple animation widget to initialize.
 */
void ituSimpleAnimationInit(ITUSimpleAnimation* sanim);

/**
 * Loads the simple animation widget. This is called by scene manager.
 *
 * @param sanim The simple animation widget to load.
 * @param base The address in the scene file buffer.
 */
void ituSimpleAnimationLoad(ITUSimpleAnimation* sanim, uint32_t base);

/**
 * Clones the simple animation widget.
 *
 * @param widget The simple animation widget to clone.
 * @param cloned Retrieved simple animation widget.
 * @return true if clone is success, false otherwise.
 */
bool ituSimpleAnimationClone(ITUWidget* widget, ITUWidget** cloned);

/**
 * Updates the simple animation widget by specified event.
 *
 * @param widget The simple animation widget to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if the simple animation widget is modified and need to be redraw, false if no need to be redraw.
 */
bool ituSimpleAnimationUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the simple animation widget to the specified surface.
 *
 * @param widget The simple animation widget to draw.
 * @param dest The surface to draw to.
 * @param x The x coordinate of destination surface, in pixels.
 * @param y The y coordinate of destination surface, in pixels.
 * @param alpha the alpha value to do the constant alphablending to the surface.
 */
void ituSimpleAnimationDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
 * Do the specified action. This is triggered by other widget's event.
 *
 * @param widget The simple animation widget to do the action.
 * @param action The action to do.
 * @param param The parameter of action.
 */
void ituSimpleAnimationOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
 * This is called when the simple animation stop playing.
 *
 * @param sanim The simple animation widget.
 */
#define ituSimpleAnimationOnStop(sanim) ((ITUSimpleAnimation*)(sanim))->OnStop((ITUSimpleAnimation*)(sanim))

/**
 * Sets OnStop callback function of simple animation.
 *
 * @param sanim Pointer referring to the simple animation.
 * @param onStop The callback function to set.
 */
#define ituSimpleAnimationSetOnStop(sanim, onStop)    (((ITUSimpleAnimation*)(sanim))->OnStop = (onStop))

/**
 * Sets the delay value to the simple animation widget.
 *
 * @param sanim The simple animation widget to set.
 * @param delay The delay on playing. Unit is depend on ITU_EVENT_TIMER event.
 */
void ituSimpleAnimationSetDelay(ITUSimpleAnimation* sanim, int delay);

/**
 * Plays the simple animation widget.
 *
 * @param sanim The simple animation widget to play.
 */
void ituSimpleAnimationPlay(ITUSimpleAnimation* sanim);

/**
 * Stops the playing simple animation widget.
 *
 * @param sanim The playing simple animation widget to stop.
 */
void ituSimpleAnimationStop(ITUSimpleAnimation* sanim);

/**
 * Gotos the specified frame of sanim widget.
 *
 * @param sanim The playing simple animation widget to goto.
 * @param frame The specified frame of simple animation widget to goto.
 */
void ituSimpleAnimationGoto(ITUSimpleAnimation* sanim, int frame);

/**
 * Resets the simple animation widget.
 *
 * @param sanim The simple animation widget to reset.
 */
void ituSimpleAnimationReset(ITUSimpleAnimation* sanim);

/**
 * Reverse plays the simple animation widget.
 *
 * @param sanim The simple animation widget to reverse play.
 */
void ituSimpleAnimationReversePlay(ITUSimpleAnimation* sanim);

/** @} */ // end of itu_widget_simpleanimation

/** @defgroup itu_widget_stopanywhere StopAnywhere
*  @{
*/

#define STOPANYWHERE_TEST 0  ///< The
#define ITU_STOPANYWHERE_BOUNCING_L 0x1     ///< This stopanywhere is bouncing left.
#define ITU_STOPANYWHERE_BOUNCING_R 0x2     ///< This stopanywhere is bouncing right.
#define ITU_STOPANYWHERE_BOUNCING_T 0x4     ///< This stopanywhere is bouncing top.
#define ITU_STOPANYWHERE_BOUNCING_B 0x8     ///< This stopanywhere is bouncing bottom.
#define ITU_STOPANYWHERE_BOUNCING   0x1000  ///< This stopanywhere is bouncing.
#define ITU_STOPANYWHERE_SLIDING    0x2000  ///< This stopanywhere is sliding.

/**
* StopAnywhere widget definition.
*/
typedef struct ITUStopAnywhereTag
{
	ITUWidget widget;               ///< Base widget definition
	unsigned int stopAnywhereFlags; ///< The flags to indicate the status of the stopanywhere.
	int gap;                        ///< The gap between children.
	int touchX;                     ///< X-coordinate of touch point.
	int touchY;                     ///< Y-coordinate of touch point.
	int touchCount;                 ///< Count time for touch behavior.
	int offsetX;                    ///< X-coordinate of offset.
	int offsetY;                    ///< Y-coordinate of offset.
	int childX;                     ///< X-coordinate of child.
	int childY;                     ///< Y-coordinate of child.
	int timerX;                     ///< The X for period timer record.
	int timerY;                     ///< The Y for period timer record.
	int slideStepX;                 ///< The X direction motion level.
	int slideStepY;                 ///< The Y direction motion level.
	int lastTimerX;                 ///< The current X position for each special timer.
	int lastTimerY;                 ///< The current Y position for each special timer.
	int timerCycleCount;            ///< The cycle count for each timer.
	int factorX;                    ///< The sliding animation X direction factor.
	int factorY;                    ///< The sliding animation Y direction factor.
	int frame;                      ///< The frame when doing sliding animation.
	int totalframe;                 ///< The totalframe for doing sliding animation.
	int initX;                      ///< The X position when start dragging.
	int initY;                      ///< The Y position when start dragging.
	int bounce;                     ///< The bouncing mode of stopanywhere.(0:disable, 1: Horizontal, 2: Vertical)
	int logXs;                      ///< Log for X step.(internal usage)
	int logYs;                      ///< Log for Y step.(internal usage)
	int hv_mode;                    ///< h or v mode.(internal usage)
	int logTP;                      ///< Log touch point for h/v.(internal usage)
	int logDrag;                    ///< Log for dragging info.(internal usage)
	int itemalignmode;              ///< The item aligm mode.(1:align 0:disable)
	uint32_t clock;                 ///< time clock log.
	uint32_t lastclock;             ///< last time clock log.
	ITUWidget* dragItem;            ///< The dragging item point.
	int dragPIPE[100];              ///< The drag pipe system usage.

	ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

	/**
	* Called when the stopanywhere is changed and stoped.
	*
	* @param saw The StopAnywhere.
	*/
	void(*OnChanged)(struct ITUStopAnywhereTag* saw);

	/**
	* Called when stopanywhere working then stopped.
	*
	* @param saw The stopped StopAnywhere widget.
	*/
	void(*OnStop)(struct ITUStopAnywhereTag* saw);

} ITUStopAnywhere;

/**
* Initializes the stopanywhere widget.
*
* @param saw The StopAnywhere widget to initialize.
*/
void ituStopAnywhereInit(ITUStopAnywhere* saw);

/**
* Loads the StopAnywhere widget. This is called by scene manager.
*
* @param saw The StopAnywhere widget to load.
* @param base The address in the scene file buffer.
*/
void ituStopAnywhereLoad(ITUStopAnywhere* saw, uint32_t base);

/**
* Updates the StopAnywhere widget by specified event.
*
* @param widget The StopAnywhere widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the StopAnywhere widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituStopAnywhereUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Draws the StopAnywhere widget to the specified surface.
*
* @param widget The StopAnywhere widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituStopAnywhereDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
* Do the specified action. This is triggered by other widget's event.
*
* @param widget The StopAnywhere widget to do the action.
* @param action The action to do.
* @param param The parameter of action.
*/
void ituStopAnywhereOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* Called when the value is changed.
*
* @param saw The StopAnywhere.
* @param value The new value.
*/
#define ituStopAnywhereOnChanged(saw)   ((ITUStopAnywhere*)(saw))->OnChanged((ITUStopAnywhere*)(saw))

/**
* Sets OnChanged callback function of StopAnywhere.
*
* @param saw Pointer referring to the StopAnywhere.
* @param onChanged The callback function to set.
*/
#define ituStopAnywhereSetChanged(saw, onChanged)  (((ITUStopAnywhere*)(saw))->OnChanged = (onChanged))

/**
* This is called when the StopAnywhere stop playing.
*
* @param saw The StopAnywhere widget.
*/
#define ituStopAnywhereOnStop(saw) ((ITUStopAnywhere*)(saw))->OnStop((ITUStopAnywhere*)(saw))

/**
* Sets OnStop callback function of StopAnywhere.
*
* @param saw Pointer referring to the StopAnywhere.
* @param onStop The callback function to set.
*/
#define ituStopAnywhereSetOnStop(saw, onStop)    (((ITUStopAnywhere*)(saw))->OnStop = (onStop))

/**
* Get X-position of the child of the StopAnywhere widget.
*
* @param saw The StopAnywhere widget.
* @return the X position.
*/
int ituStopAnywhereGetChildX(ITUStopAnywhere* saw);

/**
* Get Y-position of the child of the StopAnywhere widget.
*
* @param saw The StopAnywhere widget.
* @return the Y position.
*/
int ituStopAnywhereGetChildY(ITUStopAnywhere* saw);

/**
* Reset position of the child of the StopAnywhere widget.
*
* @param saw The StopAnywhere widget.
*/
void ituStopAnywhereResetXY(ITUStopAnywhere* saw);

/**
* Load config to change item sequence of the StopAnywhere widget.
*
* @param saw The StopAnywhere widget to change.
* @param pConfigWord The config string from config inifile.
* @return true if success. false if failed.
*/
bool ituStopAnywhereLoadTreeSeq(ITUStopAnywhere* saw, char* pConfigWord);

/**
* Sync the current tree to the config tree file of the StopAnywhere widget.
*
* @param saw The StopAnywhere widget.
* @param pConfigWord The config string from config inifile.
*/
void ituStopAnywhereSyncTreeData(ITUStopAnywhere* saw, char* pConfigWord);

/**
* Set the position of the StopAnywhere widget.
*
* @param saw The StopAnywhere widget.
* @param x The position X.
* @param y The position Y.
* @param line Debug info (use __LINE__).
*/
void ituStopAnywhereSetXY(ITUStopAnywhere* saw, int x, int y, int line);

/** @} */ // end of itu_widget_stopanywhere

/** @defgroup itu_widget_oscilloscope Oscilloscope
*  @{
*/

/**
* Oscilloscope widget definition.
*/
typedef struct
{
    ITUBackground bg;                       ///< Base background widget definition
    int resolutionX;                        ///< X coordinate of resolution
    int resolutionY;                        ///< Y coordinate of resolution
    int lineWidth;                          ///< Line width
    ITUColor fgColor;                       ///< Foreground color
    int gridResolutionX;                    ///< X coordinate of grid resolution
    int gridResolutionY;                    ///< Y coordinate of grid resolution
    int gridLineWidth;                      ///< Grid-line width
    ITUColor gridColor;                     ///< Grid color
    int* values;                            ///< Total values
    int offsetX;                            ///< Current X coordinate of offset

} ITUOscilloscope;

/**
* Initializes the oscilloscope widget.
*
* @param osc The oscilloscope widget to initialize.
*/
void ituOscilloscopeInit(ITUOscilloscope* osc);

/**
* Loads the oscilloscope widget. This is called by scene manager.
*
* @param osc The oscilloscope widget to load.
* @param base The address in the scene file buffer.
*/
void ituOscilloscopeLoad(ITUOscilloscope* osc, uint32_t base);

/**
* Exits the oscilloscope widget.
*
* @param widget The oscilloscope widget to exit.
*/
void ituOscilloscopeExit(ITUWidget* widget);

/**
* Clones the oscilloscope widget.
*
* @param widget The oscilloscope widget to clone.
* @param cloned Retrieved cloned oscilloscope widget.
* @return true if clone is success, false otherwise.
*/
bool ituOscilloscopeClone(ITUWidget* widget, ITUWidget** cloned);

/**
* Updates the oscilloscope widget by specified event.
*
* @param widget The oscilloscope widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the oscilloscope widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituOscilloscopeUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Draws the oscilloscope widget to the specified surface.
*
* @param widget The oscilloscope widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituOscilloscopeDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
* Inputs the value to oscilloscope widget.
*
* @param osc The oscilloscope widget to input.
* @param value The input value. Must in the range of Y resolution.
*/
void ituOscilloscopeInput(ITUOscilloscope* osc, int value);

/** @} */ // end of itu_widget_oscilloscope

/** @defgroup itu_widget_hwheel HWheel
*  @{
*/

/**
* HWheel widget definition.
*/
typedef struct ITUHWheelTag
{
	ITUFlowWindow fwin;         ///< Base flow window definition.
	ITUColor focusColor;        ///< Focused text color
	ITUColor normalColor;       ///< Normal text color  
	int tempy;                  ///< temp use for y position
	int shift_one;              ///< self check to use shift not slide
	int sliding;                ///< self check sliding status
	int scal;                   ///< self check scal factor
	int moving_step;            ///< self check moving progress
	int inside;                 ///< self check inside
	int slide_step;             ///< step of each item when sliding
	int slide_itemcount;        ///< slide total item range
	int idle;                   ///< wheel idle state
	int focusIndex;             ///< The index of focused item
	int itemCount;              ///< Item count.
	int totalframe;             ///< Total frame count of animation.
	int inc;                    ///< Increment difference.
	int frame;                  ///< Current frame.
	int touchY;                 ///< Y coordinate of touch point.
	int touchCount;             ///< Count time for touch behavior.
	int fontHeight;             ///< Font height.
	int focusFontHeight;        ///< Font height of focused text.

	int cycle_tor;              ///< cycle mode or not
	int cycle_arr_count;        ///< the variable to calculate active cycle item realtime
	int maxci;                  ///< the variable to log max valid cycle index
	int minci;                  ///< the variable to log min valid cycle index
	int layout_ci;              ///< the variable to calculate layout shift for flowwindow
	int fix_count;              ///< the count to log the drag change item
	int focus_c;                ///< memo focus index for cycle mode.
	int focus_dev;              ///< the variable use for calculate font size animation.

	int fontsquare;             ///< the font square mode.
	int mouseup_change_factor;  ///< the change factor when mouseup
	int drag_change_factor;     ///< the change factor when dragging
	int drag_last_pos;          ///< the variable to log last focus item position
	int slide;                  ///< slide able or not
	int org_totalframe;         ///< use to backup the original totalframe
	int temp1;                  ///< temporary usage
	int temp2;                  ///< temporary usage
	int temp3;                  ///< temporary usage
	int temp4;                  ///< temporary usage
	int temp5;                  ///< temporary usage
	int temp6;                  ///< temporary usage

	int cycle_arr[ITU_WHEEL_CYCLE_ARR_LIMIT];

	ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

	/**
	* Called when the value is changed.
	*
	* @param wheel The hwheel.
	* @param value The new value.
	*/
	void(*OnValueChanged)(struct ITUHWheelTag* wheel, char* value);

} ITUHWheel;

/**
* Initializes the hwheel widget.
*
* @param wheel The hwheel widget to initialize.
*/
void ituHWheelInit(ITUHWheel* wheel);

/**
* Loads the hwheel widget. This is called by scene manager.
*
* @param wheel The hwheel widget to load.
* @param base The address in the scene file buffer.
*/
void ituHWheelLoad(ITUHWheel* wheel, uint32_t base);

/**
* Updates the hwheel widget by specified event.
*
* @param widget The hwheel widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the hwheel widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituHWheelUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Draws the hwheel widget to the specified surface.
*
* @param widget The hwheel widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituHWheelDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
* Called when the value is changed.
*
* @param wheel The hwheel.
* @param value The new value.
*/
#define ituHWheelOnValueChanged(wheel, value)   ((ITUHWheel*)(wheel))->OnValueChanged((ITUHWheel*)(wheel), (value))

/**
* Sets OnValueChanged callback function of hwheel.
*
* @param wheel Pointer referring to the hwheel.
* @param onValueChanged The callback function to set.
*/
#define ituHWheelSetValueChanged(wheel, onValueChanged)  (((ITUHWheel*)(wheel))->OnValueChanged = (onValueChanged))

/**
* Do the specified action. This is triggered by other widget's event.
*
* @param widget The hwheel widget to do the action.
* @param action The action to do.
* @param param The parameter of action.
*/
void ituHWheelOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* Gotos the previous item of the hwheel widget.
*
* @param wheel The hwheel widget to goto.
*/
void ituHWheelPrev(ITUHWheel* wheel);

/**
* Gotos the next item of the hwheel widget.
*
* @param wheel The hwheel widget to goto.
*/
void ituHWheelNext(ITUHWheel* wheel);

/**
* Gets the focus item of hwheel widget.
*
* @param wheel The hwheel widget.
* @return the focus item. NULL if not focused item.
*/
ITUWidget* ituHWheelGetFocusItem(ITUHWheel* wheel);

/**
* Gotos the specified index of item of the hwheel widget.
*
* @param wheel The hwheel widget to goto.
* @param index The index of page to goto.
*/
void ituHWheelGoto(ITUHWheel* wheel, int index);

/**
* Set the specified scaling factor for the mouse move distance of the hwheel widget.
*
* @param wheel The hwheel widget.
* @param scal The scal default is 1 and could be >= 1.
*/
void ituHWheelScal(ITUHWheel* wheel, int scal);

/**
* Check the hwheel is undel idle state or not when mouse up.
*
* @param wheel The hwheel widget.
* @return true if the wheel is idle, false if the hwheel is working.
*/
bool ituHWheelCheckIdle(ITUHWheel* wheel);

/**
* Get the hwheel total item count.
*
* @param wheel The hwheel widget.
* @return the itemcount.
*/
int ituHWheelItemCount(ITUHWheel* wheel);

/**
* Check the hwheel is undel idle state or not when mouse up.
*
* @param wheel The hwheel widget.
* @param stringarr The string array used to set.
* @param itemcount The new item count.(0 < itemcount <= stringarr_size or ITU_WHEEL_CYCLE_ARR_LIMIT)
* @return true if successful, false for fail.
*/
bool ituHWheelSetItemTree(ITUHWheel* wheel, char** stringarr, int itemcount);

/**
* Set the hwheel slide able or not.
*
* @param wheel The hwheel widget.
* @param slidable The slide enable or not.(true for enable, false for disable)
*/
void ituHWheelSetSlidable(ITUHWheel* wheel, bool slidable);

/** @} */ // end of itu_widget_hwheel

/** @defgroup itu_widget_tablebar TableBar
*  @{
*/
/**
* Table bar widget definition.
*/
typedef struct ITUTableBarTag
{
    ITUBackground bg;                               ///< Base background widget definition.
    ITUColor fgColor;                               ///< Color of tableing bar.
    int pos;                                        ///< Current position of tableing bar.
    int length;                                     ///< Total length of tableing bar.
    int hideDelay;                                  ///< The delay to hide when stopped. Unit is depend on ITU_EVENT_TIMER event.
    int hideDelayCount;                             ///< The delay count to hide when stopped.
    int fadeOutDelay;                               ///< The delay to fade-out when hidden. Unit is depend on ITU_EVENT_TIMER event.
    int fadeOutDelayCount;                          ///< The delay count to fade-out when hidden.
    char tableListBoxName[ITU_WIDGET_NAME_SIZE];    ///< The name of table list box widget to show the progress.
    ITUTableListBox* tableListBox;                  ///< The table list box widget to show the progress.


} ITUTableBar;

/**
* Initializes the table bar widget.
*
* @param bar The table bar widget to initialize.
*/
void ituTableBarInit(ITUTableBar* bar);

/**
* Loads the table bar widget. This is called by scene manager.
*
* @param bar The table bar widget to load.
* @param base The address in the scene file buffer.
*/
void ituTableBarLoad(ITUTableBar* bar, uint32_t base);

/**
* Updates the table bar widget by specified event.
*
* @param widget The table bar widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the table bar widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituTableBarUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Draws the table bar widget to the specified surface.
*
* @param widget The table bar widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituTableBarDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
* Do the specified action. This is triggered by other widget's event.
*
* @param widget The table bar widget to do the action.
* @param action The action to do.
* @param param The parameter of action.
*/
void ituTableBarOnAction(ITUWidget* widget, ITUActionType action, char* param);

/** @} */ // end of itu_widget_tablebar

/** @defgroup itu_widget_tablegrid TableGrid
*  @{
*/

#define ITU_TABLEGRID_SINGLE_SELECT    0x1    ///< Select a item only on selection.

/**
* Table grid widget definition.
*/
typedef struct ITUTableGridTag
{
    ITUBackground bg;                               ///< Base background widget definition.
    unsigned int tableGridFlags;                    ///< Flags for the table grid.
    ITUColor lineColor;                             ///< Color of grid line.
    int lineSize;                                   ///< Grid line size.
    int lastRow;                                    ///< Last to add row position.
    int lastColumn;                                 ///< Last to add column position.
    int selectRow;                                  ///< Selected row position.
    int selectColumn;                               ///< Selected column position.

    ITUAction actions[ITU_ACTIONS_SIZE];            ///< Actions for events to trigger
} ITUTableGrid;

/**
* Initializes the table grid widget.
*
* @param grid The table grid widget to initialize.
*/
void ituTableGridInit(ITUTableGrid* grid);

/**
* Loads the table grid widget. This is called by scene manager.
*
* @param grid The table grid widget to load.
* @param base The address in the scene file buffer.
*/
void ituTableGridLoad(ITUTableGrid* grid, uint32_t base);

/**
* Exits the table grid widget.
*
* @param widget The table grid widget to exit.
*/
void ituTableGridExit(ITUWidget* widget);

/**
* Updates the table grid widget by specified event.
*
* @param widget The table grid widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the table grid widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituTableGridUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Draws the table grid widget to the specified surface.
*
* @param widget The table grid widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituTableGridDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
* Do the specified action. This is triggered by other widget's event.
*
* @param widget The table grid widget to do the action.
* @param action The action to do.
* @param param The parameter of action.
*/
void ituTableGridOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* Adds item to table grid.
*
* @param grid The table grid widget.
* @param templateItem The template item widget to add.
* @param clone Clone the template item or not.
* @return the Added item.
*/
ITUWidget* ituTableGridAddItem(ITUTableGrid* grid, ITUWidget* templateItem, bool clone);

/**
* Adds all items to table grid.
*
* @param grid The table grid widget.
* @param parent The parent widget of all items to add.
* @param clone Clone the template item or not.
*/
void ituTableGridAddAll(ITUTableGrid* grid, ITUWidget* parent, bool clone);

/**
* Gets item of table grid.
*
* @param grid The table grid widget.
* @param row The row of grid to set.
* @param col The column of grid to set.
* @return the item to get.
*/
ITUWidget* ituTableGridGetItem(ITUTableGrid* grid, int row, int col);

/**
* Sets item to table grid.
*
* @param grid The table grid widget.
* @param templateItem The template item widget to set.
* @param clone Clone the template item or not.
* @param row The row of grid to set.
* @param col The column of grid to set.
* @return the set item.
*/
ITUWidget* ituTableGridSetItem(ITUTableGrid* grid, ITUWidget* templateItem, bool clone, int row, int col);

/**
* Selects item of table grid.
*
* @param grid The table grid widget.
* @param row The row of grid to select. -1 to unselect.
* @param col The column of grid to select.
* @return the set item.
*/
void ituTableGridSelect(ITUTableGrid* grid, int row, int col);

/** @} */ // end of itu_widget_tablegrid

/** @defgroup itu_widget_combotable ComboTable
*  @{
*/

#define ITU_COMBOTABLE_MIT  200  //check time(msec) for click item
#define ITU_COMBOTABLE_SLIDING  0x1  ///< This combotable is under sliding.

typedef enum
{
	ITU_CBT_ERR_NOCHILD = 1, ///< without initial child
	ITU_CBT_ERR_CHILDNOTCOT = 2, ///< first child not container
} ITUComboTableErrNO;

/**
* ComboTable widget definition.
*/
typedef struct ITUComboTableTag
{
	ITUWidget widget;               ///< Base widget definition
	unsigned int comboTableFlags;   ///< The flags to indicate the status of the combotable.
	ITUSurface* staticSurf;         ///< The static surface for the background of the combotable.
	ITUSurface* surf;               ///< The work surface for the background of the combotable.
	ITUSurface* itemStaticSurf;     ///< The static surface for the item of the combotable.
	ITUSurface* itemSurf;           ///< The work surface for the item of the combotable.
	int gap;                        ///< The gap between item.
	int frame;                      ///< The frame when doing sliding animation.
	int totalframe;                 ///< The totalframe for doing sliding animation.
	int bounce;                     ///< The bouncing mode of combotable.(0:disable, 1: Horizontal, 2: Vertical)
	int hv_mode;                    ///< h or v mode.(internal usage)
	int inc;                        ///< Log for dragging info.(internal usage)
	int initX;                      ///< The X position when start dragging.
	int initY;                      ///< The Y position when start dragging.
	int touchX;                     ///< X-coordinate of touch point.
	int touchY;                     ///< Y-coordinate of touch point.
	int itemalignmode;              ///< The item aligm mode.(1:align 0:disable)
	uint32_t clock;                 ///< time clock log.
	uint32_t lastclock;             ///< last time clock log.
	ITUWidget* tmpObj;              ///< exchange object.(internal usage)
	ITUWidget* itemArr[ITU_WIDGET_CHILD_MAX];      ///< The main item array for each combotable.(internal usage)
	int itemcount;                  ///< available item count.(internal usage)
	int pagemaxitem;                ///< The max item count for each page.(default 20)
	int currPage;                   ///< The current page.(internal usage)//lastselectIndex
	int lastselectIndex;            ///< The index of the item that last selected.
	int selectIndex;                ///< The index of the item that current selected.
	int sliding;                    ///< The combotable could be sliding or not.(internal usage)
	int dragPIPE[100];              ///< The drag pipe system usage.

	ITUAction actions[ITU_ACTIONS_SIZE];    ///< Actions for events to trigger

	/**
	* Called when the combotable is changed and stoped.
	*
	* @param cbt The ComboTable.
	*/
	void(*OnChanged)(struct ITUComboTableTag* cbt);

	/**
	* Called when combotable working then stopped.
	*
	* @param cbt The stopped ComboTable widget.
	*/
	void(*OnStop)(struct ITUComboTableTag* cbt);

} ITUComboTable;

/**
* Initializes the ComboTable widget.
*
* @param cbt The ComboTable widget to initialize.
*/
void ituComboTableInit(ITUComboTable* cbt);

/**
* Loads the ComboTable widget. This is called by scene manager.
*
* @param cbt The ComboTable widget to load.
* @param base The address in the scene file buffer.
*/
void ituComboTableLoad(ITUComboTable* cbt, uint32_t base);

/**
* Updates the ComboTable widget by specified event.
*
* @param widget The ComboTable widget to update.
* @param ev The event to notify.
* @param arg1 The event related argument #1.
* @param arg2 The event related argument #2.
* @param arg3 The event related argument #3.
* @return true if the ComboTable widget is modified and need to be redraw, false if no need to be redraw.
*/
bool ituComboTableUpdate(ITUWidget* widget, ITUEvent ev, int arg1, int arg2, int arg3);

/**
* Draws the ComboTable widget to the specified surface.
*
* @param widget The ComboTable widget to draw.
* @param dest The surface to draw to.
* @param x The x coordinate of destination surface, in pixels.
* @param y The y coordinate of destination surface, in pixels.
* @param alpha the alpha value to do the constant alphablending to the surface.
*/
void ituComboTableDraw(ITUWidget* widget, ITUSurface* dest, int x, int y, uint8_t alpha);

/**
* Do the specified action. This is triggered by other widget's event.
*
* @param widget The ComboTable widget to do the action.
* @param action The action to do.
* @param param The parameter of action.
*/
void ituComboTableOnAction(ITUWidget* widget, ITUActionType action, char* param);

/**
* Called when the value is changed.
*
* @param cbt The ComboTable.
* @param value The new value.
*/
#define ituComboTableOnChanged(saw)   ((ITUComboTable*)(cbt))->OnChanged((ITUComboTable*)(cbt))

/**
* Sets OnChanged callback function of ComboTable.
*
* @param cbt Pointer referring to the ComboTable.
* @param onChanged The callback function to set.
*/
#define ituComboTableSetChanged(cbt, onChanged)  (((ITUComboTable*)(cbt))->OnChanged = (onChanged))

/**
* This is called when the ComboTable stop playing.
*
* @param cbt The ComboTable widget.
*/
#define ituComboTableOnStop(cbt) ((ITUComboTable*)(cbt))->OnStop((ITUComboTable*)(cbt))

/**
* Sets OnStop callback function of ComboTable.
*
* @param cbt Pointer referring to the ComboTable.
* @param onStop The callback function to set.
*/
#define ituComboTableSetOnStop(cbt, onStop)    (((ITUComboTable*)(cbt))->OnStop = (onStop))

/**
* Loads background static data to the ComboTable widget.
*
* @param cbt The ComboTable widget to load.
*/
void ituComboTableLoadStaticData(ITUComboTable* cbt);

/**
* Add item to the list of the ComboTable widget.
* @param cbtWidget The ComboTable widget to add item.
* @param widget The item to the tree callback.
*/
void ituComboTableAddItem(ITUWidget* cbtWidget, ITUWidget* widget);

/**
* Delete item from the list of the ComboTable widget.
*
* @param cbt The ComboTable widget to delete item.
* @param item_index The index of item what you want to delete.
*/
void ituComboTableDelItem(ITUComboTable* cbt, int item_index);

/**
* move the content items for current page.
*
* @param cbt The ComboTable widget.
* @param distX The X move distance.
* @param distY The Y move distance.
*/
void ituComboTableMoveXY(ITUComboTable* cbt, int distX, int distY);

/**
* Get the maximum page index of the ComboTable widget.
*
* @param cbt The ComboTable widget to get.
* @return value is the max page index (return -1 when itemcount is 0).
*/
int ituComboTableGetMaxPageIndex(ITUComboTable* cbt);

/**
* Goto the special page of ComboTable widget.
*
* @param cbt The ComboTable widget.
* @param page The target page index.
*/
void ituComboTableGotoPage(ITUComboTable* cbt, int page);


/**
* Search the specify child of the specify item for the ComboTable widget.
*
* @param cbt The target ComboTable widget.
* @param item_index The index of item what you specify.
* @param type The ITUWidgetType to search in this item.
* @param child_index The index of child with type what you special in this item.
* @return value is the widget what you search. (return NULL when found nothing or something wrong).
*/
ITUWidget* ituComboTableGetItemChild(ITUComboTable* cbt, int item_index, ITUWidgetType type, int child_index);

/**
* Get the item index of this child widget.
*
* @param cbt The ComboTable widget.
* @param widget The child widget to find.
* @param currPage find current page or not.(true: current page, false: all page)
* @return value is the item index.(return -1 when found nothing.)
*/
int ituComboTableGetItemIndexOfWidget(ITUComboTable* cbt, ITUWidget* widget, bool currPage);

/**
* Get the current page number of this ComboTable widget.
*
* @param cbt The ComboTable widget.
* @return value is the current page number.(return -1 when error)
*/
int ituComboTableGetCurrentPageNumber(ITUComboTable* cbt);

/**
* Get the selected item index of this ComboTable widget.
*
* @param cbt The ComboTable widget.
* @param current find current selected or not.(true: current selected, false: last selected)
* @return value is the selected item index.(return -1 when no item selected)
*/
int ituComboTableGetSelectedIndex(ITUComboTable* cbt, bool current);


/** @} */ // end of itu_widget_combotable

/** @} */ // end of itu_widget

/** @defgroup itu_scene Scene
 *  @{
 */

#define ITU_SCENE_SPEEDY  0x1    ///< This scene is compressed by Speedy algorithm.

/**
 * Action callback function definition.
 *
 * @param widget The widget to trigger the event.
 * @param param The parameter.
 * @return true to continue execute actions, false to stop execute the following actions on the widget.
 */
typedef bool (*ITUActionFunc)(ITUWidget* widget, char* param);

/**
 * Action function definition. This is used for executing event-trigged function.
 */
typedef struct
{
    const char* name;   ///< Action function name.
    ITUActionFunc func; ///< Action function pointer.
} ITUActionFunction;

/**
 * Command function definition.
 *
 * @param arg The argument.
 */
typedef void (*ITUCommandFunc)(int arg);

/**
 * Command definition. This is used for executing delayed commands.
 */
typedef struct
{
    int delay;              ///< Delay timer count to execute this command.
    ITUCommandFunc func;    ///< Command function.
    int arg;                ///< Command function argument.
} ITUCommand;

/**
 * Variable definition. This is used for executing set commands.
 */
typedef struct
{
    char target[ITU_WIDGET_NAME_SIZE];  ///< Target widget/function name to do the action.
    char param[ITU_ACTION_PARAM_SIZE];  ///< Action parameter.
    void* cachedTarget;                 ///< Cached target widget/function pointer to do the action.
} ITUVariable;

/**
* Load callback function definition.
*
* @param widget The widget to load.
* @param base The base address.
*/
typedef void(*ITULoadFunc)(ITUWidget* widget, uint32_t base);

/**
* Load function definition. This is used for loading custom widget.
*/
typedef struct
{
    ITUWidgetType type;   ///< Custom widget type.
    ITULoadFunc func;   ///< Load function pointer.
} ITULoadFunction;

/**
 * Scene manager definition. This is used for load/manage/draw scene file.
 */
typedef struct
{
    uint8_t* buffer;                                            ///< Loaded file buffer
    ITUWidget* root;                                            ///< Root node of widget tree
    bool bufferAllocated;                                       ///< Whether the buffer is allocated or static buffer
    ITURotation rotation;                                       ///< Rotation
    int screenWidth;                                            ///< Screen width
    int screenHeight;                                           ///< Screen height
    ITUActionFunction* actionFuncTable;                         ///< Action function table.
    ITULoadFunction* loadFuncTable;                             ///< Load function table.
    int leftKey;                                                ///< Left-key code
    int upKey;                                                  ///< Up-key code
    int rightKey;                                               ///< Right-key code
    int downKey;                                                ///< Down-key code
    int enterKey;                                               ///< Enter-key code
    ITUWidget* focused;                                         ///< Current focused widget
    ITUWidget* dragged;                                         ///< Current dragged widget
    ITUAudio* playingAudio;                                     ///< Current playing audio widget
    int lastMouseX;                                             ///< Last mouse position in X coordinate, in pixels
    int lastMouseY;                                             ///< Last mouse position in Y coordinate, in pixels
    int lastMouseEvent;                                         ///< Last mouse event
    int lastMouseDist;                                          ///< Last pitch distance, in pixels
    int actionQueueLen;                                         ///< Current length of action queue.
    ITUActionExecution actionQueue[ITU_ACTION_QUEUE_SIZE];      ///< Action queue.
    ITUActionExecution delayQueue[ITU_ACTION_QUEUE_SIZE];       ///< Delay queue.
    ITUCommand commands[ITU_COMMAND_SIZE];                      ///< Queued command to execute.
    char path[PATH_MAX];                                        ///< Loaded file path
    ITUVariable variables[ITU_VARIABLE_SIZE];                   ///< Variables
    pthread_t threadID;                                         ///< The current thread ID
    bool cancel;                                                ///< Cancel loading scene file or not
	unsigned int flags;                                         ///< The flags to indicate the status of the scene.
} ITUScene;

extern ITUScene* ituScene;  ///< Current loaded scene

/**
 * Initializes scene.
 *
 * @param scene The scene to initialize.
 * @param root The root widget. If you want to build the widget tree on runtime, set the root widget here, else set it the NULL value.
 */
void ituSceneInit(ITUScene* scene, ITUWidget* root);

/**
 * Exits scene.
 *
 * @param scene the scene to exit.
 */
void ituSceneExit(ITUScene* scene);

/**
 * Loads scene from a file.
 *
 * @param scene The scene to load to.
 * @param filename The file path to load.
 * @return 0 if success, else failed otherwise.
 */
int ituSceneLoadFile(ITUScene* scene, const char* filename);

/**
 * Loads scene from an array.
 *
 * @param scene The scene to load to.
 * @param array The array buffer to load.
 * @param size The array size.
 * @return 0 if success, else failed otherwise.
 */
int ituSceneLoadArray(ITUScene* scene, const char* array, int size);

/**
 * Loads scene from a file without start.
 *
 * @param scene The scene to load to.
 * @param filename The file path to load.
 * @return 0 if success, else failed otherwise.
 */
int ituSceneLoadFileCore(ITUScene* scene, const char* filename);

/**
* Loads scene from a file without start and cancelable by ituSceneLoadFileCancel().
*
* @param scene The scene to load to.
* @param filename The file path to load.
* @param segnum The segment number to load file.
* @return 0 if success, else failed otherwise.
*/
int ituSceneLoadFileCancelable(ITUScene* scene, const char* filename, int segnum);

/**
* Cancels loading scene from a file.
*
* @param scene The scene to cancel loading.
*/
void ituSceneLoadFileCancel(ITUScene* scene);

/**
 * Starts scene.
 *
 * @param scene The scene to start.
 */
void ituSceneStart(ITUScene* scene);

/**
 * Updates the scene to notify all the visible widgets by specified event.
 *
 * @param scene The scene to update.
 * @param ev The event to notify.
 * @param arg1 The event related argument #1.
 * @param arg2 The event related argument #2.
 * @param arg3 The event related argument #3.
 * @return true if any widget is modified and need to redraw the scene, false if no need to redraw.
 */
bool ituSceneUpdate(ITUScene* scene, ITUEvent ev, int arg1, int arg2, int arg3);

/**
 * Draws the scene to the specified surface.
 *
 * @param scene The scene to draw.
 * @param dest The surface to draw to.
 * @par Example:
 * @code
    static ITUScene scene;
 
    ituLcdInit();
    ituSWInit();
    ituFtInit();
    ituFtLoadFont(0, "A:/font/DroidSansMono.ttf", ITU_GLYPH_8BPP);

    ituSceneInit(&scene, NULL);
    ituSceneLoadFile(&scene, "A:/helloworld.itu");

    for (;;)
    {
        if (ituSceneUpdate(&scene, ITU_EVENT_TIMER, 0, 0, 0))
        {
            ituSceneDraw(&scene, ituGetDisplaySurface());
            ituFlip(ituGetDisplaySurface());
        }
        usleep(33000);
    }

 * @endcode
 */
void ituSceneDraw(ITUScene* scene, ITUSurface* dest);

/**
 * Pre-draws the scene to the specified surface.
 *
 * @param scene The scene to draw.
 * @param dest The surface to draw to.
 */
void ituScenePreDraw(ITUScene *scene, ITUSurface* dest);

/**
 * Rotates the scene.
 *
 * @param scene The scene to rotate.
 * @param rot The rotation.
 * @param screenWidth Screen width
 * @param screenHeight Screen height
 */
void ituSceneSetRotation(ITUScene* scene, ITURotation rot, int screenWidth, int screenHeight);

/**
 * Sets the action function table.
 *
 * @param scene The scene to set.
 * @param funcTable The action function table to set.
 */
void ituSceneSetFunctionTable(ITUScene* scene, ITUActionFunction* funcTable);

/**
* Sets the load function table.
*
* @param scene The scene to set.
* @param funcTable The load function table to set.
*/
void ituSceneSetLoadFunctionTable(ITUScene* scene, ITULoadFunction* funcTable);

/**
 * Finds the action function by the specified name.
 *
 * @param scene The scene to find.
 * @param name The action function name to find.
 * @return The found action function. NULL if not found.
 */
ITUActionFunc ituSceneFindFunction(ITUScene* scene, const char* name);

/**
 * Finds the widget by the specified name.
 *
 * @param scene The scene to find.
 * @param name The widget name to find.
 * @return The found widget. NULL if not found.
 * @par Example:
 * @code
    ITUWidget* widget;

    widget = ituSceneFindWidget(&scene, "Button2");
    if (widget)
        ituWidgetSetOnPress(widget, OnPress);

 * @endcode
 */
void* ituSceneFindWidget(ITUScene* scene, const char* name);

/**
 * Finds the previous widget to focus.
 *
 * @param scene The scene to find.
 * @return The found widget. NULL if not found.
 */
ITUWidget* ituSceneFocusPrev(ITUScene* scene);

/**
 * Finds the next widget to focus.
 *
 * @param scene The scene to find.
 * @return The found widget. NULL if not found.
 */
ITUWidget* ituSceneFocusNext(ITUScene* scene);

/**
 * Execute delayed command.
 *
 * @param scene The scene to execute the command.
 * @param delay Delay timer count to execute this command.
 * @param func The command function to execute.
 * @param arg The command function argument.
 */
void ituSceneExecuteCommand(ITUScene* scene, int delay, ITUCommandFunc func, int arg);

/**
 * Sends custom event.
 *
 * @param scene The scene to send.
 * @param ev The custom event to notify.
 * @param param The event related parameter.
 */
static inline void ituSceneSendEvent(ITUScene* scene, int ev, char* param)
{
    ituSceneUpdate(scene, (ITUEvent)ev, (int)param, 0, 0);
}

/** @} */ // end of itu_scene

/** @defgroup itu_util Utilities
 *  @{
 */

// Utilities
/**
 * Gets bit-per-pixel value of specified pixel format.
 *
 * @param format the pixel format.
 * @return the bit-per-pixel value.
 */
unsigned int ituFormat2Bpp(ITUPixelFormat format);

/**
 * The implemntation of focuses the specified widget and unfocuses last focused widget.
 *
 * @param widget the widget to focus.
 */
void ituFocusWidgetImpl(ITUWidget* widget);

/**
 * Focuses the specified widget and unfocuses last focused widget.
 *
 * @param widget the widget to focus.
 */
#define ituFocusWidget(widget) ituFocusWidgetImpl((ITUWidget*)(widget))

/**
 * The implemntation of makes dirty the specified widget and it's children.
 *
 * @param widget the widget to make dirty.
 * @param dirty make dirty or not.
 */
void ituDirtyWidgetImpl(ITUWidget* widget, bool dirty);

/**
 * Make dirties the specified widget and it's children.
 *
 * @param widget the widget to make dirty.
 * @param dirty make dirty or not.
 */
#define ituDirtyWidget(widget, dirty) ituDirtyWidgetImpl((ITUWidget*)(widget), (dirty))

/**
 * The implemntation of finding the child widget.
 *
 * @param widget the widget to find child widget.
 * @param name the name of child widget to find.
 * @return the child widget. NULL if not found.
 */
ITUWidget* ituFindWidgetChildImpl(ITUWidget* widget, const char *name);

/**
 * Find the child widget.
 *
 * @param widget the widget to find child widget.
 * @param name the name of child widget to find.
 * @return the child widget. NULL if not found.
 */
#define ituFindWidgetChild(widget, name) ituFindWidgetChildImpl((ITUWidget*)(widget), (name))

/**
* The implemntation of un-pressed the specified widget and it's children.
*
* @param widget the widget to set pressed.
*/
void ituUnPressWidgetImpl(ITUWidget* widget);

/**
* Set pressed the specified widget and it's children.
*
* @param widget the widget to set pressed.
*/
#define ituUnPressWidget(widget) ituUnPressWidgetImpl((ITUWidget*)(widget))

/**
 * Takes a screenshot to the specified file path.
 *
 * @param surf The surface to take the screenshot.
 * @param filepath The file path to save to. The file format is PPM.
 */
void ituScreenshot(ITUSurface* surf, char* filepath);

/**
* Takes a screenshot with specified rectangle to the specified file path.
*
* @param surf The surface to take the screenshot.
* @param x the x coordinate of surface, in pixels.
* @param y the y coordinate of surface, in pixels.
* @param w the width to screenshot, in pixels. 0 will be the width of surface.
* @param h the height to screenshot, in pixels. 0 will be the height of surface.
* @param filepath The file path to save to.
*/
void ituScreenshotRect(ITUSurface* surf, int x, int y, int w, int h, char* filepath);

/**
 * Gets layer widget.
 *
 * @param widget the widget belong to.
 * @return The found layer widget. NULL if not found.
 */
ITULayer* ituGetLayer(ITUWidget* widget);

/**
* Preloads font cache.
*
* @param widget the widget to preload.
* @param surf The surface to do the dummy draw.
*/
void ituPreloadFontCache(ITUWidget* widget, ITUSurface* surf);

/**
* Empty draw glyph function for preloads font cache.
*
* @param surf the surface to draw.
* @param x the x coordinate, in pixels.
* @param y the y coordinate, in pixels.
* @param format the glyph format.
* @param bitmap the glyph bitmap.
* @param w the glyph width, in pixels.
* @param h the glyph height, in pixels.
*/
void ituDrawGlyphEmpty(ITUSurface* surf, int x, int y, ITUGlyphFormat format, const uint8_t* bitmap, int w, int h);

/**
* Gets target widget on specified variable.
*
* @param index the variable index.
* @return The target widget. NULL if not set or out of bound.
*/
ITUWidget* ituGetVarTarget(int index);

/**
* Sets target widget to specified variable.
*
* @param index the variable index.
* @param target The target widget. NULL to clear target variable.
*/
void ituSetVarTarget(int index, ITUWidget* target);

/**
* Gets parameter on specified variable.
*
* @param index the variable index.
* @return The parameter. NULL if out of bound.
*/
char* ituGetVarParam(int index);

/**
* Sets parameter to specified variable.
*
* @param index the variable index.
* @param param The parameter. NULL to clear parameter variable.
*/
void ituSetVarParam(int index, char* param);

/**
* Checks current thread is valid or not.
*
* @param file the file line.
*/
void ituAssertThread(const char *file);

#if defined(CFG_ITU_ASSERT_THREAD) || !defined(NDEBUG)

#define FUNCTIONIZE(a,b)    a(b)
#define STRINGIZE(a)        #a
#define INT2STRING(i)       FUNCTIONIZE(STRINGIZE,i)
#define ITU_FILE_POS        __FILE__ ":" INT2STRING(__LINE__)

#define ITU_ASSERT_THREAD() ituAssertThread(ITU_FILE_POS)

#else

#define ITU_ASSERT_THREAD()

#endif // defined(CFG_ITU_ASSERT_THREAD) || !defined(NDEBUG)

/**
* Sets clipping region to surface.
*
* @param surf the surface to set to.
* @param x the clipping x coordinate.
* @param y the clipping y coordinate.
* @param width the clipping width.
* @param height the clipping height.
* @param prevClip The previous clipping of surface to backup.
*/
void ituSetClipping(ITUSurface* surf, int x, int y, int width, int height, ITURectangle* prevClip);

/**
* Wrapper of malloc, internal use only.
*
* @param size memory size to allocate.
* @return The allocated memory. NULL if out of memory.
*/
void* ituMalloc(size_t size);

/**
* Wrapper of free, internal use only.
*
* @param ptr memory to free.
*/
void ituFree(void* ptr);
/** @} */ // end of itu_util

#ifdef __cplusplus
}
#endif

#endif // ITE_ITU_H
/** @} */ // end of itu
