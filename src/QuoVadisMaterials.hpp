// Procedural UI materials. No pixels from a reference photograph are embedded.
// Image handles belong to each NanoVG context, just like the display textures.
struct ClassicPanelMaterials {
    std::map<NVGcontext*,std::pair<int,int>> images;
    static float noise(int x,int y,int width,int height,uint32_t seed) {
        x=(x%width+width)%width;y=(y%height+height)%height;
        uint32_t n=seed ^ uint32_t(x)*0x9e3779b9u ^ uint32_t(y)*0x85ebca6bu;
        n^=n>>16;n*=0x7feb352du;n^=n>>15;n*=0x846ca68bu;n^=n>>16;
        return float(n&65535u)/32767.5f-1.f;
    }
    int image(NVGcontext* vg,bool brushed) {
        auto& pair=images[vg];int& handle=brushed?pair.first:pair.second;
        if(handle>0)return handle;
        const int width=brushed?512:256,height=256;
        std::vector<unsigned char> pixels(width*height*4);
        for(int y=0;y<height;++y) {
          float trail=0;
          if(brushed)for(int k=-16;k<=16;++k)trail+=noise(k,y,width,height,0x51564d31u);
          for(int x=0;x<width;++x) {
            float grain;
            if(brushed) {
                // Long, fine horizontal marks, with a little uncorrelated grain.
                grain=trail*(46.f/33)+.8f*noise(x,y,width,height,0x51564d32u)
                    +1.5f*noise(0,y,width,height,0x51564d33u);
                trail+=noise(x+17,y,width,height,0x51564d31u)-noise(x-16,y,width,height,0x51564d31u);
            } else {
                // Small isotropic pits in powder-coated charcoal, without shine.
                grain=6.f*noise(x,y,width,height,0x51564331u)
                    +2.f*noise(x/3,y/3,width/3,height/3,0x51564332u);
            }
            const int base[3]={brushed?203:43,brushed?202:46,brushed?201:49};
            size_t offset=(y*width+x)*4;
            for(int c=0;c<3;++c)pixels[offset+c]=static_cast<unsigned char>(std::max(0,std::min(255,int(std::round(base[c]+grain)))));
            pixels[offset+3]=255;
          }
        }
        handle=nvgCreateImageRGBA(vg,width,height,NVG_IMAGE_REPEATX|NVG_IMAGE_REPEATY|NVG_IMAGE_GENERATE_MIPMAPS,pixels.data());
        return handle;
    }
    void draw(NVGcontext* vg,float y,float height,bool brushed) {
        int handle=image(vg,brushed);
        if(handle>0) {
            nvgBeginPath(vg);nvgRect(vg,0,y,510,height);
            nvgFillPaint(vg,nvgImagePattern(vg,0,y,(brushed?512.f:256.f)/3,256.f/3,0,handle,1));nvgFill(vg);
        } else rect(vg,0,y,510,height,brushed?nvgRGB(203,202,201):nvgRGB(43,46,49),0);
        nvgBeginPath(vg);nvgRect(vg,0,y,510,height);
        nvgFillPaint(vg,nvgLinearGradient(vg,0,y,0,y+height,nvgRGBA(0,0,0,0),nvgRGBA(0,0,0,brushed?27:80)));nvgFill(vg);
        if(brushed) {
            // Broad, quiet reflection across the silver, rather than a blue tint.
            float middle=y+height*.58f;
            for(int half=0;half<2;++half) {
                float top=half?middle:y,bottom=half?y+height:middle;
                nvgBeginPath(vg);nvgRect(vg,0,top,510,bottom-top);
                nvgFillPaint(vg,nvgLinearGradient(vg,0,top,0,bottom,nvgRGBA(0,0,0,half?8:0),nvgRGBA(0,0,0,half?0:8)));nvgFill(vg);
            }
        }
    }
    void clear() {
        for(auto& item:images){if(item.second.first>0)nvgDeleteImage(item.first,item.second.first);if(item.second.second>0)nvgDeleteImage(item.first,item.second.second);}
        images.clear();
    }
    ~ClassicPanelMaterials(){clear();}
};
