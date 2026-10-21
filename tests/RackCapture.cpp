// The QA executable is allowed to use Rack's internal screenshot method.
// This file is never linked into the production plugin.
#include <window/Window.hpp>
#undef PRIVATE
#include <rack.hpp>
#include "RackCapture.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <vector>
#include <algorithm>
#include <functional>

namespace metroqa {
bool region(rack::widget::Widget* root,rack::math::Rect bounds,float scale,const std::string& path) {
    using namespace rack;
    if(!root || scale<=0 || bounds.size.x<=0 || bounds.size.y<=0)return false;
    struct RegionView : widget::Widget {
        widget::Widget* root=nullptr;math::Rect region;float zoom=1;
        void draw(const DrawArgs& args) override {
            auto live=args;live.fb=nullptr;live.clipBox=region;
            nvgSave(args.vg);nvgScale(args.vg,zoom,zoom);nvgTranslate(args.vg,-region.pos.x,-region.pos.y);
            root->draw(live);root->drawLayer(live,1);nvgRestore(args.vg);
        }
    };
    std::vector<std::pair<widget::FramebufferWidget*,bool>> caches;
    std::function<void(widget::Widget*)> bypass=[&](widget::Widget* w){
        if(auto* fb=dynamic_cast<widget::FramebufferWidget*>(w)){caches.emplace_back(fb,fb->bypassed);fb->bypassed=true;}
        for(auto* child:w->children)bypass(child);
    };
    bypass(root);
    GLint oldFramebuffer,oldReadBuffer,viewport[4];
    glGetIntegerv(GL_FRAMEBUFFER_BINDING,&oldFramebuffer);glGetIntegerv(GL_READ_BUFFER,&oldReadBuffer);glGetIntegerv(GL_VIEWPORT,viewport);
    widget::FramebufferWidget framebuffer;framebuffer.box.size=bounds.size.mult(scale);
    auto* view=new RegionView;view->root=root;view->region=bounds;view->zoom=scale;view->box.size=framebuffer.box.size;framebuffer.addChild(view);
    framebuffer.render();nvgluBindFramebuffer(framebuffer.getFramebuffer());
    auto size=framebuffer.getFramebufferSize();int width=int(size.x),height=int(size.y);
    std::vector<unsigned char> pixels(size_t(width)*height*4);
    glReadBuffer(GL_COLOR_ATTACHMENT0);glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());
    for(int y=0;y<height/2;++y)std::swap_ranges(pixels.begin()+size_t(y)*width*4,pixels.begin()+size_t(y+1)*width*4,pixels.begin()+size_t(height-y-1)*width*4);
    bool written=stbi_write_png(path.c_str(),width,height,4,pixels.data(),width*4)!=0;
    glBindFramebuffer(GL_FRAMEBUFFER,oldFramebuffer);glReadBuffer(oldReadBuffer);glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
    for(auto& cache:caches)cache.first->bypassed=cache.second;
    return written;
}
}

#if defined ARCH_LIN

namespace metroqa {
void screenshot(const std::string& path) {
    using namespace rack;
    // Reference the live scene without transferring ownership or stepping it.
    struct SceneView : widget::Widget {
        void draw(const DrawArgs& args) override {
            auto live=args;
            // Preserve the normal scene's light/halo behavior. Rack suppresses
            // halos for module-browser framebuffers when args.fb is non-null.
            live.fb=nullptr;
            APP->scene->draw(live);
        }
    };
    std::vector<std::pair<widget::FramebufferWidget*,bool>> caches;
    std::function<void(widget::Widget*)> bypass=[&](widget::Widget* w) {
        if(auto* fb=dynamic_cast<widget::FramebufferWidget*>(w)) {
            caches.emplace_back(fb,fb->bypassed);fb->bypassed=true;
        }
        for(auto* child:w->children)bypass(child);
    };
    // Avoid nested cache renders rebinding the default framebuffer. Drawing
    // their real children directly also keeps this proof independent of caches.
    bypass(APP->scene);
    GLint oldFramebuffer,oldReadBuffer,viewport[4];
    glGetIntegerv(GL_FRAMEBUFFER_BINDING,&oldFramebuffer);
    glGetIntegerv(GL_READ_BUFFER,&oldReadBuffer);
    glGetIntegerv(GL_VIEWPORT,viewport);
    widget::FramebufferWidget framebuffer;
    framebuffer.box.size=APP->scene->box.size;
    auto* scene=new SceneView;
    scene->box.size=framebuffer.box.size;
    framebuffer.addChild(scene);
    framebuffer.render();
    nvgluBindFramebuffer(framebuffer.getFramebuffer());
    auto size=framebuffer.getFramebufferSize();
    int width=int(size.x),height=int(size.y);
    std::vector<unsigned char> pixels(size_t(width)*height*4);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glReadPixels(0,0,width,height,GL_RGBA,GL_UNSIGNED_BYTE,pixels.data());
    for(int y=0;y<height/2;++y)
        std::swap_ranges(pixels.begin()+size_t(y)*width*4,pixels.begin()+size_t(y+1)*width*4,
                         pixels.begin()+size_t(height-y-1)*width*4);
    stbi_write_png(path.c_str(),width,height,4,pixels.data(),width*4);
    glBindFramebuffer(GL_FRAMEBUFFER,oldFramebuffer);
    glReadBuffer(oldReadBuffer);
    glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
    for(auto& cache:caches)cache.first->bypassed=cache.second;
}
}
#else
namespace metroqa {
void screenshot(const std::string& path) { APP->window->screenshot(path); }
}
#endif
