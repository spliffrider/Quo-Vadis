void panelStyleRackTests() {
    auto source=std::unique_ptr<Metropolix>(new Metropolix);
    auto restored=std::unique_ptr<Metropolix>(new Metropolix);
    require(source->panelStyle.load()==Metropolix::CLASSIC_METAL,"new modules use Classic metal");
    for(int style:{Metropolix::CLASSIC_METAL,Metropolix::ATRIUM}) {
        source->panelStyle.store(style);auto* data=source->dataToJson();
        restored->panelStyle.store(1-style);restored->dataFromJson(data);json_decref(data);
        require(restored->panelStyle.load()==style,"both panel styles survive patch restoration");
    }
    auto* data=source->dataToJson();json_object_del(data,"panelStyle");
    restored->panelStyle.store(Metropolix::ATRIUM);restored->dataFromJson(data);
    require(restored->panelStyle.load()==Metropolix::CLASSIC_METAL,"legacy patches safely choose Classic metal");
    for(auto* invalid:{json_integer(-1),json_integer(2),json_integer(4294967297LL),json_string("Atrium"),json_real(1),json_true()}) {
        json_object_set_new(data,"panelStyle",invalid);restored->panelStyle.store(Metropolix::ATRIUM);restored->dataFromJson(data);
        require(restored->panelStyle.load()==Metropolix::CLASSIC_METAL,"invalid style values safely choose Classic metal");
    }
    json_decref(data);
    source->panelStyle.store(Metropolix::ATRIUM);source->putPreset(0,source->capturePreset());
    source->requestPreset(0);source->sendPresetCommand();
    rack::engine::Module::ProcessArgs args;args.sampleRate=48000;args.sampleTime=1.f/48000;source->process(args);
    require(source->panelStyle.load()==Metropolix::ATRIUM,"musical preset recall preserves panel style");
}
