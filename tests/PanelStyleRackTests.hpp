void panelStyleRackTests() {
    auto source=std::unique_ptr<Metropolix>(new Metropolix);
    auto restored=std::unique_ptr<Metropolix>(new Metropolix);
    require(source->panelStyle.load()==Metropolix::BLACK_SILVER,"new modules use Black / silver");
    for(int style:{Metropolix::BLACK_SILVER,Metropolix::BRUSHED_SILVER}) {
        source->panelStyle.store(style);auto* data=source->dataToJson();
        restored->panelStyle.store(1-style);restored->dataFromJson(data);json_decref(data);
        require(restored->panelStyle.load()==style,"both panel styles survive patch restoration");
    }
    auto* data=source->dataToJson();json_object_del(data,"panelStyle");
    restored->panelStyle.store(Metropolix::BRUSHED_SILVER);restored->dataFromJson(data);
    require(restored->panelStyle.load()==Metropolix::BLACK_SILVER,"legacy patches safely choose Black / silver");
    for(auto* invalid:{json_integer(-1),json_integer(2),json_integer(4294967297LL),json_string("Brushed silver"),json_real(1),json_true()}) {
        json_object_set_new(data,"panelStyle",invalid);restored->panelStyle.store(Metropolix::BRUSHED_SILVER);restored->dataFromJson(data);
        require(restored->panelStyle.load()==Metropolix::BLACK_SILVER,"invalid style values safely choose Black / silver");
    }
    // Saved Atrium patches used numeric style ID 1; they now select silver.
    json_object_set_new(data,"panelStyle",json_integer(1));restored->dataFromJson(data);
    require(restored->panelStyle.load()==Metropolix::BRUSHED_SILVER,"former style ID 1 migrates to brushed silver");
    json_decref(data);
    source->panelStyle.store(Metropolix::BRUSHED_SILVER);source->putPreset(0,source->capturePreset());
    source->requestPreset(0);source->sendPresetCommand();
    rack::engine::Module::ProcessArgs args;args.sampleRate=48000;args.sampleTime=1.f/48000;source->process(args);
    require(source->panelStyle.load()==Metropolix::BRUSHED_SILVER,"musical preset recall preserves panel style");
}
