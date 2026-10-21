using M=Metropolix;
APP->scene->rackScroll->setGridOffset(Vec(-1.f,-.05f));
auto chooseStyle=[&](int index){
    ui::Menu root;appendContextMenu(&root);ui::MenuItem* selector=nullptr;
    for(auto* child:root.children)if(auto* item=dynamic_cast<ui::MenuItem*>(child))if(item->text=="Panel style")selector=item;
    check(selector!=nullptr,"module context menu exposes Panel style");if(!selector)return;
    auto* choices=selector->createChildMenu();check(choices!=nullptr,"Panel style submenu opens");if(!choices)return;
    int i=0;for(auto* child:choices->children)if(auto* item=dynamic_cast<ui::MenuItem*>(child)){if(i++==index){widget::EventContext context;event::Action action;action.context=&context;item->onAction(action);break;}}
    delete choices;
};
if(std::getenv("QUO_VADIS_PANEL_REOPEN_QA")) {
    if(qaFrame==8){check(m->panelStyle.load()==M::ATRIUM,"actual Rack save/reopen restores Atrium");metroqa::screenshot(dir+"/atrium-reopened.png");check(!qaFailed,"PANEL REOPEN QA COMPLETE");APP->window->close();}return;
}
if(qaFrame==5){
    APP->scene->rackScroll->setZoom(2);
    m->params[M::RUN].setValue(0);m->expanded.store(false);m->uiPage.store(M::HOME);m->uiHome.store(0);m->uiMod.store(false);m->uiAlt.store(false);
    settings::knobMode=settings::KNOB_MODE_LINEAR;settings::knobLinearSensitivity=.001f;
    for(int i=0;i<2;++i)m->params[M::ctrlParam(i,M::CTRL_DESTINATION)].setValue(metro::CtrlNone);
    int count[3]={};
    for(auto* child:children)if(auto* slider=dynamic_cast<HardwareSlider*>(child)) {
        ++count[slider->kind];float centre=slider->box.pos.x+slider->box.size.x/2;
        check(std::abs(centre-stageX(slider->stage))<.01f,"pitch, pulse and gate controls share stage centres");
        slider->bind();auto* q=slider->getParamQuantity();float base=slider->kind==0?.5f:slider->kind==1?4:1;q->setValue(base);
        drag(slider,-30);check(q->getValue()>base,"upward drag increases each stage control");
        drag(slider,30);check(std::abs(q->getValue()-base)<.001f,"downward drag returns each stage control");
    }
    check(count[0]==8 && count[1]==8 && count[2]==8,"all 24 stage controls remain interactive");
    int buttons=0;for(auto* child:children){if(auto* c=dynamic_cast<HardwareControl*>(child)){if(!c->caption.empty())++buttons;}else if(dynamic_cast<TransportButton*>(child))++buttons;}
    check(buttons==31,"all 31 labelled pushbuttons remain on the panel");
    std::vector<widget::Widget*> controls;
    for(auto* child:children)if(dynamic_cast<HardwareControl*>(child) || dynamic_cast<TransportButton*>(child))controls.push_back(child);
    bool clearBounds=true;
    for(size_t i=0;i<controls.size();++i){
        const auto& a=controls[i]->box;
        clearBounds&=a.pos.x>=0 && a.pos.y>=0 && a.pos.x+a.size.x<=510 && a.pos.y+a.size.y<=380;
        for(size_t j=i+1;j<controls.size();++j){const auto& b=controls[j]->box;
            bool overlaps=a.pos.x<b.pos.x+b.size.x && b.pos.x<a.pos.x+a.size.x && a.pos.y<b.pos.y+b.size.y && b.pos.y<a.pos.y+a.size.y;
            clearBounds&=!overlaps;
        }
    }
    check(clearBounds,"all compact-layout controls fit the panel with no overlapping hit targets");
    std::ofstream coordinates(dir+"/stage-button-alignment.csv");coordinates<<"stage,gate_left,function_left,gate_center,function_center,gate_right,function_right\n";
    for(int s=0;s<8;++s){
        HardwareControl* function=nullptr;HardwareSlider* gate=nullptr;
        for(auto* child:children){if(auto* c=dynamic_cast<HardwareControl*>(child))if(c->stageNumber==s+1)function=c;if(auto* c=dynamic_cast<HardwareSlider*>(child))if(c->kind==HardwareSlider::GATE_SWITCH && c->stage==s)gate=c;}
        check(gate && function,"both buttons exist for each paired stage column");if(!gate || !function)continue;
        float gl=gate->box.pos.x,fl=function->box.pos.x,gw=gate->box.size.x,fw=function->box.size.x;
        check(gl==fl && gw==fw,"gate/function button left edge, centre and right edge match exactly");
        coordinates<<s+1<<','<<gl<<','<<fl<<','<<gl+gw/2<<','<<fl+fw/2<<','<<gl+gw<<','<<fl+fw<<'\n';
    }
    chooseStyle(0);check(m->panelStyle.load()==M::CLASSIC_METAL,"Classic menu item changes style");
    m->clearPitchPreview();
}
if(qaFrame==9){metroqa::screenshot(dir+"/classic.png");check(metroqa::region(this,Rect(Vec(264,GATE_TOP-28),Vec(235,371-(GATE_TOP-28))),3,dir+"/classic-stage-rows.png"),"exact Classic button rows rendered directly from Rack widgets");check(metroqa::region(this,Rect(Vec(0,0),Vec(510,380)),2,dir+"/classic-whole-panel.png"),"whole Classic panel with narrowed right bank rendered directly");}
if(qaFrame==10){chooseStyle(1);check(m->panelStyle.load()==M::ATRIUM,"Atrium menu item changes style");}
if(qaFrame==14){metroqa::screenshot(dir+"/atrium.png");check(metroqa::region(this,Rect(Vec(264,GATE_TOP-28),Vec(235,371-(GATE_TOP-28))),3,dir+"/atrium-stage-rows.png"),"exact Atrium button rows rendered directly from Rack widgets");check(metroqa::region(this,Rect(Vec(0,0),Vec(510,380)),2,dir+"/atrium-whole-panel.png"),"whole Atrium panel with narrowed right bank rendered directly");}
if(qaFrame==15){click(find("SETUP"));}
if(qaFrame==19)metroqa::screenshot(dir+"/settings.png");
if(qaFrame==20){click(find("Stage 6 / PROB"));}
if(qaFrame==24)metroqa::screenshot(dir+"/stage-graph.png");
if(qaFrame==25){m->uiPage.store(M::HOME);m->clearPitchPreview();m->uiHome.store(0);APP->scene->rackScroll->setZoom(1);}
if(qaFrame==29)metroqa::screenshot(dir+"/atrium-100.png");
if(qaFrame==30){chooseStyle(0);}
if(qaFrame==34)metroqa::screenshot(dir+"/classic-100.png");
if(qaFrame==35){chooseStyle(1);APP->patch->save(dir+"/panel-roundtrip.vcv");}
if(qaFrame==37){check(!qaFailed,"PANEL UI QA COMPLETE");APP->window->close();}
