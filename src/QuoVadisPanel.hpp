#include "QuoVadisMaterials.hpp"
// Cacheable, original panel art. All moving controls are separate Rack widgets.
struct HardwarePanel : widget::Widget {
    Metropolix* module=nullptr;
    ClassicPanelMaterials materials;
    void onContextDestroy(const ContextDestroyEvent& e) override {
        materials.clear();widget::Widget::onContextDestroy(e);
    }
    void draw(const DrawArgs& args) override {
        auto* vg=args.vg;bool stone=atrium(module);
        NVGcolor body=stone?nvgRGB(226,214,190):nvgRGB(192,191,190);
        NVGcolor edge=stone?nvgRGB(140,123,97):nvgRGB(115,116,116);
        NVGcolor pale=stone?nvgRGB(235,234,222):nvgRGB(237,241,244),dark=nvgRGB(30,36,41);
        rect(vg,0,0,510,380,body,0);
        // Material textures are generated once per context and cached with the art.
        if(stone) {
            for(int y=91;y<380;++y)stroke(vg,0,y,510,y,nvgRGBA(255,255,239,(y*17)%9),.4f);
        } else materials.draw(vg,90,290,true);
        if(stone) {
            for(int i=0;i<45;++i){float x=7+(i*113)%496,y=95+(i*47)%277;stroke(vg,x,y,std::min(504.f,x+12+(i%17)),y+1,nvgRGBA(118,98,72,18),.4f);}
            // A small mosaic border stays clear of labels and sockets.
            for(int i=0;i<63;++i){float x=4+i*8;rect(vg,x,373,3,3,edge,.2f);rect(vg,x+3.5f,376,3,2,nvgRGB(191,173,144),.2f);}
            for(int i=0;i<34;++i){float y=96+i*8;rect(vg,3,y,2,3,edge,.1f);rect(vg,505,y,2,3,edge,.1f);}
        }
        if(stone) {
            rect(vg,0,0,510,90,nvgRGB(28,31,31),0);
            for(int y=1;y<90;y+=2)stroke(vg,1,y,509,y,nvgRGBA(255,255,255,(y*7)%4),.4f);
        } else materials.draw(vg,0,90,false);
        stroke(vg,244,7,244,85,nvgRGB(184,184,172),.6f);
        stroke(vg,1,90,509,90,edge,.8f);
        font(vg);
        auto label=[&](float x,float y,const std::string& s,float size=6.7f,NVGcolor c=nvgRGB(38,42,42)){
            text(vg,x,y,s,size,c,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
        };
        const float px[]={30,64,98,132,166,200};
        const char* top[]={"A","B","CLK OUT","CLOCK IN","P1","P2"};
        const char* bottom[]={"X","Y","Z","RESET IN","G1","G2"};
        for(int i=0;i<6;++i) {
            bool output=i!=3;
            if(output)rect(vg,px[i]-13,17,26,9,pale,.7f);
            label(px[i],21.5f,top[i],i==2 || i==3?5.8f:6.5f,output?dark:pale);
            if(i>=4)rect(vg,px[i]-9,79,18,8,pale,.7f);
            label(px[i],83,bottom[i],6.1f,i>=4?dark:pale);
        }
        auto face=APP->window->loadFont(asset::plugin(pluginInstance,"res/Cinzel.ttf"));
        if(face)nvgFontFaceId(vg,face->handle);
        const std::string title="Quo Vadis¿";float titleSize=14;
        nvgFontSize(vg,titleSize);nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
        float bounds[4];nvgTextBounds(vg,380,13,title.c_str(),nullptr,bounds);
        text(vg,380,13,title,titleSize,pale,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
        laurel(vg,bounds[0]-titleSize,13,false,pale);
        laurel(vg,bounds[2]+titleSize,13,true,pale);
        font(vg);
        // A note identifies the pitch bank without another text heading.
        disc(vg,250,56,2.3f,pale);stroke(vg,252,55,252,45,pale,1.2f);stroke(vg,252,45,256,48,pale,1.2f);
        for(int s=0;s<8;++s) {
            float x=stageX(s);label(x,32,std::to_string(s+1),7,pale);
            label(x,114,std::to_string(s+1),6.5f);
            doricColumn(vg,x,PULSE_TOP,PULSE_HEIGHT,stone);
            for(int n=1;n<=8;++n){float y=PULSE_TOP+PULSE_HEIGHT-8.5f-(n-1)*(PULSE_HEIGHT-17)/7;stroke(vg,x-12,y,x-10,y,edge,.5f);stroke(vg,x+10,y,x+12,y,edge,.5f);if(s==0)label(x-17,y,std::to_string(n),5.5f);}
            // Stage numbers live in their own row, above every arch peak.
            label(x,GATE_TOP-12,std::to_string(s+1),6.8f);
            if(stone){
                nvgBeginPath(vg);nvgMoveTo(vg,x-13,GATE_TOP+23);nvgLineTo(vg,x-13,GATE_TOP+2);nvgBezierTo(vg,x-13,GATE_TOP-8,x+13,GATE_TOP-8,x+13,GATE_TOP+2);nvgLineTo(vg,x+13,GATE_TOP+23);
                nvgStrokeColor(vg,edge);nvgStrokeWidth(vg,1);nvgStroke(vg);
            }
        }
        auto heading=[&](float x,float y,const std::string& title) {
            if(stone){text(vg,x,y,title,7,dark);return;}
            rect(vg,x,y-2,3.5f,4,nvgRGB(158,43,54),.1f);
            text(vg,x+8,y,title,7,dark);
            float bounds[4];nvgTextBounds(vg,x+8,y,title.c_str(),nullptr,bounds);
            float start=bounds[2]+5;
            if(start<STAGE_BANK_RIGHT)rect(vg,start,y-1.2f,STAGE_BANK_RIGHT-start,2.4f,nvgRGB(135,151,165),0);
        };
        heading(STAGE_BANK_LEFT+7,101,"PULSES (PER STEP)");
        heading(STAGE_BANK_LEFT+7,GATE_TOP-23,"GATE MODE (PER STEP)");
        stroke(vg,STAGE_BANK_LEFT+3,GATE_TOP-29,STAGE_BANK_RIGHT,GATE_TOP-29,edge,.6f);
        stroke(vg,10,FOOTER_TOP,STAGE_BANK_RIGHT,FOOTER_TOP,edge,.7f);
        stroke(vg,51,97,51,FOOTER_TOP-9,edge,.6f);stroke(vg,STAGE_BANK_LEFT,97,STAGE_BANK_LEFT,FOOTER_TOP-9,edge,.6f);
        if(!stone)rect(vg,10,98,3.5f,4,nvgRGB(158,43,54),.1f);
        text(vg,stone?15:18,100,"AUX",7,dark);
        for(int i=0;i<3;++i)label(30,140+i*60,std::string(1,char('X'+i)),6);
        text(vg,63,222,"VALUE / ENTER",6.6f,dark);
        label(25,367,"CTRL 1",6.5f);label(65,367,"CTRL 2",6.5f);
        stroke(vg,87,FOOTER_TOP+5,87,370,edge,.6f);stroke(vg,STAGE_BANK_LEFT,FOOTER_TOP+5,STAGE_BANK_LEFT,370,edge,.6f);
        const char* alt[]={"LOAD","SAVE","OUT A","OUT B","CLOCK","EXP","LIMIT","TYPE"};
        for(int s=0;s<8;++s)label(stageX(s),FOOTER_TOP+13,alt[s],5.4f);
        if(!stone)rect(vg,STAGE_BANK_LEFT+3,FOOTER_TOP+2,3.5f,4,nvgRGB(158,43,54),.1f);
        text(vg,STAGE_BANK_LEFT+(stone?3:11),FOOTER_TOP+4,"ALT",stone?4.5f:5.2f,stone?nvgRGB(14,114,134):nvgRGB(65,97,124));
        if(stone) {
            // Display surround, with two restrained pilasters. No foliage below.
            outline(vg,57,96,186,DISPLAY_HEIGHT+8,edge,1.2f,2);
            nvgBeginPath(vg);nvgMoveTo(vg,57,108);nvgBezierTo(vg,75,88,225,88,244,108);nvgStrokeColor(vg,edge);nvgStrokeWidth(vg,2.2f);nvgStroke(vg);
            for(float x:{55.f,245.f}){nvgSave(vg);nvgTranslate(vg,x,100);nvgScale(vg,.4f,1);doricColumn(vg,0,0,FOOTER_TOP-111,true);nvgRestore(vg);}
            for(float x:{4.f,506.f}){nvgSave(vg);nvgTranslate(vg,x,14);nvgScale(vg,.35f,1);doricColumn(vg,0,0,73,true);nvgRestore(vg);}
        }
        for(float x:{8.f,502.f})for(float y:{8.f,372.f}) {
            disc(vg,x,y,4.5f,nvgRGB(12,14,14));metalDisc(vg,x,y,3.5f,nvgRGB(111,114,111),nvgRGB(24,27,26));
            stroke(vg,x-1.7f,y+1,x+1.7f,y-1,nvgRGB(9,11,11),1);
        }
    }
};
