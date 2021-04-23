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
#ifdef TM1637_CLOCK
#include "tm.h"				// Подключаем функции
#endif
#include LANG_FILE                  //"text_res.h"

/**
 * можно нарисовать свой собственный интефейс/обработчики с нуля, либо
 * подключить статический класс с готовыми формами для базовых системных натсроек и дополнить интерфейс.
 * необходимо помнить что существуют системные переменные конфигурации с зарезервированными именами.
 * Список имен системных переменных можно найти в файле "constants.h"
 */
#include "basicui.h"

#ifdef ESP32
    #include "esp_littlefs.h"
#endif

namespace INTERFACE {
// ------------- глобальные переменные построения интерфейса
// планировщик заполнения списка
Task *optionsTask = nullptr;     // задача для отложенной генерации списка
Task *delayedOptionTask = nullptr; // текущая отложенная задача, для сброса при повторных входах
Task *ctrlsTask = nullptr;       // планировщик контролов

static EffectListElem *confEff = nullptr; // эффект, который сейчас конфигурируется на странице "Управление списком эффектов"
static EVENT *cur_edit_event = NULL; // текущее редактируемое событие, сбрасывается после сохранения
// ------------- глобальные переменные построения интерфейса
} // namespace INTERFACE
using namespace INTERFACE;

// функция пересоздания/отмены генерации списка эффектов
void recreateoptionsTaskTask(bool isCancelOnly=false){
    if(optionsTask)
        optionsTask->cancel();
    if(delayedOptionTask)
        delayedOptionTask->cancel(); // отмена предыдущей задачи, если была запущена
    optionsTask = new Task(5 * TASK_SECOND, TASK_ONCE, delayedcall_show_effects, &ts, false, nullptr, [](){
        TASK_RECYCLE;
        optionsTask=nullptr;
    });
    if(!isCancelOnly)
        optionsTask->enableDelayed();
}

bool check_recovery_state(bool isSet){
    return false; // оключено до выяснения... какого-то хрена не работает :(
    if(LittleFS.begin()){
        String eff=embui.param(FPSTR(TCONST_0016));
        if(isSet) {
            File r = LittleFS.open(String(F("/recovery.chk")), "r"); // read
            String data = r.readString();
            LOG(printf_P, PSTR("prev check_recovery_state = %s\n"), data.c_str());
            if(data.startsWith(F("start"))){
                // похоже бутлуп, т.к. до конца инициализации не дошли...
                r.close();
                return true; // все плохо
            } else {
                r.close();
                File w = LittleFS.open(String(F("/recovery.chk")), "w"); // write
                w.print(F("start - "));
                w.println(eff); // пишу номер эффекта
                w.close();
                return false; // все хорошо
            }
        } else {
            File w = LittleFS.open(String(F("/recovery.chk")), "w"); // write
            w.print(F("end - "));
            w.println(eff); // пишу номер эффекта
            w.close();
            return false; // все хорошо
        }
    }
}

void resetAutoTimers(bool isEffects=false) // сброс таймера демо и настройка автосохранений
{
    myLamp.demoTimer(T_RESET);
    if(isEffects)
        myLamp.effects.autoSaveConfig();
}

#ifdef AUX_PIN
void AUX_toggle(bool key)
{
    if (key)
    {
        digitalWrite(AUX_PIN, AUX_LEVEL);
        embui.var(FPSTR(TCONST_000E), ("1"));
    }
    else
    {
        digitalWrite(AUX_PIN, !AUX_LEVEL);
        embui.var(FPSTR(TCONST_000E), ("0"));
    }
}
#endif

/**
 * @brief - callback function that is triggered every PUB_PERIOD seconds via EmbUI scheduler
 * used to publish periodic updates to websocket clients, if any
 * 
 */
void pubCallback(Interface *interf){
    LOG(println, F("pubCallback :"));
    if (!interf) return;
    interf->json_frame_value();
    interf->value(FPSTR(TCONST_0001), embui.timeProcessor.getFormattedShortTime(), true);
    interf->value(FPSTR(TCONST_0002), String(myLamp.getLampState().freeHeap), true);
    char fuptime[16];
    uint32_t tm = millis()/1000;
    sprintf_P(fuptime, PSTR("%u.%02u:%02u:%02u"),tm/86400,(tm/3600)%24,(tm/60)%60,tm%60);
    interf->value(FPSTR(TCONST_008F), String(fuptime), true);
    interf->value(FPSTR(TCONST_00C2), String(myLamp.getLampState().fsfreespace), true);
    int32_t rssi = myLamp.getLampState().rssi;
    interf->value(FPSTR(TCONST_00CE), String(constrain(map(rssi, -85, -40, 0, 100),0,100)) + F("% (") + String(rssi) + F("dBm)"), true);
    interf->json_frame_flush();
}

void block_menu(Interface *interf, JsonObject *data){
    if (!interf) return;
    // создаем меню
    interf->json_section_menu();

    interf->option(FPSTR(TCONST_0000), FPSTR(TINTF_000));   //  Эффекты
    interf->option(FPSTR(TCONST_0003), FPSTR(TINTF_001));   //  Вывод текста
    interf->option(FPSTR(TCONST_00C8), FPSTR(TINTF_0CE));   //  Рисование
    interf->option(FPSTR(TCONST_0004), FPSTR(TINTF_002));   //  настройки
#ifdef SHOWSYSCONFIG
    if(myLamp.isShowSysMenu())
        interf->option(FPSTR(TCONST_009A), FPSTR(TINTF_08F));
#endif
    interf->json_section_end();
}

/**
 * Страница с контролами параметров эфеекта
 * 
 */
void block_effects_config_param(Interface *interf, JsonObject *data){
    if (!interf || !confEff) return;

    String tmpName, tmpSoundfile;
    myLamp.effects.loadeffname(tmpName,confEff->eff_nb);
    myLamp.effects.loadsoundfile(tmpSoundfile,confEff->eff_nb);
    interf->json_section_begin(FPSTR(TCONST_0005));
    interf->text(FPSTR(TCONST_0092), tmpName, FPSTR(TINTF_089), false);
#ifdef MP3PLAYER
    interf->text(FPSTR(TCONST_00AB), tmpSoundfile, FPSTR(TINTF_0B2), false);
#endif
    interf->checkbox(FPSTR(TCONST_0006), confEff->canBeSelected()? "1" : "0", FPSTR(TINTF_003), false);
    interf->checkbox(FPSTR(TCONST_0007), confEff->isFavorite()? "1" : "0", FPSTR(TINTF_004), false);

    interf->spacer();

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

    interf->button_submit(FPSTR(TCONST_0005), FPSTR(TINTF_008), FPSTR(P_GRAY));
    interf->button_submit_value(FPSTR(TCONST_0005), FPSTR(TCONST_0009), FPSTR(TINTF_005));
    //if (confEff->eff_nb&0xFF00) { // пока удаление только для копий, но в теории можно удалять что угодно
        // interf->button_submit_value(FPSTR(TCONST_0005), FPSTR(TCONST_000A), FPSTR(TINTF_006), FPSTR(P_RED));
    //}

    interf->json_section_line();
    interf->button_submit_value(FPSTR(TCONST_0005), FPSTR(TCONST_00B0), FPSTR(TINTF_0B5), FPSTR(TCONST_00B3));
    interf->button_submit_value(FPSTR(TCONST_0005), FPSTR(TCONST_00B1), FPSTR(TINTF_0B4), FPSTR(P_RED));
    interf->json_section_end();

    interf->button_submit_value(FPSTR(TCONST_0005), FPSTR(TCONST_000B), FPSTR(TINTF_007), FPSTR(TCONST_000D));
    interf->button_submit_value(FPSTR(TCONST_0005), FPSTR(TCONST_0093), FPSTR(TINTF_08B), FPSTR(TCONST_000D));

    interf->json_section_end();
}

/**
 * Сформировать и вывести контролы для настроек параметров эффекта
 */
void show_effects_config_param(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_effects_config_param(interf, data);
    interf->json_frame_flush();
}

/**
 * обработчик установок эффекта
 */
void set_effects_config_param(Interface *interf, JsonObject *data){
    if (!confEff || !data) return;
    recreateoptionsTaskTask(true); // only cancel task
    EffectListElem *effect = confEff;
    
    SETPARAM(FPSTR(TCONST_0050), myLamp.effects.setEffSortType((*data)[FPSTR(TCONST_0050)].as<SORT_TYPE>()));
    
    String act = (*data)[FPSTR(TCONST_0005)];
    if (act == FPSTR(TCONST_0009)) {
        myLamp.effects.copyEffect(effect); // копируем текущий
    //} else if (act == FPSTR(TCONST_000A)) {
    } else if (act == FPSTR(TCONST_00B0) || act == FPSTR(TCONST_00B1)) {
        uint16_t tmpEffnb = effect->eff_nb;
        bool isCfgRemove = (act == FPSTR(TCONST_00B1));
        LOG(printf_P,PSTR("confEff->eff_nb=%d\n"), tmpEffnb);
        if(tmpEffnb==myLamp.effects.getCurrent()){
            myLamp.effects.directMoveBy(EFF_ENUM::EFF_NONE);
            myLamp.effects.deleteEffect(effect, isCfgRemove); // удаляем текущий
            remote_action(RA_EFF_NEXT, NULL);
        } else {
            myLamp.effects.deleteEffect(effect, isCfgRemove); // удаляем текущий
        }
        String tmpStr=F("- ");
        tmpStr+=String(tmpEffnb);
        myLamp.sendString(tmpStr.c_str(), CRGB::Red);
        //confEff = myLamp.effects.getEffect(EFF_ENUM::EFF_NONE);
        if(isCfgRemove){
            Task *_t = new Task(
                INDEX_BUILD_DELAY * TASK_SECOND,
                TASK_ONCE, [](){
                                   myLamp.effects.makeIndexFileFromFS(); // создаем индекс по файлам ФС и на выход
                                   recreateoptionsTaskTask();
                                   TASK_RECYCLE; },
                &ts, false);
            _t->enableDelayed();
        } else {
            Task *_t = new Task(
                INDEX_BUILD_DELAY * TASK_SECOND,
                TASK_ONCE, [](){
                                    myLamp.effects.makeIndexFileFromList(); // создаем индекс по текущему списку и на выход
                                    recreateoptionsTaskTask();
                                    TASK_RECYCLE; },
                &ts, false);
            _t->enableDelayed();
        }
        section_main_frame(interf, data);
        return;
    } else if (act == FPSTR(TCONST_000B)) {
        Task *_t = new Task(
            INDEX_BUILD_DELAY * TASK_SECOND,
            TASK_ONCE, [](){
                                myLamp.effects.makeIndexFileFromFS(); // создаем индекс по файлам ФС и на выход
                                recreateoptionsTaskTask();
                                TASK_RECYCLE; },
            &ts, false);
        _t->enableDelayed();
        section_main_frame(interf, data);
        return;
    } else if (act == FPSTR(TCONST_0093)) {
        LOG(printf_P,PSTR("confEff->eff_nb=%d\n"), effect->eff_nb);
        if(effect->eff_nb==myLamp.effects.getCurrent()){
            myLamp.effects.directMoveBy(EFF_ENUM::EFF_NONE);
            myLamp.effects.removeConfig(effect->eff_nb);
            remote_action(RA_EFF_NEXT, NULL);
        } else {
            myLamp.effects.removeConfig(effect->eff_nb);
        }
        String tmpStr=F("- ");
        tmpStr+=effect->eff_nb;
        myLamp.sendString(tmpStr.c_str(), CRGB::Red);
        //confEff = myLamp.effects.getEffect(EFF_ENUM::EFF_NONE);
        section_main_frame(interf, data);
        return;
    }
    else {
        effect->canBeSelected((*data)[FPSTR(TCONST_0006)] == "1");
        effect->isFavorite((*data)[FPSTR(TCONST_0007)] == "1");
        myLamp.effects.setSoundfile((*data)[FPSTR(TCONST_00AB)], effect);
#ifdef CASHED_EFFECTS_NAMES
        effect->setName((*data)[FPSTR(TCONST_0092)]);
#endif
        myLamp.effects.setEffectName((*data)[FPSTR(TCONST_0092)], effect);
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
        while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
            effname = FPSTR(T_EFFNAMEID[(uint8_t)eff->eff_nb]);
            interf->option(String(eff->eff_nb),
                String(eff->eff_nb) + (eff->eff_nb>255 ? String(F(" (")) + String(eff->eff_nb&0xFF) + String(F(")")) : String("")) + String(F(". ")) +
                String(effname) + 
                MIC_SYMBOL
            );
            #ifdef ESP8266
            ESP.wdtFeed();
            #elif defined ESP32
            delay(1);
            #endif
        }
        //interf->option(String(0),"");
    } else {
        EffectListElem *eff = nullptr;
        LOG(println,F("DBG1: using slow Names generation"));
        String effname((char *)0);
        MIC_SYMB;
        while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
            myLamp.effects.loadeffname(effname, eff->eff_nb);
            interf->option(String(eff->eff_nb),
                String(eff->eff_nb) + (eff->eff_nb>255 ? String(F(" (")) + String(eff->eff_nb&0xFF) + String(F(")")) : String("")) + String(F(". ")) +
                String(effname) + 
                MIC_SYMBOL
            );
            #ifdef ESP8266
            ESP.wdtFeed();
            #elif defined ESP32
            delay(1);
            #endif
        }
    }
    //interf->option(String(0),"");
    interf->json_section_end();
    LOG(printf_P,PSTR("DBG1: generating Names list took %ld ms\n"), millis() - timest);

    block_effects_config_param(interf, nullptr);

    interf->spacer();
    interf->button(FPSTR(TCONST_0000), FPSTR(TINTF_00B));
    interf->json_section_end();
}

// Построение выпадающего списка эффектов для вебморды
void delayedcall_show_effects(){

    LOG(println, F("=== GENERATE EffLIst for GUI ==="));
    uint16_t effnb = confEff?(int)confEff->eff_nb:myLamp.effects.getSelected(); // если confEff не NULL, то мы в конфирурировании, иначе в основном режиме
    
    Interface *interf = embui.ws.count()? new Interface(&embui, &embui.ws, 2048) : nullptr;
    if (!interf) return;

    if(delayedOptionTask)
        delayedOptionTask->cancel(); // отмена предыдущей задачи, если была запущена

    interf->json_frame_interface();
    interf->json_section_content();
    interf->select(confEff?FPSTR(TCONST_0010):FPSTR(TCONST_0016), String(effnb), String(FPSTR(TINTF_00A)), true, true); // не выводить метку

    EffectListElem **peff = new (EffectListElem *); // выделяем память под укзатель на указатель
    //LOG(print,(uint32_t)peff); LOG(print," "); LOG(println,(uint32_t)*peff);
    *peff = nullptr; // чистим содержимое
    delayedOptionTask = new Task(300, TASK_FOREVER,
        // loop
        [interf, peff](){
            EffectListElem *&eff = *peff; // здесь ссылка на указатель, т.к. нам нужно менять значение :)
            //LOG(print,(uint32_t)peff); LOG(print," "); LOG(println,(uint32_t)*peff);
            String effname((char *)0);
            MIC_SYMB;
            size_t cnt = 5; // генерим по 5 элементов
            bool numList = myLamp.getLampSettings().numInList;
            while (--cnt) {
                eff = myLamp.effects.getNextEffect(eff);
                if (eff != nullptr){
                    myLamp.effects.loadeffname(effname, eff->eff_nb);
                    LOG(println, effname);
                    if(confEff || eff->eff_nb || (!eff->eff_nb && eff->canBeSelected())){ // если в конфигурировании или не 0 или 0 эффект и он может быть выбран
                        interf->option(String(eff->eff_nb),
                            (!confEff ? EFF_NUMBER : String(eff->eff_nb) + (eff->eff_nb>255 ? String(F(" (")) + String(eff->eff_nb&0xFF) + String(F(")")) : String("")) + String(F(". "))) +
                            effname +
                            MIC_SYMBOL
                        );
                    }
                } else {
                    // тут перебрали все элементы и готовы к завершению
                    EffectListElem * first_eff=myLamp.effects.getFirstEffect();
                    if(!confEff && first_eff && !first_eff->canBeSelected()) // если мы не в конфигурировании эффектов и первый не может быть выбран, то пустой будет добавлен в конец
                        interf->option(String(0),"");
                    interf->json_section_end();
                    interf->json_section_end();
                    interf->json_frame_flush();

                    // восстановить позицию, если переключение произошло во время генерации списка
                    interf->json_frame_value();
                    interf->value(confEff?FPSTR(TCONST_0010):FPSTR(TCONST_0016), myLamp.effects.getSelected(), false);
                    interf->json_frame_flush();

                    Task *_t = &ts.currentTask();
                    _t->disable();
                    return;
                }
            }
        },
        &ts, true,
        nullptr,
        //onDisable
        [interf, peff](){
            LOG(println, F("=== GENERATE EffLIst for GUI completed ==="));
            delete peff; // освободить указатель на указатель
            delete interf;
            delayedOptionTask = nullptr;
            TASK_RECYCLE;
        }
    );
}

void show_effects_config(Interface *interf, JsonObject *data){
#ifdef DELAYED_EFFECTS
    if (!interf) return;
    interf->json_frame_interface();
    block_effects_config(interf, data);
    interf->json_frame_flush();
    recreateoptionsTaskTask();
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

    if(confEff){ // если переключаемся, то сохраняем предыдущие признаки в эффект до переключения
        LOG(printf_P, PSTR("eff_sel: %d eff_fav : %d\n"), (*data)[FPSTR(TCONST_0006)].as<bool>(),(*data)[FPSTR(TCONST_0007)].as<bool>());
    }

    confEff = myLamp.effects.getEffect(num);
    show_effects_config_param(interf, data);
    resetAutoTimers();
}

void publish_ctrls_vals()
{
  embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00AE), myLamp.effects.geteffconfig(String(myLamp.effects.getCurrent()).toInt(), myLamp.getNormalizedLampBrightness()), true);
}

void block_effects_param(Interface *interf, JsonObject *data){
    //if (!interf) return;
    bool isinterf = (interf != nullptr); // буду публиковать, даже если WebUI клиентов нет

    if(isinterf) interf->json_section_begin(FPSTR(TCONST_0011));

    LList<UIControl*>&controls = myLamp.effects.getControls();
    uint8_t ctrlCaseType; // тип контрола, старшие 4 бита соответствуют CONTROL_CASE, младшие 4 - CONTROL_TYPE
#ifdef MIC_EFFECTS
   bool isMicOn = myLamp.isMicOnOff();
    for(int i=0; i<controls.size();i++)
        if(controls[i]->getId()==7 && controls[i]->getName().startsWith(FPSTR(TINTF_020)))
            isMicOn = isMicOn && controls[i]->getVal().toInt();
#endif

    for(int i=0; i<controls.size();i++){
        ctrlCaseType = controls[i]->getType();
        switch(ctrlCaseType>>4){
            case CONTROL_CASE::HIDE :
                continue;
                break;
            case CONTROL_CASE::ISMICON :
#ifdef MIC_EFFECTS
                //if(!myLamp.isMicOnOff()) continue;
                if(!isMicOn && (!myLamp.isMicOnOff() || controls[i]->getId()!=7)) continue;
#else
                continue;
#endif          
                break;
            case CONTROL_CASE::ISMICOFF :
#ifdef MIC_EFFECTS
                //if(myLamp.isMicOnOff()) continue;
                if(isMicOn && (myLamp.isMicOnOff() || controls[i]->getId()!=7)) continue;
#else
                continue;
#endif   
                break;
            default: break;
        }
        bool isRandDemo = (myLamp.getLampSettings().dRand && myLamp.getMode()==LAMPMODE::MODE_DEMO);
        switch(ctrlCaseType&0x0F){
            case CONTROL_TYPE::RANGE :
                {
                    // String ctrlId = controls[i]->getId()==0 ? String(FPSTR(TCONST_0012))
                    //     : controls[i]->getId()==1 ? String(FPSTR(TCONST_0013))
                    //     : controls[i]->getId()==2 ? String(FPSTR(TCONST_0014))
                    //     : String(FPSTR(TCONST_0015)) + String(controls[i]->getId());
                    String ctrlId = String(FPSTR(TCONST_0015)) + String(controls[i]->getId());
                    String ctrlName = i ? controls[i]->getName() : (myLamp.IsGlobalBrightness() ? FPSTR(TINTF_00C) : FPSTR(TINTF_00D));
                    if(isRandDemo && controls[i]->getId()>0 && !(controls[i]->getName().startsWith(FPSTR(TINTF_020))==1 && controls[i]->getId()==7))
                        ctrlName=String(FPSTR(TINTF_0C9))+ctrlName;
                    int value = i ? controls[i]->getVal().toInt() : myLamp.getNormalizedLampBrightness();
                    if(isinterf) interf->range(
                        ctrlId
                        ,value
                        ,controls[i]->getMin().toInt()
                        ,controls[i]->getMax().toInt()
                        ,controls[i]->getStep().toInt()
                        , ctrlName
                        , true);
                    if(controls[i]->getId()<3)
                        embui.publish(String(FPSTR(TCONST_008B)) + ctrlId, String(value), true);
                }
                break;
            case CONTROL_TYPE::EDIT :
                {
                    String ctrlName = controls[i]->getName();
                    if(isRandDemo && controls[i]->getId()>0 && !(controls[i]->getName().startsWith(FPSTR(TINTF_020))==1 && controls[i]->getId()==7))
                        ctrlName=String(FPSTR(TINTF_0C9))+ctrlName;
                    
                    if(isinterf) interf->text(String(FPSTR(TCONST_0015)) + String(controls[i]->getId())
                    , controls[i]->getVal()
                    , ctrlName
                    , true
                    );
                    //embui.publish(String(FPSTR(TCONST_008B)) + String(FPSTR(TCONST_0015)) + String(controls[i]->getId()), String(controls[i]->getVal()), true);
                    break;
                }
            case CONTROL_TYPE::CHECKBOX :
                {
                    String ctrlName = controls[i]->getName();
                    if(isRandDemo && controls[i]->getId()>0 && !(controls[i]->getName().startsWith(FPSTR(TINTF_020))==1 && controls[i]->getId()==7))
                        ctrlName=String(FPSTR(TINTF_0C9))+ctrlName;

                    if(isinterf) interf->checkbox(String(FPSTR(TCONST_0015)) + String(controls[i]->getId())
                    , controls[i]->getVal()
                    , ctrlName
                    , true
                    );
                    //embui.publish(String(FPSTR(TCONST_008B)) + String(FPSTR(TCONST_0015)) + String(controls[i]->getId()), String(controls[i]->getVal()), true);
                    break;
                }
            default:
                break;
        }
    }
    publish_ctrls_vals();
    if(isinterf) interf->json_section_end();
}

void show_effects_param(Interface *interf, JsonObject *data){
    //if (!interf) return;
    bool isinterf = (interf != nullptr); // буду публиковать, даже если WebUI клиентов нет
    if(isinterf) interf->json_frame_interface();
    block_effects_param(interf, data);
    if(isinterf) interf->json_frame_flush();
}

void set_effects_list(Interface *interf, JsonObject *data){
    if (!data) return;
    uint16_t num = (*data)[FPSTR(TCONST_0016)].as<uint16_t>();
    uint16_t curr = myLamp.effects.getSelected();
    EffectListElem *eff = myLamp.effects.getEffect(num);
    if (!eff) return;

    if(myLamp.getMode()==LAMPMODE::MODE_WHITELAMP && num!=1){
        myLamp.startNormalMode(true);
        DynamicJsonDocument doc(512);
        JsonObject obj = doc.to<JsonObject>();
        CALL_INTF(FPSTR(TCONST_001A), myLamp.isLampOn() ? "1" : "0", set_onflag);
        return;
    }

    myLamp.setDRand(myLamp.getLampSettings().dRand); // сборосить флаг рандомного демо
    LOG(printf_P, PSTR("EFF LIST n:%d, o:%d, on:%d, md:%d\n"), eff->eff_nb, curr, myLamp.isLampOn(), myLamp.getMode());
    if (eff->eff_nb != curr) {
        if (!myLamp.isLampOn()) {
            myLamp.effects.directMoveBy(eff->eff_nb); // переходим на выбранный эффект для начальной инициализации
        } else {
            myLamp.switcheffect(SW_SPECIFIC, myLamp.getFaderFlag(), eff->eff_nb);
        }
        if(myLamp.getMode()==LAMPMODE::MODE_NORMAL)
            embui.var(FPSTR(TCONST_0016), (*data)[FPSTR(TCONST_0016)]);
        resetAutoTimers();
    }

    show_effects_param(interf, data);
    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_0082), String(eff->eff_nb), true);
    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00AE), myLamp.effects.geteffconfig(String(eff->eff_nb).toInt(), myLamp.getNormalizedLampBrightness()), true); // publish_ctrls_vals
}

// этот метод меняет контролы БЕЗ синхронизации со внешними системами
void direct_set_effects_dynCtrl(JsonObject *data){
    if (!data) return;

    String ctrlName;
    LList<UIControl*>&controls = myLamp.effects.getControls();
    for(int i=0; i<controls.size();i++){
        ctrlName = String(FPSTR(TCONST_0015))+String(controls[i]->getId());
        if((*data).containsKey(ctrlName)){
            if(!i){ // яркость???
                byte bright = (*data)[ctrlName];
                if (myLamp.getNormalizedLampBrightness() != bright) {
                    myLamp.setLampBrightness(bright);
                    if(myLamp.isLampOn())
                        myLamp.setBrightness(myLamp.getNormalizedLampBrightness(), !((*data)[FPSTR(TCONST_0017)]));
                    if (myLamp.IsGlobalBrightness()) {
                        embui.var(FPSTR(TCONST_0018), (*data)[ctrlName]);
                    } else
                        resetAutoTimers(true);
                } else {
                    myLamp.setLampBrightness(bright);
                    if (!myLamp.IsGlobalBrightness())
                        resetAutoTimers(true);
                }
            } else {
                controls[i]->setVal((*data)[ctrlName]); // для всех остальных
                resetAutoTimers(true);
            }
            if(myLamp.effects.worker) // && myLamp.effects.getEn()
                myLamp.effects.worker->setDynCtrl(controls[i]);
            break;
        }
    }
}

void set_effects_dynCtrl(Interface *interf, JsonObject *data){
    if (!data) return;

    // попытка повышения стабильности, отдаем управление браузеру как можно быстрее...
    if((*data).containsKey(FPSTR(TCONST_00D5)))
        direct_set_effects_dynCtrl(data);

    if(ctrlsTask && ctrlsTask->isEnabled())
        ctrlsTask->disable();

    DynamicJsonDocument *_str = new DynamicJsonDocument(256);
    (*_str)=(*data);
    //LOG(println, "Delaying dynctrl");

    ctrlsTask = new Task(300, TASK_ONCE,
        [_str](){
            JsonObject dataStore = (*_str).as<JsonObject>();
            JsonObject *data = &dataStore;
            
            LOG(println, "publishing & sending dynctrl...");
            String tmp; serializeJson(*data,tmp);
            direct_set_effects_dynCtrl(data);

            LOG(println, tmp);
            publish_ctrls_vals();
            // отправка данных в WebUI
            {
                bool isLocalMic = false;
                Interface *interf = embui.ws.count()? new Interface(&embui, &embui.ws, 512) : nullptr;
                if (interf) {
                    interf->json_frame_value();
                    for (JsonPair kv : *data){
                        interf->value(kv.key().c_str(), kv.value(), false);
                        if(String(kv.key().c_str())==String(F("dynCtrl7"))) // будем считать что это микрофон дергается, тогда обновим все контролы
                            isLocalMic = true;
                    }
                    interf->json_frame_flush();
                    delete interf;
                }
                if(isLocalMic){
                    Interface *interf = embui.ws.count()? new Interface(&embui, &embui.ws, 1024) : nullptr;
                    show_effects_param(interf, data);
                    delete interf;
                }
            }
        },
        &ts,
        false,
        nullptr,
        [_str](){
            //LOG(println, "Clearing dynctrl");
            delete _str;
            ctrlsTask = nullptr;
            TASK_RECYCLE;
        }
    );
    ctrlsTask->enableDelayed();
}

/**
 * Блок с наборами основных переключателей лампы
 * вкл/выкл, демо, кнопка и т.п.
 */
void block_main_flags(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_begin(FPSTR(TCONST_0019));
    interf->json_section_line("");
    interf->checkbox(FPSTR(TCONST_001A), String(myLamp.isLampOn()), FPSTR(TINTF_00E), true);
    interf->checkbox(FPSTR(TCONST_001B), String(myLamp.getMode() == MODE_DEMO), FPSTR(TINTF_00F), true);
    interf->checkbox(FPSTR(TCONST_001C), String(myLamp.IsGlobalBrightness()), FPSTR(TINTF_010), true);
    interf->checkbox(FPSTR(TCONST_001D), String(myLamp.IsEventsHandled()), FPSTR(TINTF_011), true);
    interf->checkbox(FPSTR(TCONST_00C4), String(myLamp.isDrawOn()), FPSTR(TINTF_0CE), true);

#ifdef MIC_EFFECTS
    interf->checkbox(FPSTR(TCONST_001E), myLamp.isMicOnOff()? "1" : "0", FPSTR(TINTF_012), true);
#endif
#ifdef AUX_PIN
    interf->checkbox(FPSTR(TCONST_000E), FPSTR(TCONST_000E), true);
#endif
#ifdef ESP_USE_BUTTON
    interf->checkbox(FPSTR(TCONST_001F), myButtons->isButtonOn()? "1" : "0", FPSTR(TINTF_013), true);
#endif
#ifdef MP3PLAYER
    interf->checkbox(FPSTR(TCONST_009D), myLamp.isONMP3()? "1" : "0", FPSTR(TINTF_099), true);
#endif
#ifdef LAMP_DEBUG
    interf->checkbox(FPSTR(TCONST_0095), myLamp.isDebugOn()? "1" : "0", FPSTR(TINTF_08E), true);
#endif
    interf->json_section_end();
#ifdef MP3PLAYER
    interf->json_section_line(F("line124")); // спец. имя - разбирается внутри html
    if(mp3->isMP3Mode()){
        interf->button(FPSTR(TCONST_00BE), FPSTR(TINTF_0BD), FPSTR(P_GRAY));
        interf->button(FPSTR(TCONST_00BF), FPSTR(TINTF_0BE), FPSTR(P_GRAY));
        interf->button(FPSTR(TCONST_00C0), FPSTR(TINTF_0BF), FPSTR(P_GRAY));
        interf->button(FPSTR(TCONST_00C1), FPSTR(TINTF_0C0), FPSTR(P_GRAY));
    }
    //interf->button("time", FPSTR(TINTF_016), FPSTR(TCONST_0025));    
    interf->json_section_end();
    interf->range(FPSTR(TCONST_00A2), 1, 30, 1, FPSTR(TINTF_09B), true);
#endif
    interf->json_section_end();
}

/**
 * Формирование и вывод интерфейса с основными переключателями
 * вкл/выкл, демо, кнопка и т.п.
 */
void show_main_flags(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_main_flags(interf, data);
    interf->spacer();
    interf->button(FPSTR(TCONST_0000), FPSTR(TINTF_00B));
    interf->json_frame_flush();
}

// Страница "Эффекты" (начальная страница)
void block_effects_main(Interface *interf, JsonObject *data, bool fast=true){
#ifndef DELAYED_EFFECTS
    fast=false;
#endif
    confEff = NULL; // т.к. не в конфигурировании, то сбросить данное значение

    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_0000), FPSTR(TINTF_000));

    interf->json_section_line(FPSTR(TCONST_0019));
    interf->checkbox(FPSTR(TCONST_001A), myLamp.isLampOn()? "1" : "0", FPSTR(TINTF_00E), true);
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
        bool isEmptyHidden=false;
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
                #ifdef ESP8266
                ESP.wdtFeed();
                #elif defined ESP32
                delay(1);
                #endif
            } else if(!eff->eff_nb){
                isEmptyHidden=true;
            }
        }
        if(isEmptyHidden)
            interf->option(String(0),"");
    } else {
        LOG(println,F("DBG2: using slow Names generation"));
        bool isEmptyHidden=false;
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
                #ifdef ESP8266
                ESP.wdtFeed();
                #elif defined ESP32
                yield();
                #endif
            } else if(!eff->eff_nb){
                isEmptyHidden=true;
            }
        }
        if(isEmptyHidden)
            interf->option(String(0),"");
    }
    interf->json_section_end();
    LOG(printf_P,PSTR("DBG2: generating Names list took %ld ms\n"), millis() - timest);

    block_effects_param(interf, data);

    interf->button(FPSTR(TCONST_000F), FPSTR(TINTF_009));

    interf->json_section_end();
#ifdef DELAYED_EFFECTS
    recreateoptionsTaskTask();
#endif
}

void set_eff_prev(Interface *interf, JsonObject *data){
    remote_action(RA::RA_EFF_PREV, NULL);
}

void set_eff_next(Interface *interf, JsonObject *data){
    remote_action(RA::RA_EFF_NEXT, NULL);
}

/**
 * Обработка вкл/выкл лампы
 */
void set_onflag(Interface *interf, JsonObject *data){
    if (!data) return;

    bool newpower = TOGLE_STATE((*data)[FPSTR(TCONST_001A)], myLamp.isLampOn());
    if (newpower != myLamp.isLampOn()) {
        if (newpower) {
            // включаем через switcheffect, т.к. простого isOn недостаточно чтобы запустить фейдер и поменять яркость (при необходимости)
            myLamp.switcheffect(SW_SPECIFIC, myLamp.getFaderFlag(), myLamp.effects.getEn());
            myLamp.changePower(newpower);
#ifdef MP3PLAYER
            if(myLamp.getLampSettings().isOnMP3)
                mp3->setIsOn(true);
#endif
#ifndef ESP_USE_BUTTON
            if(millis()<10000){
                Task *_t = new Task(
                    INDEX_BUILD_DELAY * TASK_SECOND,
                    TASK_ONCE, [](){ myLamp.sendString(WiFi.localIP().toString().c_str(), CRGB::White); TASK_RECYCLE; },
                    &ts, false);
                _t->enableDelayed();
            }
#endif
            embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_0070), "1", true);
            embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_0021), String(myLamp.getMode()), true);
            embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00AA), String(myLamp.getMode()==LAMPMODE::MODE_DEMO?"1":"0"), true);
        } else {
            resetAutoTimers(); // автосохранение конфига будет отсчитываться от этого момента
            //myLamp.changePower(newpower);
            Task *_t = new Task(300, TASK_ONCE,
                                [](){ // при выключении бывает эксепшен, видимо это слишком длительная операция, разносим во времени и отдаем управление
                                myLamp.changePower(false);
                #ifdef MP3PLAYER
                                mp3->setIsOn(false);
                #endif
                #ifdef RESTORE_STATE
                                save_lamp_flags(); // злобный баг, забыть передернуть флаги здесь)))), не вздумать убрать!!! Отлавливал его кучу времени
                #endif
                                embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_0070), "0", true);
                                embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_0021), String(myLamp.getMode()), true);
                                embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00AA), String(myLamp.getMode()==LAMPMODE::MODE_DEMO?"1":"0"), true);
                                TASK_RECYCLE; },
                                &ts, false);
            _t->enableDelayed();
        }
    }
#ifdef RESTORE_STATE
    save_lamp_flags();
#endif
}

void set_demoflag(Interface *interf, JsonObject *data){
    if (!data) return;
    resetAutoTimers();
    // Специально не сохраняем, считаю что демо при старте не должно запускаться
    bool newdemo = TOGLE_STATE((*data)[FPSTR(TCONST_001B)], (myLamp.getMode() == MODE_DEMO));
    switch (myLamp.getMode()) {
        case MODE_OTA:
        case MODE_ALARMCLOCK:
        case MODE_NORMAL:
            if (newdemo) myLamp.startDemoMode(embui.param(FPSTR(TCONST_0026)).toInt()); break;
        case MODE_DEMO:
        case MODE_WHITELAMP:
            if (!newdemo) myLamp.startNormalMode(); break;
        default:;
    }
#ifdef RESTORE_STATE
    embui.var(FPSTR(TCONST_001B), (*data)[FPSTR(TCONST_001B)]);
#endif
    myLamp.setDRand(myLamp.getLampSettings().dRand);
    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_0021), String(myLamp.getMode()), true);
    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00AA), String(myLamp.getMode()==LAMPMODE::MODE_DEMO?"1":"0"), true);
}

#ifdef OTA
void set_otaflag(Interface *interf, JsonObject *data){
    //if (!data) return;
    //myLamp.startOTAUpdate();
    remote_action(RA_OTA, NULL, NULL);

    interf->json_frame_interface();
    interf->json_section_content();
    interf->button(FPSTR(TCONST_0027), FPSTR(TINTF_017), FPSTR(P_GRAY));
    interf->json_section_end();
    interf->json_frame_flush();
    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_0021), String(myLamp.getMode()), true);
}
#endif

#ifdef AUX_PIN
void set_auxflag(Interface *interf, JsonObject *data){
    if (!data) return;
    if (((*data)[FPSTR(TCONST_000E)] == "1") != (digitalRead(AUX_PIN) == AUX_LEVEL ? true : false)) {
        AUX_toggle(!(digitalRead(AUX_PIN) == AUX_LEVEL ? true : false));
    }
}
#endif

void set_gbrflag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setIsGlobalBrightness((*data)[FPSTR(TCONST_001C)] == "1");
    embui.publish(String(FPSTR(TCONST_008B)) + String(FPSTR(TCONST_00B4)), String(myLamp.IsGlobalBrightness() ? "1" : "0"), true);
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
    String filename=embui.param(FPSTR(TCONST_002A));
    String cfg(FPSTR(TINTF_018)); cfg+=" ("; cfg+=filename; cfg+=")";

    // проверка на наличие конфигураций
    if(LittleFS.begin()){
#ifdef ESP32
        File tst = LittleFS.open(FPSTR(TCONST_002B));
        if(tst.openNextFile())
#else
        Dir tst = LittleFS.openDir(FPSTR(TCONST_002B));
        if(tst.next())
#endif    
        {
            interf->select(FPSTR(TCONST_002A), cfg);
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
            }
#else
            }
#endif
            interf->json_section_end(); // select

            interf->json_section_line();
                interf->button_submit_value(FPSTR(TCONST_0029), FPSTR(TCONST_002D), FPSTR(TINTF_019), FPSTR(P_GREEN));
                interf->button_submit_value(FPSTR(TCONST_0029), FPSTR(TCONST_002E), FPSTR(TINTF_008));
                interf->button_submit_value(FPSTR(TCONST_0029), FPSTR(TCONST_00B2), FPSTR(TINTF_006), FPSTR(P_RED));
            interf->json_section_end(); // json_section_line
            filename.clear();
            interf->spacer();
        }
    }
    interf->json_section_begin(FPSTR(TCONST_0030));
        interf->text(FPSTR(TCONST_00CF), filename, FPSTR(TINTF_01A), false);
        interf->button_submit(FPSTR(TCONST_0030), FPSTR(TINTF_01B));
    interf->json_section_end();

    interf->json_section_end(); // json_section_begin
    interf->json_section_end(); // json_section_hidden
}

void show_lamp_config(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_lamp_config(interf, data);
    interf->json_frame_flush();
}

void edit_lamp_config(Interface *interf, JsonObject *data){
    // Рбоата с конфигурациями в ФС
    if (!data) return;
    String name = (data->containsKey(FPSTR(TCONST_002A)) ? (*data)[FPSTR(TCONST_002A)] : (*data)[FPSTR(TCONST_00CF)]);
    String act = (*data)[FPSTR(TCONST_0029)];

    if(name.isEmpty() || act.isEmpty())
        name = (*data)[FPSTR(TCONST_00CF)].as<String>();
    LOG(printf_P, PSTR("name=%s, act=%s\n"), name.c_str(), act.c_str());

    if(name.isEmpty()) return;

    if (act == FPSTR(TCONST_00B2)) { // удаление
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
    } else if (act == FPSTR(TCONST_002D)) { // загрузка
        //myLamp.changePower(false);
        resetAutoTimers();

        String filename = String(FPSTR(TCONST_0031)) + name;
        embui.load(filename.c_str());

        filename = String(FPSTR(TCONST_002C)) + name;
        myLamp.effects.initDefault(filename.c_str());

        filename = String(FPSTR(TCONST_0032)) + name;
        myLamp.events.loadConfig(filename.c_str());
#ifdef ESP_USE_BUTTON
        filename = String(FPSTR(TCONST_0033)) + name;
        myButtons->clear();
        if (!myButtons->loadConfig()) {
            default_buttons();
        }
#endif
        //embui.var(FPSTR(TCONST_002A), name);

        String str = String(F("CFG:")) + name;
        myLamp.sendString(str.c_str(), CRGB::Red);

        Task *_t = new Task(3*TASK_SECOND, TASK_ONCE, [](){ sync_parameters(); TASK_RECYCLE; }, &ts, false);
        _t->enableDelayed();

    } else { // создание
        if(!name.endsWith(F(".json"))){
            name.concat(F(".json"));
        }

        String filename = String(FPSTR(TCONST_0031)) + name;
        embui.save(filename.c_str(), true);

        filename = String(FPSTR(TCONST_002C)) + name;
        myLamp.effects.makeIndexFileFromList(filename.c_str());

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
    interf->button_submit(FPSTR(TCONST_0034), FPSTR(TINTF_01F), FPSTR(P_GRAY));

    interf->json_section_hidden(FPSTR(TCONST_00B9), FPSTR(TINTF_002));
        interf->json_section_begin(FPSTR(TCONST_00BA));
            interf->spacer(FPSTR(TINTF_001));
                interf->range(FPSTR(TCONST_0051), 10, 100, 5, FPSTR(TINTF_044));
                interf->range(FPSTR(TCONST_0052), -1, (int)(HEIGHT>6?HEIGHT:6)-6, 1, FPSTR(TINTF_045));
                interf->range(FPSTR(TCONST_00C3), 0, 255, 1, FPSTR(TINTF_0CA));
                
            interf->spacer(FPSTR(TINTF_04E));
                interf->number(FPSTR(TCONST_0054), FPSTR(TINTF_04F));
                //interf->number(FPSTR(TCONST_0055), FPSTR(TINTF_050));
                String datetime;
                TimeProcessor::getDateTimeString(datetime, embui.param(FPSTR(TCONST_0055)).toInt());
                interf->text(FPSTR(TCONST_0055), datetime, FPSTR(TINTF_050), false);
                interf->button_submit(FPSTR(TCONST_00BA), FPSTR(TINTF_008), FPSTR(P_GRAY));
            interf->spacer();
                //interf->button(FPSTR(TCONST_0000), FPSTR(TINTF_00B));
                interf->button(FPSTR(TCONST_0003), FPSTR(TINTF_00B));
        interf->json_section_end();
    interf->json_section_end();

    interf->json_section_end();
}

void set_lamp_textsend(Interface *interf, JsonObject *data){
    if (!data) return;
    resetAutoTimers(); // откладываем автосохранения
    String tmpStr = (*data)[FPSTR(TCONST_0036)];
    embui.var(FPSTR(TCONST_0036), tmpStr);
    embui.var(FPSTR(TCONST_0035), (*data)[FPSTR(TCONST_0035)]);

    tmpStr.replace(F("#"), F("0x"));
    myLamp.sendString((*data)[FPSTR(TCONST_0035)], (CRGB::HTMLColorCode)strtol(tmpStr.c_str(), NULL, 0));
}

void block_drawing(Interface *interf, JsonObject *data){
    //Страница "Рисование"
    if (!interf) return;
    recreateoptionsTaskTask(true); // only cancel task
    interf->json_section_main(FPSTR(TCONST_00C8), FPSTR(TINTF_0CE));

    DynamicJsonDocument doc(512);
    JsonObject param = doc.to<JsonObject>();

    param[FPSTR(TCONST_00CD)] = WIDTH;
    param[FPSTR(TCONST_00CC)] = HEIGHT;
    param[FPSTR(TCONST_00CB)] = FPSTR(TINTF_0CF);

    interf->checkbox(FPSTR(TCONST_00C4), myLamp.isDrawOn()? "1" : "0", FPSTR(TINTF_0CE), true);
    interf->custom(String(FPSTR(TCONST_00C9)),String(FPSTR(TCONST_00C8)),embui.param(FPSTR(TCONST_0036)),String(FPSTR(TINTF_0D0)), param);
    param.clear();

    interf->json_section_end();
}

void set_drawing(Interface *interf, JsonObject *data){
    if (!data) return;

    String value = (*data)[FPSTR(TCONST_00C9)];
    if((*data).containsKey(FPSTR(TCONST_00C9)) && value!=F("null"))
        remote_action(RA_DRAW, value.c_str(), NULL);
    else {
        String key = String(FPSTR(TCONST_00C9))+String(F("_fill"));
        if((*data).containsKey(key)){
            value = (*data)[key].as<String>();
            remote_action(RA_FILLMATRIX, value.c_str(), NULL);
        }
    }
}

void block_lamptext(Interface *interf, JsonObject *data){
    //Страница "Вывод текста"
    if (!interf) return;
    recreateoptionsTaskTask(true); // only cancel task
    interf->json_section_main(FPSTR(TCONST_0003), FPSTR(TINTF_001));

    block_lamp_textsend(interf, data);

    interf->json_section_end();
}

void set_text_config(Interface *interf, JsonObject *data){
    if (!data) return;

    SETPARAM(FPSTR(TCONST_0051), myLamp.setTextMovingSpeed((*data)[FPSTR(TCONST_0051)]));
    SETPARAM(FPSTR(TCONST_0052), myLamp.setTextOffset((*data)[FPSTR(TCONST_0052)]));
    SETPARAM(FPSTR(TCONST_0054), myLamp.setNYMessageTimer((*data)[FPSTR(TCONST_0054)]));
    SETPARAM(FPSTR(TCONST_00C3), myLamp.setBFade((*data)[FPSTR(TCONST_00C3)]));

    String newYearTime = (*data)[FPSTR(TCONST_0055)]; // Дата/время наструпления нового года с интерфейса
    struct tm t;
    tm *tm=&t;
    localtime_r(TimeProcessor::now(), tm);  // reset struct to local now()

    // set desired date
    tm->tm_year = newYearTime.substring(0,4).toInt()-TM_BASE_YEAR;
    tm->tm_mon  = newYearTime.substring(5,7).toInt()-1;
    tm->tm_mday = newYearTime.substring(8,10).toInt();
    tm->tm_hour = newYearTime.substring(11,13).toInt();
    tm->tm_min  = newYearTime.substring(14,16).toInt();
    tm->tm_sec  = 0;

    time_t ny_unixtime = mktime(tm);
    LOG(printf_P, PSTR("Set New Year at %d %d %d %d %d (%ld)\n"), tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, ny_unixtime);

    //SETPARAM(FPSTR(TCONST_0055), myLamp.setNYUnixTime(ny_unixtime));
    embui.var(FPSTR(TCONST_0055),String(ny_unixtime)); myLamp.setNYUnixTime(ny_unixtime);

    if(!interf){
        interf = embui.ws.count()? new Interface(&embui, &embui.ws, 1024) : nullptr;
        //section_text_frame(interf, data);
        section_main_frame(interf, nullptr); // вернемся на главный экран (то же самое при начальном запуске)
        delete interf;
    } else
        section_text_frame(interf, data);
}

#ifdef MP3PLAYER
void block_settings_mp3(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_00A1), FPSTR(TINTF_099));

    interf->checkbox(FPSTR(TCONST_009D), myLamp.isONMP3()? "1" : "0", FPSTR(TINTF_099), true);
    interf->range(FPSTR(TCONST_00A2), 1, 30, 1, FPSTR(TINTF_09B), true);
    
    interf->json_section_begin(FPSTR(TCONST_00A0));
    interf->spacer(FPSTR(TINTF_0B1));
    interf->json_section_line(); // расположить в одной линии
        interf->checkbox(FPSTR(TCONST_00A4), myLamp.getLampSettings().playName ? "1" : "0", FPSTR(TINTF_09D), false);
    interf->json_section_end();
    interf->json_section_line(); // расположить в одной линии
        interf->checkbox(FPSTR(TCONST_00A5), myLamp.getLampSettings().playEffect ? "1" : "0", FPSTR(TINTF_09E), false);
        interf->checkbox(FPSTR(TCONST_00A8), myLamp.getLampSettings().playMP3 ? "1" : "0", FPSTR(TINTF_0AF), false);
    interf->json_section_end();

    //interf->checkbox(FPSTR(TCONST_00A3), myLamp.getLampSettings().playTime ? "1" : "0", FPSTR(TINTF_09C), false);
    interf->select(FPSTR(TCONST_00A3), String(myLamp.getLampSettings().playTime), String(FPSTR(TINTF_09C)), false);
    interf->option(String(TIME_SOUND_TYPE::TS_NONE), FPSTR(TINTF_0B6));
    interf->option(String(TIME_SOUND_TYPE::TS_VER1), FPSTR(TINTF_0B7));
    interf->option(String(TIME_SOUND_TYPE::TS_VER2), FPSTR(TINTF_0B8));
    interf->json_section_end();

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
    interf->checkbox(FPSTR(TCONST_00AF), myLamp.getLampSettings().limitAlarmVolume ? "1" : "0", FPSTR(TINTF_0B3), false);

    interf->select(FPSTR(TCONST_00A7), String(myLamp.getLampSettings().MP3eq), String(FPSTR(TINTF_0A8)), false);
    interf->option(String(DFPLAYER_EQ_NORMAL), FPSTR(TINTF_0A9));
    interf->option(String(DFPLAYER_EQ_POP), FPSTR(TINTF_0AA));
    interf->option(String(DFPLAYER_EQ_ROCK), FPSTR(TINTF_0AB));
    interf->option(String(DFPLAYER_EQ_JAZZ), FPSTR(TINTF_0AC));
    interf->option(String(DFPLAYER_EQ_CLASSIC), FPSTR(TINTF_0AD));
    interf->option(String(DFPLAYER_EQ_BASS), FPSTR(TINTF_0AE));
    interf->json_section_end();
    
    interf->number(FPSTR(TCONST_00A9), mp3->getMP3count(), FPSTR(TINTF_0B0), false);
    //SETPARAM(FPSTR(TCONST_00A9), mp3->setMP3count((*data)[FPSTR(TCONST_00A9)].as<int>())); // кол-во файлов в папке мп3

    interf->button_submit(FPSTR(TCONST_00A0), FPSTR(TINTF_008), FPSTR(P_GRAY));
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

    resetAutoTimers(); // сдвинем таймеры автосейва, т.к. длительная операция
    uint8_t val = (*data)[FPSTR(TCONST_00A7)].as<uint8_t>(); myLamp.setEqType(val); mp3->setEqType(val); // пишет в плеер!

    myLamp.setPlayTime((*data)[FPSTR(TCONST_00A3)].as<int>());
    myLamp.setPlayName((*data)[FPSTR(TCONST_00A4)]=="1");
    myLamp.setPlayEffect((*data)[FPSTR(TCONST_00A5)]=="1"); mp3->setPlayEffect(myLamp.getLampSettings().playEffect);
    myLamp.setAlatmSound((ALARM_SOUND_TYPE)(*data)[FPSTR(TCONST_00A6)].as<int>());
    myLamp.setPlayMP3((*data)[FPSTR(TCONST_00A8)]=="1"); mp3->setPlayMP3(myLamp.getLampSettings().playMP3);
    myLamp.setLimitAlarmVolume((*data)[FPSTR(TCONST_00AF)]=="1");

    SETPARAM(FPSTR(TCONST_00A9), mp3->setMP3count((*data)[FPSTR(TCONST_00A9)].as<int>())); // кол-во файлов в папке мп3
    //SETPARAM(FPSTR(TCONST_00A2), mp3->setVolume((*data)[FPSTR(TCONST_00A2)].as<int>()));
    SETPARAM(FPSTR(TCONST_00A2)); // тоже пишет в плеер, разносим во времени

    save_lamp_flags();
    BasicUI::section_settings_frame(interf, data);
}
#endif

#ifdef MIC_EFFECTS
void block_settings_mic(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_0037), FPSTR(TINTF_020));

    interf->checkbox(FPSTR(TCONST_001E), myLamp.isMicOnOff()? "1" : "0", FPSTR(TINTF_012), true);

    interf->json_section_begin(FPSTR(TCONST_0038));
    if (!myLamp.isMicCalibration()) {
        interf->number(FPSTR(TCONST_0039), (float)(round(myLamp.getLampState().getMicScale() * 100) / 100), FPSTR(TINTF_022), 0.01);
        interf->number(FPSTR(TCONST_003A), (float)(round(myLamp.getLampState().getMicNoise() * 100) / 100), FPSTR(TINTF_023), 0.01);
        interf->range(FPSTR(TCONST_003B), (int)myLamp.getLampState().getMicNoiseRdcLevel(), 0, 4, 1, FPSTR(TINTF_024), false);

        interf->button_submit(FPSTR(TCONST_0038), FPSTR(TINTF_008), FPSTR(P_GRAY));
        interf->json_section_end();

        interf->spacer();
        interf->button(FPSTR(TCONST_003C), FPSTR(TINTF_025), FPSTR(P_RED));
    } else {
        interf->button(FPSTR(TCONST_003C), FPSTR(TINTF_027), FPSTR(P_RED) );
    }

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
    float scale = (*data)[FPSTR(TCONST_0039)]; //atof((*data)[FPSTR(TCONST_0039)].as<String>().c_str());
    float noise = (*data)[FPSTR(TCONST_003A)]; //atof((*data)[FPSTR(TCONST_003A)].as<String>().c_str());
    MIC_NOISE_REDUCE_LEVEL rdl = (*data)[FPSTR(TCONST_003B)];

    // LOG(printf_P, PSTR("scale=%2.3f noise=%2.3f rdl=%d\n"),scale,noise,rdl);
    // String tmpStr;
    // serializeJson(*data, tmpStr);
    // LOG(printf_P, PSTR("*data=%s\n"),tmpStr.c_str());

    SETPARAM(FPSTR(TCONST_0039), myLamp.getLampState().setMicScale(scale));
    SETPARAM(FPSTR(TCONST_003A), myLamp.getLampState().setMicNoise(noise));
    SETPARAM(FPSTR(TCONST_003B), myLamp.getLampState().setMicNoiseRdcLevel(rdl));

    BasicUI::section_settings_frame(interf, data);
}

void set_micflag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setMicOnOff((*data)[FPSTR(TCONST_001E)] == "1");
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

/*
// формирование интерфейса настроек WiFi/MQTT
void block_settings_wifi(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_003D), FPSTR(TINTF_028));
    // форма настроек Wi-Fi

    interf->json_section_hidden(FPSTR(TCONST_003E), FPSTR(TINTF_029));
    interf->spacer(FPSTR(TINTF_02A));
    interf->text(FPSTR(P_hostname), FPSTR(TINTF_02B));
    interf->text(FPSTR(TCONST_0040), WiFi.SSID(), FPSTR(TINTF_02C), false);
    interf->password(FPSTR(TCONST_0041), FPSTR(TINTF_02D));
    interf->button_submit(FPSTR(TCONST_003E), FPSTR(TINTF_02E), FPSTR(P_GRAY));
    interf->json_section_end();

    interf->json_section_hidden(FPSTR(T_SET_WIFIAP), FPSTR(TINTF_02F));
    interf->text(FPSTR(P_hostname), FPSTR(TINTF_02B));
    interf->spacer(FPSTR(TINTF_031));
    interf->comment(FPSTR(TINTF_032));
    interf->checkbox(FPSTR(P_APonly), FPSTR(TINTF_033));
    interf->password(FPSTR(TCONST_0044), FPSTR(TINTF_034));
    interf->button_submit(FPSTR(T_SET_WIFIAP), FPSTR(TINTF_008), FPSTR(P_GRAY));
    interf->json_section_end();

    // форма настроек MQTT
    interf->json_section_hidden(FPSTR(TCONST_0045), FPSTR(TINTF_035));
    interf->text(FPSTR(P_m_host), FPSTR(TINTF_036));
    interf->number(FPSTR(P_m_port), FPSTR(TINTF_037));
    interf->text(FPSTR(P_m_user), FPSTR(TINTF_038));
    interf->text(FPSTR(P_m_pass), FPSTR(TINTF_02D));
    interf->text(FPSTR(TCONST_007B), FPSTR(TINTF_08C));
    interf->number(FPSTR(TCONST_004A), FPSTR(TINTF_039));
    interf->button_submit(FPSTR(TCONST_0045), FPSTR(TINTF_03A), FPSTR(P_GRAY));
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
*/

/**
 * настройка подключения WiFi в режиме AP
void set_settings_wifiAP(Interface *interf, JsonObject *data){
    if (!data) return;

    SETPARAM(FPSTR(P_hostname));
    SETPARAM(FPSTR(P_APonly));
    SETPARAM(FPSTR(TCONST_0044));

    embui.save();
    embui.wifi_connect();

    section_settings_frame(interf, data);
}
 */

/**
 * настройка подключения WiFi в режиме клиента
void set_settings_wifi(Interface *interf, JsonObject *data){
    if (!data) return;

    SETPARAM(FPSTR(P_hostname));

    const char *ssid = (*data)[FPSTR(TCONST_0040)];
    const char *pwd = (*data)[FPSTR(TCONST_0041)];

    if(ssid){
        embui.wifi_connect(ssid, pwd);
    } else {
        LOG(println, F("WiFi: No SSID defined!"));
    }

    section_settings_frame(interf, data);
}
*/
void set_settings_mqtt(Interface *interf, JsonObject *data){
    if (!data) return;
    BasicUI::set_settings_mqtt(interf,data);
    embui.mqttReconnect();
    int interval = (*data)[FPSTR(TCONST_004A)];
    LOG(print, F("New MQTT interval: ")); LOG(println, interval);
    myLamp.setmqtt_int(interval);
}

void block_settings_other(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_004B), FPSTR(TINTF_002));

    interf->spacer(FPSTR(TINTF_030));
    interf->checkbox(FPSTR(TCONST_004C), myLamp.getLampSettings().MIRR_H ? "1" : "0", FPSTR(TINTF_03B), false);
    interf->checkbox(FPSTR(TCONST_004D), myLamp.getLampSettings().MIRR_V ? "1" : "0", FPSTR(TINTF_03C), false);
    interf->checkbox(FPSTR(TCONST_004E), myLamp.getLampSettings().isFaderON ? "1" : "0", FPSTR(TINTF_03D), false);
    interf->checkbox(FPSTR(TCONST_008E), myLamp.getLampSettings().isEffClearing ? "1" : "0", FPSTR(TINTF_083), false);
    interf->checkbox(FPSTR(TCONST_004F), myLamp.getLampSettings().dRand ? "1" : "0", FPSTR(TINTF_03E), false);
    interf->checkbox(FPSTR(TCONST_009E), myLamp.getLampSettings().showName ? "1" : "0", FPSTR(TINTF_09A), false);
    interf->range(FPSTR(TCONST_0026), 30, 250, 5, FPSTR(TINTF_03F));
    float sf = embui.param(FPSTR(TCONST_0053)).toFloat();
    interf->range(FPSTR(TCONST_0053), sf, 0.25, 4.0, 0.25, FPSTR(TINTF_0D3), false);
    interf->checkbox(FPSTR(TCONST_0090), myLamp.getLampSettings().numInList ? "1" : "0", FPSTR(TINTF_090), false); // нумерация в списке эффектов
#ifdef MIC_EFFECTS
    interf->checkbox(FPSTR(TCONST_0091), myLamp.getLampSettings().effHasMic ? "1" : "0", FPSTR(TINTF_091), false); // значек микрофона в списке эффектов
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
    interf->checkbox(FPSTR(TCONST_0096), myLamp.getLampSettings().isShowSysMenu ? "1" : "0", FPSTR(TINTF_093), false); // отображение системного меню
#endif
    interf->spacer(FPSTR(TINTF_0BA));
    interf->range(FPSTR(TCONST_00BB), myLamp.getAlarmP(), 1, 15, 1, FPSTR(TINTF_0BB), false);
    interf->range(FPSTR(TCONST_00BC), myLamp.getAlarmT(), 1, 15, 1, FPSTR(TINTF_0BC), false);

    interf->button_submit(FPSTR(TCONST_004B), FPSTR(TINTF_008), FPSTR(P_GRAY));

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

    resetAutoTimers();

    DynamicJsonDocument *_str = new DynamicJsonDocument(1024);
    (*_str)=(*data);

    Task *_t = new Task(300, TASK_ONCE, [_str](){
        JsonObject dataStore = (*_str).as<JsonObject>();
        JsonObject *data = &dataStore;

        // LOG(printf_P,PSTR("Settings: %s\n"),tmpData.c_str());
        myLamp.setMIRR_H((*data)[FPSTR(TCONST_004C)] == "1");
        myLamp.setMIRR_V((*data)[FPSTR(TCONST_004D)] == "1");
        myLamp.setFaderFlag((*data)[FPSTR(TCONST_004E)] == "1");
        myLamp.setClearingFlag((*data)[FPSTR(TCONST_008E)] == "1");
        myLamp.setDRand((*data)[FPSTR(TCONST_004F)] == "1");
        myLamp.setShowName((*data)[FPSTR(TCONST_009E)] == "1");
        myLamp.setNumInList((*data)[FPSTR(TCONST_0090)] == "1");

        SETPARAM(FPSTR(TCONST_0026), ({if (myLamp.getMode() == MODE_DEMO){ myLamp.demoTimer(T_DISABLE); myLamp.demoTimer(T_ENABLE, embui.param(FPSTR(TCONST_0026)).toInt()); }}));
        SETPARAM(FPSTR(TCONST_0050), myLamp.effects.setEffSortType((*data)[FPSTR(TCONST_0050)].as<SORT_TYPE>()));
        float sf = (*data)[FPSTR(TCONST_0053)];
        SETPARAM(FPSTR(TCONST_0053), myLamp.setSpeedFactor(sf));
    #ifdef MIC_EFFECTS
        myLamp.setEffHasMic((*data)[FPSTR(TCONST_0091)] == "1");
    #endif
        myLamp.setIsShowSysMenu((*data)[FPSTR(TCONST_0096)] == "1");

        uint8_t alatmPT = ((*data)[FPSTR(TCONST_00BB)]).as<uint8_t>()<<4; // старшие 4 бита
        alatmPT = alatmPT | ((*data)[FPSTR(TCONST_00BC)]).as<uint8_t>(); // младшие 4 бита
        embui.var(FPSTR(TCONST_00BD), String(alatmPT)); myLamp.setAlarmPT(alatmPT);
        //SETPARAM(FPSTR(TCONST_00BD), myLamp.setAlarmPT(alatmPT));
        //LOG(printf_P, PSTR("alatmPT=%d, alatmP=%d, alatmT=%d\n"), alatmPT, myLamp.getAlarmP(), myLamp.getAlarmT());

        save_lamp_flags();
        delete _str;
        TASK_RECYCLE; },
        &ts, false
    );
    _t->enableDelayed();


    BasicUI::section_settings_frame(interf, data);
}

/*
// страницу-форму настроек времени строим методом фреймворка
void show_settings_time(Interface *interf, JsonObject *data){
    BasicUI::block_settings_time(interf, data);
}
*/

void set_settings_time(Interface *interf, JsonObject *data){
    BasicUI::set_settings_time(interf, data);
    myLamp.sendStringToLamp(String(F("%TM")).c_str(), CRGB::Green);
    #ifdef TM1637_CLOCK
    tm_setted();			// Проверяем, было ли настроено время
    #endif
}

void block_settings_update(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_hidden(FPSTR(TCONST_005A), FPSTR(TINTF_056));
#ifdef OTA
    interf->spacer(FPSTR(TINTF_057));
    if (myLamp.getMode() != MODE_OTA) {
        interf->button(FPSTR(TCONST_0027), FPSTR(TINTF_058), FPSTR(TCONST_005B));
    } else {
        interf->button(FPSTR(TCONST_0027), FPSTR(TINTF_017), FPSTR(P_GRAY));
    }
#endif
    interf->spacer(FPSTR(TINTF_059));
    interf->file(FPSTR(TCONST_005A), FPSTR(TCONST_005A), FPSTR(TINTF_05A));
    interf->json_section_end();
}

void block_settings_event(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_005C), FPSTR(TINTF_011));

    interf->checkbox(FPSTR(TCONST_001D), myLamp.IsEventsHandled()? "1" : "0", FPSTR(TINTF_086), true);

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
    interf->button_submit_value(FPSTR(TCONST_005D), FPSTR(TCONST_005F), FPSTR(TINTF_05C), FPSTR(P_GREEN));
    interf->button_submit_value(FPSTR(TCONST_005D), FPSTR(TCONST_00B6), FPSTR(TINTF_006), FPSTR(P_RED));
    interf->json_section_end();

    interf->json_section_end();

    interf->button(FPSTR(TCONST_005D), FPSTR(TINTF_05D));

    interf->spacer();
    interf->button(FPSTR(TCONST_0004), FPSTR(TINTF_00B));

    interf->json_section_end();
}

void show_settings_event(Interface *interf, JsonObject *data){
    if (!interf) return;

    if(cur_edit_event && !myLamp.events.isEnumerated(*cur_edit_event)){
        LOG(println, F("Удалено временное событие!"));
        delete cur_edit_event;
        cur_edit_event = NULL;
    } else {
        cur_edit_event = NULL;
    }

    interf->json_frame_interface();
    block_settings_event(interf, data);
    interf->json_frame_flush();
}

void set_eventflag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setIsEventsHandled((*data)[FPSTR(TCONST_001D)] == "1");
    save_lamp_flags();
}

void set_event_conf(Interface *interf, JsonObject *data){
    EVENT event;
    String act;
    if (!data) return;

    //String output;
    //serializeJson((*data), output);
    //LOG(println, output.c_str());

    if(cur_edit_event){
        myLamp.events.delEvent(*cur_edit_event);
    } else if (data->containsKey(FPSTR(TCONST_005E))) {
        EVENT *curr = nullptr;
        int i = 1, num = (*data)[FPSTR(TCONST_005E)];
        while ((curr = myLamp.events.getNextEvent(curr)) && i != num) ++i;
        if (!curr) return;
        myLamp.events.delEvent(*curr);
    }

    if (data->containsKey(FPSTR(TCONST_0060))) {
        event.isEnabled = ((*data)[FPSTR(TCONST_0060)] == "1");
    } else {
        event.isEnabled = true;
    }

    event.d1 = ((*data)[FPSTR(TCONST_0061)] == "1");
    event.d2 = ((*data)[FPSTR(TCONST_0062)] == "1");
    event.d3 = ((*data)[FPSTR(TCONST_0063)] == "1");
    event.d4 = ((*data)[FPSTR(TCONST_0064)] == "1");
    event.d5 = ((*data)[FPSTR(TCONST_0065)] == "1");
    event.d6 = ((*data)[FPSTR(TCONST_0066)] == "1");
    event.d7 = ((*data)[FPSTR(TCONST_0067)] == "1");
    event.event = (EVENT_TYPE)(*data)[FPSTR(TCONST_0068)].as<long>();
    event.repeat = (*data)[FPSTR(TCONST_0069)];
    event.stopat = (*data)[FPSTR(TCONST_006A)];
    String tmEvent = (*data)[FPSTR(TCONST_006B)];

    struct tm t;
    tm *tm=&t;
    localtime_r(TimeProcessor::now(), tm);  // reset struct to local now()

    // set desired date
    tm->tm_year=tmEvent.substring(0,4).toInt()-TM_BASE_YEAR;
    tm->tm_mon = tmEvent.substring(5,7).toInt()-1;
    tm->tm_mday=tmEvent.substring(8,10).toInt();
    tm->tm_hour=tmEvent.substring(11,13).toInt();
    tm->tm_min=tmEvent.substring(14,16).toInt();
    tm->tm_sec=0;

    LOG(printf_P, PSTR("Set Event at %d %d %d %d %d\n"), tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min);

    event.unixtime = mktime(tm);

    String buf; // внешний буффер, т.к. добавление эвента ниже
    switch(event.event){
        case EVENT_TYPE::ALARM: {
                DynamicJsonDocument doc(1024);
                doc[FPSTR(TCONST_00BB)] = (*data)[FPSTR(TCONST_00BB)];
                doc[FPSTR(TCONST_00BC)] = (*data)[FPSTR(TCONST_00BC)];
                doc[FPSTR(TCONST_0035)] = (*data)[FPSTR(TCONST_0035)];

#ifdef MP3PLAYER
                doc[FPSTR(TCONST_00D1)] = (*data)[FPSTR(TCONST_00D1)];
                doc[FPSTR(TCONST_00D2)] = (*data)[FPSTR(TCONST_00D2)];
                doc[FPSTR(TCONST_00D3)] = (*data)[FPSTR(TCONST_00D3)];
#endif
                serializeJson(doc,buf);
                buf.replace("\"","'");
                event.message = (char *)buf.c_str(); // менять не будем, так что пойдет такое приведение типов
            }
            break;
        default:
            event.message = (char*)((*data)[FPSTR(TCONST_0035)].as<char*>());
            break;
    }

    myLamp.events.addEvent(event);
    myLamp.events.saveConfig();
    cur_edit_event = NULL;
    show_settings_event(interf, data);
}

void show_event_conf(Interface *interf, JsonObject *data){
    String act;
    bool edit = false;
    int i = 1, num = 0;
    if (!interf || !data) return;

    LOG(print,F("event_conf=")); LOG(println, (*data)[FPSTR(TCONST_005D)].as<String>()); //  && data->containsKey(FPSTR(TCONST_005D))

    if (data->containsKey(FPSTR(TCONST_005E))) {
        EVENT *curr = NULL;
        num = (*data)[FPSTR(TCONST_005E)];
        while ((curr = myLamp.events.getNextEvent(curr)) && i != num) ++i;
        if (!curr) return;
        act = (*data)[FPSTR(TCONST_005D)].as<String>();
        cur_edit_event = curr;
        edit = true;
    } else if(cur_edit_event != NULL){
        if(data->containsKey(FPSTR(TCONST_0068)))
            cur_edit_event->event = (*data)[FPSTR(TCONST_0068)].as<EVENT_TYPE>(); // меняем тип налету
        if(myLamp.events.isEnumerated(*cur_edit_event))
            edit = true;
    } else {
        LOG(println, "Созданан пустой эвент!");
        cur_edit_event = new EVENT();
    }

    if (act == FPSTR(TCONST_00B6)) {
        myLamp.events.delEvent(*cur_edit_event);
        cur_edit_event = NULL;
        myLamp.events.saveConfig();
        show_settings_event(interf, data);
        return;
    } else if (data->containsKey(FPSTR(TCONST_002E))) {
        set_event_conf(interf, data);
        return;
    }

    interf->json_frame_interface();

    if (edit) {
        interf->json_section_main(FPSTR(TCONST_006C), FPSTR(TINTF_05C));
        interf->constant(FPSTR(TCONST_005E), String(num), cur_edit_event->getName());
        interf->checkbox(FPSTR(TCONST_0060), (cur_edit_event->isEnabled? "1" : "0"), FPSTR(TINTF_05E), false);
    } else {
        interf->json_section_main(FPSTR(TCONST_006C), FPSTR(TINTF_05D));
    }

    interf->json_section_line();
        interf->select(FPSTR(TCONST_0068), String(cur_edit_event->event), String(FPSTR(TINTF_05F)), true);
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
            interf->option(String(EVENT_TYPE::SET_EFFECT), FPSTR(TINTF_00A));
            interf->option(String(EVENT_TYPE::SET_WARNING), FPSTR(TINTF_0CB));
        interf->json_section_end();
        interf->datetime(FPSTR(TCONST_006B), cur_edit_event->getDateTime(), FPSTR(TINTF_06D));
    interf->json_section_end();
    interf->json_section_line();
        interf->number(FPSTR(TCONST_0069), cur_edit_event->repeat, FPSTR(TINTF_06E));
        interf->number(FPSTR(TCONST_006A), cur_edit_event->stopat, FPSTR(TINTF_06F));
    interf->json_section_end();

    switch(cur_edit_event->event){
        case EVENT_TYPE::ALARM: {
                DynamicJsonDocument doc(1024);
                String buf = cur_edit_event->message;
                buf.replace("'","\"");
                DeserializationError err = deserializeJson(doc,buf);
                int alarmP = !err && doc.containsKey(FPSTR(TCONST_00BB)) ? doc[FPSTR(TCONST_00BB)] : myLamp.getAlarmP();
                int alarmT = !err && doc.containsKey(FPSTR(TCONST_00BC)) ? doc[FPSTR(TCONST_00BC)] : myLamp.getAlarmT();
                String msg = !err && doc.containsKey(FPSTR(TCONST_0035)) ? doc[FPSTR(TCONST_0035)] : (cur_edit_event->message ? cur_edit_event->message : String(""));

                interf->spacer(FPSTR(TINTF_0BA));
                interf->text(FPSTR(TCONST_0035), msg, FPSTR(TINTF_070), false);
                interf->json_section_line();
                    interf->range(FPSTR(TCONST_00BB), alarmP, 1, 15, 1, FPSTR(TINTF_0BB), false);
                    interf->range(FPSTR(TCONST_00BC), alarmT, 1, 15, 1, FPSTR(TINTF_0BC), false);
                interf->json_section_end();
#ifdef MP3PLAYER
                String limitAlarmVolume = !err && doc.containsKey(FPSTR(TCONST_00D2)) ? doc[FPSTR(TCONST_00D2)] : String(myLamp.getLampSettings().limitAlarmVolume ? "1" : "0");
                String alarmFromStart = !err && doc.containsKey(FPSTR(TCONST_00D1)) ? doc[FPSTR(TCONST_00D1)] : String("1");
                String st = !err && doc.containsKey(FPSTR(TCONST_00D3)) ? doc[FPSTR(TCONST_00D3)] : String(myLamp.getLampSettings().alarmSound);
                interf->json_section_line();
                    interf->checkbox(FPSTR(TCONST_00D1), alarmFromStart, FPSTR(TINTF_0D1), false);
                    interf->checkbox(FPSTR(TCONST_00D2), limitAlarmVolume, FPSTR(TINTF_0D2), false);
                interf->json_section_end();
                interf->select(FPSTR(TCONST_00D3), st, String(FPSTR(TINTF_0A3)), false);
                    interf->option(String(ALARM_SOUND_TYPE::AT_NONE), FPSTR(TINTF_09F));
                    interf->option(String(ALARM_SOUND_TYPE::AT_FIRST), FPSTR(TINTF_0A0));
                    interf->option(String(ALARM_SOUND_TYPE::AT_SECOND), FPSTR(TINTF_0A4));
                    interf->option(String(ALARM_SOUND_TYPE::AT_THIRD), FPSTR(TINTF_0A5));
                    interf->option(String(ALARM_SOUND_TYPE::AT_FOURTH), FPSTR(TINTF_0A6));
                    interf->option(String(ALARM_SOUND_TYPE::AT_FIFTH), FPSTR(TINTF_0A7));
                    interf->option(String(ALARM_SOUND_TYPE::AT_RANDOM), FPSTR(TINTF_0A1));
                    interf->option(String(ALARM_SOUND_TYPE::AT_RANDOMMP3), FPSTR(TINTF_0A2));
                interf->json_section_end();
#endif
            }
            break;
        default:
            interf->text(FPSTR(TCONST_0035), String(cur_edit_event->message!=NULL?cur_edit_event->message:""), FPSTR(TINTF_070), false);
            break;
    }
    interf->json_section_hidden(FPSTR(TCONST_0069), FPSTR(TINTF_071));
        interf->json_section_line();
            interf->checkbox(FPSTR(TCONST_0061), (cur_edit_event->d1? "1" : "0"), FPSTR(TINTF_072), false);
            interf->checkbox(FPSTR(TCONST_0062), (cur_edit_event->d2? "1" : "0"), FPSTR(TINTF_073), false);
            interf->checkbox(FPSTR(TCONST_0063), (cur_edit_event->d3? "1" : "0"), FPSTR(TINTF_074), false);
            interf->checkbox(FPSTR(TCONST_0064), (cur_edit_event->d4? "1" : "0"), FPSTR(TINTF_075), false);
            interf->checkbox(FPSTR(TCONST_0065), (cur_edit_event->d5? "1" : "0"), FPSTR(TINTF_076), false);
            interf->checkbox(FPSTR(TCONST_0066), (cur_edit_event->d6? "1" : "0"), FPSTR(TINTF_077), false);
            interf->checkbox(FPSTR(TCONST_0067), (cur_edit_event->d7? "1" : "0"), FPSTR(TINTF_078), false);
        interf->json_section_end();
    interf->json_section_end();

    if (edit) {
        interf->hidden(FPSTR(TCONST_002E), "1"); // режим редактирования
        interf->button_submit(FPSTR(TCONST_006C), FPSTR(TINTF_079));
    } else {
        interf->hidden(FPSTR(TCONST_002E), "0"); // режим добавления
        interf->button_submit(FPSTR(TCONST_006C), FPSTR(TINTF_05D), FPSTR(P_GREEN));
    }

    interf->spacer();
    interf->button(FPSTR(TCONST_005C), FPSTR(TINTF_00B));

    interf->json_section_end();
    interf->json_frame_flush();
}

void set_eventlist(Interface *interf, JsonObject *data){
    if (!data) return;
    
    if(cur_edit_event && cur_edit_event->event!=(*data)[FPSTR(TCONST_0068)].as<EVENT_TYPE>()){ // только если реально поменялось, то обновляем интерфейс
        show_event_conf(interf,data);
    } else if((*data).containsKey(FPSTR(TCONST_002E))){ // эта часть срабатывает даже если нажата кнопка "обновить, следовательно ловим эту ситуацию"
        set_event_conf(interf, data); //через какую-то хитрую жопу отработает :)
    }
}

#ifdef ESP_USE_BUTTON
void block_settings_butt(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_006D), FPSTR(TINTF_013));

    interf->checkbox(FPSTR(TCONST_001F), myButtons->isButtonOn()? "1" : "0", FPSTR(TINTF_07B), true);

    interf->json_section_begin(FPSTR(TCONST_006E));
    interf->select(FPSTR(TCONST_006F), String(0), String(FPSTR(TINTF_07A)), false);
    for (int i = 0; i < myButtons->size(); i++) {
        interf->option(String(i), (*myButtons)[i]->getName());
    }
    interf->json_section_end();

    interf->json_section_line();
    interf->button_submit_value(FPSTR(TCONST_006E), FPSTR(TCONST_005F), FPSTR(TINTF_05C), FPSTR(P_GREEN));
    interf->button_submit_value(FPSTR(TCONST_006E), FPSTR(TCONST_00B6), FPSTR(TINTF_006), FPSTR(P_RED));
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
    bool on = ((*data)[FPSTR(TCONST_0070)] == "1");
    bool hold = ((*data)[FPSTR(TCONST_0071)] == "1");
    bool onetime = ((*data)[FPSTR(TCONST_0072)] == "1");
    uint8_t clicks = (*data)[FPSTR(TCONST_0073)];
    String param = (*data)[FPSTR(TCONST_00B5)].as<String>();
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
        btn->setParam(param);
    } else {
        myButtons->add(new Button(on, hold, clicks, onetime, action, param));
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

    if (act == FPSTR(TCONST_00B6)) {
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
        interf->option(String(i), FPSTR(btn_get_desc((BA)i)));
    }
    interf->json_section_end();

    interf->text(FPSTR(TCONST_00B5),(btn? btn->getParam() : String("")),FPSTR(TINTF_0B9),false);

    interf->checkbox(FPSTR(TCONST_0070), (btn? btn->flags.on : 0)? "1" : "0", FPSTR(TINTF_07C), false);
    interf->checkbox(FPSTR(TCONST_0071), (btn? btn->flags.hold : 0)? "1" : "0", FPSTR(TINTF_07D), false);
    interf->number(FPSTR(TCONST_0073), (btn? btn->flags.click : 0), FPSTR(TINTF_07E), 0, 7);
    interf->checkbox(FPSTR(TCONST_0072), (btn? btn->flags.onetime&1 : 0)? "1" : "0", FPSTR(TINTF_07F), false);

    if (btn) {
        interf->hidden(FPSTR(TCONST_002E), "1");
        interf->button_submit(FPSTR(TCONST_0075), FPSTR(TINTF_079));
    } else {
        interf->button_submit(FPSTR(TCONST_0075), FPSTR(TINTF_05D), FPSTR(P_GREEN));
    }

    interf->spacer();
    interf->button(FPSTR(TCONST_0076), FPSTR(TINTF_00B));

    interf->json_section_end();
    interf->json_frame_flush();
}

void set_btnflag(Interface *interf, JsonObject *data){
    // в отдельном классе, т.е. не входит в флаги лампы!
    if (!data) return;
    SETPARAM(FPSTR(TCONST_001F), myButtons->setButtonOn((*data)[FPSTR(TCONST_001F)] == "1"));
}
#endif

void set_debugflag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setDebug((*data)[FPSTR(TCONST_0095)] == "1");
    save_lamp_flags();
}

void set_drawflag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setDrawBuff((*data)[FPSTR(TCONST_00C4)] == "1");
    save_lamp_flags();
}

#ifdef MP3PLAYER
void set_mp3flag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setONMP3((*data)[FPSTR(TCONST_009D)] == "1");
    if(myLamp.isLampOn())
        mp3->setIsOn(myLamp.isONMP3(), true); // при включенной лампе - форсировать воспроизведение
    else
        mp3->setIsOn(myLamp.isONMP3(), (myLamp.getLampSettings().isOnMP3 && millis()>5000)); // при выключенной - только для mp3 и не после перезагрузки
    save_lamp_flags();
}

void set_mp3volume(Interface *interf, JsonObject *data){
    if (!data) return;
    int volume = (*data)[FPSTR(TCONST_00A2)];
    SETPARAM(FPSTR(TCONST_00A2), mp3->setVolume(volume));
}

void set_mp3_player(Interface *interf, JsonObject *data){
    if (!data) return;

    if(!myLamp.isONMP3()) return;
    uint16_t cur_palyingnb = mp3->getCurPlayingNb();
    if(data->containsKey(FPSTR(TCONST_00BE))){
        mp3->playEffect(cur_palyingnb-1,"");
    } else if(data->containsKey(FPSTR(TCONST_00BF))){
        mp3->playEffect(cur_palyingnb+1,"");
    } else if(data->containsKey(FPSTR(TCONST_00C0))){
        mp3->playEffect(cur_palyingnb-5,"");
    } else if(data->containsKey(FPSTR(TCONST_00C1))){
        mp3->playEffect(cur_palyingnb+5,"");
    }
}

#endif

void section_effects_frame(Interface *interf, JsonObject *data){
    recreateoptionsTaskTask(true); // only cancel task
    if (!interf) return;
    interf->json_frame_interface(FPSTR(TINTF_080));
    block_effects_main(interf, data);
    interf->json_frame_flush();
}

void section_text_frame(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface(FPSTR(TINTF_080));
    block_lamptext(interf, data);
    interf->json_frame_flush();
}

void section_drawing_frame(Interface *interf, JsonObject *data){
    // Рисование
    if (!interf) return;
    interf->json_frame_interface(FPSTR(TINTF_080));
    block_drawing(interf, data);
    interf->json_frame_flush();
}

void user_settings_frame(Interface *interf, JsonObject *data){
    // Страница "Настройки"
    if (!interf) return;
    recreateoptionsTaskTask(true); // only cancel task
/*
    interf->json_frame_interface(FPSTR(TINTF_080));

    interf->json_section_main(FPSTR(TCONST_0004), FPSTR(TINTF_002));

    interf->button(FPSTR(TCONST_0077), FPSTR(TINTF_051));
    interf->button(FPSTR(TCONST_0078), FPSTR(TINTF_081));
*/
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

    // закрытие и отправка кадра выполняется методом из фреймворка
/*
    interf->spacer();
    block_settings_update(interf, data);

    interf->json_section_end();
    interf->json_frame_flush();
*/
}

void section_main_frame(Interface *interf, JsonObject *data){
    if (!interf) return;

    interf->json_frame_interface(FPSTR(TINTF_080));

    block_menu(interf, data);
    block_effects_main(interf, data);

    interf->json_frame_flush();

    if(!embui.sysData.wifi_sta && embui.param(FPSTR(P_APonly))=="0"){
        // форсируем выбор вкладки настройки WiFi если контроллер не подключен к внешней AP
        BasicUI::block_settings_netw(interf, data);
    }
}

void section_sys_settings_frame(Interface *interf, JsonObject *data){
    // Страница "Настройки ESP"
    if (!interf) return;
    recreateoptionsTaskTask(true); // only cancel task
    interf->json_frame_interface(FPSTR(TINTF_08F));

    block_menu(interf, data);
    interf->json_section_main(FPSTR(TCONST_0099), FPSTR(TINTF_08F));
        interf->spacer(FPSTR(TINTF_092)); // заголовок
        interf->json_section_line(FPSTR(TINTF_092)); // расположить в одной линии
#ifdef ESP_USE_BUTTON
            interf->number(FPSTR(TCONST_0097),FPSTR(TINTF_094),1,0,16);
#endif
#ifdef MP3PLAYER
            interf->number(FPSTR(TCONST_009B),FPSTR(TINTF_097),1,0,16);
            interf->number(FPSTR(TCONST_009C),FPSTR(TINTF_098),1,0,16);
#endif
        interf->json_section_end(); // конец контейнера
        interf->spacer();
        interf->number(FPSTR(TCONST_0098),FPSTR(TINTF_095),100,0,16000);

        //interf->json_section_main(FPSTR(TCONST_005F), "");
        interf->frame2(FPSTR(TCONST_005F), FPSTR(TCONST_005F));
        //interf->json_section_end();

        interf->button_submit(FPSTR(TCONST_0099), FPSTR(TINTF_008), FPSTR(P_GRAY));

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

    new Task(TASK_SECOND, TASK_ONCE, nullptr, &ts, true, nullptr, [](){ embui.autosave(true); });

    new Task(5 * TASK_SECOND, TASK_ONCE, nullptr, &ts, true, nullptr,  [](){ LOG(println, F("Rebooting...")); delay(100); ESP.restart(); });

    section_effects_frame(interf,data);
}

void set_lamp_flags(Interface *interf, JsonObject *data){
    if(!data) return;
    SETPARAM(FPSTR(TCONST_0094));
}

void save_lamp_flags(){
    DynamicJsonDocument doc(128);
    JsonObject obj = doc.to<JsonObject>();
    obj[FPSTR(TCONST_0094)] = String(myLamp.getLampFlags());
    set_lamp_flags(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();
}

/**
 * Набор конфигурационных переменных и обработчиков интерфейса
 */
void create_parameters(){
    LOG(println, F("Создание дефолтных параметров"));
    // создаем дефолтные параметры для нашего проекта
    embui.var_create(FPSTR(TCONST_0094), 0); // Дефолтный набор флагов // myLamp.getLampFlags()
    embui.var_create(FPSTR(TCONST_0016), F("1"));   // "effListMain"
    embui.var_create(FPSTR(TCONST_004A), F("30")); // "mqtt_int" интервал отправки данных по MQTT в секундах (параметр в энергонезависимой памяти)

/*
    //WiFi
    embui.var_create(FPSTR(P_hostname), F(""));
    embui.var_create(FPSTR(P_APonly),  "0");     // режим AP-only (только точка доступа), не трогать
    embui.var_create(FPSTR(TCONST_0044), F(""));      // пароль внутренней точки доступа

    // параметры подключения к MQTT
    embui.var_create(FPSTR(P_m_host), F("")); // Дефолтные настройки для MQTT
    embui.var_create(FPSTR(P_m_port), F("1883"));
    embui.var_create(FPSTR(P_m_user), F(""));
    embui.var_create(FPSTR(P_m_pass), F(""));
    embui.var_create(FPSTR(TCONST_007B), embui.mc);  // m_pref == MAC по дефолту
    //embui.var_create(FPSTR(TCONST_002A), F("cfg1.json")); "fileName"
*/
#ifdef ESP_USE_BUTTON
    embui.var_create(FPSTR(TCONST_001F), "1"); // не трогать пока...
#endif
#ifdef AUX_PIN
    embui.var_create(FPSTR(TCONST_000E), "0");
#endif
    embui.var_create(FPSTR(TCONST_0035), F(""));
    embui.var_create(FPSTR(TCONST_0036), FPSTR(TCONST_007C));
    embui.var_create(FPSTR(TCONST_00C3), String(FADETOBLACKVALUE));
    embui.var_create(FPSTR(TCONST_0051), F("100"));
    embui.var_create(FPSTR(TCONST_0052), F("0"));
    embui.var_create(FPSTR(TCONST_0050), F("1"));
    embui.var_create(FPSTR(TCONST_0018), F("127"));

    // date/time related vars
/*
    embui.var_create(FPSTR(TCONST_0057), "");
    embui.var_create(FPSTR(TCONST_0058), "");
*/
    embui.var_create(FPSTR(TCONST_0054), F("0"));
    embui.var_create(FPSTR(TCONST_0055), FPSTR(TCONST_007D));

#ifdef MIC_EFFECTS
    embui.var_create(FPSTR(TCONST_0039),F("1.28"));
    embui.var_create(FPSTR(TCONST_003A),F("0.00"));
    embui.var_create(FPSTR(TCONST_003B),F("0"));
#endif

#ifdef RESTORE_STATE
    embui.var_create(FPSTR(TCONST_001B), "0");
#endif

    embui.var_create(FPSTR(TCONST_0026), 60); // Дефолтное значение, настраивается из UI
    embui.var_create(FPSTR(TCONST_00BD), String(F("85"))); // 5<<4+5, старшие и младшие 4 байта содержат 5

    embui.var_create(FPSTR(TCONST_0053), String(F("1.0")));

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

   /**
    * регистрируем статические секции для web-интерфейса с системными настройками,
    * сюда входит:
    *  - WiFi-manager
    *  - установка часового пояса/правил перехода сезонного времени
    *  - установка текущей даты/времени вручную
    *  - базовые настройки MQTT
    *  - OTA обновление прошивки и образа файловой системы
    */
    BasicUI::add_sections();

    embui.section_handle_add(FPSTR(TCONST_0099), set_sys_settings);

    embui.section_handle_add(FPSTR(TCONST_0094), set_lamp_flags);

    embui.section_handle_add(FPSTR(TCONST_007E), section_main_frame);
    embui.section_handle_add(FPSTR(TCONST_0020), show_main_flags);

    embui.section_handle_add(FPSTR(TCONST_0000), section_effects_frame);
    embui.section_handle_add(FPSTR(TCONST_0011), show_effects_param);
    embui.section_handle_add(FPSTR(TCONST_0016), set_effects_list);
    // embui.section_handle_add(FPSTR(TCONST_0012), set_effects_bright);
    // embui.section_handle_add(FPSTR(TCONST_0013), set_effects_speed);
    // embui.section_handle_add(FPSTR(TCONST_0014), set_effects_scale);
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
    embui.section_handle_add(FPSTR(TCONST_00C8), section_drawing_frame);
    embui.section_handle_add(FPSTR(TCONST_009A), section_sys_settings_frame);
    embui.section_handle_add(FPSTR(TCONST_0003), section_text_frame);
    embui.section_handle_add(FPSTR(TCONST_0034), set_lamp_textsend);
    embui.section_handle_add(FPSTR(TCONST_0030), edit_lamp_config);
    embui.section_handle_add(FPSTR(TCONST_0029), edit_lamp_config);

    embui.section_handle_add(FPSTR(TCONST_00BA), set_text_config);

//    embui.section_handle_add(FPSTR(TCONST_0004), section_settings_frame);
//    embui.section_handle_add(FPSTR(TCONST_0078), show_settings_wifi);
//    embui.section_handle_add(FPSTR(TCONST_003E), set_settings_wifi);
//    embui.section_handle_add(FPSTR(T_SET_WIFIAP), set_settings_wifiAP);

    embui.section_handle_remove(FPSTR(TCONST_0045));
    embui.section_handle_add(FPSTR(TCONST_0045), set_settings_mqtt);
    embui.section_handle_add(FPSTR(TCONST_007A), show_settings_other);
    embui.section_handle_add(FPSTR(TCONST_004B), set_settings_other);
//    embui.section_handle_add(FPSTR(TCONST_0077), show_settings_time);
    embui.section_handle_remove(FPSTR(T_SET_TIME)); // меняю обработчик :)
    embui.section_handle_add(FPSTR(T_SET_TIME), set_settings_time);

    embui.section_handle_add(FPSTR(TCONST_00CA), set_drawing);

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
    embui.section_handle_add(FPSTR(TCONST_0068), set_eventlist);
#ifdef ESP_USE_BUTTON
    embui.section_handle_add(FPSTR(TCONST_0076), show_settings_butt);
    embui.section_handle_add(FPSTR(TCONST_006E), show_butt_conf);
    embui.section_handle_add(FPSTR(TCONST_0075), set_butt_conf);
    embui.section_handle_add(FPSTR(TCONST_001F), set_btnflag);
#endif

    embui.section_handle_add(FPSTR(TCONST_00C4), set_drawflag);

#ifdef LAMP_DEBUG
    embui.section_handle_add(FPSTR(TCONST_0095), set_debugflag);
#endif

#ifdef MP3PLAYER
    embui.section_handle_add(FPSTR(TCONST_009D), set_mp3flag);
    embui.section_handle_add(FPSTR(TCONST_00A2), set_mp3volume);
    embui.section_handle_add(FPSTR(TCONST_009F), show_settings_mp3);
    embui.section_handle_add(FPSTR(TCONST_00A0), set_settings_mp3);

    embui.section_handle_add(FPSTR(TCONST_00BE), set_mp3_player);
    embui.section_handle_add(FPSTR(TCONST_00BF), set_mp3_player);
    embui.section_handle_add(FPSTR(TCONST_00C0), set_mp3_player);
    embui.section_handle_add(FPSTR(TCONST_00C1), set_mp3_player);
#endif
}

void sync_parameters(){
    DynamicJsonDocument doc(1024);
    //https://arduinojson.org/v6/api/jsondocument/
    //JsonDocument::to<T>() clears the document and converts it to the specified type. Don’t confuse this function with JsonDocument::as<T>() that returns a reference only if the requested type matches the one in the document.
    JsonObject obj = doc.to<JsonObject>();

    if(check_recovery_state(true)){
        LOG(printf_P,PSTR("Critical Error: Lamp recovered from corrupted effect number: %s\n"),String(embui.param(FPSTR(TCONST_0016))).c_str());
        embui.var(FPSTR(TCONST_0016),String(0)); // что-то пошло не так, был ребут, сбрасываем эффект
    }

    myLamp.setmqtt_int(embui.param(FPSTR(TCONST_004A)).toInt());

    String syslampFlags(embui.param(FPSTR(TCONST_0094)));
    LAMPFLAGS tmp;
    tmp.lampflags = syslampFlags.toInt(); //atol(embui.param(FPSTR(TCONST_0094)).c_str());
//#ifndef ESP32
//    LOG(printf_P, PSTR("tmp.lampflags=%u (%s)\n"), tmp.lampflags, syslampFlags.c_str());
//#endif
    LOG(printf_P, PSTR("tmp.lampflags=%u\n"), tmp.lampflags);


    obj[FPSTR(TCONST_00C4)] = tmp.isDraw ? "1" : "0";
    set_drawflag(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>(); // https://arduinojson.org/v6/how-to/reuse-a-json-document/

#ifdef LAMP_DEBUG
    obj[FPSTR(TCONST_0095)] = tmp.isDebug ? "1" : "0";
    set_debugflag(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();
#endif

    //LOG(printf_P,PSTR("tmp.isEventsHandled=%d\n"), tmp.isEventsHandled);
    obj[FPSTR(TCONST_001D)] = tmp.isEventsHandled ? "1" : "0";
    set_eventflag(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();
    embui.timeProcessor.attach_callback(std::bind(&LAMP::setIsEventsHandled, &myLamp, myLamp.IsEventsHandled())); // только после синка будет понятно включены ли события

    myLamp.setGlobalBrightness(embui.param(FPSTR(TCONST_0018)).toInt()); // починить бросок яркости в 255 при первом включении
    obj[FPSTR(TCONST_001C)] = tmp.isGlobalBrightness ? "1" : "0";
    set_gbrflag(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();

#ifdef RESTORE_STATE
    obj[FPSTR(TCONST_001A)] = tmp.ONflag ? "1" : "0";
    if(tmp.ONflag){ // если лампа включена, то устанавливаем эффект ДО включения
        CALL_SETTER(FPSTR(TCONST_0016), embui.param(FPSTR(TCONST_0016)), set_effects_list);
    }
    set_onflag(nullptr, &obj);
    if(!tmp.ONflag){ // иначе - после
        CALL_SETTER(FPSTR(TCONST_0016), embui.param(FPSTR(TCONST_0016)), set_effects_list);
    }
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();
    if(myLamp.isLampOn())
        CALL_SETTER(FPSTR(TCONST_001B), embui.param(FPSTR(TCONST_001B)), set_demoflag); // Демо через режимы, для него нужнен отдельный флаг :(
#else
    CALL_SETTER(FPSTR(TCONST_0016), embui.param(FPSTR(TCONST_0016)), set_effects_list);
#endif

    if(tmp.isGlobalBrightness)
        CALL_SETTER(String(FPSTR(TCONST_0015)) + "0", myLamp.getLampBrightness(), set_effects_dynCtrl);

#ifdef MP3PLAYER
    //obj[FPSTR(TCONST_00A2)] = embui.param(FPSTR(TCONST_00A2));  // пишет в плеер!
    obj[FPSTR(TCONST_00A3)] = tmp.playTime;
    obj[FPSTR(TCONST_00A4)] = tmp.playName ? "1" : "0";
    obj[FPSTR(TCONST_00A5)] = tmp.playEffect ? "1" : "0";
    obj[FPSTR(TCONST_00A6)] = String(tmp.alarmSound);
    obj[FPSTR(TCONST_00A7)] = String(tmp.MP3eq); // пишет в плеер!
    obj[FPSTR(TCONST_00A8)] = tmp.playMP3 ? "1" : "0";
    obj[FPSTR(TCONST_00A9)] = embui.param(FPSTR(TCONST_00A9));
    obj[FPSTR(TCONST_00AF)] = tmp.limitAlarmVolume ? "1" : "0";

    set_settings_mp3(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();

    mp3->setupplayer(myLamp.effects.getEn(), myLamp.effects.getSoundfile()); // установить начальные значения звука
    obj[FPSTR(TCONST_009D)] = tmp.isOnMP3 ? "1" : "0";
    set_mp3flag(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();

    CALL_SETTER(FPSTR(TCONST_00A2), embui.param(FPSTR(TCONST_00A2)), set_mp3volume);

#endif

#ifdef AUX_PIN
    CALL_SETTER(FPSTR(TCONST_000E), embui.param(FPSTR(TCONST_000E)), set_auxflag);
#endif

    myLamp.setClearingFlag(tmp.isEffClearing);

#ifdef ESP_USE_BUTTON
    // в отдельном классе, в список флагов лампы не входит!
    CALL_SETTER(FPSTR(TCONST_001F), embui.param(FPSTR(TCONST_001F)), set_btnflag);
#endif

    obj[FPSTR(TCONST_0051)] = embui.param(FPSTR(TCONST_0051));
    obj[FPSTR(TCONST_0052)] = embui.param(FPSTR(TCONST_0052));
    obj[FPSTR(TCONST_0054)] = embui.param(FPSTR(TCONST_0054));
    obj[FPSTR(TCONST_00C3)] = embui.param(FPSTR(TCONST_00C3));

    String datetime;
    TimeProcessor::getDateTimeString(datetime, embui.param(FPSTR(TCONST_0055)).toInt());
    obj[FPSTR(TCONST_0055)] = datetime;
    
    set_text_config(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();

    obj[FPSTR(TCONST_004E)] = tmp.isFaderON ? "1" : "0";
    obj[FPSTR(TCONST_008E)] = tmp.isEffClearing ? "1" : "0";
    obj[FPSTR(TCONST_004C)] = tmp.MIRR_H ? "1" : "0";
    obj[FPSTR(TCONST_004D)] = tmp.MIRR_V ? "1" : "0";
    obj[FPSTR(TCONST_0090)] = tmp.numInList ? "1" : "0";
    obj[FPSTR(TCONST_004F)] = tmp.dRand ? "1" : "0";
    obj[FPSTR(TCONST_009E)] = tmp.showName ? "1" : "0";
    obj[FPSTR(TCONST_0091)] = tmp.effHasMic ? "1" : "0";
    obj[FPSTR(TCONST_0096)] = tmp.isShowSysMenu ? "1" : "0";

    uint8_t alarmPT = embui.param(FPSTR(TCONST_00BD)).toInt();
    obj[FPSTR(TCONST_00BB)] = alarmPT>>4;
    obj[FPSTR(TCONST_00BC)] = alarmPT&0x0F;

    SORT_TYPE type = (SORT_TYPE)embui.param(FPSTR(TCONST_0050)).toInt();
    obj[FPSTR(TCONST_0050)] = type;

    obj[FPSTR(TCONST_0053)] = embui.param(FPSTR(TCONST_0053));

    set_settings_other(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();

#ifdef MIC_EFFECTS
    obj[FPSTR(TCONST_001E)] = tmp.isMicOn ? "1" : "0";
    myLamp.getLampState().setMicAnalyseDivider(0);
    set_micflag(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();

    // float scale = atof(embui.param(FPSTR(TCONST_0039)).c_str());
    // float noise = atof(embui.param(FPSTR(TCONST_003A)).c_str());
    // MIC_NOISE_REDUCE_LEVEL lvl=(MIC_NOISE_REDUCE_LEVEL)embui.param(FPSTR(TCONST_003B)).toInt();

    obj[FPSTR(TCONST_0039)] = embui.param(FPSTR(TCONST_0039)); //scale;
    obj[FPSTR(TCONST_003A)] = embui.param(FPSTR(TCONST_003A)); //noise;
    obj[FPSTR(TCONST_003B)] = embui.param(FPSTR(TCONST_003B)); //lvl;
    set_settings_mic(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();
#endif

    //save_lamp_flags(); // обновить состояние флагов (закомментированно, окончательно состояние установится через 0.3 секунды, после set_settings_other)

    //--------------- начальная инициализация состояния
    myLamp.getLampState().freeHeap = ESP.getFreeHeap();
#ifdef ESP8266
    FSInfo fs_info;
    LittleFS.info(fs_info);
    myLamp.getLampState().fsfreespace = fs_info.totalBytes-fs_info.usedBytes;
    myLamp.getLampState().HeapFragmentation = ESP.getHeapFragmentation();
#endif
#ifdef ESP32
    myLamp.getLampState().fsfreespace = LittleFS.totalBytes() - LittleFS.usedBytes();
    myLamp.getLampState().HeapFragmentation = 0;
#endif
    //--------------- начальная инициализация состояния

    check_recovery_state(false); // удаляем маркер, считаем что у нас все хорошо...
    LOG(println, F("sync_parameters() done"));
}

// обработка эвентов лампы
void event_worker(const EVENT *event){
    RA action = RA_UNKNOWN;
    LOG(printf_P, PSTR("%s - %s\n"), ((EVENT *)event)->getName().c_str(), embui.timeProcessor.getFormattedShortTime().c_str());

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
    case EVENT_TYPE::SET_EFFECT: action = RA_EFFECT; break;
    case EVENT_TYPE::SET_WARNING: action = RA_WARNING; break;
    default:;
    }

    remote_action(action, event->message, NULL);
}

void show_progress(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    interf->json_section_hidden(FPSTR(TCONST_005A), String(FPSTR(TINTF_056)) + String(F(" : ")) + (*data)[FPSTR(TINTF_05A)].as<String>()+ String("%"));
    interf->json_section_end();
    interf->json_frame_flush();
}

uint8_t uploadProgress(size_t len, size_t total){
    DynamicJsonDocument doc(256);
    JsonObject obj = doc.to<JsonObject>();
    static int prev = 0; // используется чтобы не выводить повторно предыдущее значение, хрен с ней, пусть живет
    float part = total / 50.0;
    int curr = len / part;
    uint8_t progress = 100*len/total;
    if (curr != prev) {
        prev = curr;
        for (int i = 0; i < curr; i++) Serial.print(F("="));
        Serial.print(F("\n"));
        obj[FPSTR(TINTF_05A)] = String(progress);
        CALL_INTF_OBJ(show_progress);
    }
#ifdef VERTGAUGE
    myLamp.GaugeShow(len, total, 100);
#endif
    return progress;
}

// Функции обработчики и другие служебные
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
    myButtons->add(new Button(true, false, 7, true, BA::BA_EFFECT, String(F("253")))); // 7 кликов - эффект часы
    myButtons->add(new Button(true, true, 0, false, BA::BA_BRIGHT)); // удержание яркость
    myButtons->add(new Button(true, true, 1, false, BA::BA_SPEED)); // удержание + 1 клик скорость
    myButtons->add(new Button(true, true, 2, false, BA::BA_SCALE)); // удержание + 2 клика масштаб
}
#endif

void remote_action(RA action, ...){
    LOG(printf_P, PSTR("RA %d: "), action);
    DynamicJsonDocument doc(512);
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
    LOG(println);

    switch (action) {
        case RA::RA_ON:
            CALL_INTF(FPSTR(TCONST_001A), "1", set_onflag);
            break;
        case RA::RA_OFF: {
                // нажатие кнопки точно отключает ДЕМО и белую лампу возвращая в нормальный режим
                LAMPMODE mode = myLamp.getMode();
                if(mode!=LAMPMODE::MODE_NORMAL){
                    CALL_INTF(FPSTR(TCONST_001B), "0", set_demoflag); // отключить демо, если было включено
                    if (myLamp.IsGlobalBrightness()) {
                        embui.var(FPSTR(TCONST_0018), String(myLamp.getLampBrightness())); // сохранить восстановленную яркость в конфиг, если она глобальная
                    }
                }
                CALL_INTF(FPSTR(TCONST_001A), "0", set_onflag);
            }
            break;
        case RA::RA_DEMO:
            CALL_INTF(FPSTR(TCONST_001A), "1", set_onflag); // включим, если было отключено
            if(value && String(value)=="0"){
                CALL_INTF(FPSTR(TCONST_001B), "0", set_demoflag);
                myLamp.startNormalMode();
            } else {
                CALL_INTF(FPSTR(TCONST_001B), "1", set_demoflag);
                resetAutoTimers();
                myLamp.startDemoMode();
            }
            break;
        case RA::RA_DEMO_NEXT:
            if (myLamp.getLampSettings().dRand) {
                myLamp.switcheffect(SW_RND, myLamp.getFaderFlag());
            } else {
                myLamp.switcheffect(SW_NEXT_DEMO, myLamp.getFaderFlag());
            }
            new Task(TASK_SECOND, TASK_ONCE, nullptr, &ts, true, nullptr, [](){ remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()).c_str(), NULL); TASK_RECYCLE; });
            break;
        case RA::RA_EFFECT: {
            LAMPMODE mode=myLamp.getMode();
            if(mode==LAMPMODE::MODE_WHITELAMP && myLamp.effects.getSelected()!=1){
                myLamp.startNormalMode(true);
                DynamicJsonDocument doc(512);
                JsonObject obj = doc.to<JsonObject>();
                CALL_INTF(FPSTR(TCONST_001A), !myLamp.isLampOn() ? "1" : "0", set_onflag);
                break;
            } else if(mode==LAMPMODE::MODE_NORMAL){
                embui.var(FPSTR(TCONST_0016), value); // сохранить в конфиг изменившийся эффект
            }
            CALL_INTF(FPSTR(TCONST_0016), value, set_effects_list); // публикация будет здесь
            break;
        }
        case RA::RA_GLOBAL_BRIGHT:
            CALL_INTF(FPSTR(TCONST_001C), value, set_gbrflag);
            break;
        case RA::RA_BRIGHT_NF:
            obj[String(FPSTR(TCONST_0015)) + "0"] = value;
            obj[FPSTR(TCONST_0017)] = true;
            obj[FPSTR(TCONST_00D5)] = true;
            //CALL_INTF_OBJ(set_effects_dynCtrl);
            set_effects_dynCtrl(nullptr, &obj);
            break;
        case RA::RA_BRIGHT:
            obj[String(FPSTR(TCONST_0015)) + "0"] = value;
            obj[FPSTR(TCONST_00D5)] = true;
            //CALL_INTF_OBJ(set_effects_dynCtrl);
            set_effects_dynCtrl(nullptr, &obj);
            break;
        case RA::RA_SPEED:
            obj[String(FPSTR(TCONST_0015)) + "1"] = value;
            obj[FPSTR(TCONST_00D5)] = true;
            //CALL_INTF_OBJ(set_effects_dynCtrl);
            set_effects_dynCtrl(nullptr, &obj);
            break;
        case RA::RA_SCALE:
            obj[String(FPSTR(TCONST_0015)) + "2"] = value;
            obj[FPSTR(TCONST_00D5)] = true;
            //CALL_INTF_OBJ(set_effects_dynCtrl);
            set_effects_dynCtrl(nullptr, &obj);
            break;
        case RA::RA_EXTRA:
            //CALL_INTF_OBJ(set_effects_dynCtrl);
            set_effects_dynCtrl(nullptr, &obj);
            break;
#ifdef MIC_EFFECTS
        case RA::RA_MIC:
            CALL_INTF_OBJ(show_settings_mic);
            break;
#endif
        case RA::RA_EFF_NEXT:
            resetAutoTimers(); // сборс таймера демо, если есть перемещение
            myLamp.switcheffect(SW_NEXT, myLamp.getFaderFlag());
            return remote_action(RA::RA_EFFECT, String(myLamp.effects.getSelected()).c_str(), NULL);
        case RA::RA_EFF_PREV:
            resetAutoTimers(); // сборс таймера демо, если есть перемещение
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
            myLamp.startAlarm(value);
            break;
        case RA::RA_ALARM_OFF:
            myLamp.stopAlarm();
            break;
        case RA::RA_REBOOT:
            remote_action(RA::RA_WARNING, F("[16711680,3000,500]"), NULL);
            new Task(3 * TASK_SECOND, TASK_ONCE, nullptr, &ts, true, nullptr,  [](){ ESP.restart(); });
            break;
        case RA::RA_WIFI_REC:
            CALL_INTF(FPSTR(TINTF_028), FPSTR(TCONST_0080), BasicUI::set_settings_wifi);
            break;
        case RA::RA_LAMP_CONFIG:
            if (value && *value) {
                String filename = String(FPSTR(TCONST_0031));
                filename.concat(value);
                embui.load(filename.c_str());
                sync_parameters();
            }
            break;
        case RA::RA_EFF_CONFIG:
            if (value && *value) {
                String filename = String(FPSTR(TCONST_002C));
                filename.concat(value);
                myLamp.effects.initDefault(filename.c_str());
            }
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
        case RA::RA_WARNING: {
            String str=value;
            DynamicJsonDocument doc(256);
            deserializeJson(doc,str);
            JsonArray arr = doc.as<JsonArray>();
            uint32_t col=CRGB::Red, dur=1000, per=250, type=0;

            for (size_t i = 0; i < arr.size(); i++) {
                switch(i){
                    case 0: {
                        String tmpStr = arr[i];
                        tmpStr.replace(F("#"), F("0x"));
                        long val = strtol(tmpStr.c_str(), NULL, 0);
                        col = val;
                        break;
                    }
                    case 1: dur = arr[i]; break;
                    case 2: per = arr[i]; break;
                    case 3: type = arr[i]; break;
                    default : break;
                }
			}
            myLamp.showWarning(col,dur,per,type);
            break; 
        }

        case RA::RA_DRAW: {
            String str=value;
            DynamicJsonDocument doc(256);
            deserializeJson(doc,str);
            JsonArray arr = doc.as<JsonArray>();
            CRGB col=CRGB::White;
            uint16_t x=WIDTH/2U, y=HEIGHT/2U;

            for (size_t i = 0; i < arr.size(); i++) {
                switch(i){
                    case 0: {
                        String tmpStr = arr[i];
                        tmpStr.replace(F("#"), F("0x"));
                        unsigned long val = strtol(tmpStr.c_str(), NULL, 0);
                        LOG(printf_P, PSTR("%s:%ld\n"), tmpStr.c_str(), val);
                        col = val;
                        break;
                    }
                    case 1: x = arr[i]; break;
                    case 2: y = arr[i]; break;
                    default : break;
                }
			}
            myLamp.writeDrawBuf(col,x,y);
            break; 
        }

        case RA::RA_FILLMATRIX: {
            String tmpStr = value;
            tmpStr.replace(F("#"), F("0x"));
            long val = strtol(tmpStr.c_str(), NULL, 0);
            LOG(printf_P, PSTR("%s:%ld\n"), tmpStr.c_str(), val);
            CRGB color=CRGB(val);
            myLamp.fillDrawBuf(color);
            break; 
        }

        case RA::RA_SEND_IP:
            myLamp.sendString(WiFi.localIP().toString().c_str(), CRGB::White);
            break;
        case RA::RA_SEND_TIME:
            myLamp.periodicTimeHandle(true);
            //myLamp.sendString(String(F("%TM")).c_str(), CRGB::Green);
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
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();
}

String httpCallback(const String &param, const String &value, bool isset){
    String result = F("Ok");
    RA action = RA_UNKNOWN;
    LOG(printf_P, PSTR("HTTP: %s - %s\n"), param.c_str(), value.c_str());

    if(!isset) {
        LOG(println, F("GET"));
        if (param == FPSTR(TCONST_0070))
            { result = myLamp.isLampOn() ? "1" : "0"; }
        else if (param == FPSTR(TCONST_0081))
            { result = !myLamp.isLampOn() ? "1" : "0"; }
        else if (param == FPSTR(TCONST_00B4))
            { result = myLamp.IsGlobalBrightness() ? "1" : "0"; }
        else if (param == FPSTR(TCONST_00AA))
            { result = myLamp.getMode() == LAMPMODE::MODE_DEMO ? "1" : "0"; }
        // else if (param == FPSTR(TCONST_0012))
        //     { result = String(myLamp.getLampBrightness()); }
        // else if (param == FPSTR(TCONST_0013))
        //     { result = myLamp.effects.getControls()[1]->getVal(); }
        // else if (param == FPSTR(TCONST_0014))
        //     { result = myLamp.effects.getControls()[2]->getVal(); }
        else if (param == FPSTR(TCONST_0082))
            { result = String(myLamp.effects.getCurrent());  }
        else if (param == FPSTR(TCONST_00B7))
            { myLamp.showWarning(CRGB::Orange,5000,500); }
        else if (param == FPSTR(TCONST_00AE)) {
                String result = myLamp.effects.geteffconfig(myLamp.effects.getCurrent(), myLamp.getNormalizedLampBrightness());
                embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00AE), result, true);
                return result;
            }
        else if (param == FPSTR(TCONST_00D0)) {
            LList<UIControl*>&controls = myLamp.effects.getControls();
            for(int i=0; i<controls.size();i++){
                if(value == String(controls[i]->getId())){
                    result = String(F("[")) + controls[i]->getId() + String(F(",\"")) + (controls[i]->getId()==0 ? String(myLamp.getNormalizedLampBrightness()) : controls[i]->getVal()) + String(F("\"]"));
                    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00D0), result, true);
                    return result;
                }
            }
        }
        // что-то случилось с сокетом... уйдем на другой эффект, отсрочим сохранение, удалим конфиг эффекта, вернемся
        else if (param == F("sys_WS_EVT_ERROR"))  {
            resetAutoTimers();
            uint16_t effNum = myLamp.effects.getSelected();
            myLamp.effects.directMoveBy(EFF_NONE);
            myLamp.effects.removeConfig(effNum);
            myLamp.effects.directMoveBy(effNum);
            //remote_action(RA_EFFECT, String(effNum).c_str(), NULL);
            String tmpStr=F("- ");
            tmpStr+=effNum;
            myLamp.sendString(tmpStr.c_str(), CRGB::Red);
            return result;
        }
        // реализация AUTODISCOVERY
        else if (param == F("sys_AUTODISCOVERY"))  {
            DynamicJsonDocument hass_discover(1024);
            String name = embui.param(FPSTR(P_hostname));
            String unique_id = embui.mc;

            hass_discover[F("~")] = embui.id("embui/"); // embui.param(FPSTR(TCONST_007B)) + F("/embui/") //String(F("homeassistant/light/"))+name;
            hass_discover[F("name")] = name;                // name
            hass_discover[F("uniq_id")] = unique_id;        // String(ESP.getChipId(), HEX); // unique_id
            hass_discover[F("avty_t")] = F("~pub/online");  // availability_topic
            hass_discover[F("pl_avail")] = F("true");       // payload_available
            hass_discover[F("pl_not_avail")] = F("false");  // payload_not_available

            hass_discover[F("cmd_t")] = F("~set/on");             // command_topic
            hass_discover[F("stat_t")] = F("~pub/on");            // state_topic

            hass_discover[F("bri_cmd_t")] = F("~set/bright");     // brightness_command_topic
            hass_discover[F("bri_stat_t")] = F("~pub/bright");    // brightness_state_topic
            hass_discover[F("bri_scl")] = 255;

            hass_discover[F("clr_temp_cmd_t")] = F("~set/speed");     // speed as color temperature
            hass_discover[F("clr_temp_stat_t")] = F("~pub/speed");    // speed as color temperature
            hass_discover[F("min_mireds")] = 1;
            hass_discover[F("max_mireds")] = 255;

            hass_discover[F("whit_val_cmd_t")] = F("~set/scale");     // scale as white level
            hass_discover[F("whit_val_stat_t")] = F("~pub/scale");    // scale as white level
            hass_discover[F("whit_val_scl")] = 255;

            hass_discover[F("fx_cmd_t")] = F("~set/effect");                                   // effect_command_topic
            hass_discover[F("fx_stat_t")] = F("~pub/eff_config");                              // effect_state_topic
            hass_discover[F("fx_tpl")] = F("{{ value_json.nb + ':' + value_json.name }}");     // effect_template

            hass_discover[F("json_attr_t")] = F("~pub/eff_config");                            // json_attributes_topic

            // hass_discover[F("rgb_cmd_t")] = "~rgb/set";       // rgb_command_topic
            // hass_discover[F("rgb_stat_t")] = "~rgb/status";   // rgb_state_topic

            String hass_discover_str;
            serializeJson(hass_discover, hass_discover_str);
            hass_discover.clear();

            embui.publishto(String(F("homeassistant/light/")) + name + F("/config"), hass_discover_str, true);
            return hass_discover_str;
        }
        else if (param == F("list"))  {
            result = F("[");
            bool first=true;
            EffectListElem *eff = nullptr;
            String effname((char *)0);
            while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
                result = result + String(first ? F("") : F(",")) + eff->eff_nb;
                first=false;
            }
            result = result + F("]");
        }
        else if (param == F("showlist"))  {
            result = F("[");
            bool first=true;
            EffectListElem *eff = nullptr;
            String effname((char *)0);
            while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
                if (eff->canBeSelected()) {
                    result = result + String(first ? F("") : F(",")) + eff->eff_nb;
                    first=false;
                }
            }
            result = result + F("]");
        }
        else if (param == F("demolist"))  {
            result = F("[");
            bool first=true;
            EffectListElem *eff = nullptr;
            String effname((char *)0);
            while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
                if (eff->isFavorite()) {
                    result = result + String(first ? F("") : F(",")) + eff->eff_nb;
                    first=false;
                }
            }
            result = result + F("]");
        }
        else if (param == F("effname"))  {
            String effname((char *)0);
            uint16_t effnum=String(value).toInt();
            myLamp.effects.loadeffname(effname, effnum);
            result = String(F("["))+effnum+String(",\"")+effname+String("\"]");
        }
        else if (param == F("effoname"))  {
            String effname((char *)0);
            uint16_t effnum=String(value).toInt();
            effname = FPSTR(T_EFFNAMEID[(uint8_t)effnum]);
            result = String(F("["))+effnum+String(",\"")+effname+String("\"]");
        }
        else if (param == FPSTR(TCONST_0083)) { action = RA_EFF_NEXT;  remote_action(action, value.c_str(), NULL); }
        else if (param == FPSTR(TCONST_0084)) { action = RA_EFF_PREV;  remote_action(action, value.c_str(), NULL); }
        else if (param == FPSTR(TCONST_0085)) { action = RA_EFF_RAND;  remote_action(action, value.c_str(), NULL); }
        else if (param == FPSTR(TCONST_0086)) { action = RA_REBOOT;  remote_action(action, value.c_str(), NULL); }
        else if (param == FPSTR(TCONST_0087)) { result = myLamp.isAlarm() ? "1" : "0"; }
        else if (param == FPSTR(TCONST_00C6)) { char buf[32]; sprintf_P(buf, PSTR("[%d,%d]"), WIDTH, HEIGHT);  result = buf; }
        
        embui.publish(String(FPSTR(TCONST_008B)) + param, result, true);
        return result;
    } else {
        LOG(println, F("SET"));
        if (param == FPSTR(TCONST_0070)) { action = (value!="0" ? RA_ON : RA_OFF); }
        else if (param == FPSTR(TCONST_0081)) { action = (value!="0" ? RA_OFF : RA_ON); }
        else if (param == FPSTR(TCONST_00AA)) action = RA_DEMO;
        else if (param == FPSTR(TCONST_0035)) action = RA_SEND_TEXT;
        // else if (param == FPSTR(TCONST_0012)) action = RA_BRIGHT;
        // else if (param == FPSTR(TCONST_0013)) action = RA_SPEED;
        // else if (param == FPSTR(TCONST_0014)) action = RA_SCALE;
        else if (param == FPSTR(TCONST_0082)) action = RA_EFFECT;
        else if (param == FPSTR(TCONST_0083)) action = RA_EFF_NEXT;
        else if (param == FPSTR(TCONST_0084)) action = RA_EFF_PREV;
        else if (param == FPSTR(TCONST_0085)) action = RA_EFF_RAND;
        else if (param == FPSTR(TCONST_0086)) action = RA_REBOOT;
        else if (param == FPSTR(TCONST_0087)) action = RA_ALARM;
        else if (param == FPSTR(TCONST_00B4)) action = RA_GLOBAL_BRIGHT;
        else if (param == FPSTR(TCONST_00B7)) action = RA_WARNING;
        else if (param == FPSTR(TCONST_00C5)) action = RA_DRAW;
        else if (param == FPSTR(TCONST_00C7)) action = RA_FILLMATRIX;
        //else if (param.startsWith(FPSTR(TCONST_0015))) { action = RA_EXTRA; remote_action(action, param.c_str(), value.c_str(), NULL); return result; }
        else if (param == FPSTR(TCONST_00AE)) {
            return httpCallback(param, "", false); // set пока не реализована
        }
        else if (param == FPSTR(TCONST_00D0) || param == FPSTR(TCONST_00D4)) {
            String str=value;
            DynamicJsonDocument doc(256);
            deserializeJson(doc,str);
            JsonArray arr = doc.as<JsonArray>();
            uint16_t id=0;
            String val="";

            if(arr.size()<2){ // мало параметров, т.е. это GET команда, возвращаем состояние контрола
                return httpCallback(FPSTR(TCONST_00D0), value, false);
            }

            if(param == FPSTR(TCONST_00D4)){ // это команда увеличения контрола на значение, соотвественно получаем текущее
                val = arr[1].as<String>().toInt();
                str = httpCallback(FPSTR(TCONST_00D0), arr[0], false);
                doc.clear(); doc.garbageCollect();
                deserializeJson(doc,str);
                arr = doc.as<JsonArray>();
                arr[1] = arr[1].as<String>().toInt()+val.toInt();
            }

            for (size_t i = 0; i < arr.size(); i++) {
                switch(i){
                    case 0: {
                        id = arr[i].as<uint16_t>();
                        break;
                    }
                    case 1: val = arr[i].as<String>(); break;
                    default : break;
                }
			}
            remote_action(RA_EXTRA, (String(FPSTR(TCONST_0015))+id).c_str(), val.c_str(), NULL);
            //result = String(F("[")) + String(id) + String(F(",\"")) + val + String(F("\"]"));
            //embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00D0), result, true);

            return httpCallback(FPSTR(TCONST_00D0), String(id), false); // т.к. отложенный вызов, то иначе обрабатыаем
        }
        else if (param == F("effname"))  {
            String effname((char *)0);
            uint16_t effnum=String(value).toInt();
            myLamp.effects.loadeffname(effname, effnum);
            result = String(F("["))+effnum+String(",\"")+effname+String("\"]");
            embui.publish(String(FPSTR(TCONST_008B)) + param, result, true);
            return result;
        }
        else if (param == F("effoname"))  {
            String effname((char *)0);
            uint16_t effnum=String(value).toInt();
            effname = FPSTR(T_EFFNAMEID[(uint8_t)effnum]);
            result = String(F("["))+effnum+String(",\"")+effname+String("\"]");
            embui.publish(String(FPSTR(TCONST_008B)) + param, result, true);
            return result;
        }
#ifdef OTA
        else if (param == FPSTR(TCONST_0027)) action = RA_OTA;
#endif
#ifdef AUX_PIN
        else if (param == FPSTR(TCONST_0088)) action = RA_AUX_ON;
        else if (param == FPSTR(TCONST_0089))  action = RA_AUX_OFF;
        else if (param == FPSTR(TCONST_008A))  action = RA_AUX_TOGLE;
#endif
        remote_action(action, value.c_str(), NULL);
    }
    return result;
}
