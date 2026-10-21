// Small native-renderer harness; avoids compiling every unrelated UI scenario.
void MetropolixWidget::qaStep() {
    const char* directory=std::getenv("METROPOLIX_QA_DIR");auto* m=getModule<Metropolix>();if(!directory || !m)return;
    ++qaFrame;const std::string dir(directory);
    auto check=[&](bool ok,const std::string& s){std::ofstream log(dir+"/report.txt",std::ios::app);log<<(ok?"PASS ":"FAIL ")<<s<<'\n';qaFailed|=!ok;};
    auto find=[&](const std::string& name)->HardwareControl*{for(auto* w:children)if(auto* c=dynamic_cast<HardwareControl*>(w))if(c->name==name)return c;return nullptr;};
    auto drag=[&](widget::Widget* w,float dy){
        if(!w){check(false,"requested control exists");return;}
        widget::EventContext ctx;event::Button press;press.context=&ctx;press.pos=w->box.size.div(2);press.button=GLFW_MOUSE_BUTTON_LEFT;press.action=GLFW_PRESS;press.mods=0;w->onButton(press);
        event::DragStart start;start.context=&ctx;start.button=GLFW_MOUSE_BUTTON_LEFT;w->onDragStart(start);
        if(dy){event::DragMove move;move.context=&ctx;move.button=GLFW_MOUSE_BUTTON_LEFT;move.mouseDelta=Vec(0,dy);w->onDragMove(move);}
        event::DragEnd end;end.context=&ctx;end.button=GLFW_MOUSE_BUTTON_LEFT;w->onDragEnd(end);
    };
    auto click=[&](widget::Widget* w){drag(w,0);};
#include "PanelStyleWidgetQa.hpp"
}
