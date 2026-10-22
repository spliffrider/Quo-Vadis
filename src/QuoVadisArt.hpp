// Original vector artwork shared by both Quo Vadis¿ panel styles.
// Narrow the stage banks from the left; stage 8 and the right edge stay fixed.
constexpr float STAGE_RIGHT_CENTER=482.5f, STAGE_SPACING=27.5f;
constexpr float STAGE_X=STAGE_RIGHT_CENTER-7*STAGE_SPACING;
constexpr float STAGE_BUTTON_WIDTH=26.f;
constexpr float STAGE_BANK_LEFT=270.f;
constexpr float STAGE_BANK_RIGHT=STAGE_RIGHT_CENTER+STAGE_BUTTON_WIDTH/2;
// Shared vertical anchors keep panel engraving and interactive widgets together.
constexpr float PULSE_TOP=125.f, PULSE_HEIGHT=82.f;
constexpr float GATE_TOP=248.f, FOOTER_TOP=286.f;
constexpr float NAV_CENTER_Y=255.f, DISPLAY_HEIGHT=103.f;
float stageX(int s) { return STAGE_X+STAGE_SPACING*s; }
Rect stageButtonBounds(int s,float top,float height) {
    return Rect(Vec(stageX(s)-STAGE_BUTTON_WIDTH/2,top),Vec(STAGE_BUTTON_WIDTH,height));
}
NVGcolor lampBlue() { return nvgRGB(24,210,236); }
void outline(NVGcontext* vg,float x,float y,float w,float h,NVGcolor color,float width=.6f,float radius=1) {
    nvgBeginPath(vg);nvgRoundedRect(vg,x,y,w,h,radius);nvgStrokeColor(vg,color);nvgStrokeWidth(vg,width);nvgStroke(vg);
}
void stageButtonCap(NVGcontext* vg,float width,float height) {
    // Both stage-button rows use exactly the same visible left/right edges.
    rect(vg,0,0,width,height,nvgRGB(17,20,23),1.2f);
    nvgBeginPath(vg);nvgRoundedRect(vg,.5f,.5f,width-1,height-1,1.1f);
    nvgFillPaint(vg,nvgLinearGradient(vg,0,0,0,height,nvgRGB(47,51,55),nvgRGB(16,19,22)));nvgFill(vg);
    outline(vg,.4f,.4f,width-.8f,height-.8f,nvgRGB(108,115,121),.6f,1.1f);
    stroke(vg,2,1.2f,width-2,1.2f,nvgRGBA(221,228,234,90),.4f);
}
void satinKnob(NVGcontext* vg,float x,float y,float radius,float value,bool light=true,bool encoder=false) {
    disc(vg,x+.4f,y+1,radius+.4f,nvgRGBA(0,0,0,100));
    metalDisc(vg,x,y,radius,nvgRGB(97,99,98),nvgRGB(13,15,15));
    NVGcolor top=light?nvgRGB(241,242,241):nvgRGB(52,57,62);
    NVGcolor bottom=light?nvgRGB(185,188,187):nvgRGB(17,21,25);
    metalDisc(vg,x,y,radius-1.4f,top,bottom);
    if(light) {
        float inner=radius-1.9f;
        for(int i=1;i<int(inner*2);++i){float dy=i-inner,dx=std::sqrt(inner*inner-dy*dy);stroke(vg,x-dx,y+dy,x+dx,y+dy,nvgRGBA(255,255,255,5+(i*7)%10),.25f);}
    }
    // A simple linear satin finish, without a conical highlight.
    if(!encoder) {
        float a=(-.83f+1.66f*value)*M_PI;
        stroke(vg,x+std::sin(a)*radius*.3f,y-std::cos(a)*radius*.3f,x+std::sin(a)*(radius-2.6f),y-std::cos(a)*(radius-2.6f),light?nvgRGB(20,22,22):nvgRGB(243,242,234),1.9f);
    }
}
void gateGlyph(NVGcontext* vg,float x,float y,int mode) {
    NVGcolor c=mode==2 || mode==3?lampBlue():nvgRGB(234,236,225);
    if(mode==0 || mode==3) {stroke(vg,x-6,y,x+6,y,c,mode==3?2.f:.7f);return;}
    int n=mode==2?3:1;float left=x-6;
    nvgBeginPath(vg);nvgMoveTo(vg,left,y+3);
    for(int i=0;i<n;++i){float start=left+(i+.25f)*12/n;nvgLineTo(vg,start,y+3);nvgLineTo(vg,start,y-3);nvgLineTo(vg,start+6.f/n,y-3);nvgLineTo(vg,start+6.f/n,y+3);}
    nvgLineTo(vg,x+6,y+3);nvgStrokeColor(vg,c);nvgStrokeWidth(vg,.8f);nvgStroke(vg);
}
