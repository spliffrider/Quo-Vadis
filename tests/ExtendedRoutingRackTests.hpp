void clockDivisionModulationRackTests(){
    using M=Metropolix;using namespace metro;
    for(bool external:{false,true})for(int assignment=0;assignment<3;++assignment){
        auto m=std::unique_ptr<M>(new M);
        Module::ProcessArgs args;args.sampleRate=48000;args.sampleTime=1.f/48000;
        m->params[M::RUN].setValue(1);m->params[M::TEMPO].setValue(120);
        m->params[M::CLOCK_SOURCE].setValue(external?2:1);
        m->params[M::EXT_CLOCK_DIV].setValue(1);m->params[M::CV_OUT_DELAY].setValue(0);
        for(int k=0;k<2;++k)m->params[M::ctrlParam(k,M::CTRL_DESTINATION)].setValue(CtrlNone);
        for(int t=0;t<2;++t)m->params[M::trackParam(t,M::DIVISION)].setValue(1);
        m->params[M::modParam(7,M::MOD_DESTINATION)].setValue(ModClockDiv);
        m->params[M::modParam(7,M::MOD_ASSIGNMENT)].setValue(assignment);
        for(int stage=0;stage<8;++stage){
            m->params[M::PITCH+stage].setValue(stage/8.f);
            m->params[M::modParam(7,M::MOD_VALUE+stage)].setValue(stage%2?3.f/12.6f:0);
        }
        if(external){
            // Rack's Schmitt trigger must observe low before its first rise.
            m->inputs[M::CLOCK_IN].channels=1;
            m->inputs[M::CLOCK_IN].setVoltage(0);m->process(args);
        }
        uint64_t expected[2]={0,0};float precedingPitch[2]={0,0};int master=0;
        for(int frame=0;frame<96000;++frame){
            if(external)m->inputs[M::CLOCK_IN].setVoltage(frame%6000==0?5:0);
            m->process(args);
            if(!m->sequencer.masterTick)continue;
            for(int t=0;t<2;++t){
                bool targeted=assignment==2 || assignment==t;
                bool plays=!targeted || master%2==0;
                if(plays)++expected[t];
                require(m->sequencer.tracks[t].stageSerial==expected[t],"native MOD /1-/4 alternation advances assigned tracks on alternate internal/external master pulses");
                require(m->sequencer.tracks[t].stage==int((expected[t]-1)%8),"native MOD division follows the expected stage schedule");
                float pitch=m->outputs[M::PITCH_1+2*t].getVoltage();
                if(master>0)require((pitch!=precedingPitch[t])==plays,"physical pitch outputs change only at the expected divided track steps");
                if(plays)require(m->outputs[M::GATE_1+2*t].getVoltage()>1,"divided track notes reach the physical gate jack");
                precedingPitch[t]=pitch;
            }
            require(m->outputs[M::CLOCK_OUT].getVoltage()>1,"physical global CLK remains present on every master pulse");
            ++master;
        }
        if(master!=16)std::cerr<<"MOD divider clock count: external="<<external<<" assignment="<<assignment<<" pulses="<<master<<" bpm="<<m->sequencer.effectiveTempo()<<" run="<<m->integer(M::RUN)<<" source="<<m->integer(M::CLOCK_SOURCE)<<'\n';
        require(master==16,"native MOD division preserves the master clock rate");
        for(int t=0;t<2;++t){
            require(expected[t]==uint64_t(assignment==2 || assignment==t?8:16),"native track assignment isolates the divided pitch/gate signals");
            require(m->integer(M::trackParam(t,M::DIVISION))==1,"native MOD division preserves programmed track DIV");
        }
    }
    std::cout<<"PASS: live MOD clock division reaches physical pitch/gate outputs with internal/external clocks and all track assignments\n";
}
void extendedRoutingRackTests(){
    clockDivisionModulationRackTests();
    using M=Metropolix;using namespace metro;auto m=std::unique_ptr<M>(new M);Module::ProcessArgs args;args.sampleRate=48000;args.sampleTime=1.f/48000;
    require(M::modDestinations().size()==39 && m->getParamQuantity(M::auxParam(0,M::AUX_DESTINATION))->maxValue==38,"native metadata covers all38 AUX/MOD destinations");
    for(int k=0;k<2;++k)m->params[M::ctrlParam(k,M::CTRL_DESTINATION)].setValue(0);
    m->params[M::RUN].setValue(1);m->params[M::SCALE].setValue(0);m->params[M::TEMPO].setValue(120);m->params[M::auxParam(0,M::AUX_AMOUNT)].setValue(1);m->params[M::auxParam(0,M::AUX_DESTINATION)].setValue(ModTempo);m->params[M::auxParam(0,M::AUX_ASSIGNMENT)].setValue(2);m->inputs[M::AUX_X_IN].channels=1;m->inputs[M::AUX_X_IN].setVoltage(1.5f);
    int clocks=0;bool last=false;for(int f=0;f<96000;++f){m->process(args);bool high=m->outputs[M::CLOCK_OUT].getVoltage()>0;if(high && !last)++clocks;last=high;}
    require(clocks==32 && m->params[M::TEMPO].getValue()==120 && m->displayBpm.load()==240,"native tempo reaches physical CLK and live meter without rewriting BPM");
    auto* assignment=m->getParamQuantity(M::auxParam(0,M::AUX_ASSIGNMENT));require(assignment->getDisplayValueString()=="Global","native global assignment is named Global");assignment->setValue(0);require(m->integer(M::auxParam(0,M::AUX_ASSIGNMENT))==2,"global route retains inactive track assignment");
    m->params[M::auxParam(0,M::AUX_DESTINATION)].setValue(ModLoopy);m->inputs[M::AUX_X_IN].setVoltage(5);m->params[M::LOOPY_TARGET].setValue(1);m->params[M::LOOPY_PULSES].setValue(2);m->inputs[M::CLOCK_IN].channels=1;
    auto tick=[&]{m->inputs[M::CLOCK_IN].setVoltage(0);for(int f=0;f<6000;++f)m->process(args);m->inputs[M::CLOCK_IN].setVoltage(5);m->process(args);};
    for(int p=0;p<5;++p){tick();require(m->sequencer.loopPlaying[0] && m->sequencer.loopPlaying[1] && m->sequencer.sounding(0).stage==p%2,"native Loopy gate follows configured both-track target and two-pulse cycle");}
    m->params[M::auxParam(0,M::AUX_DESTINATION)].setValue(ModSliderRange);m->inputs[M::AUX_X_IN].setVoltage(5);assignment->setValue(0);m->params[M::outputTrackParam(0,M::SLIDER_OCTAVES)].setValue(1);m->params[M::PITCH].setValue(1);m->params[M::trackParam(0,M::LENGTH)].setValue(1);tick();require(m->outputs[M::PITCH_1].getVoltage()==4 && m->integer(M::outputTrackParam(0,M::SLIDER_OCTAVES))==1,"native range CV reaches four octaves without rewriting base");
    m->params[M::auxParam(0,M::AUX_DESTINATION)].setValue(ModSliderInvert);tick();require(m->outputs[M::PITCH_1].getVoltage()==0,"native slider inversion reaches physical pitch jack");
    m->params[M::auxParam(0,M::AUX_DESTINATION)].setValue(ModReset);tick();require(m->sequencer.tracks[0].stage==0,"native reset reaches target transport");
    for(int d=ModTempo;d<ModDestinationCount;++d){int lane=(d-ModTempo)%8;m->params[M::modParam(lane,M::MOD_DESTINATION)].setValue(d);m->readSettings();require(m->sequencer.settings.modLanes[lane].destination==d,"every appended destination reaches native engine adapter");}
    m->params[M::modParam(0,M::MOD_DESTINATION)].setValue(ModSlide);auto* value=m->getParamQuantity(M::modParam(0,M::MOD_VALUE));value->setDisplayValueString("On");require(value->getDisplayValueString()=="On" && value->getUnit().empty() && m->params[M::modParam(0,M::MOD_VALUE)].getValue()==5,"native gate numeric entry/readout uses On without voltage unit");value->setDisplayValueString("Off");require(value->getValue()==0,"native gate Off entry clears gate");
    m->putPreset(0,m->capturePreset());json_t* params=m->paramsToJson(),*data=m->dataToJson();auto copy=std::unique_ptr<M>(new M);copy->paramsFromJson(params);copy->dataFromJson(data);json_decref(params);json_decref(data);for(int id=0;id<M::NUM_PARAMS;++id)require(copy->params[id].getValue()==m->params[id].getValue(),"complete routing table survives native persistence");require(copy->getPreset(0).occupied,"complete routes persist in unchanged format7 bank");
}
