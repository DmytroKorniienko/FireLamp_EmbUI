/*
Copyright © 2020 Dmytro Korniienko (kDn)
JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov

    This file is part of FireLamp_JeeUI.

    FireLamp_JeeUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FireLamp_JeeUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FireLamp_JeeUI.  If not, see <https://www.gnu.org/licenses/>.

  (Этот файл — часть FireLamp_JeeUI.

   FireLamp_JeeUI - свободная программа: вы можете перераспространять ее и/или
   изменять ее на условиях Стандартной общественной лицензии GNU в том виде,
   в каком она была опубликована Фондом свободного программного обеспечения;
   либо версии 3 лицензии, либо (по вашему выбору) любой более поздней
   версии.

   FireLamp_JeeUI распространяется в надежде, что она будет полезной,
   но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА
   или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной
   общественной лицензии GNU.

   Вы должны были получить копию Стандартной общественной лицензии GNU
   вместе с этой программой. Если это не так, см.
   <https://www.gnu.org/licenses/>.)
*/

#include "main.h"
#include "interface.h"
#include "effects.h"
#include "ui.h"
#include "text_res.h"

Ticker optionsTicker;          // планировщик заполнения списка
Ticker sysTicker;              // системный планировщик

void resetAutoTimers() // сброс таймера демо и настройка автосохранений
{
    embui.autoSaveReset(); // автосохранение конфига будет отсчитываться от этого момента
    myLamp.demoTimer(T_RESET);
    myLamp.DelayedAutoEffectConfigSave(CFG_AUTOSAVE_TIMEOUT); // настройка отложенной записи
}

#ifdef AUX_PIN
void AUX_toggle(bool key)
{
    if (key)
    {
        digitalWrite(AUX_PIN, AUX_LEVEL);
        embui.var(FPSTR(TCONST_000E), (FPSTR(TCONST_FFFF)));
    }
    else
    {
        digitalWrite(AUX_PIN, !AUX_LEVEL);
        embui.var(FPSTR(TCONST_000E), (FPSTR(TCONST_FFFE)));
    }
}
#endif

// Вывод значка микрофона в списке эффектов
#ifdef MIC_EFFECTS
    #define MIC_SYMBOL (micSymb ? (pgm_read_byte(T_EFFVER + (uint8_t)eff->eff_nb) % 2 == 0 ? " \U0001F399" : "") : "")
    #define MIC_SYMB bool micSymb = myLamp.getLampSettings().effHasMic
#else
    #define MIC_SYMBOL ""
    #define MIC_SYMB
#endif

// Вывод номеров эффектов в списке, в WebUI
#define EFF_NUMBER (numList ? (String(eff->eff_nb) + ". ") : "")

void pubCallback(Interface *interf){
    if (!interf) return;
    //return; // Временно для увеличения стабильности. Пока разбираюсь с падениями.
    interf->json_frame_value();
    interf->value(FPSTR(TCONST_0001), myLamp.timeProcessor.getFormattedShortTime(), true);
    interf->value(FPSTR(TCONST_0002), String(ESP.getFreeHeap()), true);
    interf->value(FPSTR(TCONST_008F), String(millis()/1000), true);
    interf->json_frame_flush();
}

void block_menu(Interface *interf, JsonObject *data){
    if (!interf) return;
    // создаем меню
    embui.autoSaveReset(); // автосохранение конфига будет отсчитываться от этого момента
    interf->json_section_menu();

    interf->option(FPSTR(TCONST_0000), FPSTR(TINTF_000));
    interf->option(FPSTR(TCONST_0003), FPSTR(TINTF_001));
    interf->option(FPSTR(TCONST_0004), FPSTR(TINTF_002));
#ifdef SHOWSYSCONFIG
    if(myLamp.isShowSysMenu())
        interf->option(FPSTR(TCONST_009A), FPSTR(TINTF_08F));
#endif
    interf->json_section_end();
}

static EffectListElem *confEff = nullptr;
void block_effects_config_param(Interface *interf, JsonObject *data){
    if (!interf || !confEff) return;

    String tmpName;
    myLamp.effects.loadeffname(tmpName,confEff->eff_nb);
    interf->json_section_begin(FPSTR(TCONST_0005));
    interf->text(FPSTR(TCONST_0092), tmpName, FPSTR(TINTF_089), false);
    interf->checkbox(FPSTR(TCONST_0006), confEff->canBeSelected()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_003), false);
    interf->checkbox(FPSTR(TCONST_0007), confEff->isFavorite()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_004), false);

    interf->button_submit(FPSTR(TCONST_0005), FPSTR(TINTF_008), FPSTR(TCONST_0008));
    interf->button_submit_value(FPSTR(TCONST_0005), FPSTR(TCONST_0009), FPSTR(TINTF_005));
    if (confEff->eff_nb&0xFF00) { // пока удаление только для копий, но в теории можно удалять что угодно
        interf->button_submit_value(FPSTR(TCONST_0005), FPSTR(TCONST_000A), FPSTR(TINTF_006), FPSTR(TCONST_000C));
    }

    interf->button_submit_value(FPSTR(TCONST_0005), FPSTR(TCONST_000B), FPSTR(TINTF_007), FPSTR(TCONST_000D));
    interf->button_submit_value(FPSTR(TCONST_0005), FPSTR(TCONST_0093), FPSTR(TINTF_08B), FPSTR(TCONST_000D));

    interf->json_section_end();
}

void show_effects_config_param(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_effects_config_param(interf, data);
    interf->json_frame_flush();
}
void delayedcall_effects_main();
void set_effects_config_param(Interface *interf, JsonObject *data){
    if (!confEff || !data) return;
    if(optionsTicker.active())
        optionsTicker.detach();
    
    String act = (*data)[FPSTR(TCONST_0005)];
    if (act == FPSTR(TCONST_0009)) {
        myLamp.effects.copyEffect(confEff); // копируем текущий
    } else if (act == FPSTR(TCONST_000A)) {
        myLamp.effects.deleteEffect(confEff); // удаляем текущий
    } else if (act == FPSTR(TCONST_000B)) {
#ifndef DELAYED_EFFECTS
        sysTicker.once(5,std::bind([]{
#else
        sysTicker.once(1,std::bind([]{
#endif
            myLamp.effects.makeIndexFileFromFS(); // создаем индекс по файлам ФС и на выход
            delayedcall_effects_main();
        }));
        section_main_frame(interf, data);
        return;
    } else if (act == FPSTR(TCONST_0093)) {
        LOG(printf_P,PSTR("confEff->eff_nb=%d\n"), confEff->eff_nb);
        if(confEff->eff_nb==myLamp.effects.getCurrent()){
            myLamp.effects.directMoveBy(EFF_ENUM::EFF_NONE);
            myLamp.effects.removeConfig(confEff->eff_nb);
            remote_action(RA_EFF_NEXT, NULL);
        } else {
            myLamp.effects.removeConfig(confEff->eff_nb);
        }
        String tmpStr=F("- ");
        tmpStr+=confEff->eff_nb;
        myLamp.sendString(tmpStr.c_str(), CRGB::Red);
        //confEff = myLamp.effects.getEffect(EFF_ENUM::EFF_NONE);
        section_main_frame(interf, data);
        return;
    }
    else {
        confEff->canBeSelected((*data)[FPSTR(TCONST_0006)] == FPSTR(TCONST_FFFF));
        confEff->isFavorite((*data)[FPSTR(TCONST_0007)] == FPSTR(TCONST_FFFF));
        myLamp.effects.setEffectName((*data)[FPSTR(TCONST_0092)], confEff);
    }

    resetAutoTimers();
    myLamp.effects.makeIndexFileFromList(); // обновить индексный файл после возможных изменений
    section_main_frame(interf, data);
}

void block_effects_config(Interface *interf, JsonObject *data, bool fast=true){
    if (!interf) return;

    interf->json_section_main(FPSTR(TCONST_000F), FPSTR(TINTF_009));
    confEff = myLamp.effects.getSelectedListElement();
    interf->select(FPSTR(TCONST_0010), String((int)confEff->eff_nb), String(FPSTR(TINTF_00A)), true);

    uint32_t timest = millis();
    if(fast){
        // Сначала подгрузим дефолтный список, а затем спустя время - подтянем имена из конфига

        //interf->option(String(myLamp.effects.getSelected()), myLamp.effects.getEffectName());
        String effname((char *)0);
        EffectListElem *eff = nullptr;
        MIC_SYMB;
        bool numList = myLamp.getLampSettings().numInList;
        while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
            effname = FPSTR(T_EFFNAMEID[(uint8_t)eff->eff_nb]);
            interf->option(String(eff->eff_nb),
                EFF_NUMBER + 
                String(effname) + 
                MIC_SYMBOL
            );
            ESP.wdtFeed();
        }
    } else {
        EffectListElem *eff = nullptr;
        LOG(println,F("DBG1: using slow Names generation"));
        String effname((char *)0);
        MIC_SYMB;
        bool numList = myLamp.getLampSettings().numInList;
        while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
            myLamp.effects.loadeffname(effname, eff->eff_nb);
            interf->option(String(eff->eff_nb),
                EFF_NUMBER + 
                String(effname) + 
                MIC_SYMBOL
            );
            ESP.wdtFeed();
        }
    }
    interf->json_section_end();
    LOG(printf_P,PSTR("DBG1: generating Names list took %d ms\n"), millis() - timest);

    block_effects_config_param(interf, nullptr);

    interf->spacer();
    interf->button(FPSTR(TCONST_0000), FPSTR(TINTF_00B));
    interf->json_section_end();
}

void delayedcall_show_effects_config(){
    Interface *interf = embui.ws.count()? new Interface(&embui, &embui.ws, 3000) : nullptr;
    if (!interf) return;
    interf->json_frame_interface();
    interf->json_section_content();
    interf->select(FPSTR(TCONST_0010), String((int)confEff->eff_nb), String(FPSTR(TINTF_00A)), true, true); // не выводить метку
    EffectListElem *eff = nullptr;
    String effname((char *)0);
    MIC_SYMB;
    bool numList = myLamp.getLampSettings().numInList;
    while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
        myLamp.effects.loadeffname(effname, eff->eff_nb);
        interf->option(String(eff->eff_nb),
            EFF_NUMBER + 
            String(effname) + 
            MIC_SYMBOL                
        );
        ESP.wdtFeed();
    }
    interf->json_section_end();
    interf->json_section_end();
    interf->json_frame_flush();
    delete interf;
    if(optionsTicker.active())
        optionsTicker.detach();
}

void show_effects_config(Interface *interf, JsonObject *data){
#ifdef DELAYED_EFFECTS
    if (!interf) return;
    interf->json_frame_interface();
    block_effects_config(interf, data);
    interf->json_frame_flush();
    if(!optionsTicker.active())
        optionsTicker.once(10,std::bind(delayedcall_show_effects_config));
#else
    if (!interf) return;
    interf->json_frame_interface();
    block_effects_config(interf, data, false);
    interf->json_frame_flush();
#endif
}

void set_effects_config_list(Interface *interf, JsonObject *data){
    if (!data) return;
    uint16_t num = (*data)[FPSTR(TCONST_0010)].as<uint16_t>();

    // Так  нельзя :(, поскольку интерфейс отдаст только effListConf, а не весь блок...
    // А мне хотелось бы попереключать список, сделав несколько изменений в флагах, не нажимая для каждого раза "сохранить"
    // Есть красивый способ это сделать по переключению списка?
    if(confEff){ // если переключаемся, то сохраняем предыдущие признаки в эффект до переключения
        LOG(printf_P, PSTR("eff_sel: %d eff_fav : %d\n"), (*data)[FPSTR(TCONST_0006)].as<bool>(),(*data)[FPSTR(TCONST_0007)].as<bool>());
        // confEff->canBeSelected((*data)[FPSTR(TCONST_0006)] == FPSTR(TCONST_FFFF));
        // confEff->isFavorite((*data)[FPSTR(TCONST_0007)] == FPSTR(TCONST_FFFF));
    }

    confEff = myLamp.effects.getEffect(num);
    show_effects_config_param(interf, data);
    myLamp.demoTimer(T_RESET);
}

void block_effects_param(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_begin(FPSTR(TCONST_0011));

    LList<UIControl*>&controls = myLamp.effects.getControls();
    uint8_t ctrlCaseType; // тип контрола, старшие 4 бита соответствуют CONTROL_CASE, младшие 4 - CONTROL_TYPE
    for(int i=0; i<controls.size();i++){
        ctrlCaseType = controls[i]->getType();
        switch(ctrlCaseType>>4){
            case CONTROL_CASE::HIDE :
                continue;
                break;
            case CONTROL_CASE::ISMICON :
#ifdef MIC_EFFECTS
                if(!myLamp.isMicOnOff()) continue;
#else
                continue;
#endif          
                break;
            case CONTROL_CASE::ISMICOFF :
#ifdef MIC_EFFECTS
                if(myLamp.isMicOnOff()) continue;
#else
                continue;
#endif   
                break;
            default: break;
        }
        switch(ctrlCaseType&0x0F){
            case CONTROL_TYPE::RANGE :
                interf->range(
                controls[i]->getId()==0 ? String(FPSTR(TCONST_0012)) : controls[i]->getId()==1 ? String(FPSTR(TCONST_0013)) : controls[i]->getId()==2 ? String(FPSTR(TCONST_0014))
                    : String(FPSTR(TCONST_0015)) + String(controls[i]->getId())
                ,i ? controls[i]->getVal().toInt() : myLamp.getNormalizedLampBrightness()
                ,controls[i]->getMin().toInt()
                ,controls[i]->getMax().toInt()
                ,controls[i]->getStep().toInt()
                ,i ? controls[i]->getName() : myLamp.IsGlobalBrightness() ? FPSTR(TINTF_00C) : FPSTR(TINTF_00D)
                , true);
                break;
            case CONTROL_TYPE::EDIT :
                interf->text(String(FPSTR(TCONST_0015)) + String(controls[i]->getId())
                , controls[i]->getVal()
                , controls[i]->getName()
                , true
                );
                break;
            case CONTROL_TYPE::CHECKBOX :
                interf->checkbox(String(FPSTR(TCONST_0015)) + String(controls[i]->getId())
                , controls[i]->getVal()
                , controls[i]->getName()
                , true
                );
                break;
            default:
                break;
        }
    }
    interf->json_section_end();
}

void show_effects_param(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_effects_param(interf, data);
    interf->json_frame_flush();
}

void set_effects_list(Interface *interf, JsonObject *data){
    if (!data) return;
    uint16_t num = (*data)[FPSTR(TCONST_0016)].as<uint16_t>();
    uint16_t curr = myLamp.effects.getSelected();
    EffectListElem *eff = myLamp.effects.getEffect(num);
    if (!eff) return;

    LOG(printf_P, PSTR("EFF LIST n:%d, o:%d, on:%d, md:%d\n"), eff->eff_nb, curr, myLamp.isLampOn(), myLamp.getMode());
    if (eff->eff_nb != curr) {
        if (!myLamp.isLampOn()) {
            myLamp.effects.directMoveBy(eff->eff_nb); // переходим на выбранный эффект для начальной инициализации
        } else {
            myLamp.switcheffect(SW_SPECIFIC, myLamp.getFaderFlag(), eff->eff_nb);
        }

        embui.var(FPSTR(TCONST_0016), (*data)[FPSTR(TCONST_0016)]);
    }

    show_effects_param(interf, data);
    myLamp.demoTimer(T_RESET);
}

void set_effects_bright(Interface *interf, JsonObject *data){
    if (!data) return;

    byte bright = (*data)[FPSTR(TCONST_0012)];
    if (myLamp.getNormalizedLampBrightness() != bright) {
        myLamp.setLampBrightness(bright);
        if(myLamp.isLampOn())
            myLamp.setBrightness(myLamp.getNormalizedLampBrightness(), !((*data)[FPSTR(TCONST_0017)]));
        if (myLamp.IsGlobalBrightness()) {
            embui.var(FPSTR(TCONST_0018), (*data)[FPSTR(TCONST_0012)]);
        }
        if(myLamp.effects.worker)
            myLamp.effects.worker->setbrt((*data)[FPSTR(TCONST_0012)].as<byte>()); // передача значения в эффект
        LOG(printf_P, PSTR("Новое значение яркости: %d\n"), myLamp.getNormalizedLampBrightness());
    }

    resetAutoTimers();
}

void set_effects_speed(Interface *interf, JsonObject *data){
    if (!data) return;

    myLamp.effects.getControls()[1]->setVal((*data)[FPSTR(TCONST_0013)]);
    myLamp.effects.worker->setspd((*data)[FPSTR(TCONST_0013)].as<byte>()); // передача значения в эффект
    LOG(printf_P, PSTR("Новое значение скорости: %d\n"), (*data)[FPSTR(TCONST_0013)].as<byte>());

    resetAutoTimers();
}

void set_effects_scale(Interface *interf, JsonObject *data){
    if (!data) return;

    myLamp.effects.getControls()[2]->setVal((*data)[FPSTR(TCONST_0014)]);
    myLamp.effects.worker->setscl((*data)[FPSTR(TCONST_0014)].as<byte>()); // передача значения в эффект
    LOG(printf_P, PSTR("Новое значение масштаба: %d\n"), (*data)[FPSTR(TCONST_0014)].as<byte>());

    resetAutoTimers();
}

void set_effects_dynCtrl(Interface *interf, JsonObject *data){
    if (!data) return;

    LList<UIControl*>&controls = myLamp.effects.getControls();
    for(int i=3; i<controls.size();i++){
        if((*data).containsKey(String(FPSTR(TCONST_0015))+String(controls[i]->getId()))){
            controls[i]->setVal((*data)[String(FPSTR(TCONST_0015))+String(controls[i]->getId())]);
            LOG(printf_P, PSTR("Новое значение дин. контрола %d: %s\n"), controls[i]->getId(), (*data)[String(FPSTR(TCONST_0015))+String(controls[i]->getId())].as<String>().c_str());
            myLamp.effects.worker->setDynCtrl(controls[i]);
        }
    }
    resetAutoTimers();
}

void block_main_flags(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_line(FPSTR(TCONST_0019));
    interf->checkbox(FPSTR(TCONST_001A), myLamp.isLampOn()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_00E), true);
    interf->checkbox(FPSTR(TCONST_001B), (myLamp.getMode() == MODE_DEMO)? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_00F), true);
    interf->checkbox(FPSTR(TCONST_001C), myLamp.IsGlobalBrightness()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_010), true);
    interf->checkbox(FPSTR(TCONST_001D), myLamp.IsEventsHandled()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_011), true);
#ifdef MIC_EFFECTS
    interf->checkbox(FPSTR(TCONST_001E), myLamp.isMicOnOff()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_012), true);
#endif
#ifdef AUX_PIN
    interf->checkbox(FPSTR(TCONST_000E), FPSTR(TCONST_000E), true);
#endif
#ifdef ESP_USE_BUTTON
    interf->checkbox(FPSTR(TCONST_001F), myButtons->isButtonOn()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_013), true);
#endif

#ifdef MP3PLAYER
    interf->checkbox(FPSTR(TCONST_009D), myLamp.isONMP3()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_099), true);
#endif
#ifdef LAMP_DEBUG
    interf->checkbox(FPSTR(TCONST_0095), myLamp.isDebugOn()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_08E), true);
#endif
    interf->json_section_end();
}

void show_main_flags(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_main_flags(interf, data);
    interf->spacer();
    interf->button(FPSTR(TCONST_0000), FPSTR(TINTF_00B));
    interf->json_frame_flush();
}

void delayedcall_effects_main(){
    Interface *interf = embui.ws.count()? new Interface(&embui, &embui.ws, 3000) : nullptr;
    if (!interf) return;
    interf->json_frame_interface();
    interf->json_section_content();
    interf->select(FPSTR(TCONST_0016), String(myLamp.effects.getSelected()), String(FPSTR(TINTF_00A)), true, true); // не выводить метку
    EffectListElem *eff = nullptr;
    String effname((char *)0);
    MIC_SYMB;
    bool numList = myLamp.getLampSettings().numInList;
    while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
        if (eff->canBeSelected()) {
            myLamp.effects.loadeffname(effname, eff->eff_nb);
            interf->option(String(eff->eff_nb),
                EFF_NUMBER + 
                String(effname) + 
                MIC_SYMBOL
            );
            ESP.wdtFeed();
        }
    }
    interf->json_section_end();
    interf->json_section_end();
    interf->json_frame_flush();
    delete interf;
    if(optionsTicker.active())
        optionsTicker.detach();
}

void block_effects_main(Interface *interf, JsonObject *data, bool fast=true){
#ifndef DELAYED_EFFECTS
    fast=false;
#endif

    // Страница "Управление эффектами"
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_0000), FPSTR(TINTF_000));

    interf->json_section_line(FPSTR(TCONST_0019));
    interf->checkbox(FPSTR(TCONST_001A), myLamp.isLampOn()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_00E), true);
    interf->button(FPSTR(TCONST_0020), FPSTR(TINTF_014));
    interf->json_section_end();

    interf->json_section_line(FPSTR(TCONST_0021));
    interf->button(FPSTR(TCONST_0022), FPSTR(TINTF_015), FPSTR(TCONST_0024));
    interf->button(FPSTR(TCONST_0023), FPSTR(TINTF_016), FPSTR(TCONST_0025));
    interf->json_section_end();

    interf->select(FPSTR(TCONST_0016), String(myLamp.effects.getSelected()), String(FPSTR(TINTF_00A)), true);
    LOG(printf_P,PSTR("Создаю список эффектов (%d):\n"),myLamp.effects.getModeAmount());
    EffectListElem *eff = nullptr;

    uint32_t timest = millis();

    if(fast){
        // Сначала подгрузим дефолтный список, а затем спустя время - подтянем имена из конфига

        //interf->option(String(myLamp.effects.getSelected()), myLamp.effects.getEffectName());
        String effname((char *)0);
        MIC_SYMB;
        bool numList = myLamp.getLampSettings().numInList;
        while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
            if (eff->canBeSelected()) {
                effname = FPSTR(T_EFFNAMEID[(uint8_t)eff->eff_nb]);
                interf->option(String(eff->eff_nb), 
                    EFF_NUMBER + 
                    String(effname) + 
                    MIC_SYMBOL                    
                );
                ESP.wdtFeed();
            }
        }
    } else {
        LOG(println,F("DBG2: using slow Names generation"));
        String effname((char *)0);
        MIC_SYMB;
        bool numList = myLamp.getLampSettings().numInList;
        while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
            if (eff->canBeSelected()) {
                myLamp.effects.loadeffname(effname, eff->eff_nb);
                interf->option(String(eff->eff_nb), 
                    EFF_NUMBER + 
                    String(effname) + 
                    MIC_SYMBOL                    
                );
                ESP.wdtFeed();
            }
        }
    }
    interf->json_section_end();
    LOG(printf_P,PSTR("DBG2: generating Names list took %d ms\n"), millis() - timest);

    block_effects_param(interf, data);

    interf->button(FPSTR(TCONST_000F), FPSTR(TINTF_009));

    interf->json_section_end();
#ifdef DELAYED_EFFECTS
    if(!optionsTicker.active())
        optionsTicker.once(10,std::bind(delayedcall_effects_main));
#endif
}

void set_eff_prev(Interface *interf, JsonObject *data){
    remote_action(RA::RA_EFF_PREV, NULL);
}

void set_eff_next(Interface *interf, JsonObject *data){
    remote_action(RA::RA_EFF_NEXT, NULL);
}

void set_onflag(Interface *interf, JsonObject *data){
    if (!data) return;
    bool newpower = TOGLE_STATE((*data)[FPSTR(TCONST_001A)], myLamp.isLampOn());
    if (newpower != myLamp.isLampOn()) {
        if (newpower) {
            // включаем через switcheffect, т.к. простого isOn недостаточно чтобы запустить фейдер и поменять яркость (при необходимости)
            myLamp.switcheffect(SW_SPECIFIC, myLamp.getFaderFlag(), myLamp.effects.getEn());
            myLamp.changePower(newpower);
#ifndef ESP_USE_BUTTON
            if(millis()<10000)
                sysTicker.once(3,std::bind([]{
                    myLamp.sendString(WiFi.localIP().toString().c_str(), CRGB::White);
                }));
#endif
        } else {
            resetAutoTimers();; // автосохранение конфига будет отсчитываться от этого момента
            sysTicker.once(0.5,std::bind([]{ // при выключении бывает эксепшен, видимо это слишком длительная операция, разносим во времени и отдаем управление
                myLamp.changePower(false);
            }));
        }
    }
#ifdef RESTORE_STATE
    save_lamp_flags();
#endif
}

void set_demoflag(Interface *interf, JsonObject *data){
    if (!data) return;
    // Специально не сохраняем, считаю что демо при старте не должно запускаться
    bool newdemo = TOGLE_STATE((*data)[FPSTR(TCONST_001B)], (myLamp.getMode() == MODE_DEMO));
    switch (myLamp.getMode()) {
        case MODE_NORMAL:
            if (newdemo) myLamp.startDemoMode(embui.param(FPSTR(TCONST_0026)).toInt()); break;
        case MODE_DEMO:
            if (!newdemo) myLamp.startNormalMode(); break;
        default:;
    }
#ifdef RESTORE_STATE
    embui.var(FPSTR(TCONST_001B), (*data)[FPSTR(TCONST_001B)]);
#endif
}

#ifdef OTA
void set_otaflag(Interface *interf, JsonObject *data){
    //if (!data) return;
    myLamp.startOTAUpdate();

    interf->json_frame_interface();
    interf->json_section_content();
    interf->button(FPSTR(TCONST_0027), FPSTR(TINTF_017), FPSTR(TCONST_0008));
    interf->json_section_end();
    interf->json_frame_flush();
}
#endif

#ifdef AUX_PIN
void set_auxflag(Interface *interf, JsonObject *data){
    if (!data) return;
    if (((*data)[FPSTR(TCONST_000E)] == FPSTR(TCONST_FFFF)) != (digitalRead(AUX_PIN) == AUX_LEVEL ? true : false)) {
        AUX_toggle(!(digitalRead(AUX_PIN) == AUX_LEVEL ? true : false));
    }
}
#endif

void set_gbrflag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setIsGlobalBrightness((*data)[FPSTR(TCONST_001C)] == FPSTR(TCONST_FFFF));
    save_lamp_flags();
    if (myLamp.isLampOn()) {
        myLamp.setBrightness(myLamp.getNormalizedLampBrightness());
    }
    show_effects_param(interf, data);
}

void block_lamp_config(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_hidden(FPSTR(TCONST_0028), FPSTR(TINTF_018));

    interf->json_section_begin(FPSTR(TCONST_0029));
    String cfg(FPSTR(TINTF_018)); cfg+=" ("; cfg+=embui.param(FPSTR(TCONST_002A)); cfg+=")";

    interf->select(FPSTR(TCONST_002A), cfg);
    if(LittleFS.begin()){
#ifdef ESP32
        File root = LittleFS.open(FPSTR(TCONST_002B));
        File file = root.openNextFile();
#else
        Dir dir = LittleFS.openDir(FPSTR(TCONST_002B));
#endif
        String fn;
#ifdef ESP32
        while (file) {
            fn=file.name();
            if(!file.isDirectory()){
#else
        while (dir.next()) {
            fn=dir.fileName();
#endif

            fn.replace(FPSTR(TCONST_002C),F(""));
            //LOG(println, fn);
            interf->option(fn, fn);
#ifdef ESP32
            file = root.openNextFile();
        }
#endif
        }
    }
    interf->json_section_end();

    interf->json_section_line();
    interf->button_submit_value(FPSTR(TCONST_0029), FPSTR(TCONST_002D), FPSTR(TINTF_019), FPSTR(TCONST_002F));
    interf->button_submit_value(FPSTR(TCONST_0029), FPSTR(TCONST_002E), FPSTR(TINTF_008));
    interf->button_submit_value(FPSTR(TCONST_0029), FPSTR(TCONST_000A), FPSTR(TINTF_006), FPSTR(TCONST_000C));
    interf->json_section_end();

    interf->json_section_end();

    interf->spacer();
    interf->json_section_begin(FPSTR(TCONST_0030));
    interf->text(FPSTR(TCONST_002A), FPSTR(TINTF_01A));
    interf->button_submit(FPSTR(TCONST_0030), FPSTR(TINTF_01B));
    interf->json_section_end();

    interf->json_section_end();
}

void show_lamp_config(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_lamp_config(interf, data);
    interf->json_frame_flush();
}

void edit_lamp_config(Interface *interf, JsonObject *data){
    // Удаление конфигурации из ФС
    if (!data || !data->containsKey(FPSTR(TCONST_002A))) return;
    String name = (*data)[FPSTR(TCONST_002A)];
    String act = (*data)[FPSTR(TCONST_0029)];

    if (act == FPSTR(TCONST_000A)) {
        String filename = String(FPSTR(TCONST_0031)) + name;
        if (LittleFS.begin()) LittleFS.remove(filename);

        filename = String(FPSTR(TCONST_002C)) + name;
        if (LittleFS.begin()) LittleFS.remove(filename);

        filename = String(FPSTR(TCONST_0032)) + name;
        if (LittleFS.begin()) LittleFS.remove(filename);
#ifdef ESP_USE_BUTTON
        filename = String(FPSTR(TCONST_0033)) + name;
        if (LittleFS.begin()) LittleFS.remove(filename);
#endif
    } else
    if (act == FPSTR(TCONST_002D)) {
        myLamp.changePower(false);
        String filename = String(FPSTR(TCONST_0031)) + name;
        embui.load(filename.c_str());

        // filename = String(FPSTR(TCONST_002C)) + name;
        // myLamp.effects.loadConfig(filename.c_str());

        filename = String(FPSTR(TCONST_0032)) + name;
        myLamp.events.loadConfig(filename.c_str());
#ifdef ESP_USE_BUTTON
        filename = String(FPSTR(TCONST_0033)) + name;
        myButtons->clear();
        if (!myButtons->loadConfig()) {
            default_buttons();
        }
#endif
        embui.var(FPSTR(TCONST_002A), name);
        sync_parameters();
        myLamp.changePower(true);
    } else {
        String filename = String(FPSTR(TCONST_0031)) + name;
        embui.save(filename.c_str(), true);

        // filename = String(FPSTR(TCONST_002C)) + name;
        // myLamp.effects.saveConfig(filename.c_str());

        filename = String(FPSTR(TCONST_0032)) + name;
        myLamp.events.saveConfig(filename.c_str());
#ifdef ESP_USE_BUTTON
        filename = String(FPSTR(TCONST_0033)) + name;
        myButtons->saveConfig(filename.c_str());
#endif
    }

    show_lamp_config(interf, data);
}

void block_lamp_textsend(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_begin(FPSTR(TCONST_0034));

    interf->spacer(FPSTR(TINTF_01C));
    interf->text(FPSTR(TCONST_0035), FPSTR(TINTF_01D));
    interf->color(FPSTR(TCONST_0036), FPSTR(TINTF_01E));
    interf->button_submit(FPSTR(TCONST_0034), FPSTR(TINTF_01F), FPSTR(TCONST_0008));

    interf->json_section_end();
}

void set_lamp_textsend(Interface *interf, JsonObject *data){
    if (!data) return;
    String tmpStr = (*data)[FPSTR(TCONST_0036)];
    embui.var(FPSTR(TCONST_0036), tmpStr);
    embui.var(FPSTR(TCONST_0035), (*data)[FPSTR(TCONST_0035)]);

    tmpStr.replace(F("#"), F("0x"));
    myLamp.sendString((*data)[FPSTR(TCONST_0035)], (CRGB::HTMLColorCode)strtol(tmpStr.c_str(), NULL, 0));
}

void block_lamp(Interface *interf, JsonObject *data){
    //Страница "Управление лампой"
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_0003), FPSTR(TINTF_001));

    block_lamp_textsend(interf, data);

    interf->json_section_end();
}

#ifdef MP3PLAYER
void block_settings_mp3(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_00A1), FPSTR(TINTF_099));

    interf->checkbox(FPSTR(TCONST_009D), myLamp.isONMP3()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_099), true);

    interf->json_section_begin(FPSTR(TCONST_00A0));
    interf->range(FPSTR(TCONST_00A2), 1, 30, 1, FPSTR(TINTF_09B), false);
    interf->spacer(FPSTR(TINTF_0B1));
    interf->json_section_line(FPSTR(TINTF_0B1)); // расположить в одной линии
        interf->checkbox(FPSTR(TCONST_00A3), myLamp.getLampSettings().playTime ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_09C), false);
        interf->checkbox(FPSTR(TCONST_00A4), myLamp.getLampSettings().playName ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_09D), false);
    interf->json_section_end();
    interf->json_section_line(FPSTR(TINTF_0B1)); // расположить в одной линии
        interf->checkbox(FPSTR(TCONST_00A5), myLamp.getLampSettings().playEffect ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_09E), false);
        interf->checkbox(FPSTR(TCONST_00A8), myLamp.getLampSettings().playMP3 ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_0AF), false);
    interf->json_section_end();
    interf->number(FPSTR(TCONST_00A9), mp3->getMP3count(), FPSTR(TINTF_0B0), false);
    SETPARAM(FPSTR(TCONST_00A9), mp3->setMP3count((*data)[FPSTR(TCONST_00A9)].as<int>())); // кол-во файлов в папке мп3
    
    interf->select(FPSTR(TCONST_00A6), String(myLamp.getLampSettings().alarmSound), String(FPSTR(TINTF_0A3)), false);
    interf->option(String(ALARM_SOUND_TYPE::AT_NONE), FPSTR(TINTF_09F));
    interf->option(String(ALARM_SOUND_TYPE::AT_FIRST), FPSTR(TINTF_0A0));
    interf->option(String(ALARM_SOUND_TYPE::AT_SECOND), FPSTR(TINTF_0A4));
    interf->option(String(ALARM_SOUND_TYPE::AT_THIRD), FPSTR(TINTF_0A5));
    interf->option(String(ALARM_SOUND_TYPE::AT_FOURTH), FPSTR(TINTF_0A6));
    interf->option(String(ALARM_SOUND_TYPE::AT_FIFTH), FPSTR(TINTF_0A7));
    interf->option(String(ALARM_SOUND_TYPE::AT_RANDOM), FPSTR(TINTF_0A1));
    interf->option(String(ALARM_SOUND_TYPE::AT_RANDOMMP3), FPSTR(TINTF_0A2));
    interf->json_section_end();

    interf->select(FPSTR(TCONST_00A7), String(myLamp.getLampSettings().MP3eq), String(FPSTR(TINTF_0A8)), false);
    interf->option(String(DFPLAYER_EQ_NORMAL), FPSTR(TINTF_0A9));
    interf->option(String(DFPLAYER_EQ_POP), FPSTR(TINTF_0AA));
    interf->option(String(DFPLAYER_EQ_ROCK), FPSTR(TINTF_0AB));
    interf->option(String(DFPLAYER_EQ_JAZZ), FPSTR(TINTF_0AC));
    interf->option(String(DFPLAYER_EQ_CLASSIC), FPSTR(TINTF_0AD));
    interf->option(String(DFPLAYER_EQ_BASS), FPSTR(TINTF_0AE));
    interf->json_section_end();

    interf->button_submit(FPSTR(TCONST_00A0), FPSTR(TINTF_008), FPSTR(TCONST_0008));
    interf->json_section_end();

    interf->spacer();
    interf->button(FPSTR(TCONST_0004), FPSTR(TINTF_00B));

    interf->json_section_end();
}

void show_settings_mp3(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_mp3(interf, data);
    interf->json_frame_flush();
}

void set_settings_mp3(Interface *interf, JsonObject *data){
    if (!data) return;

    uint8_t val = (*data)[FPSTR(TCONST_00A7)].as<uint8_t>(); myLamp.setEqType(val); mp3->setEqType(val); // пишет в плеер!

    myLamp.setPlayTime((*data)[FPSTR(TCONST_00A3)]==FPSTR(TCONST_FFFF));
    myLamp.setPlayName((*data)[FPSTR(TCONST_00A4)]==FPSTR(TCONST_FFFF));
    myLamp.setPlayEffect((*data)[FPSTR(TCONST_00A5)]==FPSTR(TCONST_FFFF));
    myLamp.setAlatmSound((ALARM_SOUND_TYPE)(*data)[FPSTR(TCONST_00A6)].as<int>());
    myLamp.setPlayMP3((*data)[FPSTR(TCONST_00A8)]==FPSTR(TCONST_FFFF)); mp3->setPlayMP3(myLamp.getLampSettings().playMP3);

    SETPARAM(FPSTR(TCONST_00A9), mp3->setMP3count((*data)[FPSTR(TCONST_00A9)].as<int>())); // кол-во файлов в папке мп3
    //SETPARAM(FPSTR(TCONST_00A2), mp3->setVolume((*data)[FPSTR(TCONST_00A2)].as<int>()));
    SETPARAM(FPSTR(TCONST_00A2)); // тоже пишет в плеер, разносим во времени
    sysTicker.once(0.3,std::bind([](){
        mp3->setVolume(embui.param(FPSTR(TCONST_00A2)).toInt());
    }));

    save_lamp_flags();
    section_settings_frame(interf, data);
}
#endif

#ifdef MIC_EFFECTS
void block_settings_mic(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_0037), FPSTR(TINTF_020));

    interf->checkbox(FPSTR(TCONST_001E), myLamp.isMicOnOff()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_012), true);

    interf->json_section_begin(FPSTR(TCONST_0038));
    if (!myLamp.isMicCalibration()) {
        interf->number(FPSTR(TCONST_0039), (float)(round(myLamp.getMicScale() * 100) / 100), FPSTR(TINTF_022), 0.01);
        interf->number(FPSTR(TCONST_003A), (float)(round(myLamp.getMicNoise() * 100) / 100), FPSTR(TINTF_023), 0.01);
        interf->range(FPSTR(TCONST_003B), (int)myLamp.getMicNoiseRdcLevel(), 0, 4, (float)1.0, FPSTR(TINTF_024), false);
    }
    interf->button_submit(FPSTR(TCONST_0038), FPSTR(TINTF_008), FPSTR(TCONST_0008));
    interf->json_section_end();

    interf->spacer();
    interf->button(FPSTR(TCONST_003C), FPSTR(TINTF_025), myLamp.isMicCalibration()? FPSTR(TCONST_0008) : FPSTR(TCONST_000C));

    interf->spacer();
    interf->button(FPSTR(TCONST_0004), FPSTR(TINTF_00B));

    interf->json_section_end();
}

void show_settings_mic(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_mic(interf, data);
    interf->json_frame_flush();
}

void set_settings_mic(Interface *interf, JsonObject *data){
    if (!data) return;
    SETPARAM(FPSTR(TCONST_0039), myLamp.setMicScale((*data)[FPSTR(TCONST_0039)].as<float>()));
    SETPARAM(FPSTR(TCONST_003A), myLamp.setMicNoise((*data)[FPSTR(TCONST_003A)].as<float>()));
    SETPARAM(FPSTR(TCONST_003B), myLamp.setMicNoiseRdcLevel((MIC_NOISE_REDUCE_LEVEL)(*data)[FPSTR(TCONST_003B)].as<long>()));

    section_settings_frame(interf, data);
}

void set_micflag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setMicOnOff((*data)[FPSTR(TCONST_001E)] == FPSTR(TCONST_FFFF));
    save_lamp_flags();
    show_effects_param(interf,data);
}

void set_settings_mic_calib(Interface *interf, JsonObject *data){
    //if (!data) return;
    if (!myLamp.isMicOnOff()) {
        myLamp.sendStringToLamp(String(FPSTR(TINTF_026)).c_str(), CRGB::Red);
    } else if(!myLamp.isMicCalibration()) {
        myLamp.sendStringToLamp(String(FPSTR(TINTF_025)).c_str(), CRGB::Red);
        myLamp.setMicCalibration();
    } else {
        myLamp.sendStringToLamp(String(FPSTR(TINTF_027)).c_str(), CRGB::Red);
    }

    show_settings_mic(interf, data);
}
#endif

// формирование интерфейса настроек WiFi/MQTT
void block_settings_wifi(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_003D), FPSTR(TINTF_028));
    // форма настроек Wi-Fi

    interf->json_section_hidden(FPSTR(TCONST_003E), FPSTR(TINTF_029));
    interf->spacer(FPSTR(TINTF_02A));
    interf->text(FPSTR(TCONST_003F), FPSTR(TINTF_02B));
    interf->text(FPSTR(TCONST_0040), WiFi.SSID(), FPSTR(TINTF_02C), false);
    interf->password(FPSTR(TCONST_0041), FPSTR(TINTF_02D));
    interf->button_submit(FPSTR(TCONST_003E), FPSTR(TINTF_02E), FPSTR(TCONST_0008));
    interf->json_section_end();

    interf->json_section_hidden(FPSTR(TCONST_0042), FPSTR(TINTF_02F));
    interf->text(FPSTR(TCONST_003F), FPSTR(TINTF_02B));
    interf->spacer(FPSTR(TINTF_031));
    interf->comment(FPSTR(TINTF_032));
    interf->checkbox(FPSTR(TCONST_0043), FPSTR(TINTF_033));
    interf->password(FPSTR(TCONST_0044), FPSTR(TINTF_034));
    interf->button_submit(FPSTR(TCONST_0042), FPSTR(TINTF_008), FPSTR(TCONST_0008));
    interf->json_section_end();

    // форма настроек MQTT
    interf->json_section_hidden(FPSTR(TCONST_0045), FPSTR(TINTF_035));
    interf->text(FPSTR(TCONST_0046), FPSTR(TINTF_036));
    interf->number(FPSTR(TCONST_0047), FPSTR(TINTF_037));
    interf->text(FPSTR(TCONST_0048), FPSTR(TINTF_038));
    interf->text(FPSTR(TCONST_0049), FPSTR(TINTF_02D));
    interf->text(FPSTR(TCONST_007B), FPSTR(TINTF_08C));
    interf->number(FPSTR(TCONST_004A), FPSTR(TINTF_039));
    interf->button_submit(FPSTR(TCONST_0045), FPSTR(TINTF_03A), FPSTR(TCONST_0008));
    interf->json_section_end();

    interf->spacer();
    interf->button(FPSTR(TCONST_0004), FPSTR(TINTF_00B));

    interf->json_section_end();
}

void show_settings_wifi(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_wifi(interf, data);
    interf->json_frame_flush();
}

/**
 * настройка подключения WiFi в режиме AP
 */
void set_settings_wifiAP(Interface *interf, JsonObject *data){
    if (!data) return;

    SETPARAM(FPSTR(TCONST_003F));
    SETPARAM(FPSTR(TCONST_0043));
    SETPARAM(FPSTR(TCONST_0044));

    embui.save();
    embui.wifi_connect();

    section_settings_frame(interf, data);
}

/**
 * настройка подключения WiFi в режиме клиента
 */
void set_settings_wifi(Interface *interf, JsonObject *data){
    if (!data) return;

    SETPARAM(FPSTR(TCONST_003F));

    const char *ssid = (*data)[FPSTR(TCONST_0040)];
    const char *pwd = (*data)[FPSTR(TCONST_0041)];

    if(ssid){
        embui.wifi_connect(ssid, pwd);
    } else {
        LOG(println, F("WiFi: No SSID defined!"));
    }

    section_settings_frame(interf, data);
}

void set_settings_mqtt(Interface *interf, JsonObject *data){
    if (!data) return;
    SETPARAM(FPSTR(TCONST_0046), strncpy(embui.m_host, embui.param(FPSTR(TCONST_0046)).c_str(), sizeof(embui.m_host)-1));
    SETPARAM(FPSTR(TCONST_0048), strncpy(embui.m_user, embui.param(FPSTR(TCONST_0048)).c_str(), sizeof(embui.m_user)-1));
    SETPARAM(FPSTR(TCONST_0049), strncpy(embui.m_pass, embui.param(FPSTR(TCONST_0049)).c_str(), sizeof(embui.m_pass)-1));
    SETPARAM(FPSTR(TCONST_007B), strncpy(embui.m_pref, embui.param(FPSTR(TCONST_007B)).c_str(), sizeof(embui.m_pref)-1));
    SETPARAM(FPSTR(TCONST_0047), embui.m_port = embui.param(FPSTR(TCONST_0047)).toInt());
    SETPARAM(FPSTR(TCONST_004A), myLamp.semqtt_int((*data)[FPSTR(TCONST_004A)]));

    embui.save();

    section_settings_frame(interf, data);
}

void block_settings_other(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_004B), FPSTR(TINTF_002));

    interf->spacer(FPSTR(TINTF_030));
    interf->checkbox(FPSTR(TCONST_004C), myLamp.getLampSettings().MIRR_H ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_03B), false);
    interf->checkbox(FPSTR(TCONST_004D), myLamp.getLampSettings().MIRR_V ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_03C), false);
    interf->checkbox(FPSTR(TCONST_004E), myLamp.getLampSettings().isFaderON ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_03D), false);
    interf->checkbox(FPSTR(TCONST_008E), myLamp.getLampSettings().isEffClearing ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_083), false);
    interf->checkbox(FPSTR(TCONST_004F), myLamp.getLampSettings().dRand ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_03E), false);
    interf->checkbox(FPSTR(TCONST_009E), myLamp.getLampSettings().showName ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_09A), false);
    interf->range(FPSTR(TCONST_0026), 30, 250, 5, FPSTR(TINTF_03F));
    interf->checkbox(FPSTR(TCONST_0090), myLamp.getLampSettings().numInList ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_090), false); // нумерация в списке эффектов
#ifdef MIC_EFFECTS
    interf->checkbox(FPSTR(TCONST_0091), myLamp.getLampSettings().effHasMic ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_091), false); // значек микрофона в списке эффектов
#endif
    interf->select(FPSTR(TCONST_0050), FPSTR(TINTF_040));
    interf->option(String(SORT_TYPE::ST_BASE), FPSTR(TINTF_041));
    interf->option(String(SORT_TYPE::ST_END), FPSTR(TINTF_042));
    interf->option(String(SORT_TYPE::ST_IDX), FPSTR(TINTF_043));
    interf->option(String(SORT_TYPE::ST_AB), FPSTR(TINTF_085));
    interf->option(String(SORT_TYPE::ST_AB2), FPSTR(TINTF_08A));
#ifdef MIC_EFFECTS
    interf->option(String(SORT_TYPE::ST_MIC), FPSTR(TINTF_08D));  // эффекты с микрофоном
#endif
    interf->json_section_end();
#ifdef SHOWSYSCONFIG
    interf->checkbox(FPSTR(TCONST_0096), myLamp.getLampSettings().isShowSysMenu ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_093), false); // отображение системного меню
#endif

    interf->spacer(FPSTR(TINTF_001));
    interf->range(FPSTR(TCONST_0051), 10, 100, 10, FPSTR(TINTF_044));
    interf->range(FPSTR(TCONST_0052), -1, 10, 1, FPSTR(TINTF_045));

    interf->select(FPSTR(TCONST_0053), FPSTR(TINTF_046));
    interf->option(String(PERIODICTIME::PT_NOT_SHOW), FPSTR(TINTF_047));
    interf->option(String(PERIODICTIME::PT_EVERY_60), FPSTR(TINTF_048));
    interf->option(String(PERIODICTIME::PT_EVERY_30), FPSTR(TINTF_049));
    interf->option(String(PERIODICTIME::PT_EVERY_15), FPSTR(TINTF_04A));
    interf->option(String(PERIODICTIME::PT_EVERY_10), FPSTR(TINTF_04B));
    interf->option(String(PERIODICTIME::PT_EVERY_5), FPSTR(TINTF_04C));
    interf->option(String(PERIODICTIME::PT_EVERY_1), FPSTR(TINTF_04D));
    interf->json_section_end();

    interf->spacer(FPSTR(TINTF_04E));
    interf->number(FPSTR(TCONST_0054), FPSTR(TINTF_04F));
    interf->number(FPSTR(TCONST_0055), FPSTR(TINTF_050));
    
    interf->button_submit(FPSTR(TCONST_004B), FPSTR(TINTF_008), FPSTR(TCONST_0008));

    interf->spacer();
    interf->button(FPSTR(TCONST_0004), FPSTR(TINTF_00B));

    interf->json_section_end();
}

void show_settings_other(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_other(interf, data);
    interf->json_frame_flush();
}

void set_settings_other(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setMIRR_H((*data)[FPSTR(TCONST_004C)] == FPSTR(TCONST_FFFF));
    myLamp.setMIRR_V((*data)[FPSTR(TCONST_004D)] == FPSTR(TCONST_FFFF));
    myLamp.setFaderFlag((*data)[FPSTR(TCONST_004E)] == FPSTR(TCONST_FFFF));
    myLamp.setClearingFlag((*data)[FPSTR(TCONST_008E)] == FPSTR(TCONST_FFFF));
    myLamp.setDRand((*data)[FPSTR(TCONST_004F)] == FPSTR(TCONST_FFFF));
    myLamp.setShowName((*data)[FPSTR(TCONST_009E)] == FPSTR(TCONST_FFFF));
    myLamp.setNumInList((*data)[FPSTR(TCONST_0090)] == FPSTR(TCONST_FFFF));

    SETPARAM(FPSTR(TCONST_0026), ({if (myLamp.getMode() == MODE_DEMO){ myLamp.demoTimer(T_DISABLE); myLamp.demoTimer(T_ENABLE, embui.param(FPSTR(TCONST_0026)).toInt()); }}));
    SETPARAM(FPSTR(TCONST_0050), myLamp.effects.setEffSortType((*data)[FPSTR(TCONST_0050)].as<SORT_TYPE>()));
    SETPARAM(FPSTR(TCONST_0051), myLamp.setTextMovingSpeed((*data)[FPSTR(TCONST_0051)]));
    SETPARAM(FPSTR(TCONST_0052), myLamp.setTextOffset((*data)[FPSTR(TCONST_0052)]));
    SETPARAM(FPSTR(TCONST_0053), myLamp.setPeriodicTimePrint((PERIODICTIME)(*data)[FPSTR(TCONST_0053)].as<long>()));
    SETPARAM(FPSTR(TCONST_0054), myLamp.setNYMessageTimer((*data)[FPSTR(TCONST_0054)]));
    SETPARAM(FPSTR(TCONST_0055), myLamp.setNYUnixTime((*data)[FPSTR(TCONST_0055)]));
#ifdef MIC_EFFECTS
    myLamp.setEffHasMic((*data)[FPSTR(TCONST_0091)] == FPSTR(TCONST_FFFF));
#endif
    myLamp.setIsShowSysMenu((*data)[FPSTR(TCONST_0096)] == FPSTR(TCONST_FFFF));

    save_lamp_flags();
    section_settings_frame(interf, data);
}

void block_settings_time(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_0056), FPSTR(TINTF_051));

    interf->comment(FPSTR(TINTF_052));
    interf->text(FPSTR(TCONST_0057), FPSTR(TINTF_053));
    interf->text(FPSTR(TCONST_0058), FPSTR(TINTF_054));
    interf->text(FPSTR(TCONST_0059), FPSTR(TINTF_055));
    interf->button_submit(FPSTR(TCONST_0056), FPSTR(TINTF_008), FPSTR(TCONST_0008));

    interf->spacer();
    interf->button(FPSTR(TCONST_0004), FPSTR(TINTF_00B));

    interf->json_section_end();
}

void show_settings_time(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_time(interf, data);
    interf->json_frame_flush();
}

void set_settings_time(Interface *interf, JsonObject *data){
    if (!data) return;

    String datetime=(*data)[FPSTR(TCONST_0059)];
    if (datetime.length())
        myLamp.timeProcessor.setTime(datetime);

    SETPARAM(FPSTR(TCONST_0057), myLamp.timeProcessor.tzsetup((*data)[FPSTR(TCONST_0057)]));
    SETPARAM(FPSTR(TCONST_0058), myLamp.timeProcessor.setcustomntp((*data)[FPSTR(TCONST_0058)]));

    myLamp.sendStringToLamp(String(F("%TM")).c_str(), CRGB::Green);

    section_settings_frame(interf, data);
}

void block_settings_update(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_hidden(FPSTR(TCONST_005A), FPSTR(TINTF_056));
#ifdef OTA
    interf->spacer(FPSTR(TINTF_057));
    if (myLamp.getMode() != MODE_OTA) {
        interf->button(FPSTR(TCONST_0027), FPSTR(TINTF_058), FPSTR(TCONST_005B));
    } else {
        interf->button(FPSTR(TCONST_0027), FPSTR(TINTF_017), FPSTR(TCONST_0008));
    }
#endif
    interf->spacer(FPSTR(TINTF_059));
    interf->file(FPSTR(TCONST_005A), FPSTR(TCONST_005A), FPSTR(TINTF_05A));
    interf->json_section_end();
}

void block_settings_event(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_005C), FPSTR(TINTF_011));

    interf->checkbox(FPSTR(TCONST_001D), myLamp.IsEventsHandled()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_086), true);

    int num = 1;
    EVENT *next = nullptr;
    interf->json_section_begin(FPSTR(TCONST_005D));
    interf->select(FPSTR(TCONST_005E), String(0), String(FPSTR(TINTF_05B)), false);
    while ((next = myLamp.events.getNextEvent(next)) != nullptr) {
        interf->option(String(num), next->getName());
        ++num;
    }
    interf->json_section_end();

    interf->json_section_line();
    interf->button_submit_value(FPSTR(TCONST_005D), FPSTR(TCONST_005F), FPSTR(TINTF_05C), FPSTR(TCONST_002F));
    interf->button_submit_value(FPSTR(TCONST_005D), FPSTR(TCONST_000A), FPSTR(TINTF_006), FPSTR(TCONST_000C));
    interf->json_section_end();

    interf->json_section_end();

    interf->button(FPSTR(TCONST_005D), FPSTR(TINTF_05D));

    interf->spacer();
    interf->button(FPSTR(TCONST_0004), FPSTR(TINTF_00B));

    interf->json_section_end();
}

void show_settings_event(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_event(interf, data);
    interf->json_frame_flush();
}

void set_eventflag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setIsEventsHandled((*data)[FPSTR(TCONST_001D)] == FPSTR(TCONST_FFFF));
    save_lamp_flags();
}

void set_event_conf(Interface *interf, JsonObject *data){
    EVENT event;
    String act;
    if (!data) return;

    if (data->containsKey(FPSTR(TCONST_005E))) {
        EVENT *curr = nullptr;
        int i = 1, num = (*data)[FPSTR(TCONST_005E)];
        while ((curr = myLamp.events.getNextEvent(curr)) && i != num) ++i;
        if (!curr) return;
        myLamp.events.delEvent(*curr);
    }

    if (data->containsKey(FPSTR(TCONST_0060))) {
        event.isEnabled = ((*data)[FPSTR(TCONST_0060)] == FPSTR(TCONST_FFFF));
    } else {
        event.isEnabled = true;
    }

    event.d1 = ((*data)[FPSTR(TCONST_0061)] == FPSTR(TCONST_FFFF));
    event.d2 = ((*data)[FPSTR(TCONST_0062)] == FPSTR(TCONST_FFFF));
    event.d3 = ((*data)[FPSTR(TCONST_0063)] == FPSTR(TCONST_FFFF));
    event.d4 = ((*data)[FPSTR(TCONST_0064)] == FPSTR(TCONST_FFFF));
    event.d5 = ((*data)[FPSTR(TCONST_0065)] == FPSTR(TCONST_FFFF));
    event.d6 = ((*data)[FPSTR(TCONST_0066)] == FPSTR(TCONST_FFFF));
    event.d7 = ((*data)[FPSTR(TCONST_0067)] == FPSTR(TCONST_FFFF));
    event.event = (EVENT_TYPE)(*data)[FPSTR(TCONST_0068)].as<long>();
    event.repeat = (*data)[FPSTR(TCONST_0069)];
    event.stopat = (*data)[FPSTR(TCONST_006A)];
    String tmEvent = (*data)[FPSTR(TCONST_006B)];

    struct tm t;
    tm *tm=&t;
    Serial.println( tmEvent);   //debug
    Serial.println( tmEvent.substring(0,4).c_str());
    tm->tm_year=tmEvent.substring(0,4).toInt()-TM_BASE_YEAR;
    tm->tm_mon = tmEvent.substring(5,7).toInt()-1;
    tm->tm_mday=tmEvent.substring(8,10).toInt();
    tm->tm_hour=tmEvent.substring(11,13).toInt();
    tm->tm_min=tmEvent.substring(14,16).toInt();
    tm->tm_sec=0;

    LOG(printf_P, PSTR("Event at %d %d %d %d %d\n"), tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min);

    event.unixtime = mktime(tm);;
    String tmpMsg = (*data)[FPSTR(TCONST_0035)];
    event.message = (char*)(tmpMsg.c_str());

    myLamp.events.addEvent(event);
    myLamp.events.saveConfig();
    show_settings_event(interf, data);
}

void show_event_conf(Interface *interf, JsonObject *data){
    EVENT event;
    String act;
    bool edit = false;
    int i = 1, num = 0;
    if (!interf || !data) return;

    if (data->containsKey(FPSTR(TCONST_005E))) {
        EVENT *curr = nullptr;
        num = (*data)[FPSTR(TCONST_005E)];
        while ((curr = myLamp.events.getNextEvent(curr)) && i != num) ++i;
        if (!curr) return;
        act = (*data)[FPSTR(TCONST_005D)].as<String>();
        event = *curr;
        edit = true;
    }

    if (act == FPSTR(TCONST_000A)) {
        myLamp.events.delEvent(event);
        myLamp.events.saveConfig();
        show_settings_event(interf, data);
        return;
    } else
    if (data->containsKey(FPSTR(TCONST_002E))) {
        set_event_conf(interf, data);
        return;
    }


    interf->json_frame_interface();

    if (edit) {
        interf->json_section_main(FPSTR(TCONST_006C), FPSTR(TINTF_05C));
        interf->constant(FPSTR(TCONST_005E), String(num), event.getName());
        interf->checkbox(FPSTR(TCONST_0060), (event.isEnabled? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE)), FPSTR(TINTF_05E), false);
    } else {
        interf->json_section_main(FPSTR(TCONST_006C), FPSTR(TINTF_05D));
    }

    interf->select(FPSTR(TCONST_0068), String(event.event), String(FPSTR(TINTF_05F)), false);
    interf->option(String(EVENT_TYPE::ON), FPSTR(TINTF_060));
    interf->option(String(EVENT_TYPE::OFF), FPSTR(TINTF_061));
    interf->option(String(EVENT_TYPE::DEMO_ON), FPSTR(TINTF_062));
    interf->option(String(EVENT_TYPE::ALARM), FPSTR(TINTF_063));
    interf->option(String(EVENT_TYPE::LAMP_CONFIG_LOAD), FPSTR(TINTF_064));
    interf->option(String(EVENT_TYPE::EFF_CONFIG_LOAD), FPSTR(TINTF_065));
    interf->option(String(EVENT_TYPE::EVENTS_CONFIG_LOAD), FPSTR(TINTF_066));
    interf->option(String(EVENT_TYPE::SEND_TEXT), FPSTR(TINTF_067));
    interf->option(String(EVENT_TYPE::SEND_TIME), FPSTR(TINTF_068));
    interf->option(String(EVENT_TYPE::PIN_STATE), FPSTR(TINTF_069));
#ifdef AUX_PIN
    interf->option(String(EVENT_TYPE::AUX_ON), FPSTR(TINTF_06A));
    interf->option(String(EVENT_TYPE::AUX_OFF), FPSTR(TINTF_06B));
    interf->option(String(EVENT_TYPE::AUX_TOGGLE), FPSTR(TINTF_06C));
#endif
    interf->json_section_end();

    interf->datetime(FPSTR(TCONST_006B), event.getDateTime(), FPSTR(TINTF_06D));
    interf->number(FPSTR(TCONST_0069), event.repeat, FPSTR(TINTF_06E));
    interf->number(FPSTR(TCONST_006A), event.stopat, FPSTR(TINTF_06F));
    interf->text(FPSTR(TCONST_0035), String(event.message), FPSTR(TINTF_070), false);

    interf->json_section_hidden(FPSTR(TCONST_0069), FPSTR(TINTF_071));
    interf->checkbox(FPSTR(TCONST_0061), (event.d1? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE)), FPSTR(TINTF_072), false);
    interf->checkbox(FPSTR(TCONST_0062), (event.d2? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE)), FPSTR(TINTF_073), false);
    interf->checkbox(FPSTR(TCONST_0063), (event.d3? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE)), FPSTR(TINTF_074), false);
    interf->checkbox(FPSTR(TCONST_0064), (event.d4? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE)), FPSTR(TINTF_075), false);
    interf->checkbox(FPSTR(TCONST_0065), (event.d5? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE)), FPSTR(TINTF_076), false);
    interf->checkbox(FPSTR(TCONST_0066), (event.d6? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE)), FPSTR(TINTF_077), false);
    interf->checkbox(FPSTR(TCONST_0067), (event.d7? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE)), FPSTR(TINTF_078), false);
    interf->json_section_end();

    if (edit) {
        interf->hidden(FPSTR(TCONST_002E), FPSTR(TCONST_FFFF));
        interf->button_submit(FPSTR(TCONST_006C), FPSTR(TINTF_079));
    } else {
        interf->button_submit(FPSTR(TCONST_006C), FPSTR(TINTF_05D), FPSTR(TCONST_002F));
    }

    interf->spacer();
    interf->button(FPSTR(TCONST_005C), FPSTR(TINTF_00B));

    interf->json_section_end();
    interf->json_frame_flush();
}
#ifdef ESP_USE_BUTTON
void block_settings_butt(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_006D), FPSTR(TINTF_013));

    interf->checkbox(FPSTR(TCONST_001F), myButtons->isButtonOn()? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_07B), true);

    interf->json_section_begin(FPSTR(TCONST_006E));
    interf->select(FPSTR(TCONST_006F), String(0), String(FPSTR(TINTF_07A)), false);
    for (int i = 0; i < myButtons->size(); i++) {
        interf->option(String(i), (*myButtons)[i]->getName());
    }
    interf->json_section_end();

    interf->json_section_line();
    interf->button_submit_value(FPSTR(TCONST_006E), FPSTR(TCONST_005F), FPSTR(TINTF_05C), FPSTR(TCONST_002F));
    interf->button_submit_value(FPSTR(TCONST_006E), FPSTR(TCONST_000A), FPSTR(TINTF_006), FPSTR(TCONST_000C));
    interf->json_section_end();

    interf->json_section_end();

    interf->button(FPSTR(TCONST_006E), FPSTR(TINTF_05D));

    interf->spacer();
    interf->button(FPSTR(TCONST_0004), FPSTR(TINTF_00B));

    interf->json_section_end();
}

void show_settings_butt(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_butt(interf, data);
    interf->json_frame_flush();
}

void set_butt_conf(Interface *interf, JsonObject *data){
    if (!data) return;

    Button *btn = nullptr;
    bool on = ((*data)[FPSTR(TCONST_0070)] == FPSTR(TCONST_FFFF));
    bool hold = ((*data)[FPSTR(TCONST_0071)] == FPSTR(TCONST_FFFF));
    bool onetime = ((*data)[FPSTR(TCONST_0072)] == FPSTR(TCONST_FFFF));
    uint8_t clicks = (*data)[FPSTR(TCONST_0073)];
    BA action = (BA)(*data)[FPSTR(TCONST_0074)].as<long>();

    if (data->containsKey(FPSTR(TCONST_006F))) {
        int num = (*data)[FPSTR(TCONST_006F)];
        if (num < myButtons->size()) {
            btn = (*myButtons)[num];
        }
    }
    if (btn) {
        btn->action = action;
        btn->flags.on = on;
        btn->flags.hold = hold;
        btn->flags.click = clicks;
        btn->flags.onetime = onetime;
    } else {
        myButtons->add(new Button(on, hold, clicks, onetime, action));
    }

    myButtons->saveConfig();
    show_settings_butt(interf, data);
}

void show_butt_conf(Interface *interf, JsonObject *data){
    if (!interf || !data) return;

    Button *btn = nullptr;
    String act;
    int num = 0;

    if (data->containsKey(FPSTR(TCONST_006F))) {
        num = (*data)[FPSTR(TCONST_006F)];
        if (num < myButtons->size()) {
            act = (*data)[FPSTR(TCONST_006E)].as<String>();
            btn = (*myButtons)[num];
        }
    }

    if (act == FPSTR(TCONST_000A)) {
        myButtons->remove(num);
        myButtons->saveConfig();
        show_settings_butt(interf, data);
        return;
    } else
    if (data->containsKey(FPSTR(TCONST_002E))) {
        set_butt_conf(interf, data);
        return;
    }


    interf->json_frame_interface();

    if (btn) {
        interf->json_section_main(FPSTR(TCONST_0075), FPSTR(TINTF_05C));
        interf->constant(FPSTR(TCONST_006F), String(num), btn->getName());
    } else {
        interf->json_section_main(FPSTR(TCONST_0075), FPSTR(TINTF_05D));
    }

    interf->select(FPSTR(TCONST_0074), String(btn? btn->action : 0), String(FPSTR(TINTF_07A)), false);
    for (int i = 1; i < BA::BA_END; i++) {
        interf->option(String(i), btn_get_desc((BA)i));
    }
    interf->json_section_end();

    interf->checkbox(FPSTR(TCONST_0070), (btn? btn->flags.on : 0)? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_07C), false);
    interf->checkbox(FPSTR(TCONST_0071), (btn? btn->flags.hold : 0)? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_07D), false);
    interf->number(FPSTR(TCONST_0073), (btn? btn->flags.click : 0), FPSTR(TINTF_07E), 0, 7);
    interf->checkbox(FPSTR(TCONST_0072), (btn? btn->flags.onetime&1 : 0)? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE), FPSTR(TINTF_07F), false);

    if (btn) {
        interf->hidden(FPSTR(TCONST_002E), FPSTR(TCONST_FFFF));
        interf->button_submit(FPSTR(TCONST_0075), FPSTR(TINTF_079));
    } else {
        interf->button_submit(FPSTR(TCONST_0075), FPSTR(TINTF_05D), FPSTR(TCONST_002F));
    }

    interf->spacer();
    interf->button(FPSTR(TCONST_0076), FPSTR(TINTF_00B));

    interf->json_section_end();
    interf->json_frame_flush();
}

void set_btnflag(Interface *interf, JsonObject *data){
    // в отдельном классе, т.е. не входит в флаги лампы!
    if (!data) return;
    SETPARAM(FPSTR(TCONST_001F), myButtons->setButtonOn((*data)[FPSTR(TCONST_001F)] == FPSTR(TCONST_FFFF)));
}
#endif

void set_debugflag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setDebug((*data)[FPSTR(TCONST_0095)] == FPSTR(TCONST_FFFF));
    save_lamp_flags();
}

#ifdef MP3PLAYER
void set_mp3flag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setONMP3((*data)[FPSTR(TCONST_009D)] == FPSTR(TCONST_FFFF));
    mp3->setIsOn(myLamp.isONMP3());
    save_lamp_flags();
}
#endif

void section_effects_frame(Interface *interf, JsonObject *data){
    if(optionsTicker.active())
        optionsTicker.detach();
    if (!interf) return;
    interf->json_frame_interface(FPSTR(TINTF_080));
    block_effects_main(interf, data);
    interf->json_frame_flush();
}

void section_lamp_frame(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface(FPSTR(TINTF_080));
    block_lamp(interf, data);
    interf->json_frame_flush();
}

void section_settings_frame(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface(FPSTR(TINTF_080));

    interf->json_section_main(FPSTR(TCONST_0004), FPSTR(TINTF_002));

    interf->button(FPSTR(TCONST_0077), FPSTR(TINTF_051));

    interf->button(FPSTR(TCONST_0078), FPSTR(TINTF_081));
#ifdef MIC_EFFECTS
    interf->button(FPSTR(TCONST_0079), FPSTR(TINTF_020));
#endif
#ifdef MP3PLAYER
    interf->button(FPSTR(TCONST_009F), FPSTR(TINTF_099));
#endif

    interf->button(FPSTR(TCONST_005C), FPSTR(TINTF_011));
#ifdef ESP_USE_BUTTON
    interf->button(FPSTR(TCONST_0076), FPSTR(TINTF_013));
#endif
    interf->button(FPSTR(TCONST_007A), FPSTR(TINTF_082));

    block_lamp_config(interf, data);

    interf->spacer();
    block_settings_update(interf, data);

    interf->json_section_end();
    interf->json_frame_flush();
}

void section_main_frame(Interface *interf, JsonObject *data){
    if (!interf) return;

    interf->json_frame_interface(FPSTR(TINTF_080));

    block_menu(interf, data);
    block_effects_main(interf, data);

    interf->json_frame_flush();

    if(!embui.wifi_sta){
        // форсируем выбор вкладки настройки WiFi если контроллер не подключен к внешней AP
        show_settings_wifi(interf, data);
    }
}

void section_sys_settings_frame(Interface *interf, JsonObject *data){
    if (!interf) return;

    interf->json_frame_interface(FPSTR(TINTF_08F));

    block_menu(interf, data);
    interf->json_section_main(FPSTR(TCONST_0099), FPSTR(TINTF_08F));
        interf->spacer(FPSTR(TINTF_092)); // заголовок
        interf->json_section_line(FPSTR(TINTF_092)); // расположить в одной линии
#ifdef ESP_USE_BUTTON
            interf->number(FPSTR(TCONST_0097),FPSTR(TINTF_094),0,16);
#endif
#ifdef MP3PLAYER
            interf->number(FPSTR(TCONST_009B),FPSTR(TINTF_097),0,16);
            interf->number(FPSTR(TCONST_009C),FPSTR(TINTF_098),0,16);
#endif
        interf->json_section_end(); // конец контейнера
        interf->spacer();
        interf->number(FPSTR(TCONST_0098),FPSTR(TINTF_095),0,16000);
        interf->button_submit(FPSTR(TCONST_0099), FPSTR(TINTF_008), FPSTR(TCONST_0008));

        interf->spacer();
        interf->button(FPSTR(TCONST_0000), FPSTR(TINTF_00B));
    interf->json_section_end();
    
    interf->json_frame_flush();
}

void set_sys_settings(Interface *interf, JsonObject *data){
    if(!data) return;

#ifdef ESP_USE_BUTTON
    {String tmpChk = (*data)[FPSTR(TCONST_0097)]; if(tmpChk.toInt()>16) return;}
#endif
#ifdef MP3PLAYER
    {String tmpChk = (*data)[FPSTR(TCONST_009B)]; if(tmpChk.toInt()>16) return;}
    {String tmpChk = (*data)[FPSTR(TCONST_009C)]; if(tmpChk.toInt()>16) return;}
#endif
    {String tmpChk = (*data)[FPSTR(TCONST_0098)]; if(tmpChk.toInt()>16000) return;}

#ifdef ESP_USE_BUTTON
    SETPARAM(FPSTR(TCONST_0097));
#endif
#ifdef MP3PLAYER
    SETPARAM(FPSTR(TCONST_009B));
    SETPARAM(FPSTR(TCONST_009C));
#endif
    SETPARAM(FPSTR(TCONST_0098));
    myLamp.sendString(String(FPSTR(TINTF_096)).c_str(), CRGB::Red);
    sysTicker.once(10,std::bind([]{
        embui.save();
        ESP.restart();
    }));
    section_effects_frame(interf,data);
}

void set_lamp_flags(Interface *interf, JsonObject *data){
    if(!data) return;
    SETPARAM(FPSTR(TCONST_0094));
}

void save_lamp_flags(){
    DynamicJsonDocument doc(128);
    JsonObject obj = doc.to<JsonObject>();
    obj[FPSTR(TCONST_0094)] = myLamp.getLampFlags();
    set_lamp_flags(nullptr, &obj);
    obj.clear();
}

void create_parameters(){
    LOG(println, F("Создание дефолтных параметров"));
    // создаем дефолтные параметры для нашего проекта
    embui.var_create(FPSTR(TCONST_0094), "0"); // Дефолтный набор флагов // myLamp.getLampFlags()

    //WiFi
    embui.var_create(FPSTR(TCONST_003F), F(""));
    embui.var_create(FPSTR(TCONST_0043),  FPSTR(TCONST_FFFE));     // режим AP-only (только точка доступа), не трогать
    embui.var_create(FPSTR(TCONST_0044), F(""));      // пароль внутренней точки доступа

    // параметры подключения к MQTT
    embui.var_create(FPSTR(TCONST_0046), F("")); // Дефолтные настройки для MQTT
    embui.var_create(FPSTR(TCONST_0047), F("1883"));
    embui.var_create(FPSTR(TCONST_0048), F(""));
    embui.var_create(FPSTR(TCONST_0049), F(""));
    embui.var_create(FPSTR(TCONST_007B), embui.mc);  // m_pref == MAC по дефолту
    embui.var_create(FPSTR(TCONST_004A), F("30")); // интервал отправки данных по MQTT в секундах (параметр в энергонезависимой памяти)
    embui.var_create(FPSTR(TCONST_0016), F("1"));
    embui.var_create(FPSTR(TCONST_002A),F("cfg1"));
#ifdef ESP_USE_BUTTON
    embui.var_create(FPSTR(TCONST_001F), FPSTR(TCONST_FFFF)); // не трогать пока...
#endif
#ifdef AUX_PIN
    embui.var_create(FPSTR(TCONST_000E), FPSTR(TCONST_FFFE));
#endif
    embui.var_create(FPSTR(TCONST_0035), F(""));
    embui.var_create(FPSTR(TCONST_0036), FPSTR(TCONST_007C));
    embui.var_create(FPSTR(TCONST_0051), F("100"));
    embui.var_create(FPSTR(TCONST_0052), F("0"));
    embui.var_create(FPSTR(TCONST_0053), F("0"));
    embui.var_create(FPSTR(TCONST_0050), F("1"));
    embui.var_create(FPSTR(TCONST_0018), F("127"));

    // date/time related vars
    embui.var_create(FPSTR(TCONST_0057), "");
    embui.var_create(FPSTR(TCONST_0058), "");
    embui.var_create(FPSTR(TCONST_0054), F("0"));
    embui.var_create(FPSTR(TCONST_0055), FPSTR(TCONST_007D));

#ifdef MIC_EFFECTS
    embui.var_create(FPSTR(TCONST_0039),F("1.28"));
    embui.var_create(FPSTR(TCONST_003A),F("0.00"));
    embui.var_create(FPSTR(TCONST_003B),F("0"));
#endif

#ifdef RESTORE_STATE
    embui.var_create(FPSTR(TCONST_001B), FPSTR(TCONST_FFFE));
#endif

    embui.var_create(FPSTR(TCONST_0026), String(F("60"))); // Дефолтное значение, настраивается из UI

    // пины и системные настройки
#ifdef ESP_USE_BUTTON
    embui.var_create(FPSTR(TCONST_0097), String(BTN_PIN)); // Пин кнопки
#endif
#ifdef MP3PLAYER
    embui.var_create(FPSTR(TCONST_009B), String(MP3_RX_PIN)); // Пин RX плеера
    embui.var_create(FPSTR(TCONST_009C), String(MP3_TX_PIN)); // Пин TX плеера
    embui.var_create(FPSTR(TCONST_00A2),F("15")); // громкость
    embui.var_create(FPSTR(TCONST_00A9),F("255")); // кол-во файлов в папке mp3
#endif
    embui.var_create(FPSTR(TCONST_0098), String(CURRENT_LIMIT)); // Лимит по току

    // далее идут обработчики параметров
    embui.section_handle_add(FPSTR(TCONST_0099), set_sys_settings);

    embui.section_handle_add(FPSTR(TCONST_0094), set_lamp_flags);

    embui.section_handle_add(FPSTR(TCONST_007E), section_main_frame);
    embui.section_handle_add(FPSTR(TCONST_0020), show_main_flags);

    embui.section_handle_add(FPSTR(TCONST_0000), section_effects_frame);
    embui.section_handle_add(FPSTR(TCONST_0011), show_effects_param);
    embui.section_handle_add(FPSTR(TCONST_0016), set_effects_list);
    embui.section_handle_add(FPSTR(TCONST_0012), set_effects_bright);
    embui.section_handle_add(FPSTR(TCONST_0013), set_effects_speed);
    embui.section_handle_add(FPSTR(TCONST_0014), set_effects_scale);
    embui.section_handle_add(FPSTR(TCONST_007F), set_effects_dynCtrl);

    embui.section_handle_add(FPSTR(TCONST_0022), set_eff_prev);
    embui.section_handle_add(FPSTR(TCONST_0023), set_eff_next);

    embui.section_handle_add(FPSTR(TCONST_000F), show_effects_config);
    embui.section_handle_add(FPSTR(TCONST_0010), set_effects_config_list);
    embui.section_handle_add(FPSTR(TCONST_0005), set_effects_config_param);

    embui.section_handle_add(FPSTR(TCONST_001A), set_onflag);
    embui.section_handle_add(FPSTR(TCONST_001B), set_demoflag);
    embui.section_handle_add(FPSTR(TCONST_001C), set_gbrflag);
#ifdef OTA
    embui.section_handle_add(FPSTR(TCONST_0027), set_otaflag);
#endif
#ifdef AUX_PIN
    embui.section_handle_add(FPSTR(TCONST_000E), set_auxflag);
#endif
    embui.section_handle_add(FPSTR(TCONST_009A), section_sys_settings_frame);
    embui.section_handle_add(FPSTR(TCONST_0003), section_lamp_frame);
    embui.section_handle_add(FPSTR(TCONST_0034), set_lamp_textsend);
    embui.section_handle_add(FPSTR(TCONST_0030), edit_lamp_config);
    embui.section_handle_add(FPSTR(TCONST_0029), edit_lamp_config);

    embui.section_handle_add(FPSTR(TCONST_0004), section_settings_frame);
    embui.section_handle_add(FPSTR(TCONST_0078), show_settings_wifi);
    embui.section_handle_add(FPSTR(TCONST_003E), set_settings_wifi);
    embui.section_handle_add(FPSTR(TCONST_0042), set_settings_wifiAP);
    embui.section_handle_add(FPSTR(TCONST_0045), set_settings_mqtt);
    embui.section_handle_add(FPSTR(TCONST_007A), show_settings_other);
    embui.section_handle_add(FPSTR(TCONST_004B), set_settings_other);
    embui.section_handle_add(FPSTR(TCONST_0077), show_settings_time);
    embui.section_handle_add(FPSTR(TCONST_0056), set_settings_time);
#ifdef MIC_EFFECTS
    embui.section_handle_add(FPSTR(TCONST_0079), show_settings_mic);
    embui.section_handle_add(FPSTR(TCONST_0038), set_settings_mic);
    embui.section_handle_add(FPSTR(TCONST_001E), set_micflag);
    embui.section_handle_add(FPSTR(TCONST_003C), set_settings_mic_calib);
#endif
    embui.section_handle_add(FPSTR(TCONST_005C), show_settings_event);
    embui.section_handle_add(FPSTR(TCONST_005D), show_event_conf);
    embui.section_handle_add(FPSTR(TCONST_006C), set_event_conf);
    embui.section_handle_add(FPSTR(TCONST_001D), set_eventflag);
#ifdef ESP_USE_BUTTON
    embui.section_handle_add(FPSTR(TCONST_0076), show_settings_butt);
    embui.section_handle_add(FPSTR(TCONST_006E), show_butt_conf);
    embui.section_handle_add(FPSTR(TCONST_0075), set_butt_conf);
    embui.section_handle_add(FPSTR(TCONST_001F), set_btnflag);
    embui.section_handle_add(FPSTR(TCONST_0095), set_debugflag);
#endif
#ifdef MP3PLAYER
    embui.section_handle_add(FPSTR(TCONST_009D), set_mp3flag);
    embui.section_handle_add(FPSTR(TCONST_009F), show_settings_mp3);
    embui.section_handle_add(FPSTR(TCONST_00A0), set_settings_mp3);
#endif
}

void sync_parameters(){
    DynamicJsonDocument doc(1024);
    JsonObject obj = doc.to<JsonObject>();

    LAMPFLAGS tmp;
    tmp.lampflags = embui.param(FPSTR(TCONST_0094)).toInt();

#ifdef LAMP_DEBUG
    obj[FPSTR(TCONST_0095)] = tmp.isDebug ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    set_debugflag(nullptr, &obj);
    obj.clear();
#endif

#ifdef MP3PLAYER
    obj[FPSTR(TCONST_009D)] = tmp.isOnMP3 ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    set_mp3flag(nullptr, &obj);
    obj.clear();

    obj[FPSTR(TCONST_00A2)] = embui.param(FPSTR(TCONST_00A2));  // пишет в плеер!
    obj[FPSTR(TCONST_00A3)] = tmp.playTime ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    obj[FPSTR(TCONST_00A4)] = tmp.playName ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    obj[FPSTR(TCONST_00A5)] = tmp.playEffect ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    obj[FPSTR(TCONST_00A6)] = String(tmp.alarmSound);
    obj[FPSTR(TCONST_00A7)] = String(tmp.MP3eq); // пишет в плеер!
    obj[FPSTR(TCONST_00A8)] = tmp.playMP3 ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    obj[FPSTR(TCONST_00A9)] = embui.param(FPSTR(TCONST_00A9));

    set_settings_mp3(nullptr, &obj);
    obj.clear();
#endif

    obj[FPSTR(TCONST_001D)] = tmp.isEventsHandled ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    set_eventflag(nullptr, &obj);
    obj.clear();

    obj[FPSTR(TCONST_001C)] = tmp.isGlobalBrightness ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    set_gbrflag(nullptr, &obj);
    obj.clear();

    if (myLamp.IsGlobalBrightness()) {
        CALL_SETTER(FPSTR(TCONST_0012), embui.param(FPSTR(TCONST_0018)), set_effects_bright);
    } else {
        myLamp.setGlobalBrightness(embui.param(FPSTR(TCONST_0018)).toInt()); // починить бросок яркости в 255 при первом включении
    }

#ifdef RESTORE_STATE
    obj[FPSTR(TCONST_001A)] = tmp.ONflag ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    set_onflag(nullptr, &obj);
    obj.clear();
    CALL_SETTER(FPSTR(TCONST_001B), embui.param(FPSTR(TCONST_001B)), set_demoflag); // Демо через режимы, для него нужнен отдельный флаг :(
#endif

    CALL_SETTER(FPSTR(TCONST_0016), embui.param(FPSTR(TCONST_0016)), set_effects_list);

#ifdef AUX_PIN
    CALL_SETTER(FPSTR(TCONST_000E), embui.param(FPSTR(TCONST_000E)), set_auxflag);
#endif

    myLamp.setClearingFlag(tmp.isEffClearing);

// do{ yield(); } while (1==0);
// ниже возникает wdt reset при включенной build_type = debug - причина неустановлена

#ifdef MIC_EFFECTS
    obj[FPSTR(TCONST_001E)] = tmp.isMicOn ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    set_micflag(nullptr, &obj);
    obj.clear();

    obj[FPSTR(TCONST_0039)] = embui.param(FPSTR(TCONST_0039));
    obj[FPSTR(TCONST_003A)] = embui.param(FPSTR(TCONST_003A));
    obj[FPSTR(TCONST_003B)] = embui.param(FPSTR(TCONST_003B));
    set_settings_mic(nullptr, &obj);
    obj.clear();
#endif

#ifdef ESP_USE_BUTTON
    // в отдельном классе, в список флагов лампы не входит!
    CALL_SETTER(FPSTR(TCONST_001F), embui.param(FPSTR(TCONST_001F)), set_btnflag);
#endif

    obj[FPSTR(TCONST_004E)] = tmp.isFaderON ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    obj[FPSTR(TCONST_008E)] = tmp.isEffClearing ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    obj[FPSTR(TCONST_004C)] = tmp.MIRR_H ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    obj[FPSTR(TCONST_004D)] = tmp.MIRR_V ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    obj[FPSTR(TCONST_0090)] = tmp.numInList ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    obj[FPSTR(TCONST_004F)] = tmp.dRand ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    obj[FPSTR(TCONST_009E)] = tmp.showName ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    obj[FPSTR(TCONST_0091)] = tmp.effHasMic ? FPSTR(TCONST_FFFF) : FPSTR(TCONST_FFFE);
    
    obj[FPSTR(TCONST_0050)] = embui.param(FPSTR(TCONST_0050));
    obj[FPSTR(TCONST_0051)] = embui.param(FPSTR(TCONST_0051));
    obj[FPSTR(TCONST_0052)] = embui.param(FPSTR(TCONST_0052));
    obj[FPSTR(TCONST_0053)] = embui.param(FPSTR(TCONST_0053));
    obj[FPSTR(TCONST_0054)] = embui.param(FPSTR(TCONST_0054));
    obj[FPSTR(TCONST_0055)] = embui.param(FPSTR(TCONST_0055));

    set_settings_other(nullptr, &obj);
    obj.clear();
}

void remote_action(RA action, ...){
    LOG(printf_P, PSTR("RA %d: "), action);
    StaticJsonDocument<128> doc;
    JsonObject obj = doc.to<JsonObject>();

    char *key = NULL, *val = NULL, *value = NULL;
    va_list prm;
    va_start(prm, action);
    while ((key = (char *)va_arg(prm, char *)) && (val = (char *)va_arg(prm, char *))) {
        LOG(printf_P, PSTR("%s = %s"), key, val);
        obj[key] = val;
    }
    va_end(prm);
    if (key && !val) {
        value = key;
        LOG(printf_P, PSTR("%s"), value);
    }
    LOG(println, PSTR(""));

    switch (action) {
        case RA::RA_ON:
            CALL_INTF(FPSTR(TCONST_001A), FPSTR(TCONST_FFFF), set_onflag);
            break;
        case RA::RA_OFF:
            CALL_INTF(FPSTR(TCONST_001A), FPSTR(TCONST_FFFE), set_onflag);
            break;
        case RA::RA_DEMO:
            CALL_INTF(FPSTR(TCONST_001A), FPSTR(TCONST_FFFF), set_onflag); // включим, если было отключено
            CALL_INTF(FPSTR(TCONST_001B), FPSTR(TCONST_FFFF), set_demoflag);
            myLamp.startDemoMode();
            break;
        case RA::RA_DEMO_NEXT:
            if (myLamp.getLampSettings().dRand) {
                myLamp.switcheffect(SW_RND, myLamp.getFaderFlag());
            } else {
                myLamp.switcheffect(SW_NEXT_DEMO, myLamp.getFaderFlag());
            }
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()).c_str(), NULL);
        case RA::RA_EFFECT: {
            embui.var(FPSTR(TCONST_0016), value); // сохранить в конфиг изменившийся эффект
            CALL_INTF(FPSTR(TCONST_0016), value, set_effects_list);
            break;
        }
        case RA::RA_BRIGHT_NF:
            obj[FPSTR(TCONST_0017)] = true;
        case RA::RA_BRIGHT:
            CALL_INTF(FPSTR(TCONST_0012), value, set_effects_bright);
            break;
        case RA::RA_SPEED:
            CALL_INTF(FPSTR(TCONST_0013), value, set_effects_speed);
            break;
        case RA::RA_SCALE:
            CALL_INTF(FPSTR(TCONST_0014), value, set_effects_scale);
            break;
        case RA::RA_EXTRA:
            CALL_INTF(FPSTR(TCONST_0015), value, set_effects_dynCtrl);
            break;
#ifdef MIC_EFFECTS
        case RA::RA_MIC:
            CALL_INTF_OBJ(show_settings_mic);
            break;
#endif
        case RA::RA_EFF_NEXT:
            myLamp.switcheffect(SW_NEXT, myLamp.getFaderFlag());
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()).c_str(), NULL);
        case RA::RA_EFF_PREV:
            myLamp.switcheffect(SW_PREV, myLamp.getFaderFlag());
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()).c_str(), NULL);
        case RA::RA_EFF_RAND:
            myLamp.switcheffect(SW_RND, myLamp.getFaderFlag());
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()).c_str(), NULL);
        case RA::RA_WHITE_HI:
            myLamp.switcheffect(SW_WHITE_HI);
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()).c_str(), NULL);
        case RA::RA_WHITE_LO:
            myLamp.switcheffect(SW_WHITE_LO);
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()).c_str(), NULL);
        case RA::RA_ALARM:
            myLamp.startAlarm();
            break;
        case RA::RA_ALARM_OFF:
            myLamp.stopAlarm();
            break;
        case RA::RA_REBOOT:
            ESP.restart(); // так лучше :)
            break;
        case RA::RA_WIFI_REC:
            CALL_INTF(FPSTR(TINTF_028), FPSTR(TCONST_0080), set_settings_wifi);
            break;
        case RA::RA_LAMP_CONFIG:
            if (value && *value) {
                String filename = String(FPSTR(TCONST_0031));
                filename.concat(value);
                embui.load(filename.c_str());
            }
            break;
        case RA::RA_EFF_CONFIG:
            // if (value && *value) {
            //     String filename = String(FPSTR(TCONST_002C));
            //     filename.concat(value);
            //     myLamp.effects.loadConfig(filename.c_str());
            // }
            break;
        case RA::RA_EVENTS_CONFIG:
            if (value && *value) {
                String filename = String(FPSTR(TCONST_0032));
                filename.concat(value);
                myLamp.events.loadConfig(filename.c_str());
            }
            break;
        case RA::RA_SEND_TEXT: {
            String tmpStr = embui.param(FPSTR(TCONST_0036));
            if (value && *value) {
                String tmpStr = embui.param(FPSTR(TCONST_0036));
                tmpStr.replace(F("#"),F("0x"));
                CRGB::HTMLColorCode color = (CRGB::HTMLColorCode)strtol(tmpStr.c_str(), NULL, 0);

                myLamp.sendString(value, color);
            }
            break;
        }
        case RA::RA_SEND_IP:
            myLamp.sendString(WiFi.localIP().toString().c_str(), CRGB::White);
            break;
        case RA::RA_SEND_TIME:
            myLamp.sendString(String(F("%TM")).c_str(), CRGB::Green);
            break;
#ifdef OTA
        case RA::RA_OTA:
            myLamp.startOTAUpdate();
            break;
#endif
#ifdef AUX_PIN
        case RA::RA_AUX_ON:
            obj[FPSTR(TCONST_000E)] = true;
            set_auxflag(nullptr, &obj);
            break;
        case RA::RA_AUX_OFF:
            obj[FPSTR(TCONST_000E)] = false;
            set_auxflag(nullptr, &obj);
            break;
        case RA::RA_AUX_TOGLE:
            AUX_toggle(!digitalRead(AUX_PIN));
            break;
#endif
        default:;
    }
}

void httpCallback(const String &param, const String &value){
    RA action = RA_UNKNOWN;
    LOG(printf_P, PSTR("HTTP: %s - %s\n"), param.c_str(), value.c_str());

    if (param == FPSTR(TCONST_0070)) action = RA_ON;
    else if (param == FPSTR(TCONST_0081)) action = RA_OFF;
    else if (param == FPSTR(TCONST_00AA)) action = RA_DEMO;
    else if (param == FPSTR(TCONST_0035)) action = RA_SEND_TEXT;
    else if (param == FPSTR(TCONST_0012)) action = RA_BRIGHT;
    else if (param == FPSTR(TCONST_0013)) action = RA_SPEED;
    else if (param == FPSTR(TCONST_0014)) action = RA_SCALE;
    else if (param == FPSTR(TCONST_0082)) action = RA_EFFECT;
    else if (param == FPSTR(TCONST_0083)) action = RA_EFF_NEXT;
    else if (param == FPSTR(TCONST_0084)) action = RA_EFF_PREV;
    else if (param == FPSTR(TCONST_0085)) action = RA_EFF_RAND;
    else if (param == FPSTR(TCONST_0086)) action = RA_REBOOT;
    else if (param == FPSTR(TCONST_0087)) action = RA_ALARM;
#ifdef OTA
    else if (param == FPSTR(TCONST_0027)) action = RA_OTA;
#endif
#ifdef AUX_PIN
    else if (param == FPSTR(TCONST_0088)) action = RA_AUX_ON;
    else if (param == FPSTR(TCONST_0089))  action = RA_AUX_OFF;
    else if (param == FPSTR(TCONST_008A))  action = RA_AUX_TOGLE;
#endif
    remote_action(action, value.c_str(), NULL);
    embui.publish(String(FPSTR(TCONST_008B)) + param,value,false); // отправляем обратно в MQTT в топик jee/pub/
}

// обработка эвентов лампы
void event_worker(const EVENT *event){
    RA action = RA_UNKNOWN;
    LOG(printf_P, PSTR("%s - %s\n"), ((EVENT *)event)->getName().c_str(), myLamp.timeProcessor.getFormattedShortTime().c_str());

    switch (event->event) {
    case EVENT_TYPE::ON: action = RA_ON; break;
    case EVENT_TYPE::OFF: action = RA_OFF; break;
    case EVENT_TYPE::DEMO_ON: action = RA_DEMO; break;
    case EVENT_TYPE::ALARM: action = RA_ALARM; break;
    case EVENT_TYPE::LAMP_CONFIG_LOAD: action = RA_LAMP_CONFIG; break;
    case EVENT_TYPE::EFF_CONFIG_LOAD:  action = RA_EFF_CONFIG; break;
    case EVENT_TYPE::EVENTS_CONFIG_LOAD: action = RA_EVENTS_CONFIG; break;
    case EVENT_TYPE::SEND_TEXT:  action = RA_SEND_TEXT; break;
    case EVENT_TYPE::SEND_TIME:  action = RA_SEND_TIME; break;
#ifdef AUX_PIN
    case EVENT_TYPE::AUX_ON: action = RA_AUX_ON; break;
    case EVENT_TYPE::AUX_OFF: action = RA_AUX_OFF; break;
    case EVENT_TYPE::AUX_TOGGLE: action = RA_AUX_TOGLE; break;
#endif
    case EVENT_TYPE::PIN_STATE: {
        if (event->message == nullptr) break;

        String tmpS(event->message);
        tmpS.replace(F("'"),F("\"")); // так делать не красиво, но шопаделаешь...
        StaticJsonDocument<256> doc;
        deserializeJson(doc, tmpS);
        JsonArray arr = doc.as<JsonArray>();
        for (size_t i = 0; i < arr.size(); i++) {
            JsonObject item = arr[i];
            uint8_t pin = item[FPSTR(TCONST_008C)].as<int>();
            String action = item[FPSTR(TCONST_008D)].as<String>();
            pinMode(pin, OUTPUT);
            switch(action.c_str()[0]){
                case 'H':
                    digitalWrite(pin, HIGH); // LOW
                    break;
                case 'L':
                    digitalWrite(pin, LOW); // LOW
                    break;
                case 'T':
                    digitalWrite(pin, !digitalRead(pin)); // inverse
                    break;
                default:
                    break;
            }
        }
        break;
    }
    default:;
    }

    remote_action(action, event->message, NULL);
}
#ifdef ESP_USE_BUTTON
void default_buttons(){
    myButtons->clear();
    // Выключена
    myButtons->add(new Button(false, false, 1, true, BA::BA_ON)); // 1 клик - ON
    myButtons->add(new Button(false, false, 2, true, BA::BA_DEMO)); // 2 клика - Демо
    myButtons->add(new Button(false, true, 0, true, BA::BA_WHITE_LO)); // удержание Включаем белую лампу в мин яркость
    myButtons->add(new Button(false, true, 1, true, BA::BA_WHITE_HI)); // удержание + 1 клик Включаем белую лампу в полную яркость
    myButtons->add(new Button(false, true, 0, false, BA::BA_BRIGHT)); // удержание из выключенного - яркость
    myButtons->add(new Button(false, true, 1, false, BA::BA_BRIGHT)); // удержание из выключенного - яркость

    // Включена
    myButtons->add(new Button(true, false, 1, true, BA::BA_OFF)); // 1 клик - OFF
    myButtons->add(new Button(true, false, 2, true, BA::BA_EFF_NEXT)); // 2 клика - след эффект
    myButtons->add(new Button(true, false, 3, true, BA::BA_EFF_PREV)); // 3 клика - пред эффект
#ifdef OTA
    myButtons->add(new Button(true, false, 4, true, BA::BA_OTA)); // 4 клика - OTA
#endif
    myButtons->add(new Button(true, false, 5, true, BA::BA_SEND_IP)); // 5 клика - показ IP
    myButtons->add(new Button(true, false, 6, true, BA::BA_SEND_TIME)); // 6 клика - показ времени
    myButtons->add(new Button(true, true, 0, false, BA::BA_BRIGHT)); // удержание яркость
    myButtons->add(new Button(true, true, 1, false, BA::BA_SPEED)); // удержание + 1 клие скорость
    myButtons->add(new Button(true, true, 2, false, BA::BA_SCALE)); // удержание + 2 клика масштаб
}
#endif

void uploadProgress(size_t len, size_t total){
    static int prev = 0;
    float part = total / 50.0;
    int curr = len / part;
    if (curr != prev) {
        prev = curr;
        for (int i = 0; i < curr; i++) Serial.print(F("="));
        Serial.print(F("\n"));
    }
#ifdef VERTGAUGE
    myLamp.GaugeShow(len, total, 100);
#endif
}