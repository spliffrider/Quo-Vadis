#pragma once
#include <string>
namespace rack { namespace widget { struct Widget; } namespace math { struct Rect; } }
namespace metroqa {
// Xvfb/Mesa's default front buffer can be empty. Render the real scene to a
// Rack-owned framebuffer on Linux; use Rack's front buffer on other platforms.
void screenshot(const std::string& path);
// Render current vector widgets directly at the requested region/scale.
// This is a fresh Rack render, not a crop or edit of a mockup bitmap.
bool region(rack::widget::Widget* root,rack::math::Rect bounds,float scale,const std::string& path);
}
