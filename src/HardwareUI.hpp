// Included inside Metropolix.cpp's private UI namespace.
// Original Quo Vadis¿ interface, retaining the legacy 34 HP patch footprint.
constexpr float HARDWARE_WIDTH = 510.f;
constexpr float EDITOR_WIDTH = 480.f;
void chainEncoderPress(Metropolix* m);
void openChainPage(Metropolix* m);
void appendChainMenu(ui::Menu* menu,Metropolix* m);
void stroke(NVGcontext* vg,float x,float y,float x2,float y2,NVGcolor color,float width=1.f) {
    nvgBeginPath(vg);nvgMoveTo(vg,x,y);nvgLineTo(vg,x2,y2);nvgStrokeColor(vg,color);nvgStrokeWidth(vg,width);nvgStroke(vg);
}

void disc(NVGcontext* vg, float x, float y, float r, NVGcolor color) {
    nvgBeginPath(vg); nvgCircle(vg, x, y, r); nvgFillColor(vg, color); nvgFill(vg);
}
void metalDisc(NVGcontext* vg, float x, float y, float r, NVGcolor top, NVGcolor bottom) {
    nvgBeginPath(vg); nvgCircle(vg, x, y, r);
    nvgFillPaint(vg, nvgLinearGradient(vg, x-r, y-r, x+r, y+r, top, bottom)); nvgFill(vg);
}
void socketDrawing(NVGcontext* vg, float x, float y,bool stone=false) {
    disc(vg,x+.5f,y+1,11.9f,nvgRGBA(0,0,0,60));
    // Concentric circular surfaces give the washer and bore a smooth silhouette.
    // Continuous shading avoids the old polygon nut and segmented reflections.
    metalDisc(vg,x,y,11.5f,nvgRGB(230,232,226),nvgRGB(100,105,103));
    metalDisc(vg,x,y,10.5f,nvgRGB(123,129,126),nvgRGB(224,226,218));
    metalDisc(vg,x,y,9.6f,stone?nvgRGB(250,250,239):nvgRGB(241,247,251),stone?nvgRGB(151,157,151):nvgRGB(151,162,171));
    disc(vg,x,y,8.1f,nvgRGB(51,56,53));
    metalDisc(vg,x,y,7.5f,stone?nvgRGB(110,119,113):nvgRGB(110,122,132),stone?nvgRGB(223,226,213):nvgRGB(223,233,240));
    disc(vg,x,y,6.35f,nvgRGB(7,10,9));
}
struct MetalPort : app::PortWidget {
    MetalPort() { box.size=Vec(24,24); }
    void draw(const DrawArgs& args) override { auto* m=dynamic_cast<Metropolix*>(module);socketDrawing(args.vg,12,12,m && m->panelStyle.load()==Metropolix::ATRIUM); }
};

#include "QuoVadisArt.hpp"

int screenParam(Metropolix* m) {
    if(!m) return -1;
    int t=m->selectedTrack.load(), l=m->selectedLane.load(), f=m->uiField.load();
    bool mod=m->uiMod.load();
    if(m->uiChooseFunction.load())return -1;
    switch(m->uiPage.load()) {
        case Metropolix::TRACK_MENU_PAGE: return f?trackMenuParam(m):-1;
        case Metropolix::LOOPY_PAGE: return Metropolix::LOOPY_PULSES;
        case Metropolix::BPM_PAGE: return m->integer(Metropolix::CLOCK_SOURCE)==3?-1:m->externalClock.load()?Metropolix::EXT_CLOCK_DIV:Metropolix::TEMPO;
        case Metropolix::CLOCK_OUT_PAGE: return f?(m->integer(Metropolix::CLOCK_OUT_DIV)>1?Metropolix::CLOCK_OUT_OFFSET:-1):Metropolix::CLOCK_OUT_DIV;
        case Metropolix::SCALE_PAGE: return f ? (m->scaleEditing.load()?Metropolix::SCALE_NOTE:Metropolix::SCALE) : m->scaleRootParam();
        case Metropolix::ORDER_PAGE: return mod ? Metropolix::modParam(l,Metropolix::MOD_ORDER) : Metropolix::trackParam(t,Metropolix::ORDER);
        case Metropolix::LENGTH_PAGE:
            if(m->uiAlt.load())return mod?Metropolix::modParam(l,Metropolix::MOD_STAGE_OFFSET):Metropolix::trackParam(t,Metropolix::OFFSET);
            return mod?Metropolix::modParam(l,f?Metropolix::MOD_PULSE_LIMIT:Metropolix::MOD_LENGTH):(f?Metropolix::extraParam(t,Metropolix::PULSE_LIMIT):Metropolix::trackParam(t,Metropolix::LENGTH));
        case Metropolix::DIV_PAGE: return mod ? Metropolix::modParam(l,Metropolix::MOD_DIVISION) : (f?Metropolix::extraParam(t,Metropolix::PULSE_DIV):Metropolix::trackParam(t,Metropolix::DIVISION));
        case Metropolix::SWING_PAGE: return mod ? -1 : Metropolix::swingParam(t);
        case Metropolix::SLIDE_PAGE: return mod ? -1 : Metropolix::trackParam(t,Metropolix::SLIDE_TIME);
        case Metropolix::CV_TYPE_PAGE: return mod?Metropolix::modParam(l,Metropolix::MOD_TYPE):Metropolix::cvTypeParam(t);
        case Metropolix::GATE_PAGE: return mod ? -1 : f?Metropolix::gateScaleParam(t):Metropolix::trackParam(t,Metropolix::GATE_LENGTH);
        case Metropolix::STAGE_PAGE: return mod ? Metropolix::modParam(l,Metropolix::MOD_VALUE+m->selectedStage.load()) : Metropolix::stageParam(t,m->selectedStage.load(),m->selectedRow.load());
        case Metropolix::A_PAGE: case Metropolix::B_PAGE: return -1; // Unconfirmed choices use navigation, not the audio parameter.
        case Metropolix::AUX_PAGE: return f && metro::modGlobalDestination(static_cast<metro::ModDestination>(m->integer(Metropolix::auxParam(m->selectedAux.load(),Metropolix::AUX_DESTINATION))))?-1:Metropolix::auxParam(m->selectedAux.load(),f?Metropolix::AUX_ASSIGNMENT:Metropolix::AUX_DESTINATION);
        case Metropolix::CTRL_PAGE: return Metropolix::ctrlParam(m->selectedCtrl.load(),f?Metropolix::CTRL_ASSIGNMENT:Metropolix::CTRL_DESTINATION);
        case Metropolix::SETUP_PAGE: return f?setupMenuParam(m):-1;
        case Metropolix::GLOBAL_SETUP_PAGE:return f?globalMenuParam(m):-1;
        case Metropolix::UI_SETUP_PAGE:return f?uiSetupParam(m):-1;
        case Metropolix::MIDI_SETUP_PAGE:return f?midiSetupParam(m):-1;
        case Metropolix::MX_SETUP_PAGE:return mxParam(m);
        case Metropolix::MOD_MENU_PAGE:return f?modMenuParam(m):-1;
        case Metropolix::MOD_ASSIGN_PAGE:return f && metro::modGlobalDestination(static_cast<metro::ModDestination>(m->integer(Metropolix::modParam(l,Metropolix::MOD_DESTINATION))))?-1:Metropolix::modParam(l,f?Metropolix::MOD_ASSIGNMENT:Metropolix::MOD_DESTINATION);
        case Metropolix::TUNER_PAGE:return Metropolix::TUNER_OCTAVE;
        case Metropolix::PRESET_SETUP_PAGE: return m->presetSetupEditing.load()?presetSetupParam(m):-1;
        case Metropolix::ACCUM_PAGE: return Metropolix::accumScreenParam(t,f);
        default: return -1;
    }
}
void screenPage(Metropolix* m, int page) {
    if(!m) return;
    m->clearPitchPreview();
    if(m->timingShortcutHeld.load() && m->timingShortcutHeld.load()!=page){m->timingShortcutHeld.store(0);m->timingShortcutUsed.store(true);}
    if(page!=Metropolix::TUNER_PAGE)m->tunerActive.store(false);
    if(m->uiPage.load()==Metropolix::LOOPY_PAGE && page!=Metropolix::LOOPY_PAGE && m->loopyHeld.load() && m->integer(Metropolix::RUN))m->latchLoopy();
    m->cancelDraft();
    m->loopyMuteHeld.store(false);
    if(page!=Metropolix::SCALE_PAGE)m->scaleEditing.store(false);
    if(page!=Metropolix::BACKUP_PRESET_PAGE && !(page==Metropolix::BACKUP_PAGE && m->uiPage.load()==Metropolix::BACKUP_PRESET_PAGE))m->setupDirectChild.store(false);
    m->presetSetupEditing.store(false);
    m->uiPage.store(page); m->uiField.store(page==Metropolix::PRESET_SETUP_PAGE?1:0); m->uiChooseFunction.store(false);
    m->outputDraft[0].store(-1);m->outputDraft[1].store(-1);
}
int parentPage(Metropolix* m){
    switch(m->uiPage.load()){
        case Metropolix::BACKUP_PAGE:return m->setupDirectChild.load()?Metropolix::SETUP_PAGE:Metropolix::MIDI_SETUP_PAGE;
        case Metropolix::BACKUP_PRESET_PAGE:return Metropolix::BACKUP_PAGE;
        case Metropolix::FACTORY_PAGE:return Metropolix::SETUP_PAGE;
        case Metropolix::FACTORY_CONFIRM_PAGE:return Metropolix::FACTORY_PAGE;
        case Metropolix::TUNER_PAGE:return m->setupDirectChild.load()?Metropolix::SETUP_PAGE:Metropolix::GLOBAL_SETUP_PAGE;
        case Metropolix::GLOBAL_SETUP_PAGE:case Metropolix::UI_SETUP_PAGE:case Metropolix::MIDI_SETUP_PAGE:case Metropolix::PRESET_SETUP_PAGE:return Metropolix::SETUP_PAGE;
        default:return Metropolix::HOME;
    }
}
void encoderPress(Metropolix* m) {
    if(!m) return;
    if(confirmDraft(m)){if(m->uiPage.load()==Metropolix::SETUP_PAGE)m->uiField.store(0);if(presetSetupPage(m))m->presetSetupEditing.store(false);return;}
    int p=m->uiPage.load();
    if(trackMenuPage(m)){trackMenuEncoderPress(m);return;}
    if(outputPage(m)){outputEncoderPress(m);return;}
    if(p==Metropolix::SLIDE_PAGE && !m->uiMod.load()) {
        int id=Metropolix::slideTypeParam(m->selectedTrack.load());
        setWithHistory(m->getParamQuantity(id),metro::nextSlideType(m->integer(id)));return;
    }
    if(p==Metropolix::SCALE_PAGE) {
        if(m->scaleEditing.load() && m->uiField.load())scaleToggleNote(m);
        else if(m->uiField.load())scaleLockPress(m);
        else m->uiField.store(1);
        return;
    }
    if(p==Metropolix::LOOPY_PAGE) {
        if(m->loopyLatched.load())m->clearLoopy();
        else setWithHistory(m->getParamQuantity(Metropolix::LOOPY_TARGET),1-m->integer(Metropolix::LOOPY_TARGET));
        return;
    }
    if(p==Metropolix::CHAIN_PAGE) {chainEncoderPress(m);return;}
    if(p==Metropolix::SETUP_PAGE || p==Metropolix::GLOBAL_SETUP_PAGE || p==Metropolix::UI_SETUP_PAGE || p==Metropolix::TUNER_PAGE){setupEncoderPress(m);return;}
    if(p==Metropolix::HOME) { m->uiHome.store((m->uiHome.load()+1)%4); return; }
    if(p==Metropolix::BACKUP_PAGE){backupChoose(m,m->backupCursor.load());return;}
    if(p==Metropolix::BACKUP_PRESET_PAGE){backupPresetChoose(m,m->backupPresetCursor.load());return;}
    if(factoryPage(m)){factoryEncoderPress(m);return;}
    if(p==Metropolix::MX_SETUP_PAGE){m->uiField.store(mxField(m)==2?0:mxAssignment(m)<0?0:1-m->uiField.load());return;}
    if(p==Metropolix::MOD_MENU_PAGE || p==Metropolix::MIDI_SETUP_PAGE){midiEncoderPress(m);return;}
    if(p==Metropolix::PRESET_SETUP_PAGE) { presetSetupEncoderPress(m); return; }
    if(p==Metropolix::LOAD_PAGE) { setWithHistory(m->getParamQuantity(Metropolix::LOAD_DELAY), (m->integer(Metropolix::LOAD_DELAY)+1)%3); return; }
    if(p==Metropolix::SAVE_PAGE) { m->presetSaveAction.store((m->presetSaveAction.load()+1)%3); return; }
    if(p==Metropolix::ACCUM_PAGE) { m->uiField.store((m->uiField.load()+1)%6); return; }
    if(p==Metropolix::ORDER_PAGE) {
        int id=m->uiMod.load()?Metropolix::modParam(m->selectedLane.load(),Metropolix::MOD_REVERSED):Metropolix::extraParam(m->selectedTrack.load(),Metropolix::REVERSED);
        auto* q=m->getParamQuantity(id);setWithHistory(q,1-q->getValue());return;
    }
    if(p==Metropolix::STAGE_PAGE) {
        if(simpleStagePage(m)){if(binaryStagePage(m) && m->uiAlt.load())invertStagePattern(m);return;}
        if(stagePairPage(m)){stagePairPress(m);return;}
        if(cvPage(m)){cvEncoderPress(m);return;}
        if(overridePage(m) && m->selectedRow.load()==Metropolix::PITCH_OVERRIDE){syncPitchOverrides(m);return;}
        int id=-1, count=2, t=m->selectedTrack.load(),s=m->selectedStage.load();
        if(m->uiMod.load()){modStagePress(m);return;}
        else if(m->selectedRow.load()==Metropolix::PROBABILITY)id=Metropolix::stageOption(t,s,Metropolix::PROB_TARGET);
        else if(m->selectedRow.load()==Metropolix::ACCUMULATION){id=Metropolix::stageOption(t,s,Metropolix::ACCUM_TRIGGER);count=3;}
        if(id>=0){auto* q=m->getParamQuantity(id);setWithHistory(q,(static_cast<int>(q->getValue())+1)%count);}return;
    }
    if(p==Metropolix::CLOCK_OUT_PAGE || p==Metropolix::SCALE_PAGE || p==Metropolix::LENGTH_PAGE || p==Metropolix::AUX_PAGE || p==Metropolix::MOD_ASSIGN_PAGE || p==Metropolix::CTRL_PAGE || (p==Metropolix::GATE_PAGE && !m->uiMod.load()) ||
       (p==Metropolix::DIV_PAGE && !m->uiMod.load()))
        m->uiField.store(1-m->uiField.load());
}


void openLoopy(Metropolix* m) {
    if(!m)return;
    if(m->uiMod.load()){m->selectedTrack.store(0);m->uiMod.store(false);}
    screenPage(m,Metropolix::LOOPY_PAGE);m->uiAlt.store(false);m->uiEdit.store(false);
}
void loopRange(Metropolix* m,int first,int last) {
    if(!m)return;
    if(!m->integer(Metropolix::RUN))return;
    openLoopy(m);m->loopyHeld.store(0);m->requestLoopy((1u<<first)|(1u<<last));m->latchLoopy();
}
void appendLoopyMenu(ui::Menu* menu,Metropolix* m,int start=-1) {
    if(!m)return;
    menu->addChild(createMenuLabel("Loopy performance"));
    menu->addChild(createMenuItem("Open Loopy","",[m]{openLoopy(m);}));
    menu->addChild(createSubmenuItem("Loop pulses","",[m](ui::Menu* child){
        for(int n=0;n<=64;++n)child->addChild(createCheckMenuItem(n?std::to_string(n):"Auto","",[m,n]{return m->integer(Metropolix::LOOPY_PULSES)==n;},[m,n]{setWithHistory(m->getParamQuantity(Metropolix::LOOPY_PULSES),n);}));
    }));
    menu->addChild(createCheckMenuItem("Target both tracks","",[m]{return m->integer(Metropolix::LOOPY_TARGET)!=0;},[m]{setWithHistory(m->getParamQuantity(Metropolix::LOOPY_TARGET),1-m->integer(Metropolix::LOOPY_TARGET));}));
    if(start>=0 && m->integer(Metropolix::RUN))menu->addChild(createSubmenuItem("Latch range from stage "+std::to_string(start+1),"",[m,start](ui::Menu* child){
        for(int end=0;end<8;++end)child->addChild(createMenuItem("Through stage "+std::to_string(end+1),"",[m,start,end]{loopRange(m,start,end);}));
    }));
    menu->addChild(createMenuItem("Latch current loop","",[m]{if(m->integer(Metropolix::RUN))m->latchLoopy();}));
    menu->addChild(createMenuItem("Release loop / audition","",[m]{m->clearLoopy();}));
    menu->addChild(createCheckMenuItem("Mute all MOD lanes","",[m]{return m->integer(Metropolix::MOD_GROUP_MUTE)!=0;},[m]{setWithHistory(m->getParamQuantity(Metropolix::MOD_GROUP_MUTE),1-m->integer(Metropolix::MOD_GROUP_MUTE));}));
    menu->addChild(createCheckMenuItem("Mute tracks / MOD lanes","",[m]{return m->loopyMuteHeld.load();},[m]{m->loopyMuteHeld.store(!m->loopyMuteHeld.load());m->loopyMuteUsed.store(true);}));
}

// An actual Rack Knob under every continuous/discrete value control. This
// preserves fine adjustment, snapping, numeric entry, and one undo per drag.
struct HardwareTooltip : ui::Tooltip {
    widget::Widget* owner=nullptr;
    void step() override {
        ui::Tooltip::step();
        if(owner && parent) box.pos=owner->getAbsoluteOffset(owner->box.size).round();
        if(parent) box=box.nudge(parent->box.zeroPos());
    }
};
struct HardwareControl : ValueKnob {
    enum Look { BUTTON, DARK_BUTTON, WHITE_KNOB, BLACK_KNOB, ENCODER, INVISIBLE, SOCKET, BLUE_BUTTON, CREAM_BUTTON };
    Look look=BUTTON;
    Metropolix* metroModule=nullptr;
    std::string name, help;
    std::string caption;
    int stageNumber=0;
    std::function<int()> binding;
    std::function<void()> action, rightAction, dragAction;
    std::function<void()> longAction, releaseAction, navigationStart, navigationEnd;
    std::function<bool()> longEnabled;
    std::function<std::string()> navigationValue;
    std::function<bool()> navigationMode;
    std::function<void(int)> navigationTurn;
    std::function<void(ui::Menu*)> extraMenu;
    std::function<bool()> active;
    std::function<bool()> dimActive;
    std::function<std::string()> detailHelp;
    std::function<NVGcolor()> activeColor;
    NVGcolor lightColor=nvgRGB(135,187,227);
    ui::Tooltip* tip=nullptr;
    bool dragging=false,finishingClick=false,shortcutEdited=false;
    bool previewCaptured=false;std::uint64_t previewSerial=0;
    int gestureParam=-1,gestureOwner=-1;
    bool ctrlBlocked=false,ctrlTakeover=false;
    int pressMods=0;
    bool navigating=false;
    float navigationDelta=0;
    float moved=0;
    float customSpeed=0;
    bool doubleClickReset=true;
    bool confirmationCapable=false,confirming=false;
    bool longFired=false;
    double heldSince=0, longSeconds=1;
    int controllerOwner=-1;
    std::function<std::vector<int>()> groupedParams;
    std::function<std::vector<int>()> dragParams;
    std::vector<int> gestureIds;
    std::vector<float> gestureBefore;
    int pickupGestureParam=-1;float pickupPhysicalBefore=0;metro::Pickup pickupBefore,pickupAfter;
    int pickupScrollParam=-1;float pickupScrollPhysicalBefore=0,pickupScrollPhysicalAfter=0;metro::Pickup pickupScrollBefore,pickupScrollAfter;std::uint64_t pickupScrollSerial=0;
    bool finishPickupScroll(const event::Leave& e) {
        if(pickupScrollParam<0 || !metroModule)return false;
        int id=pickupScrollParam;float current=metroModule->params[id].getValue();
        if(pickupScrollSerial!=metroModule->uiRestoreSerial.load()) {
            auto* boundModule=module;module=nullptr;bool enabled=settings::knobScroll;settings::knobScroll=true;ValueKnob::onLeave(e);settings::knobScroll=enabled;module=boundModule;pickupScrollParam=-1;return true;
        }
        // Native Knob commits scroll history on Leave. Hide its raw-only
        // difference, then record the physical value and pickup state together.
        metroModule->params[id].setValue(pickupScrollPhysicalBefore);
        bool enabled=settings::knobScroll;settings::knobScroll=true;ValueKnob::onLeave(e);settings::knobScroll=enabled;
        metroModule->params[id].setValue(current);pickupScrollParam=-1;
        panelControlHistory(metroModule,id,pickupScrollPhysicalBefore,pickupScrollPhysicalAfter,pickupScrollBefore,pickupScrollAfter,"scroll recalled control");return true;
    }
    HardwareControl() { smooth=false; forceLinear=true; speed=.55f; box.size=Vec(16,16); }
    std::string adjustmentText() override {
        auto* q=getParamQuantity();if(!q)return {};
        if(metroModule && metroModule->uiDraftParam==paramId){std::string value=previewDisplayString(metroModule,paramId);if(value.size()>2 && value.substr(value.size()-2)==" ?")value.resize(value.size()-2);return q->getLabel()+": "+value+q->getUnit()+" ?\nPress encoder to apply.";}
        return ValueKnob::adjustmentText();
    }
    ~HardwareControl() { clearTip(); }
    void capturePreview(){
        previewCaptured=confirming && metroModule && metroModule->uiDraftParam==paramId && pendingConfirmation(metroModule);
        previewSerial=metroModule?metroModule->uiDraftSerial.load():0;
    }
    bool blockCtrlGesture() {
        if(!dragging || gestureParam<0)return false;
        int owner=metroModule?metroModule->ctrlOwner(gestureParam):-1;
        if(owner!=gestureOwner){ctrlBlocked=true;ctrlTakeover=true;}
        if(ctrlBlocked){
            module=nullptr;clearAdjustmentTip();clearTip();if(owner>=0)controllerOwner=owner;
            if(ctrlTakeover && metroModule && metroModule->uiDraftParam==gestureParam)metroModule->cancelDraft();
        }
        return ctrlBlocked;
    }
    void bind() {
        if(dragging) return;
        int id=binding ? binding() : -1;
        controllerOwner=metroModule?metroModule->ctrlOwner(id):-1;
        confirming=confirmationCapable && requiresConfirmation(metroModule,id);
        module=(metroModule && id>=0 && controllerOwner<0) ? metroModule : nullptr;
        paramId=id>=0 ? id : 0;
        if(auto* q=getParamQuantity()) speed=customSpeed>0?customSpeed:(q->snapEnabled && q->getRange()>0?std::max(.7f,30.f/q->getRange()):.7f);
    }
    virtual std::string tooltipText() {
        auto* q=getParamQuantity();
        std::string s=q ? q->getString() : name;
        if(controllerOwner>=0 && metroModule) s=metroModule->getParamQuantity(paramId)->getString()+string::f("\nControlled by CTRL %d. Turn that knob; right-click to change its assignment.",controllerOwner+1);
        if(!help.empty()) s += "\n"+help;
        if(metroModule && paramId==metroModule->uiDraftParam)s+="\nPending: "+previewDisplayString(metroModule,paramId)+". Press encoder to apply.";
        if(detailHelp) s += "\n"+detailHelp();
        if(metroModule && metroModule->waitingPickup(paramId)){float physical=metroModule->params[paramId].getValue(),saved=metroModule->panelValue(paramId);s+=string::f("\nWaiting for pickup: saved %+.3f, physical %+.3f. Move %s to reach saved position, or right-click to take over.",saved,physical,saved>physical?"up / clockwise":"down / counterclockwise");}
        if(q) s += "\nHold + drag up/down to increase/decrease. Ctrl = fine. Right-click = values/reset.";
        return s;
    }
    void clearTip() {
        if(!tip) return;
        if(tip->parent) tip->parent->removeChild(tip);
        delete tip; tip=nullptr;
    }
    void onEnter(const event::Enter&) override {
        bind();
        if(!settings::tooltips || tip || dragging) return;
        auto* tooltip=new HardwareTooltip; tooltip->owner=this; tip=tooltip;
        tip->text=tooltipText(); APP->scene->addChild(tip);
    }
    void onLeave(const event::Leave& e) override { clearTip();if(!finishPickupScroll(e))ValueKnob::onLeave(e); }
    void step() override {
        invalidateDraft(metroModule);
        invalidateOutputDrafts(metroModule);
        blockCtrlGesture();
        if(metroModule && metroModule->scaleEditing.load() && !metro::userScale(metroModule->integer(Metropolix::SCALE)))metroModule->scaleEditing.store(false);
        bind(); ValueKnob::step();
        if(tip) { tip->text=tooltipText(); tip->box.pos=getAbsoluteOffset(box.size); }
        if(dragging && !ctrlTakeover && moved<3 && !longFired && longAction && (!longEnabled || longEnabled()) && system::getTime()-heldSince>=longSeconds) {
            // A hold acts now, so its undo origin is the latest applied state,
            // including edits made by another control while the timer ran.
            auto ids=groupedParams?groupedParams():std::vector<int>{};
            if(ids.empty() && getParamQuantity())ids.push_back(paramId);
            std::vector<float> before;for(int id:ids)before.push_back(metroModule->params[id].getValue());
            longFired=true;longAction();
            if(!ids.empty())parameterHistory(metroModule,ids,before,"long press "+name);
        }
        if(dragging && navigating && moved>0 && navigationValue && settings::tooltips) {
            if(!tip){auto* t=new HardwareTooltip;t->owner=this;tip=t;APP->scene->addChild(t);}
            tip->text=navigationValue();
        }
    }
    void onHover(const event::Hover& e) override { app::ParamWidget::onHover(e); }
    void onButton(const event::Button& e) override {
        if(e.action==GLFW_PRESS && pickupScrollParam>=0){event::Leave leave;leave.context=e.context;finishPickupScroll(leave);}
        if(e.button==GLFW_MOUSE_BUTTON_LEFT && e.action==GLFW_PRESS)pressMods=e.mods;
        bind();
        if(metroModule && (e.button!=GLFW_MOUSE_BUTTON_LEFT || paramId<Metropolix::PITCH || paramId>=Metropolix::PITCH+8))metroModule->clearPitchPreview();
        if(e.button==GLFW_MOUSE_BUTTON_RIGHT && e.action==GLFW_PRESS) {
            if(metroModule && std::find(metroModule->uiDraftIds.begin(),metroModule->uiDraftIds.end(),paramId)!=metroModule->uiDraftIds.end())metroModule->cancelDraft();
            clearTip();
            if(controllerOwner>=0)appendCtrlMenu(createMenu(),metroModule,controllerOwner);
            else if(rightAction) rightAction();
            else if(getParamQuantity()) createContextMenu();
            else { auto* menu=createMenu(); menu->addChild(createMenuLabel(name)); menu->addChild(createMenuLabel(help)); }
            e.consume(this); return;
        }
        app::ParamWidget::onButton(e);
    }
    void onDragStart(const event::DragStart& e) override {
        if(e.button!=GLFW_MOUSE_BUTTON_LEFT)return;
        if(pickupScrollParam>=0){event::Leave leave;leave.context=e.context;finishPickupScroll(leave);}
        bind();if(metroModule && (paramId<Metropolix::PITCH || paramId>=Metropolix::PITCH+8))metroModule->clearPitchPreview();
        dragging=true; moved=0; shortcutEdited=false;clearTip();heldSince=system::getTime();longFired=false;
        gestureParam=(module || controllerOwner>=0)?paramId:-1;gestureOwner=controllerOwner;ctrlBlocked=controllerOwner>=0;ctrlTakeover=false;
        navigating=navigationMode && navigationMode(); navigationDelta=0;
        if(dragAction) dragAction();
        if(navigating && navigationStart)navigationStart();
        ValueKnob::onDragStart(e);
        capturePreview();
        gestureIds=dragParams?dragParams():groupedParams?groupedParams():std::vector<int>{};gestureBefore.clear();
        for(int id:gestureIds)gestureBefore.push_back(metroModule->params[id].getValue());
        pickupGestureParam=metroModule && metroModule->waitingPickup(paramId)?paramId:-1;
        if(pickupGestureParam>=0){pickupPhysicalBefore=metroModule->params[paramId].getValue();pickupBefore=pickupAfter=uiPickup(metroModule,paramId);}
    }
    void onDragMove(const event::DragMove& e) override {
        // Once the hold fires, consume the rest of this press. The action can
        // open another page, whose values need a fresh binding and gesture.
        if(longFired)return;
        moved+=e.mouseDelta.norm();
        if(blockCtrlGesture())return;
        if(e.mouseDelta.norm()==0){updateAdjustmentTip();return;}
        if(confirming && getParamQuantity()){
            turnDraft(metroModule,paramId,e.mouseDelta.y,navigationDelta,APP->window->getMods() & GLFW_MOD_CONTROL);
            capturePreview();
        }
        else if(navigating && navigationTurn) {
            navigationDelta-=e.mouseDelta.y;
            int turns=static_cast<int>(navigationDelta/22.f);
            if(turns){navigationTurn(turns);navigationDelta-=22.f*turns;}
        } else {
            ValueKnob::onDragMove(e);
            stagePairDrag(metroModule,paramId,gestureIds,gestureBefore);
            ratchetStageDrag(metroModule,paramId,gestureIds,gestureBefore);
            if(gestureIds.size()>1 && ((cvPage(metroModule) && !cvGate(metroModule)) || (modStagePage(metroModule) && !modStageGates(metroModule)))) {
                auto it=std::find(gestureIds.begin(),gestureIds.end(),paramId);
                if(it!=gestureIds.end()) {
                    auto type=metroModule->effectiveCvType(metroModule->selectedTrack.load());
                    bool mod=modStagePage(metroModule);
                    auto origin=[&](size_t index){
                        if(!mod)return metro::trackCvVoltage(gestureBefore[index],type);
                        auto* q=metroModule->getParamQuantity(gestureIds[index]);
                        return metro::clamp(gestureBefore[index],q->getMinValue(),q->getMaxValue());
                    };
                    float delta=metroModule->params[paramId].getValue()-origin(it-gestureIds.begin());
                    for(size_t i=0;i<gestureIds.size();++i)if(gestureIds[i]!=paramId)metroModule->getParamQuantity(gestureIds[i])->setValue(origin(i)+delta);
                }
            }
            if(gestureIds.size()>1 && overridePage(metroModule)) {
                auto it=std::find(gestureIds.begin(),gestureIds.end(),paramId);
                if(it!=gestureIds.end()) {
                    int index=int(it-gestureIds.begin());
                    int id=metroModule->integer(Metropolix::SCALE_LOCK);if(id<0)id=metroModule->integer(Metropolix::SCALE);
                    auto scale=metroModule->uiScale(id);
                    bool pitch=!metroModule->uiMod.load() && metroModule->selectedRow.load()==Metropolix::PITCH_OVERRIDE;
                    int offDegree=metro::nearestDegree(0,scale)-1,maxDegree=metro::nearestDegree(60,scale);
                    auto degree=[&](float v){return v<0?offDegree:metro::nearestDegree(v,scale);};
                    float current=metroModule->params[paramId].getValue();
                    float delta=pitch?float(degree(current)-degree(gestureBefore[index])):current-gestureBefore[index];
                    for(size_t i=0;i<gestureIds.size();++i)if(gestureIds[i]!=paramId) {
                        float v=gestureBefore[i]+delta;
                        if(pitch){int d=metro::clamp(degree(gestureBefore[i])+int(delta),offDegree,maxDegree);v=d==offDegree?-1.f:float(metro::clamp(metro::degreeToSemitone(d,scale),0,60));}
                        metroModule->getParamQuantity(gestureIds[i])->setValue(v);
                    }
                }
            }
        }
        if(pickupGestureParam>=0){bool pitch=Metropolix::panelIndex(pickupGestureParam)<8;int mode=metroModule->integer(pitch?Metropolix::LOAD_PITCH:Metropolix::LOAD_CTRL);pickupAfter.process(metroModule->params[pickupGestureParam].getValue(),mode==(pitch?Metropolix::PitchThru:Metropolix::CtrlThru),mode>=(pitch?Metropolix::PitchThru:Metropolix::CtrlThru));}
        updateAdjustmentTip();
    }
    void onDragEnd(const event::DragEnd& e) override {
        if(e.button!=GLFW_MOUSE_BUTTON_LEFT)return;
        if(longFired){
            // The hold already recorded its own action at firing time. Finish
            // Rack's cursor/knob state without attributing later writes to it.
            auto* boundModule=module;module=nullptr;ValueKnob::onDragEnd(e);module=boundModule;
            gestureIds.clear();gestureBefore.clear();pickupGestureParam=-1;
            if(releaseAction)releaseAction();dragging=false;pressMods=0;ctrlTakeover=false;clearTip();return;
        }
        if(blockCtrlGesture() && (!longFired || ctrlTakeover)){
            ValueKnob::onDragEnd(e);gestureIds.clear();gestureBefore.clear();pickupGestureParam=-1;
            invalidateDraft(metroModule);
            if(releaseAction)releaseAction();dragging=false;pressMods=0;ctrlTakeover=false;clearTip();return;
        }
        if(previewCaptured && (previewSerial!=metroModule->uiDraftSerial.load() || !validDraft(metroModule))){
            // A preview invalidated while held cannot become a fresh click on
            // release, even if a UI step has already removed its draft.
            auto* boundModule=module;module=nullptr;finishingClick=true;ValueKnob::onDragEnd(e);module=boundModule;finishingClick=false;
            invalidateDraft(metroModule);gestureIds.clear();gestureBefore.clear();pickupGestureParam=-1;previewCaptured=false;
            if(releaseAction)releaseAction();dragging=false;pressMods=0;clearTip();return;
        }
        if(moved==0 && !shortcutEdited){
            // A stationary press does not own parameter writes made elsewhere.
            // Finish native capture first, then perform the real click with
            // the quantity rebound so its toggle/menu retains normal history.
            auto* boundModule=module;module=nullptr;finishingClick=true;ValueKnob::onDragEnd(e);module=boundModule;finishingClick=false;
            event::Action click;onAction(click);gestureIds.clear();gestureBefore.clear();pickupGestureParam=-1;
            if(releaseAction)releaseAction();dragging=false;pressMods=0;clearTip();return;
        }
        if((confirming || (navigating && outputPage(metroModule))) && moved>=3 && !longFired){
            // A preview drag never writes parameters. Finish Rack's cursor and
            // knob state without attributing external writes to this gesture.
            auto* boundModule=module;module=nullptr;ValueKnob::onDragEnd(e);module=boundModule;
            invalidateDraft(metroModule);invalidateOutputDrafts(metroModule);gestureIds.clear();gestureBefore.clear();
            if(releaseAction)releaseAction();dragging=false;pressMods=0;clearTip();return;
        }
        bool rawQuantity=needsRawDragHistory(getParamQuantity()) && std::find(gestureIds.begin(),gestureIds.end(),paramId)!=gestureIds.end();
        bool rawMicro=moved>0 && moved<3 && rawQuantity;
        bool batch=!gestureIds.empty() && (gestureIds.size()>1 || !getParamQuantity() || cvPage(metroModule) || rawQuantity) && (moved>=3 || longFired || rawMicro);
        float pickupPhysicalAfter=pickupGestureParam>=0?metroModule->params[pickupGestureParam].getValue():0;
        if(pickupGestureParam>=0)metroModule->params[pickupGestureParam].setValue(pickupPhysicalBefore);
        // The batch owns its raw receipt. Native cleanup needs no quantity and
        // must not compare a getter whose range/type changed during the drag.
        auto* boundModule=module;if(batch)module=nullptr;
        // Tiny movement still qualifies for the existing click action.
        // Record its raw edit first, then let that click act on the new value.
        finishingClick=rawMicro;ValueKnob::onDragEnd(e);module=boundModule;finishingClick=false;
        if(pickupGestureParam>=0){metroModule->params[pickupGestureParam].setValue(pickupPhysicalAfter);panelControlHistory(metroModule,pickupGestureParam,pickupPhysicalBefore,pickupPhysicalAfter,pickupBefore,pickupAfter,"move recalled control");pickupGestureParam=-1;}
        if(batch)parameterHistory(metroModule,gestureIds,gestureBefore,"edit track settings");
        if(rawMicro){event::Action click;onAction(click);}
        gestureIds.clear();gestureBefore.clear();
        if(navigating && navigationEnd)navigationEnd();
        if(releaseAction)releaseAction();
        dragging=false;pressMods=0;clearTip();
    }
    void onHoverScroll(const event::HoverScroll& e) override {
        if(e.scrollDelta.y==0)return;
        if(pickupScrollParam>=0 && metroModule && pickupScrollSerial!=metroModule->uiRestoreSerial.load()){event::Leave leave;leave.context=e.context;finishPickupScroll(leave);}
        bind();if(metroModule && (paramId<Metropolix::PITCH || paramId>=Metropolix::PITCH+8))metroModule->clearPitchPreview();
        int candidate=-1;float candidateValue=0;metro::Pickup candidatePickup;std::uint64_t candidateSerial=0;
        if(pickupScrollParam<0 && !dragging && !adjusting && metroModule && getParamQuantity() && metroModule->waitingPickup(paramId) && settings::knobScroll && !APP->scene->rackScroll->isScrolling()) {
            // Finish an earlier native scroll before capturing a newly recalled
            // target, so its old-value cell cannot generate a duplicate receipt.
            event::Leave leave;leave.context=e.context;ValueKnob::onLeave(leave);
            candidate=paramId;candidateValue=metroModule->params[paramId].getValue();candidatePickup=uiPickup(metroModule,paramId);candidateSerial=metroModule->uiRestoreSerial.load();
        }
        float before=getParamQuantity()?getParamQuantity()->ParamQuantity::getValue():0;
        ValueKnob::onHoverScroll(e);
        if(candidate>=0) {
            if(metroModule->params[candidate].getValue()!=candidateValue){pickupScrollParam=candidate;pickupScrollPhysicalBefore=pickupScrollPhysicalAfter=candidateValue;pickupScrollBefore=pickupScrollAfter=candidatePickup;pickupScrollSerial=candidateSerial;}
            else {event::Leave leave;leave.context=e.context;ValueKnob::onLeave(leave);}
        }
        if(pickupScrollParam>=0 && metroModule->params[pickupScrollParam].getValue()!=before){pickupScrollPhysicalAfter=metroModule->params[pickupScrollParam].getValue();pickupScrollAfter=pickupAfterValue(metroModule,pickupScrollParam,pickupScrollAfter,pickupScrollPhysicalAfter);}
        if(pickupGestureParam>=0 && getParamQuantity() && getParamQuantity()->ParamQuantity::getValue()!=before)pickupAfter=pickupAfterValue(metroModule,pickupGestureParam,pickupAfter,getParamQuantity()->ParamQuantity::getValue());
    }
    void onAction(const event::Action&) override { if(!finishingClick && !ctrlTakeover && moved<3.f && !longFired && action) action(); }
    void onDoubleClick(const event::DoubleClick& e) override { if(pickupScrollParam>=0){event::Leave leave;leave.context=e.context;finishPickupScroll(leave);}if(doubleClickReset && getParamQuantity()){if(metroModule)metroModule->cancelDraft();ValueKnob::onDoubleClick(e);} }
    void appendContextMenu(ui::Menu* menu) override { if(extraMenu) extraMenu(menu);auto* m=metroModule;int id=paramId;if(m && m->waitingPickup(id))menu->addChild(createMenuItem("Take over at current physical position","",[m,id]{takeOverPanel(m,id);})); }
    void draw(const DrawArgs& args) override {
        auto* vg=args.vg;float x=box.size.x/2,y=box.size.y/2,r=std::min(x,y)-.7f;
        if(look==INVISIBLE)return;
        bool stone=atrium(metroModule);
        if(look==SOCKET){socketDrawing(vg,x,y,stone);return;}
        if(look==BUTTON || look==DARK_BUTTON || look==BLUE_BUTTON || look==CREAM_BUTTON) {
            stageButtonCap(vg,box.size.x,box.size.y,stone);
            bool on=active && active(),dim=dimActive && dimActive();
            float level=metroModule?metroModule->params[Metropolix::BUTTON_LEVEL].getValue()*.25f:1;
            NVGcolor light=activeColor?activeColor():lampBlue();
            rect(vg,x-3.2f,stageNumber?y-1:4,6.4f,1.8f,on?color::mult(light,level):dim?color::mult(light,.3f*level):nvgRGB(58,62,61),.7f);
            font(vg);
            auto label=stone?paper():nvgRGB(234,240,244);
            if(stageNumber)text(vg,x,11,std::to_string(stageNumber),6.5f,label,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
            if(!caption.empty())text(vg,x,box.size.y-5,caption,caption.size()>6?4.6f:5.2f,label,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
            return;
        }
        auto* q=getParamQuantity();
        satinKnob(vg,x,y,r,q?q->getScaledValue():.5f,look==WHITE_KNOB,look==ENCODER,stone);
    }
};

// Rack's native emissive layer keeps the single cyan lens responsive to Bloom.
struct PitchSliderLight : app::LightWidget {
    PitchSliderLight(){box.size=Vec(4,4);}
    void drawBackground(const DrawArgs&) override {}
    void drawLight(const DrawArgs& args) override {disc(args.vg,2,2,1.65f,color);}
};
struct HardwareSlider : HardwareControl {
    enum Kind {PITCH_SLIDER,PULSE_SWITCH,GATE_SWITCH};
    Kind kind=PITCH_SLIDER;int stage=0;
    PitchSliderLight* pitchLight=nullptr;
    HardwareSlider(){box.size=Vec(22,82);pitchLight=new PitchSliderLight;addChild(pitchLight);}
    void onDragMove(const event::DragMove& e) override {
        float before=getParamQuantity()?getParamQuantity()->ParamQuantity::getValue():0;
        HardwareControl::onDragMove(e);
        if(kind==PITCH_SLIDER && metroModule && getParamQuantity())metroModule->previewPitchEdit(paramId,before,getParamQuantity()->ParamQuantity::getValue());
    }
    void onHoverScroll(const event::HoverScroll& e) override {
        bind();float before=getParamQuantity()?getParamQuantity()->ParamQuantity::getValue():0;
        HardwareControl::onHoverScroll(e);
        if(kind==PITCH_SLIDER && metroModule && getParamQuantity())metroModule->previewPitchEdit(paramId,before,getParamQuantity()->ParamQuantity::getValue());
    }
    float handleHeight() const{return 13.f;}
    float handleCenter(){
        auto* q=getParamQuantity();float v=q?q->getScaledValue():0;
        return 8.5f+(1-v)*(box.size.y-17);
    }
    void step() override {
        HardwareControl::step();pitchLight->visible=kind==PITCH_SLIDER || kind==GATE_SWITCH;
        pitchLight->box.pos=Vec(box.size.x/2-2,kind==PITCH_SLIDER?box.size.y-4:2);
        float brightness=metroModule?std::max(metroModule->lights[Metropolix::POSITION_1+stage].getBrightness(),metroModule->lights[Metropolix::POSITION_2+stage].getBrightness()):0;
        float level=metroModule?metroModule->params[Metropolix::SLIDER_LEVEL].getValue()*.25f:1;
        pitchLight->color=color::mult(lampBlue(),(.12f+.88f*brightness)*level);
    }
    void draw(const DrawArgs& args) override {
        auto* vg=args.vg;auto* q=getParamQuantity();float x=box.size.x/2;
        font(vg);
        if(kind==PITCH_SLIDER){
            satinKnob(vg,x,14,13,q?q->getScaledValue():.5f,true,false,atrium(metroModule));
            if(metroModule && metroModule->waitingPickup(Metropolix::PITCH+stage)){
                float a=(-.83f+1.66f*metroModule->panelValue(Metropolix::PITCH+stage))*M_PI;
                disc(vg,x+std::sin(a)*14.3f,14-std::cos(a)*14.3f,.8f,lampBlue());
            }
            return;
        }
        if(kind==GATE_SWITCH){
            int mode=q?int(std::round(q->getValue())):1;
            stageButtonCap(vg,box.size.x,box.size.y,atrium(metroModule));
            gateGlyph(vg,x,14,mode);
            const char* names[]={"REST","SINGLE","MULTI","HOLD"};
            text(vg,x,box.size.y+7,names[std::max(0,std::min(3,mode))],5.5f,ink(),NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
            return;
        }
        float h=box.size.y,cy=handleCenter();
        rect(vg,x-2.1f,7,4.2f,h-14,nvgRGB(8,11,11),.8f);
        rect(vg,x-5,cy-6.5f,10,13,nvgRGB(44,44,40),1);
        if(atrium(metroModule))rect(vg,x-4.3f,cy-5.8f,8.6f,11.3f,nvgRGB(210,207,192),1);
        else {
            nvgBeginPath(vg);nvgRoundedRect(vg,x-4.3f,cy-5.8f,8.6f,11.3f,1);
            nvgFillPaint(vg,nvgLinearGradient(vg,x,cy-5.8f,x,cy+5.5f,nvgRGB(222,229,234),nvgRGB(160,173,184)));nvgFill(vg);
        }
        stroke(vg,x-3.6f,cy-4.9f,x+3.6f,cy-4.9f,atrium(metroModule)?nvgRGB(248,246,229):nvgRGB(247,251,254),.6f);
        text(vg,x,h+7,q?std::to_string(int(std::round(q->getValue()))):"1",6.5f,ink(),NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);
    }
};

#include "HardwareDisplay.hpp"

#include "QuoVadisPanel.hpp"
