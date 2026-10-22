#include "QuoVadisMaterials.hpp"
// Original mixer-style silkscreen. Moving controls remain separate Rack widgets.
struct HardwarePanel : widget::Widget {
    Metropolix* module=nullptr;
    ClassicPanelMaterials materials;
    void onContextDestroy(const ContextDestroyEvent& e) override {
        materials.clear();widget::Widget::onContextDestroy(e);
    }
    void draw(const DrawArgs& args) override {
        auto* vg=args.vg;
        bool silver=module && module->panelStyle.load()==Metropolix::BRUSHED_SILVER;
        NVGcolor edge=nvgRGB(105,109,110),pale=nvgRGB(237,241,244),dark=nvgRGB(30,34,36);
        NVGcolor topInk=silver?dark:pale,accent=nvgRGB(174,42,49);
        // Both finishes use the same brushed aluminium, with optional black top.
        materials.draw(vg,0,380,true);
        if(!silver)materials.draw(vg,0,90,false);
        stroke(vg,244,7,244,85,silver?edge:nvgRGB(168,174,178),.6f);
        stroke(vg,1,90,509,90,edge,.8f);
        font(vg);
        auto label=[&](float x,float y,const std::string& s,float size=6.7f,NVGcolor c=nvgRGB(30,34,36)){
            text(vg,x,y,s,size,c,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
        };
        const float px[]={30,64,98,132,166,200};
        const char* top[]={"A","B","CLK OUT","CLOCK IN","P1","P2"};
        const char* bottom[]={"X","Y","Z","RESET IN","G1","G2"};
        for(int i=0;i<6;++i) {
            bool output=i!=3;
            NVGcolor badge=silver?dark:pale,badgeInk=silver?pale:dark;
            if(output)rect(vg,px[i]-13,17,26,9,badge,.7f);
            label(px[i],21.5f,top[i],i==2 || i==3?5.8f:6.5f,output?badgeInk:topInk);
            if(i>=4)rect(vg,px[i]-9,79,18,8,badge,.7f);
            label(px[i],83,bottom[i],6.1f,i>=4?badgeInk:topInk);
        }
        // Compact sans-serif product print, without decorative surrounds.
        nvgSave(vg);nvgTextLetterSpacing(vg,.85f);
        text(vg,386.25f,13,"QUO VADIS¿",13.5f,silver?accent:pale,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
        nvgRestore(vg);
        rect(vg,STAGE_BANK_LEFT+7,10,4,6,accent,0);
        // A note identifies the pitch bank without another text heading.
        disc(vg,250,56,2.3f,topInk);stroke(vg,252,55,252,45,topInk,1.2f);stroke(vg,252,45,256,48,topInk,1.2f);
        for(int s=0;s<8;++s) {
            float x=stageX(s);label(x,32,std::to_string(s+1),7,topInk);
            label(x,114,std::to_string(s+1),6.5f);
            // Printed mixer graduations share the fader's actual travel centres.
            for(int n=1;n<=8;++n){
                float y=PULSE_TOP+PULSE_HEIGHT-8.5f-(n-1)*(PULSE_HEIGHT-17)/7;
                stroke(vg,x-10,y,x-3.5f,y,edge,.5f);stroke(vg,x+3.5f,y,x+10,y,edge,.5f);
                if(s==0)label(x-17,y,std::to_string(n),5.5f);
            }
            label(x,GATE_TOP-12,std::to_string(s+1),6.8f);
        }
        auto heading=[&](float x,float y,const std::string& title) {
            rect(vg,x,y-2,3.5f,4,accent,0);
            text(vg,x+8,y,title,7,dark);
            float bounds[4];nvgTextBounds(vg,x+8,y,title.c_str(),nullptr,bounds);
            float start=bounds[2]+5;
            if(start<STAGE_BANK_RIGHT)rect(vg,start,y-1.2f,STAGE_BANK_RIGHT-start,2.4f,nvgRGB(139,158,170),0);
        };
        heading(STAGE_BANK_LEFT+7,101,"PULSES (PER STEP)");
        heading(STAGE_BANK_LEFT+7,GATE_TOP-23,"GATE MODE (PER STEP)");
        stroke(vg,STAGE_BANK_LEFT+3,GATE_TOP-29,STAGE_BANK_RIGHT,GATE_TOP-29,edge,.6f);
        stroke(vg,10,FOOTER_TOP,STAGE_BANK_RIGHT,FOOTER_TOP,edge,.7f);
        stroke(vg,51,97,51,FOOTER_TOP-9,edge,.6f);stroke(vg,STAGE_BANK_LEFT,97,STAGE_BANK_LEFT,FOOTER_TOP-9,edge,.6f);
        rect(vg,10,98,3.5f,4,accent,0);text(vg,18,100,"AUX",7,dark);
        for(int i=0;i<3;++i)label(30,140+i*60,std::string(1,char('X'+i)),6);
        text(vg,63,222,"VALUE / ENTER",6.6f,dark);
        label(25,367,"CTRL 1",6.5f);label(65,367,"CTRL 2",6.5f);
        stroke(vg,87,FOOTER_TOP+5,87,370,edge,.6f);stroke(vg,STAGE_BANK_LEFT,FOOTER_TOP+5,STAGE_BANK_LEFT,370,edge,.6f);
        const char* alt[]={"LOAD","SAVE","OUT A","OUT B","CLOCK","EXP","LIMIT","TYPE"};
        for(int s=0;s<8;++s)label(stageX(s),FOOTER_TOP+13,alt[s],5.4f);
        rect(vg,STAGE_BANK_LEFT+3,FOOTER_TOP+2,3.5f,4,accent,0);
        text(vg,STAGE_BANK_LEFT+11,FOOTER_TOP+4,"ALT",5.2f,nvgRGB(65,97,124));
        for(float x:{8.f,502.f})for(float y:{8.f,372.f}) {
            disc(vg,x,y,4.5f,nvgRGB(12,14,14));metalDisc(vg,x,y,3.5f,nvgRGB(111,114,111),nvgRGB(24,27,26));
            stroke(vg,x-1.7f,y+1,x+1.7f,y-1,nvgRGB(9,11,11),1);
        }
    }
};
