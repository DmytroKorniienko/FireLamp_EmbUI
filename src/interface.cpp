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

(Цей файл є частиною FireLamp_JeeUI.

   FireLamp_JeeUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_JeeUI поширюється в надії, що вона буде корисною,
   але БЕЗ ВСЯКИХ ГАРАНТІЙ; навіть без неявної гарантії ТОВАРНОГО ВИГЛЯДУ
   або ПРИДАТНОСТІ ДЛЯ ВИЗНАЧЕНИХ ЦІЛЕЙ. Докладніше див. у Стандартній
   громадська ліцензія GNU.

   Ви повинні були отримати копію Стандартної громадської ліцензії GNU
   разом із цією програмою. Якщо це не так, див.
   <https://www.gnu.org/licenses/>.)
*/

#include "main.h"
#include "interface.h"
#include "effects.h"
#include "ui.h"
#include "extra_tasks.h"
#include "events.h"
#ifdef TM1637_CLOCK
    #include "tm.h"				// Подключаем функции
    #ifdef DS18B20
        #include "DS18B20.h"				// Подключаем функции
    #endif
#endif
#ifdef ENCODER
    #include "enc.h"
#endif
#ifdef RTC
    #include "rtc.h"
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
Task *optionsTask = nullptr;       // задача для отложенной генерации списка
Task *delayedOptionTask = nullptr; // текущая отложенная задача, для сброса при повторных входах
CtrlsTask *ctrlsTask = nullptr;    // планировщик контролов
StringTask *pubEffTask = nullptr;  // отложенная публикация эффекта

static EffectListElem *confEff = nullptr; // эффект, который сейчас конфигурируется на странице "Управление списком эффектов"
static DEV_EVENT *cur_edit_event = NULL; // текущее редактируемое событие, сбрасывается после сохранения
// ------------- глобальные переменные построения интерфейса
} // namespace INTERFACE
using namespace INTERFACE;

// функция пересоздания/отмены генерации списка эффектов
void recreateoptionsTask(bool isCancelOnly=false){
    if(optionsTask)
        optionsTask->cancel();
    if(delayedOptionTask)
        delayedOptionTask->cancel(); // отмена предыдущей задачи, если была запущена
    if(!isCancelOnly){
        embui.autosave();
        optionsTask = new Task(INDEX_BUILD_DELAY * TASK_SECOND, TASK_ONCE, delayedcall_show_effects, &ts, false, nullptr, [](){
            TASK_RECYCLE;
            optionsTask=nullptr;
        });
        optionsTask->enableDelayed();
    }
}

// Функция преобразования для конфига
uint64_t stoull(const String &str){
    uint64_t tmp = 0;
    LOG(printf_P, PSTR("STOULL %s \n"), str.c_str());
    for (uint8_t i = 0; i < str.length(); i++){
        if (i)
            tmp *= 10;
        tmp += (int)str[i] - 48;
    }
    return tmp;
}
// Функция преобразования для конфига
String ulltos(uint64_t longlong){
    String bfr;
    while (longlong){
        int8_t i = longlong % 10;
        bfr += String(i);
        longlong /= 10;
    }
    String tmp;
    for (int i = bfr.length()-1; i >= 0; i--)
        tmp += (String)bfr[i];
    return tmp;
}

bool check_recovery_state(bool isSet){
    //return false; // оключено до выяснения... какого-то хрена не работает :(
#ifndef ESP8266
    static RTC_DATA_ATTR uint32_t chk;
#else
    uint32_t chk;
    ESP.rtcUserMemoryRead(0,&chk,sizeof(chk));
#endif
    if(isSet && (chk&0xFF00)==0xDB00){
        uint16_t data = chk&0x00FF;
        data++;
        chk=0xDB00|data;
        LOG(printf_P, PSTR("Reboot count=%d\n"), data);
#ifdef ESP8266
        ESP.rtcUserMemoryWrite(0, &chk, sizeof(chk));
#endif
        if(data>3)
            return true; // все плохо, три перезагрузки...
        else
            return false; // все хорошо
    } else {
        chk=0xDB00; // сбрасываем цикл перезагрузок
#ifdef ESP8266
        ESP.rtcUserMemoryWrite(0, &chk, sizeof(chk));
#endif
    }
    return false;
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
 * @brief - callback function that is triggered every EMBUI_PUB_PERIOD seconds via EmbUI scheduler
 * used to publish periodic updates to websocket clients, if any
 * 
 */
void pubCallback(Interface *interf){
    LOG(println, F("pubCallback :"));
    if (!interf) return;
    interf->json_frame_value();
    interf->value(FPSTR(TCONST_0001), embui.timeProcessor.getFormattedShortTime(), true);

#if !defined(ESP32) || !defined(BOARD_HAS_PSRAM)    
    #ifdef PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED
        uint32_t iram;
        uint32_t dram;
        {
            HeapSelectIram ephemeral;
            iram = ESP.getFreeHeap();
        }
        {
            HeapSelectDram ephemeral;
            dram = ESP.getFreeHeap();
        }
        interf->value(FPSTR(TCONST_0002), String(dram)+" / "+String(iram), true);
    #else
        interf->value(FPSTR(TCONST_0002), String(myLamp.getLampState().freeHeap), true);
    #endif
#else
    if(psramFound()){
        interf->value(FPSTR(TCONST_0002), String(ESP.getFreeHeap())+" / "+String(ESP.getFreePsram()), true);
        LOG(printf_P, PSTR("Free PSRAM: %d\n"), ESP.getFreePsram());
    } else {
        interf->value(FPSTR(TCONST_0002), String(myLamp.getLampState().freeHeap), true);
    }
#endif
    char fuptime[16];
    uint32_t tm = embui.getUptime();
    sprintf_P(fuptime, PSTR("%u.%02u:%02u:%02u"),tm/86400,(tm/3600)%24,(tm/60)%60,tm%60);
    interf->value(FPSTR(TCONST_008F), String(fuptime), true);
    interf->value(FPSTR(TCONST_00C2), String(myLamp.getLampState().fsfreespace), true);
#ifdef DS18B20
    interf->value(FPSTR(TCONST_003E), String(getTemp())+F("°C"), true);
#endif
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
#ifdef USE_STREAMING
    interf->option(FPSTR(TCONST_0044), FPSTR(TINTF_0E2));   //  Трансляция
#endif
    interf->option(FPSTR(TCONST_005C), FPSTR(TINTF_011));   //  События
    interf->option(FPSTR(TCONST_0004), FPSTR(TINTF_002));   //  настройки

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
    interf->checkbox(FPSTR(TCONST_0090), myLamp.getLampSettings().numInList ? "1" : "0", FPSTR(TINTF_090), false); // нумерация в списке эффектов
#ifdef MIC_EFFECTS
    interf->checkbox(FPSTR(TCONST_0091), myLamp.getLampSettings().effHasMic ? "1" : "0", FPSTR(TINTF_091), false); // значек микрофона в списке эффектов
#endif

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
    recreateoptionsTask(true); // only cancel task
    EffectListElem *effect = confEff;
    
    bool isNumInList =  (*data)[FPSTR(TCONST_0090)] == "1";
#ifdef MIC_EFFECTS
    bool isEffHasMic =  (*data)[FPSTR(TCONST_0091)] == "1";
#endif
    SORT_TYPE st = (*data)[FPSTR(TCONST_0050)].as<SORT_TYPE>();

    if(myLamp.getLampState().isInitCompleted){
        LOG(printf_P, PSTR("Settings: call removeLists()\n"));
        bool isRecreate = false;
        isRecreate = myLamp.getLampSettings().numInList!=isNumInList;
#ifdef MIC_EFFECTS
        isRecreate = (myLamp.getLampSettings().effHasMic!=isEffHasMic) || isRecreate;
#endif
        isRecreate = (myLamp.effects.getEffSortType()!=st) || isRecreate;

        if(isRecreate){
            myLamp.effects.setEffSortType(st);
            myLamp.setNumInList(isNumInList);
#ifdef MIC_EFFECTS
            myLamp.setEffHasMic(isEffHasMic);
#endif
            myLamp.effects.removeLists();
            recreateoptionsTask();
        }
    }
    myLamp.setNumInList(isNumInList);
#ifdef MIC_EFFECTS
    myLamp.setEffHasMic(isEffHasMic);
#endif
    SETPARAM(FPSTR(TCONST_0050), myLamp.effects.setEffSortType(st));
    save_lamp_flags();
    
    String act = (*data)[FPSTR(TCONST_0005)];
    if (act == FPSTR(TCONST_0009)) {
        Task *_t = new Task(
            300,
            TASK_ONCE, [effect](){
                                myLamp.effects.copyEffect(effect); // копируем текущий
                                myLamp.effects.makeIndexFileFromList(); // создаем индекс по списку и на выход
                                Interface *interf = embui.ws.count()? new Interface(&embui, &embui.ws, 1024) : nullptr;
                                section_main_frame(interf, nullptr);
                                delete interf;
                                recreateoptionsTask();
                                TASK_RECYCLE; },
            &ts, false);
        _t->enableDelayed();
        return;
    //} else if (act == FPSTR(TCONST_000A)) {
    } else if (act == FPSTR(TCONST_00B0) || act == FPSTR(TCONST_00B1)) {
        uint16_t tmpEffnb = effect->eff_nb;
        bool isCfgRemove = (act == FPSTR(TCONST_00B1));
        LOG(printf_P,PSTR("confEff->eff_nb=%d\n"), tmpEffnb);
        if(tmpEffnb==myLamp.effects.getCurrent()){
            myLamp.effects.directMoveBy(EFF_ENUM::EFF_NONE);
            remote_action(RA_EFF_NEXT, NULL);
        }
        String tmpStr=F("- ");
        tmpStr+=String(tmpEffnb);
        myLamp.sendString(tmpStr.c_str(), CRGB::Red);
        confEff = myLamp.effects.getEffect(EFF_ENUM::EFF_NONE);
        if(isCfgRemove){
            Task *_t = new Task(
                300,
                TASK_ONCE, [effect](){
                                    myLamp.effects.deleteEffect(effect, true); // удаляем эффект из ФС
                                    myLamp.effects.makeIndexFileFromFS(); // создаем индекс по файлам ФС и на выход
                                    Interface *interf = embui.ws.count()? new Interface(&embui, &embui.ws, 1024) : nullptr;
                                    section_main_frame(interf, nullptr);
                                    delete interf;
                                    recreateoptionsTask();
                                    TASK_RECYCLE; },
                &ts, false);
            _t->enableDelayed();
        } else {
            Task *_t = new Task(
                300,
                TASK_ONCE, [effect](){
                                    myLamp.effects.deleteEffect(effect, false); // удаляем эффект из списка
                                    myLamp.effects.makeIndexFileFromList(); // создаем индекс по текущему списку и на выход
                                    Interface *interf = embui.ws.count()? new Interface(&embui, &embui.ws, 1024) : nullptr;
                                    section_main_frame(interf, nullptr);
                                    delete interf;
                                    recreateoptionsTask();
                                    TASK_RECYCLE; },
                &ts, false);
            _t->enableDelayed();
        }
        return;
    } else if (act == FPSTR(TCONST_000B)) {
        Task *_t = new Task(
            300,
            TASK_ONCE, [](){
                                myLamp.effects.makeIndexFileFromFS(); // создаем индекс по файлам ФС и на выход
                                Interface *interf = embui.ws.count()? new Interface(&embui, &embui.ws, 1024) : nullptr;
                                section_main_frame(interf, nullptr);
                                delete interf;
                                recreateoptionsTask();
                                TASK_RECYCLE; },
            &ts, false);
        _t->enableDelayed();
        return;
    } else {
        effect->canBeSelected((*data)[FPSTR(TCONST_0006)] == "1");
        effect->isFavorite((*data)[FPSTR(TCONST_0007)] == "1");
        myLamp.effects.setSoundfile((*data)[FPSTR(TCONST_00AB)], effect);
// #ifdef CASHED_EFFECTS_NAMES
//         effect->setName((*data)[FPSTR(TCONST_0092)]);
// #endif
        myLamp.effects.setEffectName((*data)[FPSTR(TCONST_0092)], effect);
    }

    resetAutoTimers();
    myLamp.effects.makeIndexFileFromList(); // обновить индексный файл после возможных изменений
    section_main_frame(interf, nullptr);
}

void block_effects_config(Interface *interf, JsonObject *data, bool fast=true){
    if (!interf) return;

    interf->json_section_main(FPSTR(TCONST_000F), FPSTR(TINTF_009));
    confEff = myLamp.effects.getSelectedListElement();
    //interf->select(FPSTR(TCONST_0010), String((int)confEff->eff_nb), String(FPSTR(TINTF_00A)), true);

    bool firsttime = false;
    File *fquiklist = nullptr;
    if(!LittleFS.exists(FPSTR(TCONST_0082))){
        fquiklist = new fs::File;
        *fquiklist = LittleFS.open(FPSTR(TCONST_0081), "w");
        fquiklist->print('[');
        firsttime = true;
        fast = true;
    } else {
        // формируем и отправляем кадр с запросом подгрузки внешнего ресурса
        interf->json_frame_custom(FPSTR(T_XLOAD));
        interf->json_section_content();
        interf->select(FPSTR(TCONST_0010), String((int)confEff->eff_nb), String(FPSTR(TINTF_00A)), true, false, String(LittleFS.exists(FPSTR(TCONST_0083)) ? FPSTR(TCONST_0083) : FPSTR(TCONST_0082))+'?'+myLamp.effects.getlistsuffix());
        interf->json_section_end();
        block_effects_config_param(interf, nullptr);
        interf->spacer();
        interf->button(FPSTR(TCONST_0000), FPSTR(TINTF_00B));
        interf->json_section_end();
        return;
    }

    uint32_t timest = millis();
    if(fast){
        // Сначала подгрузим дефолтный список, а затем спустя время - подтянем имена из конфига

        //interf->option(String(myLamp.effects.getSelected()), myLamp.effects.getEffectName());
        String effname((char *)0);
        EffectListElem *eff = nullptr;
        MIC_SYMB;
        while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
            effname = String(eff->eff_nb) + (eff->eff_nb>255 ? String(F(" (")) + String(eff->eff_nb&0xFF) + String(F(")")) : String("")) + String(F(". ")) + String(FPSTR(T_EFFNAMEID[(uint8_t)eff->eff_nb])) + MIC_SYMBOL;
            if(fquiklist){
                fquiklist->printf_P(PSTR("%s{\"label\":\"%s\",\"value\":\"%s\"}"), firsttime?"":",", effname.c_str(), String(eff->eff_nb).c_str());
                firsttime = false;
            }
            //interf->option(String(eff->eff_nb), effname);
            #ifdef ESP8266
            ESP.wdtFeed();
            #elif defined ESP32
            delay(1);
            #endif
        }
        //interf->option(String(0),"");
        if(fquiklist){
            fquiklist->print(']');
            fquiklist->close();
            LittleFS.rename(FPSTR(TCONST_0081),FPSTR(TCONST_0082));
            delete (fs::FS *)fquiklist;
        }
    } else {
        EffectListElem *eff = nullptr;
        LOG(println,F("DBG1: using slow Names generation"));
        String effname((char *)0);
        MIC_SYMB;
        while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
            myLamp.effects.loadeffname(effname, eff->eff_nb);
            effname = String(eff->eff_nb) + (eff->eff_nb>255 ? String(F(" (")) + String(eff->eff_nb&0xFF) + String(F(")")) : String("")) + String(F(". ")) + String(FPSTR(T_EFFNAMEID[(uint8_t)eff->eff_nb])) + MIC_SYMBOL;
            if(fquiklist){
                fquiklist->printf_P(PSTR("%s{\"label\":\"%s\",\"value\":\"%s\"}"), firsttime?"":",", effname.c_str(), String(eff->eff_nb).c_str());
                firsttime = false;
            }
            //interf->option(String(eff->eff_nb), effname);
            #ifdef ESP8266
            ESP.wdtFeed();
            #elif defined ESP32
            delay(1);
            #endif
        }
        if(fquiklist){
            fquiklist->print(']');
            fquiklist->close();
            LittleFS.rename(FPSTR(TCONST_0081),FPSTR(TCONST_0082));
            delete (fs::FS *)fquiklist;
        }
    }
    //interf->option(String(0),"");
    //interf->json_section_end();
    LOG(printf_P,PSTR("DBG1: generating Names list took %ld ms\n"), millis() - timest);
    // формируем и отправляем кадр с запросом подгрузки внешнего ресурса
    interf->json_frame_custom(FPSTR(T_XLOAD));
    interf->json_section_content();
    interf->select(FPSTR(TCONST_0010), String((int)confEff->eff_nb), String(FPSTR(TINTF_00A)), true, false, String(FPSTR(TCONST_0082))+'?'+myLamp.effects.getlistsuffix());
    interf->json_section_end();
    block_effects_config_param(interf, nullptr);

    interf->spacer();
    interf->button(FPSTR(TCONST_0000), FPSTR(TINTF_00B));
    interf->json_section_end();
}

// Построение выпадающего списка эффектов для вебморды
void delayedcall_show_effects(){

    LOG(println, F("=== GENERATE EffLIst for GUI ==="));
    uint16_t effnb = confEff?(int)confEff->eff_nb:myLamp.effects.getSelected(); // если confEff не NULL, то мы в конфирурировании, иначе в основном режиме

    if(delayedOptionTask)
        delayedOptionTask->cancel(); // отмена предыдущей задачи, если была запущена

    EffectListElem **peff = new (EffectListElem *); // выделяем память под укзатель на указатель
    *peff = nullptr; // чистим содержимое
    File *slowlist = nullptr;
    if(!LittleFS.exists(confEff?FPSTR(TCONST_0083):FPSTR(TCONST_0084))){
        slowlist = new fs::File;
        *slowlist = LittleFS.open(FPSTR(TCONST_0085), "w");
    } else {
        // формируем и отправляем кадр с запросом подгрузки внешнего ресурса
        Interface *interf = embui.ws.count()? new Interface(&embui, &embui.ws, 512) : nullptr;
        interf->json_frame_custom(FPSTR(T_XLOAD));
        interf->json_section_content();
        interf->select(confEff?FPSTR(TCONST_0010):FPSTR(TCONST_0016), String(effnb), String(FPSTR(TINTF_00A)), true, true, String(confEff?FPSTR(TCONST_0083):FPSTR(TCONST_0084))+'?'+myLamp.effects.getlistsuffix());
        interf->json_section_end();
        interf->json_frame_flush();
        delete interf;
        interf = nullptr;
        return;
    }
    //LOG(print,(uint32_t)peff); LOG(print," "); LOG(println,(uint32_t)*peff);

    delayedOptionTask = new Task(300, TASK_FOREVER,
        // loop
        [peff, slowlist](){
            EffectListElem *&eff = *peff; // здесь ссылка на указатель, т.к. нам нужно менять значение :)
            //LOG(print,(uint32_t)peff); LOG(print," "); LOG(println,(uint32_t)*peff);
            bool firsttime = false;
            if(eff == nullptr && slowlist){
                slowlist->print('[');
                firsttime = true;
            }

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
                        String name =                             (!confEff ? EFF_NUMBER : String(eff->eff_nb) + (eff->eff_nb>255 ? String(F(" (")) + String(eff->eff_nb&0xFF) + String(F(")")) : String("")) + String(F(". "))) +
                            effname +
                            MIC_SYMBOL;
                        if(slowlist){
                            slowlist->printf_P(PSTR("%s{\"label\":\"%s\",\"value\":\"%s\"}"), firsttime?"":",", name.c_str(), String(eff->eff_nb).c_str());
                            firsttime = false;
                        }
                    }
                } else {
                    // тут перебрали все элементы и готовы к завершению
                    EffectListElem * first_eff=myLamp.effects.getFirstEffect();
                    if(!confEff && first_eff && !first_eff->canBeSelected()){ // если мы не в конфигурировании эффектов и первый не может быть выбран, то пустой будет добавлен в конец
                        if(slowlist){
                            slowlist->printf_P(PSTR(",{\"label\":\"\",\"value\":\"0\"}"));
                        }
                    }
                    if(slowlist){
                        slowlist->print(']');
                        slowlist->close();
#ifdef ESP32
                        delay(500);
#endif
                        LittleFS.rename(FPSTR(TCONST_0085),confEff?FPSTR(TCONST_0083):FPSTR(TCONST_0084));
                        delete (fs::FS *)slowlist;
                    }

                    Task *_t = &ts.currentTask();
                    _t->disable();
                    return;
                }
            }
        },
        &ts, true,
        nullptr,
        //onDisable
        [peff](){
            LOG(println, F("=== GENERATE EffLIst for GUI completed ==="));
            // формируем и отправляем кадр с запросом подгрузки внешнего ресурса
            Interface *interf = embui.ws.count()? new Interface(&embui, &embui.ws, 512) : nullptr;
            uint16_t effnb = confEff?(int)confEff->eff_nb:myLamp.effects.getSelected(); // если confEff не NULL, то мы в конфирурировании, иначе в основном режиме
            interf->json_frame_custom(FPSTR(T_XLOAD));
            interf->json_section_content();
            interf->select(confEff?FPSTR(TCONST_0010):FPSTR(TCONST_0016), String(effnb), String(FPSTR(TINTF_00A)), true, true, String(confEff?FPSTR(TCONST_0083):FPSTR(TCONST_0084))+'?'+myLamp.effects.getlistsuffix());
            interf->json_section_end();
            interf->json_frame_flush();
            delete interf;
            delete peff; // освободить указатель на указатель
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
#else
    if (!interf) return;
    interf->json_frame_interface();
    block_effects_config(interf, data, false);
    interf->json_frame_flush();
#endif
    if(!LittleFS.exists(FPSTR(TCONST_0083)))
        recreateoptionsTask();
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

#ifdef EMBUI_USE_MQTT
void publish_ctrls_vals()
{
  embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00AE), myLamp.effects.geteffconfig(String(myLamp.effects.getSelected()).toInt(), myLamp.getNormalizedLampBrightness()), true);
}
#endif

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
                if(!isMicOn && (!myLamp.isMicOnOff() || !(controls[i]->getId()==7 && controls[i]->getName().startsWith(FPSTR(TINTF_020))==1) )) continue;
#else
                continue;
#endif          
                break;
            case CONTROL_CASE::ISMICOFF :
#ifdef MIC_EFFECTS
                //if(myLamp.isMicOnOff()) continue;
                if(isMicOn && (myLamp.isMicOnOff() || !(controls[i]->getId()==7 && controls[i]->getName().startsWith(FPSTR(TINTF_020))==1) )) continue;
#else
                continue;
#endif   
                break;
            default: break;
        }
        bool isRandDemo = (myLamp.getLampSettings().dRand && myLamp.getMode()==LAMPMODE::MODE_DEMO);
        String ctrlId = String(FPSTR(TCONST_0015)) + String(controls[i]->getId());
        String ctrlName = i ? controls[i]->getName() : (myLamp.IsGlobalBrightness() ? FPSTR(TINTF_00C) : FPSTR(TINTF_00D));
        switch(ctrlCaseType&0x0F){
            case CONTROL_TYPE::RANGE :
                {
                    if(isRandDemo && controls[i]->getId()>0 && !(controls[i]->getId()==7 && controls[i]->getName().startsWith(FPSTR(TINTF_020))==1))
                        ctrlName=String(FPSTR(TINTF_0C9))+ctrlName;
                    int value = i ? controls[i]->getVal().toInt() : myLamp.getNormalizedLampBrightness();
                    if(isinterf) interf->range(
                        ctrlId
                        ,String(value)
                        ,controls[i]->getMin()
                        ,controls[i]->getMax()
                        ,controls[i]->getStep()
                        , ctrlName
                        , true);
#ifdef EMBUI_USE_MQTT
                    embui.publish(String(FPSTR(TCONST_008B)) + ctrlId, String(value), true);
#endif
                }
                break;
            case CONTROL_TYPE::EDIT :
                {
                    String ctrlName = controls[i]->getName();
                    if(isRandDemo && controls[i]->getId()>0 && !(controls[i]->getId()==7 && controls[i]->getName().startsWith(FPSTR(TINTF_020))==1))
                        ctrlName=String(FPSTR(TINTF_0C9))+ctrlName;
                    
                    if(isinterf) interf->text(String(FPSTR(TCONST_0015)) + String(controls[i]->getId())
                    , controls[i]->getVal()
                    , ctrlName
                    , true
                    );
#ifdef EMBUI_USE_MQTT
                    embui.publish(String(FPSTR(TCONST_008B)) + ctrlId, controls[i]->getVal(), true);
#endif
                    break;
                }
            case CONTROL_TYPE::CHECKBOX :
                {
                    String ctrlName = controls[i]->getName();
                    if(isRandDemo && controls[i]->getId()>0 && !(controls[i]->getId()==7 && controls[i]->getName().startsWith(FPSTR(TINTF_020))==1))
                        ctrlName=String(FPSTR(TINTF_0C9))+ctrlName;

                    if(isinterf) interf->checkbox(String(FPSTR(TCONST_0015)) + String(controls[i]->getId())
                    , controls[i]->getVal()
                    , ctrlName
                    , true
                    );
#ifdef EMBUI_USE_MQTT
                    embui.publish(String(FPSTR(TCONST_008B)) + ctrlId, controls[i]->getVal(), true);
#endif
                    break;
                }
            default:
#ifdef EMBUI_USE_MQTT
                    embui.publish(String(FPSTR(TCONST_008B)) + ctrlId, controls[i]->getVal(), true);
#endif
                break;
        }
    }
#ifdef EMBUI_USE_MQTT
    publish_ctrls_vals();
#endif
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
    LOG(printf_P, PSTR("EFF LIST num:%d, eff_nb:%d, o:%d, on:%d, md:%d\n"), num, eff->eff_nb, curr, myLamp.isLampOn(), myLamp.getMode());
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

    LOG(printf_P, PSTR("interf: %p\n"), interf);
    show_effects_param(interf, data);
#ifdef EMBUI_USE_MQTT
    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(CMD_TCONST_0009), String(eff->eff_nb), true);
    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00AE), myLamp.effects.geteffconfig(String(eff->eff_nb).toInt(), myLamp.getNormalizedLampBrightness()), true); // publish_ctrls_vals
#endif
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

    // static unsigned long timeout = 0;
    // if(timeout+110UL>millis()) return;
    // timeout = millis();

    // попытка повышения стабильности, отдаем управление браузеру как можно быстрее...
    if((*data).containsKey(FPSTR(TCONST_00D5)))
        direct_set_effects_dynCtrl(data);

    if(ctrlsTask){
        if(ctrlsTask->replaceIfSame(data)){
            ctrlsTask->restartDelayed();
            return;
        }
    }
   
    //LOG(println, "Delaying dynctrl");
    //String tmp; serializeJson(*data,tmp); LOG(println, tmp);
    ctrlsTask = new CtrlsTask(data, 300, TASK_ONCE,
        [](){
            CtrlsTask *task = (CtrlsTask *)ts.getCurrentTask();
            JsonObject storage = task->getData();
            JsonObject *data = &storage; // task->getData();
            if(!data) return;

            LOG(println, "publishing & sending dynctrl...");
            String tmp; serializeJson(*data,tmp);LOG(println, tmp);

            direct_set_effects_dynCtrl(data);
#ifdef EMBUI_USE_MQTT
            publish_ctrls_vals();
            for (JsonPair kv : *data){
                embui.publish(String(FPSTR(TCONST_008B)) + String(kv.key().c_str()), kv.value().as<String>(), true);
            }
#endif
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
            if(task==ctrlsTask)
                ctrlsTask = nullptr;
            //TASK_RECYCLE;
            delete task;
        },
        &ts,
        false
    );
    ctrlsTask->restartDelayed();
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
    interf->checkbox(FPSTR(TCONST_001B), String(myLamp.getMode() == LAMPMODE::MODE_DEMO), FPSTR(TINTF_00F), true);
    interf->checkbox(FPSTR(TCONST_001C), String(myLamp.IsGlobalBrightness()), FPSTR(TINTF_010), true);
#ifndef MOOT
    interf->checkbox(FPSTR(TCONST_001D), String(myLamp.IsEventsHandled()), FPSTR(TINTF_011), true);
    interf->checkbox(FPSTR(TCONST_00C4), String(myLamp.isDrawOn()), FPSTR(TINTF_0CE), true);
#endif
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
        interf->button(FPSTR(CMD_TCONST_0006), FPSTR(TINTF_0BD), FPSTR(P_GRAY));
        interf->button(FPSTR(CMD_TCONST_0007), FPSTR(TINTF_0BE), FPSTR(P_GRAY));
        interf->button(FPSTR(TCONST_00C0), FPSTR(TINTF_0BF), FPSTR(P_GRAY));
        interf->button(FPSTR(TCONST_00C1), FPSTR(TINTF_0C0), FPSTR(P_GRAY));
    }
    //interf->button("time", FPSTR(TINTF_016), FPSTR(TCONST_0025));    
    interf->json_section_end();
    interf->range(String(FPSTR(TCONST_00A2)), String(1), String(30), String(1), String(FPSTR(TINTF_09B)), true);
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

    bool firsttime = false;
    File *quicklist = nullptr;
    if(!LittleFS.exists(FPSTR(TCONST_0086))){
        quicklist = new fs::File;
        *quicklist = LittleFS.open(FPSTR(TCONST_0081), "w");
        quicklist->print('[');
        firsttime = true;
        fast=true;
    } else {
        // формируем и отправляем кадр с запросом подгрузки внешнего ресурса
        bool isSlowExist = LittleFS.exists(FPSTR(TCONST_0084));
        interf->json_frame_custom(FPSTR(T_XLOAD));
        interf->json_section_content();
        interf->select(FPSTR(TCONST_0016), String(myLamp.effects.getSelected()), String(FPSTR(TINTF_00A)), true, false, String(isSlowExist ? FPSTR(TCONST_0084) : FPSTR(TCONST_0086))+'?'+myLamp.effects.getlistsuffix());
        interf->json_section_end();
        block_effects_param(interf, data);
        interf->button(FPSTR(TCONST_000F), FPSTR(TINTF_009));
        interf->json_section_end();
        if(!isSlowExist)
            recreateoptionsTask();
        return;
    }

    //interf->select(FPSTR(TCONST_0016), String(myLamp.effects.getSelected()), String(FPSTR(TINTF_00A)), true);
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
                effname = EFF_NUMBER + FPSTR(T_EFFNAMEID[(uint8_t)eff->eff_nb]) + MIC_SYMBOL;
                if(quicklist){
                    quicklist->printf_P(PSTR("%s{\"label\":\"%s\",\"value\":\"%s\"}"), firsttime?"":",", effname.c_str(), String(eff->eff_nb).c_str());
                    firsttime = false;
                }
                //interf->option(String(eff->eff_nb), effname);
                #ifdef ESP8266
                ESP.wdtFeed();
                #elif defined ESP32
                delay(1);
                #endif
            } else if(!eff->eff_nb){
                isEmptyHidden=true;
            }
        }
        if(isEmptyHidden){
            //interf->option(String(0),"");
            if(quicklist){
                quicklist->printf_P(PSTR(",{\"label\":\"\",\"value\":\"0\"}"));
            }
        }
        if(quicklist){
            quicklist->print(']');
            quicklist->close();
            LittleFS.rename(FPSTR(TCONST_0081),FPSTR(TCONST_0086));
            delete (fs::FS *)quicklist;
        }
    } else {
        LOG(println,F("DBG2: using slow Names generation"));
        bool isEmptyHidden=false;
        String effname((char *)0);
        MIC_SYMB;
        bool numList = myLamp.getLampSettings().numInList;
        while ((eff = myLamp.effects.getNextEffect(eff)) != nullptr) {
            if (eff->canBeSelected()) {
                myLamp.effects.loadeffname(effname, eff->eff_nb);
                effname = EFF_NUMBER + effname + MIC_SYMBOL;
                if(quicklist){
                    quicklist->printf_P(PSTR("%s{\"label\":\"%s\",\"value\":\"%s\"}"), firsttime?"":",", effname.c_str(), String(eff->eff_nb).c_str());
                    firsttime = false;
                }
                //interf->option(String(eff->eff_nb), effname);
                #ifdef ESP8266
                ESP.wdtFeed();
                #elif defined ESP32
                yield();
                #endif
            } else if(!eff->eff_nb){
                isEmptyHidden=true;
            }
        }
        if(isEmptyHidden){
            //interf->option(String(0),"");
            if(quicklist){
                quicklist->printf_P(PSTR(",{\"label\":\"\",\"value\":\"0\"}"));
            }
        }
        if(quicklist){
            quicklist->print(']');
            quicklist->close();
            LittleFS.rename(FPSTR(TCONST_0081),FPSTR(TCONST_0086));
            delete (fs::FS *)quicklist;
        }
    }
    interf->json_frame_custom(FPSTR(T_XLOAD));
    interf->json_section_content();
    interf->select(FPSTR(TCONST_0016), String(myLamp.effects.getSelected()), String(FPSTR(TINTF_00A)), true, false, String(FPSTR(TCONST_0086))+'?'+myLamp.effects.getlistsuffix());
    interf->json_section_end();
    LOG(printf_P,PSTR("DBG2: generating Names list took %ld ms\n"), millis() - timest);

    block_effects_param(interf, data);

    interf->button(FPSTR(TCONST_000F), FPSTR(TINTF_009));

    interf->json_section_end();
#ifdef DELAYED_EFFECTS
    recreateoptionsTask();
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
    bool ra_call = (*data).containsKey(FPSTR(TCONST_00D4));

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
#if !defined(ESP_USE_BUTTON) && !defined(ENCODER)
            if(millis()<20000){        // 10 секунд мало, как показала практика, ставим 20
                Task *_t = new Task(
                    INDEX_BUILD_DELAY * TASK_SECOND,
                    TASK_ONCE, [](){ myLamp.sendString(WiFi.localIP().toString().c_str(), CRGB::White); TASK_RECYCLE; },
                    &ts, false);
                _t->enableDelayed();
            }
#endif
#ifdef EMBUI_USE_MQTT
            embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_0070), "1", true);
#endif
        } else {
            resetAutoTimers(); // автосохранение конфига будет отсчитываться от этого момента
            if(!ra_call)
                myLamp.startNormalMode();
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
                #ifdef EMBUI_USE_MQTT
                                embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_0070), "0", true);
                #endif
                                TASK_RECYCLE; },
                                &ts, false);
            _t->enableDelayed();
        }
    }
#ifdef EMBUI_USE_MQTT
    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_0021), String(myLamp.getMode()), true);
    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00AA), String(myLamp.getMode()==LAMPMODE::MODE_DEMO?"1":"0"), true);
#endif
#ifdef RESTORE_STATE
    save_lamp_flags();
#endif
}

void set_demoflag(Interface *interf, JsonObject *data){
    if (!data) return;
    resetAutoTimers();
    bool newdemo = TOGLE_STATE((*data)[FPSTR(TCONST_001B)], (myLamp.getMode() == LAMPMODE::MODE_DEMO));
    switch (myLamp.getMode()) {
        case LAMPMODE::MODE_OTA:
        case LAMPMODE::MODE_ALARMCLOCK:
        case LAMPMODE::MODE_NORMAL:
        case LAMPMODE::MODE_RGBLAMP:
            if(newdemo)
                myLamp.startDemoMode(embui.param(FPSTR(TCONST_0026)).toInt());
            break;
        case LAMPMODE::MODE_DEMO:
        case LAMPMODE::MODE_WHITELAMP:
            if(!newdemo)
                myLamp.startNormalMode();
            break;
        default:;
    }
#ifdef RESTORE_STATE
    embui.var(FPSTR(TCONST_001B), (*data)[FPSTR(TCONST_001B)]);
#endif
    myLamp.setDRand(myLamp.getLampSettings().dRand);
#ifdef EMBUI_USE_MQTT
    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_0021), String(myLamp.getMode()), true);
    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00AA), String(myLamp.getMode()==LAMPMODE::MODE_DEMO?"1":"0"), true);
#endif
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
#ifdef EMBUI_USE_MQTT
    embui.publish(String(FPSTR(TCONST_008B)) + String(FPSTR(TCONST_00B4)), String(myLamp.IsGlobalBrightness() ? "1" : "0"), true);
#endif
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

        Task *_t = new Task(3*TASK_SECOND, TASK_ONCE, [](){ myLamp.effects.makeIndexFileFromFS(); sync_parameters(); TASK_RECYCLE; }, &ts, false);
        _t->enableDelayed();

    } else { // создание
        if(!name.endsWith(F(".json"))){
            name.concat(F(".json"));
        }

        String filename = String(FPSTR(TCONST_0031)) + name;
        embui.save(filename.c_str(), true);

        filename = String(FPSTR(TCONST_002C)) + name;
        myLamp.effects.makeIndexFileFromList(filename.c_str(), false);

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
                interf->range(FPSTR(TCONST_0051), String(110U-embui.param((FPSTR(TCONST_0051))).toInt()), String(10), String(100), String(5), String(FPSTR(TINTF_044)));
                interf->range(FPSTR(TCONST_0052), String(-1), String((int)(HEIGHT>6?HEIGHT:6)-6), String(1), FPSTR(TINTF_045));
                interf->range(FPSTR(TCONST_00C3), String(0), String(255), String(1), FPSTR(TINTF_0CA));
                
            interf->spacer(FPSTR(TINTF_04E));
                interf->number(FPSTR(TCONST_0054), FPSTR(TINTF_04F));
                //interf->number(FPSTR(TCONST_0055), FPSTR(TINTF_050));
                String datetime;
                TimeProcessor::getDateTimeString(datetime, embui.param(FPSTR(TCONST_0055)).toInt());
                interf->datetime(FPSTR(TCONST_0055), datetime, FPSTR(TINTF_050), false);
                //interf->text(FPSTR(TCONST_0055), datetime, FPSTR(TINTF_050), false);
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
    recreateoptionsTask(true); // only cancel task
    interf->json_section_main(FPSTR(TCONST_00C8), FPSTR(TINTF_0CE));

    DynamicJsonDocument doc(512);
    JsonObject param = doc.to<JsonObject>();

    param[FPSTR(TCONST_00CD)] = WIDTH;
    param[FPSTR(TCONST_00CC)] = HEIGHT;
    param[FPSTR(TCONST_00CB)] = FPSTR(TINTF_0CF);
    param[FPSTR(TCONST_00DC)] = FPSTR(TINTF_0D9);


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
void set_clear(Interface *interf, JsonObject *data){
    if (!data) return;
    remote_action(RA_FILLMATRIX, "#000000", NULL);
}

void block_lamptext(Interface *interf, JsonObject *data){
    //Страница "Вывод текста"
    if (!interf) return;
    recreateoptionsTask(true); // only cancel task
    interf->json_section_main(FPSTR(TCONST_0003), FPSTR(TINTF_001));

    block_lamp_textsend(interf, data);

    interf->json_section_end();
}

void set_text_config(Interface *interf, JsonObject *data){
    if (!data) return;
    (*data)[FPSTR(TCONST_0051)]=String(110U-(*data)[FPSTR(TCONST_0051)].as<int>());
    SETPARAM(FPSTR(TCONST_0051), myLamp.setTextMovingSpeed((*data)[FPSTR(TCONST_0051)].as<int>()));
    SETPARAM(FPSTR(TCONST_0052), myLamp.setTextOffset((*data)[FPSTR(TCONST_0052)]));
    SETPARAM(FPSTR(TCONST_0054), myLamp.setNYMessageTimer((*data)[FPSTR(TCONST_0054)]));
    SETPARAM(FPSTR(TCONST_00C3), myLamp.setBFade((*data)[FPSTR(TCONST_00C3)]));

    String newYearTime = (*data)[FPSTR(TCONST_0055)]; // Дата/время наструпления нового года с интерфейса
    struct tm t;
    tm *tm=&t;
    //localtime_r(TimeProcessor::now(), tm);  // reset struct to local now()
    gmtime_r(TimeProcessor::now(), tm);

    // set desired date
    tm->tm_year = newYearTime.substring(0,4).toInt()-EMBUI_TM_BASE_YEAR;
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
    interf->range(FPSTR(TCONST_00A2), String(1), String(30), String(1), FPSTR(TINTF_09B), true);
    
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
    
    interf->number(String(FPSTR(TCONST_00A9)), String(mp3->getMP3count()), String(FPSTR(TINTF_0B0)));
    //SETPARAM(FPSTR(TCONST_00A9), mp3->setMP3count((*data)[FPSTR(TCONST_00A9)].as<int>())); // кол-во файлов в папке мп3

    interf->button_submit(FPSTR(TCONST_00A0), FPSTR(TINTF_008), FPSTR(P_GRAY));
    interf->json_section_end();

    interf->spacer();
    interf->button(FPSTR(TCONST_0004), FPSTR(TINTF_00B));

    interf->json_section_end();
}

void show_settings_mp3(Interface *interf, JsonObject *data){
    if (!interf || !mp3->isReady()) return;
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
    //BasicUI::section_settings_frame(interf, data);
    section_settings_frame(interf, data);
}
#endif

#ifdef MIC_EFFECTS
void block_settings_mic(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_0037), FPSTR(TINTF_020));

    interf->checkbox(FPSTR(TCONST_001E), myLamp.isMicOnOff()? "1" : "0", FPSTR(TINTF_012), true);

    interf->json_section_begin(FPSTR(TCONST_0038));
    if (!myLamp.isMicCalibration()) {
        interf->number(String(FPSTR(TCONST_0039)), String((float)(round(myLamp.getLampState().getMicScale() * 100) / 100)), String(FPSTR(TINTF_022)), String(0.01), String(0), String(2));
        interf->number(String(FPSTR(TCONST_003A)), String((float)(round(myLamp.getLampState().getMicNoise() * 100) / 100)), String(FPSTR(TINTF_023)), String(0.01), String(0), String(32));
        interf->range(String(FPSTR(TCONST_003B)), String((int)myLamp.getLampState().getMicNoiseRdcLevel()), String(0), String(4), String(1), String(FPSTR(TINTF_024)), false);

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

    //BasicUI::section_settings_frame(interf, data);
    section_settings_frame(interf, data);
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
        myLamp.sendString(String(FPSTR(TINTF_026)).c_str(), CRGB::Red);
    } else if(!myLamp.isMicCalibration()) {
        myLamp.sendString(String(FPSTR(TINTF_025)).c_str(), CRGB::Red);
        myLamp.setMicCalibration();
    } else {
        myLamp.sendString(String(FPSTR(TINTF_027)).c_str(), CRGB::Red);
    }

    show_settings_mic(interf, data);
}
#endif

// после завершения сканирования обновляем список WiFi
void scan_complete(int n){
    Interface *interf = EmbUI::GetInstance()->ws.count()? new Interface(EmbUI::GetInstance(), &EmbUI::GetInstance()->ws) : nullptr;
    LOG(printf_P, PSTR("UI WiFi: Scan complete %d networks found\n"), n);
    if(interf){
        interf->json_frame_interface();
        interf->json_section_line(FPSTR(T_LOAD_WIFI));
        String ssid = WiFi.SSID();
        interf->select_edit(FPSTR(P_WCSSID), ssid, String(FPSTR(TINTF_02C)));
        for (int i = 0; i < WiFi.scanComplete(); i++) {
            interf->option(WiFi.SSID(i), WiFi.SSID(i));
            LOG(printf_P, PSTR("UI WiFi: WiFi Net %s\n"), WiFi.SSID(i).c_str());
        }
        if(ssid.isEmpty())
            interf->option("", ""); // at the end of list
        interf->json_section_end();
        interf->button(FPSTR(T_SET_SCAN), FPSTR(TINTF_0DA), FPSTR(P_GREEN), 22);
        interf->json_section_end();
        interf->json_frame_flush();

        delete interf;
    }
    Task *_t = new Task(
        TASK_SECOND,
        TASK_ONCE, [](){
            if (WiFi.scanComplete() >= 0) {
                EmbUI::GetInstance()->sysData.isWiFiScanning = false;
                WiFi.scanDelete();
                LOG(printf_P, PSTR("UI WiFi: Scan List deleted\n"));
            }
        },
        &ts, false);
    _t->enableDelayed();
}

void set_scan_wifi(Interface *interf, JsonObject *data){
    if (!interf) return;

    if (WiFi.scanComplete() == -2) {
        LOG(printf_P, PSTR("UI WiFi: WiFi scan starting\n"));
        interf->json_frame_custom(FPSTR(T_XLOAD));
        interf->json_section_content();
        interf->constant(FPSTR(T_SET_SCAN), FPSTR(TINTF_0DA), true, FPSTR(P_GREEN), 22);
        interf->json_section_end();
        interf->json_frame_flush();

        Task *t = new Task(300, TASK_ONCE, nullptr, &ts, false, nullptr, [](){
            EmbUI::GetInstance()->sysData.isWiFiScanning = true;
            #ifdef ESP8266
            WiFi.scanNetworksAsync(scan_complete);     // Сканируем с коллбеком, по завершению скана запустится scan_complete()
            #endif
            #ifdef ESP32
            EmbUI::GetInstance()->setWiFiScanCB(&scan_complete);
            WiFi.scanNetworks(true);         // У ESP нет метода с коллбеком, поэтому просто сканируем
            #endif
            TASK_RECYCLE;
        });
        t->enableDelayed();
    }
};

// Блок настроек WiFi
void block_only_wifi(Interface *interf, JsonObject *data) {
    interf->spacer(FPSTR(TINTF_031));
    interf->select(String(FPSTR(P_WIFIMODE)), embui.param(FPSTR(P_WIFIMODE)), String(FPSTR(TINTF_033)));
        interf->option("0", FPSTR(TINTF_029));
        interf->option("1", FPSTR(TINTF_02F));
        interf->option("2", FPSTR(TINTF_046));
    interf->json_section_end();

    interf->comment(FPSTR(TINTF_032));

    interf->text(FPSTR(P_hostname), FPSTR(TINTF_02B));
    interf->password(FPSTR(P_APpwd), FPSTR(TINTF_034));

    interf->spacer(FPSTR(TINTF_02A));
    interf->json_section_line(FPSTR(T_LOAD_WIFI));
    interf->select_edit(FPSTR(P_WCSSID), String(WiFi.SSID()), String(FPSTR(TINTF_02C)));
    interf->json_section_end();
    interf->button(FPSTR(T_SET_SCAN), FPSTR(TINTF_0DA), FPSTR(P_GREEN), 22); // отступ
    interf->json_section_end();
    interf->password(FPSTR(TCONST_0041), FPSTR(TINTF_02D));
    interf->button_submit(FPSTR(T_SET_WIFI), FPSTR(TINTF_02E), FPSTR(P_GRAY));
}

// формирование интерфейса настроек WiFi/MQTT
void block_settings_wifi(Interface *interf, JsonObject *data){
    if (!interf) return;

    interf->json_section_main(FPSTR(TCONST_003D), FPSTR(TINTF_081));

    // форма настроек Wi-Fi
    interf->json_section_hidden(FPSTR(T_SET_WIFI), FPSTR(TINTF_028));
        block_only_wifi(interf, data);
    interf->json_section_end();

#ifdef EMBUI_USE_MQTT
    // форма настроек MQTT
    interf->json_section_hidden(FPSTR(TCONST_0045), FPSTR(TINTF_035));
    interf->text(FPSTR(P_m_host), FPSTR(TINTF_036));
    interf->number(FPSTR(P_m_port), FPSTR(TINTF_037));
    interf->text(FPSTR(P_m_user), FPSTR(TINTF_038));
    interf->password(FPSTR(P_m_pass), FPSTR(TINTF_02D));
    interf->text(FPSTR(P_m_pref), FPSTR(TINTF_08C));
    interf->number(FPSTR(P_m_tupd), FPSTR(TINTF_039));
    interf->button_submit(FPSTR(TCONST_0045), FPSTR(TINTF_03A), FPSTR(P_GRAY));
    interf->json_section_end();
#endif

#ifdef EMBUI_USE_FTP
    // форма настроек FTP
    interf->json_section_hidden("H", FPSTR(TINTF_0DB));
        interf->json_section_begin("C", "");
            interf->checkbox(FPSTR(T_CHK_FTP), String(embui.cfgData.isftp), FPSTR(TINTF_0DB), true);
        interf->json_section_end();
        interf->json_section_begin(FPSTR(T_SET_FTP), "");
            interf->text(FPSTR(P_ftpuser), FPSTR(TINTF_038));
            interf->password(FPSTR(P_ftppass), FPSTR(TINTF_02D));
            interf->button_submit(FPSTR(T_SET_FTP), FPSTR(TINTF_008), FPSTR(P_GRAY));
        interf->json_section_end();
    interf->json_section_end();
#endif

    interf->spacer();
    interf->button(FPSTR(TCONST_0004), FPSTR(TINTF_00B));

    interf->json_section_end();
}

void show_settings_wifi(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_wifi(interf, data);
    interf->json_frame_flush();
    if(!EmbUI::GetInstance()->sysData.isWiFiScanning){ // автосканирование при входе в настройки
        EmbUI::GetInstance()->sysData.isWiFiScanning = true;
        set_scan_wifi(interf, data);
    }
}

// настройка подключения WiFi
void set_settings_wifi(Interface *interf, JsonObject *data){
    if (!data) return;

    BasicUI::set_settings_wifi(interf, data);
    section_settings_frame(interf, data);
}

#ifdef EMBUI_USE_MQTT
void set_settings_mqtt(Interface *interf, JsonObject *data){
    if (!data) return;
    BasicUI::set_settings_mqtt(interf,data);
    embui.mqttReconnect();
    int interval = (*data)[FPSTR(P_m_tupd)];
    LOG(print, F("New MQTT interval: ")); LOG(println, interval);
    myLamp.setmqtt_int(interval);
    section_settings_frame(interf, data);
}
#endif

#ifdef EMBUI_USE_FTP
// настройка ftp
void set_ftp(Interface *interf, JsonObject *data){
    if (!data) return;

    BasicUI::set_ftp(interf, data);
    section_settings_frame(interf, data);
}
#endif

void block_settings_other(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_004B), FPSTR(TINTF_002));
    
    interf->spacer(FPSTR(TINTF_030));
#if !defined(MATRIXx4) and !defined(XY_EXTERN)
    interf->checkbox(FPSTR(TCONST_004C), myLamp.getLampSettings().MIRR_H ? "1" : "0", FPSTR(TINTF_03B), false);
    interf->checkbox(FPSTR(TCONST_004D), myLamp.getLampSettings().MIRR_V ? "1" : "0", FPSTR(TINTF_03C), false);
#endif
    interf->checkbox(FPSTR(TCONST_004E), myLamp.getLampSettings().isFaderON ? "1" : "0", FPSTR(TINTF_03D), false);
    interf->checkbox(FPSTR(TCONST_008E), myLamp.getLampSettings().isEffClearing ? "1" : "0", FPSTR(TINTF_083), false);
    interf->checkbox(FPSTR(TCONST_004F), myLamp.getLampSettings().dRand ? "1" : "0", FPSTR(TINTF_03E), false);
    interf->checkbox(FPSTR(TCONST_009E), myLamp.getLampSettings().showName ? "1" : "0", FPSTR(TINTF_09A), false);
    interf->range(FPSTR(TCONST_0026), String(30), String(250), String(5), FPSTR(TINTF_03F));
    float sf = embui.param(FPSTR(TCONST_0053)).toFloat();
    interf->range(String(FPSTR(TCONST_0053)), String(sf), String(0.25), String(4.0), String(0.25), String(FPSTR(TINTF_0D3)), false);
#ifdef SHOWSYSCONFIG
    interf->checkbox(FPSTR(TCONST_0096), myLamp.getLampSettings().isShowSysMenu ? "1" : "0", FPSTR(TINTF_093), false); // отображение системного меню
#endif
#ifdef TM1637_CLOCK
    interf->spacer(FPSTR(TINTF_0D4));
    interf->checkbox(FPSTR(TCONST_00DA), myLamp.getLampSettings().tm24 ? String("1") : String("0"), FPSTR(TINTF_0D7), false);
    interf->checkbox(FPSTR(TCONST_00DB), myLamp.getLampSettings().tmZero ? String("1") : String("0"), FPSTR(TINTF_0D8), false);
    interf->range(FPSTR(TCONST_00D8), String(myLamp.getBrightOn()), String(0), String(7), String(1), FPSTR(TINTF_0D5), false);
    interf->range(FPSTR(TCONST_00D9), String(myLamp.getBrightOff()), String(0), String(7), String(1), FPSTR(TINTF_0D6), false);
    #ifdef DS18B20
    interf->checkbox(FPSTR(TCONST_002F), myLamp.getLampSettings().isTempOn ? String("1") : String("0"), FPSTR(TINTF_0E0), false);
    #endif
#endif
    interf->spacer(FPSTR(TINTF_0BA));
    interf->range(FPSTR(TCONST_00BB), String(myLamp.getAlarmP()), String(1), String(15), String(1), FPSTR(TINTF_0BB), false);
    interf->range(FPSTR(TCONST_00BC), String(myLamp.getAlarmT()), String(1), String(15), String(1), FPSTR(TINTF_0BC), false);

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

        SETPARAM(FPSTR(TCONST_0026), ({if (myLamp.getMode() == LAMPMODE::MODE_DEMO){ myLamp.demoTimer(T_DISABLE); myLamp.demoTimer(T_ENABLE, embui.param(FPSTR(TCONST_0026)).toInt()); }}));

        float sf = (*data)[FPSTR(TCONST_0053)];
        SETPARAM(FPSTR(TCONST_0053), myLamp.setSpeedFactor(sf));

        myLamp.setIsShowSysMenu((*data)[FPSTR(TCONST_0096)] == "1");

    #ifdef TM1637_CLOCK
        uint8_t tmBri = ((*data)[FPSTR(TCONST_00D8)]).as<uint8_t>()<<4; // старшие 4 бита
        tmBri = tmBri | ((*data)[FPSTR(TCONST_00D9)]).as<uint8_t>(); // младшие 4 бита
        embui.var(FPSTR(TCONST_00D7), String(tmBri)); myLamp.setTmBright(tmBri);
        myLamp.settm24((*data)[FPSTR(TCONST_00DA)] == "1");
        myLamp.settmZero((*data)[FPSTR(TCONST_00DB)] == "1");
        #ifdef DS18B20
        myLamp.setTempDisp((*data)[FPSTR(TCONST_002F)] == "1");
        #endif
    #endif

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

    //BasicUI::section_settings_frame(interf, data);
    if(interf)
        section_settings_frame(interf, data);
}

// страницу-форму настроек времени строим методом фреймворка (ломает переводы, возвращено обратно)
void show_settings_time(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();

    // Headline
    interf->json_section_main(FPSTR(T_SET_TIME), FPSTR(TINTF_051));

    interf->comment(FPSTR(TINTF_052));     // комментарий-описание секции

    // сперва рисуем простое поле с текущим значением правил временной зоны из конфига
    interf->text(FPSTR(P_TZSET), FPSTR(TINTF_053));

    // user-defined NTP server
    interf->text(FPSTR(P_userntp), FPSTR(TINTF_054));
    // manual date and time setup
    interf->comment(FPSTR(TINTF_055));
    interf->datetime(FPSTR(P_DTIME), "", true);
    interf->hidden(FPSTR(P_DEVICEDATETIME),""); // скрытое поле для получения времени с устройства
    interf->button_submit(FPSTR(T_SET_TIME), FPSTR(TINTF_008), FPSTR(P_GRAY));

    interf->spacer();

    // exit button
    interf->button(FPSTR(T_SETTINGS), FPSTR(TINTF_00B));

    // close and send frame
    interf->json_section_end();
    interf->json_frame_flush();

    // формируем и отправляем кадр с запросом подгрузки внешнего ресурса со списком правил временных зон
    // полученные данные заместят предыдущее поле выпадающим списком с данными о всех временных зонах
    interf->json_frame_custom(FPSTR(T_XLOAD));
    interf->json_section_content();
                    //id            val                         label   direct  skipl URL for external data
    interf->select(FPSTR(P_TZSET), embui.param(FPSTR(P_TZSET)), "",     false,  true, F("/js/tz.json"));
    interf->json_section_end();
    interf->json_frame_flush();
}

void set_settings_time(Interface *interf, JsonObject *data){
    BasicUI::set_settings_time(interf, data);
    myLamp.sendString(String(F("%TM")).c_str(), CRGB::Green);
#ifdef RTC
    rtc.updateRtcTime();
#endif
    section_settings_frame(interf, data);
}

void block_settings_update(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_hidden(FPSTR(T_DO_OTAUPD), FPSTR(TINTF_056));
#ifdef OTA
    interf->spacer(FPSTR(TINTF_057));
    if (myLamp.getMode() != LAMPMODE::MODE_OTA) {
        interf->button(FPSTR(TCONST_0027), FPSTR(TINTF_058), FPSTR(TCONST_005B));
    } else {
        interf->button(FPSTR(TCONST_0027), FPSTR(TINTF_017), FPSTR(P_GRAY));
    }
#endif
    interf->spacer(FPSTR(TINTF_059));
    interf->file(FPSTR(T_DO_OTAUPD), FPSTR(T_DO_OTAUPD), FPSTR(TINTF_05A));
    interf->button_confirm(FPSTR(T_REBOOT), FPSTR(TINTF_096), FPSTR(TINTF_0E1), !data?String(FPSTR(P_RED)):String(""));       // кнопка перехода в настройки времени
    interf->json_section_end();
}

void block_settings_event(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_005C), FPSTR(TINTF_011));

    interf->checkbox(FPSTR(TCONST_001D), myLamp.IsEventsHandled()? "1" : "0", FPSTR(TINTF_086), true);

    interf->json_section_begin(FPSTR(TCONST_005D));
    interf->select(FPSTR(TCONST_005E), String(0), String(FPSTR(TINTF_05B)), false);

    int num = 0;
    LList<DEV_EVENT *> *events= myLamp.events.getEvents();
    for(int i=0; i<events->size(); i++){
        interf->option(String(num), (*events)[i]->getName());
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
    interf->button(FPSTR(TCONST_0000), FPSTR(TINTF_00B));

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
    DEV_EVENT event;
    String act;
    if (!data) return;

    //String output;
    //serializeJson((*data), output);
    //LOG(println, output.c_str());

    if(cur_edit_event){
        myLamp.events.delEvent(*cur_edit_event);
    } else if (data->containsKey(FPSTR(TCONST_005E))) {
        int num = (*data)[FPSTR(TCONST_005E)];
        LList<DEV_EVENT *> *events = myLamp.events.getEvents();
        if(events->size()>num)
            events->remove(num);
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
    event.setEvent((EVENT_TYPE)(*data)[FPSTR(TCONST_0068)].as<long>());
    event.setRepeat((*data)[FPSTR(TCONST_0069)]);
    event.setStopat((*data)[FPSTR(TCONST_006A)]);
    String tmEvent = (*data)[FPSTR(TCONST_006B)];

    struct tm t;
    tm *tm=&t;
    localtime_r(TimeProcessor::now(), tm);  // reset struct to local now()

    // set desired date
    tm->tm_year=tmEvent.substring(0,4).toInt()-EMBUI_TM_BASE_YEAR;
    tm->tm_mon = tmEvent.substring(5,7).toInt()-1;
    tm->tm_mday=tmEvent.substring(8,10).toInt();
    tm->tm_hour=tmEvent.substring(11,13).toInt();
    tm->tm_min=tmEvent.substring(14,16).toInt();
    tm->tm_sec=0;

    time_t ut = mktime(tm);
    event.setUnixtime(ut);
    LOG(printf_P, PSTR("Set Event at %4d-%2d-%2d %2d:%2d:00 -> %lu\n"), tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour, tm->tm_min, (unsigned long)ut);

    String buf; // внешний буффер, т.к. добавление эвента ниже
    switch(event.getEvent()){
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
                event.setMessage(buf);
                myLamp.events.addEvent(event);
            }
            break;
        case EVENT_TYPE::SEND_TIME: {
                DynamicJsonDocument doc(1024);
                doc[FPSTR(TCONST_0048)] = (*data)[FPSTR(TCONST_0048)];
#ifdef MP3PLAYER
                doc[FPSTR(TCONST_0056)] = (*data)[FPSTR(TCONST_0056)];
#endif
                serializeJson(doc,buf);
                buf.replace("\"","'");
                event.setMessage(buf);
                myLamp.events.addEvent(event);
            }
            break;
        default:
            event.setMessage((*data)[FPSTR(TCONST_0035)]);
            myLamp.events.addEvent(event);
            break;
    }
    myLamp.events.saveConfig();
    cur_edit_event = NULL;
    show_settings_event(interf, data);
}

void show_event_conf(Interface *interf, JsonObject *data){
    String act;
    bool edit = false;
    int num = 0;
    if (!interf || !data) return;

    LOG(print,F("event_conf=")); LOG(println, (*data)[FPSTR(TCONST_005D)].as<String>()); //  && data->containsKey(FPSTR(TCONST_005D))

    if (data->containsKey(FPSTR(TCONST_005E))) {
        DEV_EVENT *curr = NULL;
        num = (*data)[FPSTR(TCONST_005E)];

        LList<DEV_EVENT *> *events = myLamp.events.getEvents();
        if(events->size()>num)
            curr = events->get(num);

        if (!curr) return;
        act = (*data)[FPSTR(TCONST_005D)].as<String>();
        cur_edit_event = curr;
        edit = true;
    } else if(cur_edit_event != NULL){
        if(data->containsKey(FPSTR(TCONST_0068)))
            cur_edit_event->setEvent((*data)[FPSTR(TCONST_0068)].as<EVENT_TYPE>()); // меняем тип налету
        if(myLamp.events.isEnumerated(*cur_edit_event))
            edit = true;
    } else {
        LOG(println, "Созданан пустой эвент!");
        cur_edit_event = new DEV_EVENT();
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
        interf->select(FPSTR(TCONST_0068), String(cur_edit_event->getEvent()), String(FPSTR(TINTF_05F)), true);
            interf->option(String(EVENT_TYPE::ON), FPSTR(TINTF_060));
            interf->option(String(EVENT_TYPE::OFF), FPSTR(TINTF_061));
            interf->option(String(EVENT_TYPE::DEMO), FPSTR(TINTF_062));
            interf->option(String(EVENT_TYPE::ALARM), FPSTR(TINTF_063));
            interf->option(String(EVENT_TYPE::SEND_TEXT), FPSTR(TINTF_067));
            interf->option(String(EVENT_TYPE::SEND_TIME), FPSTR(TINTF_068));
            interf->option(String(EVENT_TYPE::SET_EFFECT), FPSTR(TINTF_00A));
            interf->option(String(EVENT_TYPE::SET_WARNING), FPSTR(TINTF_0CB));

            interf->option(String(EVENT_TYPE::SET_GLOBAL_BRIGHT), FPSTR(TINTF_00C));
            interf->option(String(EVENT_TYPE::SET_WHITE_LO), FPSTR(TINTF_0EA));
            interf->option(String(EVENT_TYPE::SET_WHITE_HI), FPSTR(TINTF_0EB));

#ifndef MOOT
#ifdef AUX_PIN
            interf->option(String(EVENT_TYPE::AUX_ON), FPSTR(TINTF_06A));
            interf->option(String(EVENT_TYPE::AUX_OFF), FPSTR(TINTF_06B));
            interf->option(String(EVENT_TYPE::AUX_TOGGLE), FPSTR(TINTF_06C));
#endif
            interf->option(String(EVENT_TYPE::LAMP_CONFIG_LOAD), FPSTR(TINTF_064));
            interf->option(String(EVENT_TYPE::EFF_CONFIG_LOAD), FPSTR(TINTF_065));
#ifdef ESP_USE_BUTTON
            interf->option(String(EVENT_TYPE::BUTTONS_CONFIG_LOAD), FPSTR(TINTF_0E9));
#endif
            interf->option(String(EVENT_TYPE::EVENTS_CONFIG_LOAD), FPSTR(TINTF_066));
            interf->option(String(EVENT_TYPE::PIN_STATE), FPSTR(TINTF_069));
#endif
        interf->json_section_end();
        interf->datetime(FPSTR(TCONST_006B), cur_edit_event->getDateTime(), String(FPSTR(TINTF_06D)));
    interf->json_section_end();
    interf->json_section_line();
        interf->number(FPSTR(TCONST_0069), String(cur_edit_event->getRepeat()), FPSTR(TINTF_06E));
        interf->number(FPSTR(TCONST_006A), String(cur_edit_event->getStopat()), FPSTR(TINTF_06F));
    interf->json_section_end();

    switch(cur_edit_event->getEvent()){
        case EVENT_TYPE::ALARM: {
                DynamicJsonDocument doc(1024);
                String buf = cur_edit_event->getMessage();
                buf.replace("'","\"");
                DeserializationError err = deserializeJson(doc,buf);
                int alarmP = !err && doc.containsKey(FPSTR(TCONST_00BB)) ? doc[FPSTR(TCONST_00BB)].as<uint8_t>() : myLamp.getAlarmP();
                int alarmT = !err && doc.containsKey(FPSTR(TCONST_00BC)) ? doc[FPSTR(TCONST_00BC)].as<uint8_t>() : myLamp.getAlarmT();
                String msg = !err && doc.containsKey(FPSTR(TCONST_0035)) ? doc[FPSTR(TCONST_0035)] : cur_edit_event->getMessage();

                interf->spacer(FPSTR(TINTF_0BA));
                interf->text(FPSTR(TCONST_0035), msg, FPSTR(TINTF_070), false);
                interf->json_section_line();
                    interf->range(FPSTR(TCONST_00BB), String(alarmP), String(1), String(15), String(1), FPSTR(TINTF_0BB), false);
                    interf->range(FPSTR(TCONST_00BC), String(alarmT), String(1), String(15), String(1), FPSTR(TINTF_0BC), false);
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
        case EVENT_TYPE::SEND_TIME: {
                DynamicJsonDocument doc(1024);
                String buf = cur_edit_event->getMessage();
                buf.replace("'","\"");
                DeserializationError err = deserializeJson(doc,buf);
                String isShowOff  = !err && doc.containsKey(FPSTR(TCONST_0048)) ? doc[FPSTR(TCONST_0048)] : String("0");
                String isPlayTime = !err && doc.containsKey(FPSTR(TCONST_0056)) ? doc[FPSTR(TCONST_0056)] : String("0");
                
                //String msg = !err && doc.containsKey(FPSTR(TCONST_0035)) ? doc[FPSTR(TCONST_0035)] : cur_edit_event->getMessage();

                interf->spacer("");
                //interf->text(FPSTR(TCONST_0035), msg, FPSTR(TINTF_070), false);
                interf->json_section_line();
                    interf->checkbox(FPSTR(TCONST_0048), isShowOff, FPSTR(TINTF_0EC), false);
#ifdef MP3PLAYER
                    interf->checkbox(FPSTR(TCONST_0056), isPlayTime, FPSTR(TINTF_0ED), false);
#endif
                interf->json_section_end();
            }
            break;
        default:
            interf->text(FPSTR(TCONST_0035), cur_edit_event->getMessage(), FPSTR(TINTF_070), false);
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
    
    if(cur_edit_event && cur_edit_event->getEvent()!=(*data)[FPSTR(TCONST_0068)].as<EVENT_TYPE>()){ // только если реально поменялось, то обновляем интерфейс
        show_event_conf(interf,data);
    } else if((*data).containsKey(FPSTR(TCONST_002E))){ // эта часть срабатывает даже если нажата кнопка "обновить, следовательно ловим эту ситуацию"
        set_event_conf(interf, data); //через какую-то хитрую жопу отработает :)
    }
}
#ifdef ESP_USE_BUTTON
    void set_gaugetype(Interface *interf, JsonObject *data){
        if (!data) return;
        myLamp.setGaugeType((*data)[FPSTR(TCONST_003F)].as<GAUGETYPE>());
        save_lamp_flags();
    }
#endif

#ifdef ESP_USE_BUTTON
void block_settings_butt(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_006D), FPSTR(TINTF_013));

    interf->checkbox(FPSTR(TCONST_001F), myButtons->isButtonOn()? "1" : "0", FPSTR(TINTF_07B), true);
    interf->select(String(FPSTR(TCONST_003F)), String(myLamp.getLampSettings().GaugeType), String(FPSTR(TINTF_0DD)), true);
        interf->option(String(GAUGETYPE::GT_NONE), String(FPSTR(TINTF_0EE)));
        interf->option(String(GAUGETYPE::GT_VERT), String(FPSTR(TINTF_0EF)));
        interf->option(String(GAUGETYPE::GT_HORIZ), String(FPSTR(TINTF_0F0)));
    interf->json_section_end();
    interf->spacer();

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
    interf->number(String(FPSTR(TCONST_0073)), String(btn? btn->flags.click : 0), String(FPSTR(TINTF_07E)), String(1), String(0), String(7));
    interf->checkbox(String(FPSTR(TCONST_0072)), String((btn? btn->flags.onetime&1 : 0)? "1" : "0"), FPSTR(TINTF_07F), false);

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
    if (!data) return;
    //SETPARAM(FPSTR(TCONST_001F), myButtons->setButtonOn((*data)[FPSTR(TCONST_001F)] == "1"));
    bool isSet = (*data)[FPSTR(TCONST_001F)] == "1";
    myButtons->setButtonOn(isSet);
    myLamp.setButton(isSet);
    save_lamp_flags();
}
#endif  // BUTTON

#ifdef ENCODER
void block_settings_enc(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_section_main(FPSTR(TCONST_000C), FPSTR(TINTF_0DC));

    interf->select(String(FPSTR(TCONST_003F)), String(myLamp.getLampSettings().GaugeType), String(FPSTR(TINTF_0DD)), true);
        interf->option(String(GAUGETYPE::GT_NONE), String(FPSTR(TINTF_0EE)));
        interf->option(String(GAUGETYPE::GT_VERT), String(FPSTR(TINTF_0EF)));
        interf->option(String(GAUGETYPE::GT_HORIZ), String(FPSTR(TINTF_0F0)));
    interf->json_section_end();
    interf->color(FPSTR(TCONST_0040), FPSTR(TINTF_0DE));
    interf->spacer();

    interf->color(FPSTR(TCONST_0042), FPSTR(TINTF_0DF));
    interf->range(FPSTR(TCONST_0043), String(110U-getEncTxtDelay()), String(10), String(100), String(5), String(FPSTR(TINTF_044)), false);
    interf->button_submit(FPSTR(TCONST_000C), FPSTR(TINTF_008), FPSTR(P_GRAY));
    interf->spacer();
    interf->button(FPSTR(TCONST_0004), FPSTR(TINTF_00B));
    interf->json_section_end();
}
void show_settings_enc(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    block_settings_enc(interf, data);
    interf->json_frame_flush();
}
void set_settings_enc(Interface *interf, JsonObject *data){
    if (!data) return;

    myLamp.setGaugeType((*data)[FPSTR(TCONST_003F)].as<GAUGETYPE>());
    save_lamp_flags();
    SETPARAM(FPSTR(TCONST_0040));
    // String tmpStr = (*data)[FPSTR(TCONST_0040)];
    // tmpStr.replace(F("#"), F("0x"));
    // GAUGE::GetGaugeInstance()->setGaugeTypeColor((CRGB)strtol(tmpStr.c_str(), NULL, 0));

    SETPARAM(FPSTR(TCONST_0042));
    String tmpStr2 = (*data)[FPSTR(TCONST_0042)];
    tmpStr2.replace(F("#"), F("0x"));
    setEncTxtColor((CRGB)strtol(tmpStr2.c_str(), NULL, 0));
    (*data)[FPSTR(TCONST_0043)]=JsonUInt(110U-(*data)[FPSTR(TCONST_0043)].as<int>());
    SETPARAM(FPSTR(TCONST_0043), setEncTxtDelay((*data)[FPSTR(TCONST_0043)]))
    section_settings_frame(interf, data);
}
#endif  // ENCODER

void set_debugflag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setDebug((*data)[FPSTR(TCONST_0095)] == "1");
    save_lamp_flags();
}

void set_drawflag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setDraw((*data)[FPSTR(TCONST_00C4)] == "1");
    save_lamp_flags();
}

#ifdef MP3PLAYER
void set_mp3flag(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setONMP3((*data)[FPSTR(TCONST_009D)] == "1");
    if(myLamp.isLampOn())
        mp3->setIsOn(myLamp.isONMP3(), true); // при включенной лампе - форсировать воспроизведение
    else {
        mp3->setIsOn(myLamp.isONMP3(), false); // при выключенной - не форсировать, но произнести время, но не ранее чем через 10с после перезагрузки
        if(myLamp.isONMP3() && millis()>10000)
            if(!data->containsKey(FPSTR(TCONST_00D5)) || (data->containsKey(FPSTR(TCONST_00D5)) && (*data)[FPSTR(TCONST_00D5)] == "1")) // при наличие force="1" или без этого ключа
                mp3->playTime(embui.timeProcessor.getHours(), embui.timeProcessor.getMinutes(), (TIME_SOUND_TYPE)myLamp.getLampSettings().playTime);
    }
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
    if(data->containsKey(FPSTR(CMD_TCONST_0006))){
        mp3->playEffect(cur_palyingnb-1,"");
    } else if(data->containsKey(FPSTR(CMD_TCONST_0007))){
        mp3->playEffect(cur_palyingnb+1,"");
    } else if(data->containsKey(FPSTR(TCONST_00C0))){
        mp3->playEffect(cur_palyingnb-5,"");
    } else if(data->containsKey(FPSTR(TCONST_00C1))){
        mp3->playEffect(cur_palyingnb+5,"");
    }
}

#endif


void section_effects_frame(Interface *interf, JsonObject *data){
    recreateoptionsTask(true); // only cancel task
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
#ifdef USE_STREAMING
void block_streaming(Interface *interf, JsonObject *data){
    //Страница "Трансляция"
    interf->json_section_main(FPSTR(TCONST_0044), FPSTR(TINTF_0E2));
        interf->json_section_line();
            interf->checkbox(FPSTR(TCONST_001A), String(myLamp.isLampOn()), FPSTR(TINTF_00E), true);
            interf->checkbox(FPSTR(TCONST_0046), myLamp.isStreamOn() ? F("1") : F("0"), FPSTR(TINTF_0E2), true);
            interf->checkbox(FPSTR(TCONST_0049), myLamp.isDirect() ? F("1") : F("0"), FPSTR(TINTF_0E6), true);
            interf->checkbox(FPSTR(TCONST_004A), myLamp.isMapping() ? F("1") : F("0"), FPSTR(TINTF_0E7), true);
        interf->json_section_end();
        interf->select(FPSTR(TCONST_0047), embui.param(FPSTR(TCONST_0047)), (String)FPSTR(TINTF_0E3), true);
            interf->option(String(E131), FPSTR(TINTF_0E4));
            interf->option(String(SOUL_MATE), FPSTR(TINTF_0E5));
        interf->json_section_end();
        interf->range(FPSTR(TCONST_0012), (String)myLamp.getBrightness(), F("0"), F("255"), F("1"), (String)FPSTR(TINTF_00D), true);
        if (embui.param(FPSTR(TCONST_0047)).toInt() == E131){
            interf->range(FPSTR(TCONST_0077), embui.param(FPSTR(TCONST_0077)), F("1"), F("255"), F("1"), (String)FPSTR(TINTF_0E8), true);
            interf->comment(String(F("Universes:")) + String(ceil((float)HEIGHT / (512U / (WIDTH * 3))), 0U) + String(F(";    X:")) + String(WIDTH) + String(F(";    Y:")) + String(512U / (WIDTH * 3)));
            interf->comment(String(F("Как настроить разметку матрицы в Jinx! можно посмотреть <a href=\"https://github.com/DmytroKorniienko/FireLamp_EmbUI\">на форуме</a>")));
        }
    interf->json_section_end();
}
void section_streaming_frame(Interface *interf, JsonObject *data){
    // Трансляция
    if (!interf) return;
    interf->json_frame_interface(FPSTR(TINTF_080));
    block_streaming(interf, data);
    interf->json_frame_flush();
}

void set_streaming(Interface *interf, JsonObject *data){
    if (!data) return;
    bool flag = (*data)[FPSTR(TCONST_0046)] == "1";
    myLamp.setStream(flag);
    LOG(printf_P, PSTR("Stream set %d \n"), flag);
    if (flag) {
        STREAM_TYPE type = (STREAM_TYPE)embui.param(FPSTR(TCONST_0047)).toInt();
        if (ledStream) {
            if (ledStream->getStreamType() != type){
                Led_Stream::clearStreamObj();
            }
        }
        Led_Stream::newStreamObj(type);
    }
    else {
        Led_Stream::clearStreamObj();
    }
    save_lamp_flags();
}

void set_streaming_drirect(Interface *interf, JsonObject *data){
    if (!data) return;
    bool flag = (*data)[FPSTR(TCONST_0049)] == "1";
    myLamp.setDirect(flag);
    if (ledStream){
        if (flag) {
#ifdef EXT_STREAM_BUFFER
            myLamp.setStreamBuff(false);
#else
            myLamp.clearDrawBuf();
#endif
            myLamp.effectsTimer(T_DISABLE);
            FastLED.clear();
            FastLED.show();
        }
        else {
            myLamp.effectsTimer(T_ENABLE);
#ifdef EXT_STREAM_BUFFER
            myLamp.setStreamBuff(true);
#else
            if (!myLamp.isDrawOn())             // TODO: переделать с запоминанием старого стейта
                myLamp.setDrawBuff(true);
#endif
        }
    }
    save_lamp_flags();
}
void set_streaming_mapping(Interface *interf, JsonObject *data){
    if (!data) return;
    myLamp.setMapping((*data)[FPSTR(TCONST_004A)] == "1");
    save_lamp_flags();
}
void set_streaming_bright(Interface *interf, JsonObject *data){
    if (!data) return;
    remote_action(RA_CONTROL, (String(FPSTR(TCONST_0015))+F("0")).c_str(), String((*data)[FPSTR(TCONST_0012)].as<String>()).c_str(), NULL);
}

void set_streaming_type(Interface *interf, JsonObject *data){
    if (!data) return;
    SETPARAM(FPSTR(TCONST_0047));
    STREAM_TYPE type = (STREAM_TYPE)(*data)[FPSTR(TCONST_0047)].as<int>();
    LOG(printf_P, PSTR("Stream Type %d \n"), type);
    if (myLamp.isStreamOn()) {
        if (ledStream) {
            if (ledStream->getStreamType() == type)
                return;
            Led_Stream::clearStreamObj();
        }
        Led_Stream::newStreamObj(type);
    }
    section_streaming_frame(interf, data);
}

void set_streaming_universe(Interface *interf, JsonObject *data){
    if (!data) return;
    SETPARAM(FPSTR(TCONST_0077));
    if (ledStream) {
        if (ledStream->getStreamType() == E131) {
            Led_Stream::newStreamObj(E131);
        }
    }
}
#endif
// Точка входа в настройки
void user_settings_frame(Interface *interf, JsonObject *data);
void section_settings_frame(Interface *interf, JsonObject *data){
    // Страница "Настройки"
    if (!interf) return;
    recreateoptionsTask(true); // only cancel task
    interf->json_frame_interface(FPSTR(TINTF_080));

    interf->json_section_main(FPSTR(T_SETTINGS), FPSTR(TINTF_002));
#ifdef OPTIONS_PASSWORD
    if(!myLamp.getLampState().isOptPass){
        interf->json_section_line(FPSTR(TCONST_0093));
            interf->password(FPSTR(TCONST_00B8), FPSTR(TINTF_02D));
            interf->button_submit(FPSTR(TCONST_0093), FPSTR(TINTF_01F), "", 19);
        interf->json_section_end();
    } else {
        interf->button(FPSTR(T_SH_TIME), FPSTR(TINTF_051));
        interf->button(FPSTR(T_SH_NETW), FPSTR(TINTF_081));
        user_settings_frame(interf, data);
        interf->spacer();
        block_settings_update(interf, data);
    }
#else
    interf->button(FPSTR(T_SH_TIME), FPSTR(TINTF_051));
    interf->button(FPSTR(T_SH_NETW), FPSTR(TINTF_081));
    user_settings_frame(interf, data);
    interf->spacer();
    block_settings_update(interf, data);
#endif
    interf->json_section_end();
    interf->json_frame_flush();
}

#ifdef OPTIONS_PASSWORD
void set_opt_pass(Interface *interf, JsonObject *data){
    if(!data) return;

    if((*data)[FPSTR(TCONST_00B8)]==OPTIONS_PASSWORD){
        LOG(println, F("Options unlocked for 10 minutes"));
        myLamp.getLampState().isOptPass = true;
        Task *_t = new Task(TASK_MINUTE*10, TASK_ONCE, [](){ myLamp.getLampState().isOptPass = false; TASK_RECYCLE; }, &ts); // через 10 минут отключаем
        _t->enableDelayed();
        section_settings_frame(interf, nullptr);
    }
}
#endif  // OPTIONS_PASSWORD

void user_settings_frame(Interface *interf, JsonObject *data){
if (!interf) return;
#ifdef MIC_EFFECTS
    interf->button(FPSTR(TCONST_0079), FPSTR(TINTF_020));
#endif
#ifdef MP3PLAYER
    interf->button(FPSTR(TCONST_009F), FPSTR(TINTF_099));
#endif

#ifdef ESP_USE_BUTTON
    interf->button(FPSTR(TCONST_0076), FPSTR(TINTF_013));
#endif
#ifdef ENCODER
    interf->button(FPSTR(TCONST_0008), FPSTR(TINTF_0DC));
#endif
    interf->button(FPSTR(TCONST_007A), FPSTR(TINTF_082));
#ifdef SHOWSYSCONFIG
    if(myLamp.isShowSysMenu())
        interf->button(FPSTR(TCONST_009A), FPSTR(TINTF_08F));
#endif
#ifndef MOOT
    block_lamp_config(interf, data);
#endif

}

void section_main_frame(Interface *interf, JsonObject *data){
    if (!interf) return;

    interf->json_frame_interface(FPSTR(TINTF_080));

    block_menu(interf, data);
    block_effects_main(interf, data);

    interf->json_frame_flush();

    if(!embui.sysData.wifi_sta && embui.param(FPSTR(P_WIFIMODE))=="0"){
        // форсируем выбор вкладки настройки WiFi если контроллер не подключен к внешней AP
        interf->json_frame_interface();
            interf->json_section_main(FPSTR(T_SET_WIFI), FPSTR(TINTF_028));
            block_only_wifi(interf, data);
            interf->json_section_end();
        interf->json_frame_flush();
        if(!EmbUI::GetInstance()->sysData.isWiFiScanning){ // автосканирование при входе в настройки
            EmbUI::GetInstance()->sysData.isWiFiScanning = true;
            set_scan_wifi(interf, data);
        }
    }
}

void section_sys_settings_frame(Interface *interf, JsonObject *data){
    // Страница "Настройки ESP"
    if (!interf) return;
    recreateoptionsTask(true); // only cancel task
    interf->json_frame_interface(FPSTR(TINTF_08F));

    block_menu(interf, data);
    interf->json_section_main(FPSTR(TCONST_0099), FPSTR(TINTF_08F));
        interf->spacer(FPSTR(TINTF_092)); // заголовок
        interf->json_section_line(FPSTR(TINTF_092)); // расположить в одной линии
#ifdef ESP_USE_BUTTON
            interf->number(FPSTR(TCONST_0097),FPSTR(TINTF_094),String(1),String(0),String(16));
#endif
#ifdef MP3PLAYER
            interf->number(FPSTR(TCONST_009B),FPSTR(TINTF_097),String(1),String(0),String(16));
            interf->number(FPSTR(TCONST_009C),FPSTR(TINTF_098),String(1),String(0),String(16));
#endif
        interf->json_section_end(); // конец контейнера
        interf->spacer();
        interf->number(FPSTR(TCONST_0098),FPSTR(TINTF_095),String(100),String(0),String(16000));

        //interf->json_section_main(FPSTR(TCONST_005F), "");
        interf->iframe(FPSTR(TCONST_005F), FPSTR(TCONST_005F));
        //interf->json_section_end();

        interf->button_submit(FPSTR(TCONST_0099), FPSTR(TINTF_008), FPSTR(P_GRAY));

        interf->spacer();
        interf->button(FPSTR(TCONST_0004), FPSTR(TINTF_00B));
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

    if(!embui.sysData.isWSConnect){ // если последние 5 секунд не было коннекта, защита от зацикливания ребута
        myLamp.sendString(String(FPSTR(TINTF_096)).c_str(), CRGB::Red, true);
        new Task(TASK_SECOND, TASK_ONCE, nullptr, &ts, true, nullptr, [](){ embui.autosave(true); LOG(println, F("Rebooting...")); remote_action(RA::RA_REBOOT, NULL, NULL); });
    }
    section_effects_frame(interf,data);
}

void set_lamp_flags(Interface *interf, JsonObject *data){
    if(!data) return;
    SETPARAM(FPSTR(TCONST_0094));
}

void save_lamp_flags(){
    DynamicJsonDocument doc(160);
    JsonObject obj = doc.to<JsonObject>();
    obj[FPSTR(TCONST_0094)] = ulltos(myLamp.getLampFlags());
    set_lamp_flags(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();
}

// кастомный обработчик, для реализации особой обработки событий сокетов
bool ws_action_handle(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    bool res = false; // false == EmbUI default action
    switch(type){
        case AwsEventType::WS_EVT_ERROR :
            {
                resetAutoTimers();
                uint16_t effNum = myLamp.effects.getSelected();
                myLamp.effects.directMoveBy(EFF_NONE);
                myLamp.effects.removeConfig(effNum);
                myLamp.effects.directMoveBy(effNum);
                //remote_action(RA_EFFECT, String(effNum).c_str(), NULL);
                String tmpStr=F("- ");
                tmpStr+=effNum;
                myLamp.sendString(tmpStr.c_str(), CRGB::Red);

                res = true;
                break;
            }
        default :
            res = false; 
            break;
    }
    return res;
}

// кастомный обработчик, для поддержки приложения WLED APP ( https://play.google.com/store/apps/details?id=com.aircoookie.WLED )
bool notfound_handle(AsyncWebServerRequest *request, const String& req)
{
    if (!(req.indexOf(F("win")) == 1)) return false;
    LOG(println,req);

    uint8_t bright = myLamp.getLampBrightness();
    if ((req.indexOf(F("&T=2")) > 1)){
        if(myLamp.isLampOn()){
            myLamp.startNormalMode(); // З WLED перемикаємо в нормальний режим
            remote_action(RA::RA_OFF, NULL);
            bright = 0;
        }
        else
            remote_action(RA::RA_ON, NULL);
    }

    if ((req.indexOf(F("&A=")) > 1)){
        bright = req.substring(req.indexOf(F("&A="))+3).toInt();
        if(bright)
            remote_action(RA::RA_BRIGHT_NF, (String(FPSTR(TCONST_0015))+"0").c_str(), String(bright).c_str(), NULL);
    }

    String result = F("<?xml version=\"1.0\" ?><vs><ac>");
    result.concat(myLamp.isLampOn()?bright:0);
    result.concat(F("</ac><ds>"));
    result.concat(embui.param(FPSTR(P_hostname)));
    result.concat(F(".local-")); //lampname.local-IP
    result.concat(WiFi.localIP().toString());
    result.concat(F("</ds></vs>"));

    request->send(200, FPSTR(PGmimexml), result);
    return true;
}

/**
 * Набор конфигурационных переменных и обработчиков интерфейса
 */
void create_parameters(){
    LOG(println, F("Создание дефолтных параметров"));
    // создаем дефолтные параметры для нашего проекта
    embui.var_create(FPSTR(TCONST_0094), ulltos(myLamp.getLampFlags())); // Дефолтный набор флагов
    embui.var_create(FPSTR(TCONST_0016), F("1"));   // "effListMain"
    embui.var_create(FPSTR(P_m_tupd), String(DEFAULT_MQTTPUB_INTERVAL)); // "m_tupd" интервал отправки данных по MQTT в секундах (параметр в энергонезависимой памяти)

    //WiFi
    embui.var_create(FPSTR(P_hostname), F(""));
    embui.var_create(FPSTR(P_WIFIMODE), String("0"));       // STA/AP/AP+STA, STA by default
    embui.var_create(FPSTR(P_APpwd), "");                   // пароль внутренней точки доступа

    // параметры подключения к MQTT
    embui.var_create(FPSTR(P_m_host), F("")); // Дефолтные настройки для MQTT
    embui.var_create(FPSTR(P_m_port), F("1883"));
    embui.var_create(FPSTR(P_m_user), F(""));
    embui.var_create(FPSTR(P_m_pass), F(""));
    embui.var_create(FPSTR(P_m_pref), embui.mc);  // m_pref == MAC по дефолту
    embui.var_create(FPSTR(TCONST_002A), F("cfg1.json")); // "fileName"

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

    embui.var_create(FPSTR(TCONST_0026), String(60)); // Дефолтное значение, настраивается из UI
    embui.var_create(FPSTR(TCONST_00BD), String(F("85"))); // 5<<4+5, старшие и младшие 4 байта содержат 5

    embui.var_create(FPSTR(TCONST_0053), String(F("1.0")));

    // пины и системные настройки
#ifdef ESP_USE_BUTTON
    embui.var_create(FPSTR(TCONST_0097), String(BTN_PIN)); // Пин кнопки
    embui.var_create(FPSTR(TCONST_003F), String(GAUGETYPE::GT_VERT));         // Тип шкалы
#endif
#ifdef ENCODER
    embui.var_create(FPSTR(TCONST_0042), F("#FFA500"));  // Дефолтный цвет текста (Orange)
    embui.var_create(FPSTR(TCONST_0043), F("40"));        // Задержка прокрутки текста
    embui.var_create(FPSTR(TCONST_003F), String(GAUGETYPE::GT_VERT));  // Тип шкалы
    embui.var_create(FPSTR(TCONST_0040), F("#FF2A00"));  // Дефолтный цвет шкалы
#endif

#ifdef MP3PLAYER
    embui.var_create(FPSTR(TCONST_009B), String(MP3_RX_PIN)); // Пин RX плеера
    embui.var_create(FPSTR(TCONST_009C), String(MP3_TX_PIN)); // Пин TX плеера
    embui.var_create(FPSTR(TCONST_00A2),F("15")); // громкость
    embui.var_create(FPSTR(TCONST_00A9),F("255")); // кол-во файлов в папке mp3
#endif
#ifdef TM1637_CLOCK
    embui.var_create(FPSTR(TCONST_00D7), String(F("82"))); // 5<<4+5, старшие и младшие 4 байта содержат 5
    // embui.var_create(FPSTR(TCONST_00D8), F("5"));   // Яркость при вкл
    // embui.var_create(FPSTR(TCONST_00D9), F("1"));    // Яркость при выкл
#endif
    embui.var_create(FPSTR(TCONST_0098), String(CURRENT_LIMIT)); // Лимит по току
#ifdef USE_STREAMING
    embui.var_create(FPSTR(TCONST_0047), String(SOUL_MATE)); // Тип трансляции
    embui.var_create(FPSTR(TCONST_0077), F("1")); // Universe для E1.31
#endif
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
    BasicUI::add_sections(true); //

    embui.section_handle_add(FPSTR(TCONST_0099), set_sys_settings);

    embui.section_handle_add(FPSTR(TCONST_0094), set_lamp_flags);

    embui.section_handle_add(FPSTR(TCONST_007E), section_main_frame);
    embui.section_handle_add(FPSTR(TCONST_0020), show_main_flags);

    embui.section_handle_add(FPSTR(TCONST_0000), section_effects_frame);
    embui.section_handle_add(FPSTR(TCONST_0011), show_effects_param);
    embui.section_handle_add(FPSTR(TCONST_0016), set_effects_list);
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
#ifdef USE_STREAMING    
    embui.section_handle_add(FPSTR(TCONST_0044), section_streaming_frame);
    embui.section_handle_add(FPSTR(TCONST_0046), set_streaming);
    embui.section_handle_add(FPSTR(TCONST_0047), set_streaming_type);
    embui.section_handle_add(FPSTR(TCONST_0049), set_streaming_drirect);
    embui.section_handle_add(FPSTR(TCONST_004A), set_streaming_mapping);
    embui.section_handle_add(FPSTR(TCONST_0077), set_streaming_universe);
    embui.section_handle_add(FPSTR(TCONST_0012), set_streaming_bright);
#endif
    embui.section_handle_add(FPSTR(TCONST_009A), section_sys_settings_frame);
    embui.section_handle_add(FPSTR(TCONST_0003), section_text_frame);
    embui.section_handle_add(FPSTR(TCONST_0034), set_lamp_textsend);
    embui.section_handle_add(FPSTR(TCONST_0030), edit_lamp_config);
    embui.section_handle_add(FPSTR(TCONST_0029), edit_lamp_config);

    embui.section_handle_add(FPSTR(TCONST_00BA), set_text_config);
    embui.section_handle_add(FPSTR(TCONST_00CA), set_drawing);
    embui.section_handle_add(FPSTR(TCONST_00DC), set_clear);
    embui.section_handle_add(FPSTR(TCONST_00C4), set_drawflag);

    // меняю обработчики для страницы настроек :)
    embui.section_handle_remove(FPSTR(T_SETTINGS));
    embui.section_handle_add(FPSTR(T_SETTINGS), section_settings_frame); // своя главная страница настроек, со своим переводом

    embui.section_handle_remove(FPSTR(T_SH_NETW)); // своя страница настроек сети, со своим переводом
    embui.section_handle_add(FPSTR(T_SH_NETW), show_settings_wifi);

    embui.section_handle_remove(FPSTR(T_SH_TIME)); // своя страница настроек времени, со своим переводом
    embui.section_handle_add(FPSTR(T_SH_TIME), show_settings_time);

    embui.section_handle_remove(FPSTR(T_SET_WIFI));
    embui.section_handle_add(FPSTR(T_SET_WIFI), set_settings_wifi);

    embui.section_handle_remove(FPSTR(T_SET_TIME));
    embui.section_handle_add(FPSTR(T_SET_TIME), set_settings_time);
#ifdef EMBUI_USE_MQTT
    embui.section_handle_remove(FPSTR(T_SET_MQTT));
    embui.section_handle_add(FPSTR(T_SET_MQTT), set_settings_mqtt);
#endif
#ifdef EMBUI_USE_FTP
    embui.section_handle_remove(FPSTR(T_SET_FTP));
    embui.section_handle_add(FPSTR(T_SET_FTP), set_ftp);
#endif

    embui.section_handle_remove(FPSTR(T_SET_SCAN));
    embui.section_handle_add(FPSTR(T_SET_SCAN), set_scan_wifi);         // обработка сканирования WiFi

    embui.section_handle_add(FPSTR(TCONST_007A), show_settings_other);
    embui.section_handle_add(FPSTR(TCONST_004B), set_settings_other);

    #ifdef OPTIONS_PASSWORD
    embui.section_handle_add(FPSTR(TCONST_0093), set_opt_pass);
    #endif // OPTIONS_PASSWORD

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
    embui.section_handle_add(FPSTR(TCONST_003F), set_gaugetype);
#endif

#ifdef LAMP_DEBUG
    embui.section_handle_add(FPSTR(TCONST_0095), set_debugflag);
#endif

#ifdef MP3PLAYER
    embui.section_handle_add(FPSTR(TCONST_009D), set_mp3flag);
    embui.section_handle_add(FPSTR(TCONST_00A2), set_mp3volume);
    embui.section_handle_add(FPSTR(TCONST_009F), show_settings_mp3);
    embui.section_handle_add(FPSTR(TCONST_00A0), set_settings_mp3);

    embui.section_handle_add(FPSTR(CMD_TCONST_0006), set_mp3_player);
    embui.section_handle_add(FPSTR(CMD_TCONST_0007), set_mp3_player);
    embui.section_handle_add(FPSTR(TCONST_00C0), set_mp3_player);
    embui.section_handle_add(FPSTR(TCONST_00C1), set_mp3_player);
#endif
#ifdef ENCODER
    embui.section_handle_add(FPSTR(TCONST_0008), show_settings_enc);
    embui.section_handle_add(FPSTR(TCONST_000C), set_settings_enc);
#endif
}

void sync_parameters(){
    DynamicJsonDocument doc(1024);
    //https://arduinojson.org/v6/api/jsondocument/
    //JsonDocument::to<T>() clears the document and converts it to the specified type. Don’t confuse this function with JsonDocument::as<T>() that returns a reference only if the requested type matches the one in the document.
    JsonObject obj = doc.to<JsonObject>();

    if(check_recovery_state(true)){
        LOG(printf_P,PSTR("Critical Error: Lamp recovered from corrupted effect number: %s\n"),String(embui.param(FPSTR(TCONST_0016))).c_str());
        embui.var(FPSTR(TCONST_0016),String(0)); // что-то пошло не так, был циклический ребут, сбрасываем эффект
    }

#ifdef EMBUI_USE_MQTT
    myLamp.setmqtt_int(embui.param(FPSTR(P_m_tupd)).toInt());
#endif

    String syslampFlags(embui.param(FPSTR(TCONST_0094)));
    LAMPFLAGS tmp;
    tmp.lampflags = stoull(syslampFlags); //atol(embui.param(FPSTR(TCONST_0094)).c_str());
//#ifndef ESP32
//    LOG(printf_P, PSTR("tmp.lampflags=%llu (%s)\n"), tmp.lampflags, syslampFlags.c_str());
//#endif
    LOG(printf_P, PSTR("tmp.lampflags=%llu\n"), tmp.lampflags);

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
    CALL_INTF_OBJ(set_eventflag);
    //set_eventflag(nullptr, &obj);
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
Task *t = new Task(DFPALYER_START_DELAY+500, TASK_ONCE, nullptr, &ts, false, nullptr, [tmp](){
    if(!mp3->isReady()){
        LOG(println, F("DFPlayer not ready yet..."));
        if(millis()<10000){
            ts.getCurrentTask()->restartDelayed(TASK_SECOND*2);
            return;
        }
    }
    
    DynamicJsonDocument doc(1024);
    //https://arduinojson.org/v6/api/jsondocument/
    //JsonDocument::to<T>() clears the document and converts it to the specified type. Don’t confuse this function with JsonDocument::as<T>() that returns a reference only if the requested type matches the one in the document.
    JsonObject obj = doc.to<JsonObject>();
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
    TASK_RECYCLE;
});
t->enableDelayed();
#endif

#ifdef AUX_PIN
    CALL_SETTER(FPSTR(TCONST_000E), embui.param(FPSTR(TCONST_000E)), set_auxflag);
#endif

    myLamp.setClearingFlag(tmp.isEffClearing);

    obj[FPSTR(TCONST_0090)] = tmp.numInList ? "1" : "0";
    myLamp.setNumInList(tmp.numInList);
#ifdef MIC_EFFECTS
    obj[FPSTR(TCONST_0091)] = tmp.effHasMic ? "1" : "0";
    myLamp.setEffHasMic(tmp.effHasMic);
#endif
    SORT_TYPE type = (SORT_TYPE)embui.param(FPSTR(TCONST_0050)).toInt();
    obj[FPSTR(TCONST_0050)] = type;
    set_effects_config_param(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();

#ifdef ESP_USE_BUTTON
    obj[FPSTR(TCONST_001F)] = tmp.isBtn ? "1" : "0";
    CALL_INTF_OBJ(set_btnflag);
    obj[FPSTR(TCONST_003F)] = String(tmp.GaugeType);
    CALL_INTF_OBJ(set_gaugetype);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();
#endif
#ifdef ENCODER
    obj[FPSTR(TCONST_0042)] = embui.param(FPSTR(TCONST_0042));
    obj[FPSTR(TCONST_0043)] = (110U - embui.param(FPSTR(TCONST_0043)).toInt());
    obj[FPSTR(TCONST_003F)] = String(tmp.GaugeType);
    obj[FPSTR(TCONST_0040)] = embui.param(FPSTR(TCONST_0040));
    set_settings_enc(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();
#endif

    obj[FPSTR(TCONST_0051)] = String(110U - embui.param(FPSTR(TCONST_0051)).toInt());
    obj[FPSTR(TCONST_0052)] = embui.param(FPSTR(TCONST_0052));
    obj[FPSTR(TCONST_0054)] = embui.param(FPSTR(TCONST_0054));
    obj[FPSTR(TCONST_00C3)] = embui.param(FPSTR(TCONST_00C3));

    String datetime;
    TimeProcessor::getDateTimeString(datetime, embui.param(FPSTR(TCONST_0055)).toInt());
    obj[FPSTR(TCONST_0055)] = datetime;
    
    set_text_config(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();

#ifdef USE_STREAMING
    obj[FPSTR(TCONST_0046)] = tmp.isStream ? "1" : "0";
    set_streaming(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();

    obj[FPSTR(TCONST_0049)] = tmp.isDirect ? "1" : "0";
    set_streaming_drirect(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();

    obj[FPSTR(TCONST_004A)] = tmp.isMapping ? "1" : "0";
    set_streaming_mapping(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();

    obj[FPSTR(TCONST_0047)] = embui.param(FPSTR(TCONST_0047));
    set_streaming_type(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();

    obj[FPSTR(TCONST_0077)] = embui.param(FPSTR(TCONST_0077));
    set_streaming_universe(nullptr, &obj);
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();
#endif


    obj[FPSTR(TCONST_004E)] = tmp.isFaderON ? "1" : "0";
    obj[FPSTR(TCONST_008E)] = tmp.isEffClearing ? "1" : "0";
    obj[FPSTR(TCONST_004C)] = tmp.MIRR_H ? "1" : "0";
    obj[FPSTR(TCONST_004D)] = tmp.MIRR_V ? "1" : "0";
    obj[FPSTR(TCONST_004F)] = tmp.dRand ? "1" : "0";
    obj[FPSTR(TCONST_009E)] = tmp.showName ? "1" : "0";
    obj[FPSTR(TCONST_0096)] = tmp.isShowSysMenu ? "1" : "0";

#ifdef TM1637_CLOCK
    uint8_t tmBright = embui.param(FPSTR(TCONST_00D7)).toInt();
    obj[FPSTR(TCONST_00D8)] = tmBright>>4;
    obj[FPSTR(TCONST_00D9)] = tmBright&0x0F;
    obj[FPSTR(TCONST_00DA)] = tmp.tm24 ? "1" : "0";
    obj[FPSTR(TCONST_00DB)] = tmp.tmZero ? "1" : "0";
    #ifdef DS18B20
    obj[FPSTR(TCONST_002F)] = tmp.isTempOn ? "1" : "0";
    #endif
#endif

    uint8_t alarmPT = embui.param(FPSTR(TCONST_00BD)).toInt();
    obj[FPSTR(TCONST_00BB)] = alarmPT>>4;
    obj[FPSTR(TCONST_00BC)] = alarmPT&0x0F;

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
    Task *_t = new Task(TASK_SECOND, TASK_ONCE, [](){ // откладыаем задачу на 1 секунду, т.к. выше есть тоже отложенные инициализации, см. set_settings_other()
        myLamp.getLampState().isInitCompleted = true; // ставим признак того, что инициализация уже завершилась, больше его не менять и должен быть в самом конце sync_parameters() !!!
        TASK_RECYCLE;
    }, &ts, false);
    _t->enableDelayed();
    LOG(println, F("sync_parameters() done"));
}

// обработка эвентов лампы
void event_worker(DEV_EVENT *event){
    RA action = RA_UNKNOWN;
    LOG(printf_P, PSTR("%s - %s\n"), ((DEV_EVENT *)event)->getName().c_str(), embui.timeProcessor.getFormattedShortTime().c_str());

    switch (event->getEvent()) {
    case EVENT_TYPE::ON: action = RA_ON; break;
    case EVENT_TYPE::OFF: action = RA_OFF; myLamp.startNormalMode(); break; // З евенту перемикаємо в нормальний режим
    case EVENT_TYPE::DEMO: action = RA_DEMO; break;
    case EVENT_TYPE::ALARM: action = RA_ALARM; break;
    case EVENT_TYPE::LAMP_CONFIG_LOAD: action = RA_LAMP_CONFIG; break;
#ifdef ESP_USE_BUTTON
    case EVENT_TYPE::BUTTONS_CONFIG_LOAD:  action = RA_BUTTONS_CONFIG; break;
#endif
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
        if ((event->getMessage()).isEmpty()) break;

        String tmpS = event->getMessage();
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
    case EVENT_TYPE::SET_GLOBAL_BRIGHT: action = RA_GLOBAL_BRIGHT; break;
    case EVENT_TYPE::SET_WHITE_HI: action = RA_WHITE_HI; break;
    case EVENT_TYPE::SET_WHITE_LO: action = RA_WHITE_LO; break;
    default:;
    }

    remote_action(action, event->getMessage().c_str(), NULL);
}

void show_progress(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();
    interf->json_section_hidden(FPSTR(T_DO_OTAUPD), String(FPSTR(TINTF_056)) + String(F(" : ")) + (*data)[FPSTR(TINTF_05A)].as<String>()+ String("%"));
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
    if (myLamp.getGaugeType()!=GAUGETYPE::GT_NONE){
        GAUGE::GaugeShow(len, total, 100);
    }
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
            if(value){
                StringTask *t = new StringTask(value, 3 * TASK_SECOND, TASK_ONCE, nullptr, &ts, false, nullptr,  [](){
                    StringTask *cur = (StringTask *)ts.getCurrentTask();
                    remote_action(RA::RA_SEND_TEXT, cur->getData(), NULL);
                    TASK_RECYCLE;
                });
                t->enableDelayed();
            }
            break;
        case RA::RA_OFF: {
                // нажатие кнопки точно отключает ДЕМО и белую лампу возвращая в нормальный режим
                // myLamp.stopRGB(); // выключение RGB-режима
                if(value){
                   remote_action(RA::RA_SEND_TEXT, value, NULL);
                }
                new Task(500, TASK_FOREVER, [value](){
                    if((!myLamp.isPrintingNow() && value) || !value){ // отложенное выключение только для случая когда сообщение выводится в этом же экшене, а не чужое
                        Task *task = ts.getCurrentTask();
                        DynamicJsonDocument doc(512);
                        JsonObject obj = doc.to<JsonObject>();
                        obj[FPSTR(TCONST_00D4)] = "1"; // ознака виклику з REMOTE ACTION
                        // LAMPMODE mode = myLamp.getMode();
                        // if(mode!=LAMPMODE::MODE_NORMAL){
                        //     CALL_INTF(FPSTR(TCONST_001B), "0", set_demoflag); // отключить демо, если было включено
                        //     if (myLamp.IsGlobalBrightness()) {
                        //         embui.var(FPSTR(TCONST_0018), String(myLamp.getLampBrightness())); // сохранить восстановленную яркость в конфиг, если она глобальная
                        //     }
                        // }
                        // obj.clear();
                        // doc.garbageCollect();
                        CALL_INTF(FPSTR(TCONST_001A), "0", set_onflag);
                        task->disable();
                        TASK_RECYCLE;
                    }
                }, &ts, true);
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
            if(pubEffTask)
                pubEffTask->cancel();
            pubEffTask = new StringTask(value, myLamp.getLampSettings().isFaderON ? (FADE_TIME / 1000.0) * TASK_SECOND + 500 : 500, TASK_ONCE, [](){
                StringTask *cur = (StringTask *)ts.getCurrentTask();
                DynamicJsonDocument doc(512);
                JsonObject obj = doc.to<JsonObject>();
                LOG(printf_P,PSTR("EmbUI::GetInstance()->ws.count()=%d, %s\n"),EmbUI::GetInstance()->ws.count(),cur->getData());
                CALL_INTF(FPSTR(TCONST_0016), String(cur->getData()), set_effects_list); // публикация будет здесь
            }, &ts, false, nullptr,  [](){
                TASK_RECYCLE;
                pubEffTask=nullptr;
            });
            pubEffTask->enableDelayed();
            break;
        }
        case RA::RA_GLOBAL_BRIGHT:
            if (atoi(value) > 0){
                CALL_INTF(FPSTR(TCONST_001C), F("1"), set_gbrflag);
                return remote_action(RA_CONTROL, (String(FPSTR(TCONST_0015))+F("0")).c_str(), value, NULL);
            }
            else
                CALL_INTF(FPSTR(TCONST_001C), value, set_gbrflag);
            break;
        case RA::RA_BRIGHT_NF:
            obj[FPSTR(TCONST_0017)] = true;
            obj[FPSTR(TCONST_00D5)] = true;
            //CALL_INTF_OBJ(set_effects_dynCtrl);
            set_effects_dynCtrl(nullptr, &obj);
            break;
        case RA::RA_CONTROL:
            //CALL_INTF_OBJ(set_effects_dynCtrl);
            obj[FPSTR(TCONST_00D5)] = true;
            set_effects_dynCtrl(nullptr, &obj);
            break;
#ifdef MP3PLAYER
        case RA::RA_MP3_PREV:
            if(!myLamp.isONMP3()) return;
            mp3->playEffect(mp3->getCurPlayingNb()-(int)value,"");
            break;
        case RA::RA_MP3_NEXT:
            if(!myLamp.isONMP3()) return;
            mp3->playEffect(mp3->getCurPlayingNb()+(int)value,"");
            break;
        case RA::RA_MP3_SOUND:
            if(!myLamp.isONMP3()) return;
            mp3->playEffect((int)value,"");
            break;
        case RA::RA_PLAYERONOFF:
            obj[FPSTR(TCONST_00D5)] = "0"; // не озвучивать время
            CALL_INTF(FPSTR(TCONST_009D), value, set_mp3flag);
            break;
#endif
#ifdef MIC_EFFECTS
        case RA::RA_MIC:
            CALL_INTF_OBJ(show_settings_mic);
            break;
        case RA::RA_MICONOFF:
            CALL_INTF(FPSTR(TCONST_001E), value, set_micflag);
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
            return remote_action(RA::RA_EFFECT, "1", NULL);
        case RA::RA_WHITE_CUR:
            if(myLamp.getMode()!=LAMPMODE::MODE_WHITELAMP){
                myLamp.switcheffect(SW_WHITE_CUR);
                return remote_action(RA::RA_EFFECT, "1", NULL);
            } else {
                obj[F("dynCtrl1")] = value;
                obj[FPSTR(TCONST_00D5)] = true;
                set_effects_dynCtrl(nullptr, &obj);
            }
            break;
        case RA::RA_WHITE_LO:
            myLamp.switcheffect(SW_WHITE_LO);
            return remote_action(RA::RA_EFFECT, "1", NULL);
        case RA::RA_ALARM:
            ALARMTASK::startAlarm(&myLamp, value);
            break;
        case RA::RA_ALARM_OFF:
            ALARMTASK::stopAlarm();
            break;
        case RA::RA_REBOOT: {
                remote_action(RA::RA_WARNING, F("[16711680,3000,500]"), NULL);
                Task *t = new Task(3 * TASK_SECOND, TASK_ONCE, nullptr, &ts, false, nullptr, [](){ ESP.restart(); TASK_RECYCLE; });
                t->enableDelayed();
            }
            break;
        case RA::RA_WIFI_REC:
            //CALL_INTF(FPSTR(TINTF_028), FPSTR(TCONST_0080), BasicUI::set_settings_wifi);
            CALL_INTF(FPSTR(TINTF_028), FPSTR(TCONST_0080), set_settings_wifi);
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
#ifdef ESP_USE_BUTTON
        case RA::RA_BUTTONS_CONFIG:
            if (value && *value) {
                String filename = String(FPSTR(TCONST_0033));
                filename.concat(value);
                myButtons->clear();
                if (!myButtons->loadConfig()) {
                    default_buttons();
                }
            }
            break;
#endif
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
            String msg;
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
                    case 4: msg = arr[i].as<String>(); break;
                    default : break;
                }
			}
            myLamp.showWarning(col,dur,per,type,true,msg.isEmpty()?(const char *)nullptr:msg.c_str());
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
        case RA::RA_RGB: {
            String tmpStr = value;
            if(tmpStr.indexOf(",")!=-1){
                int16_t pos = 0;
                int16_t frompos = 0;
                uint8_t val = 0;
                uint32_t res = 0;
                do {
                    frompos = pos;
                    pos = tmpStr.indexOf(",", pos);
                    if(pos!=-1){
                        val = tmpStr.substring(frompos,pos).toInt();
                        res=(res<<8)|val;
                        pos++;
                    } else if(frompos<(signed)tmpStr.length()){
                        val = tmpStr.substring(frompos,tmpStr.length()).toInt();
                        res=(res<<8)|val; 
                    }
                } while(pos!=-1);
                CRGB color=CRGB(res);
                myLamp.startRGB(color);
                break;
            }
            tmpStr.replace(F("#"), F("0x"));
            long val = strtol(tmpStr.c_str(), NULL, 0);
            LOG(printf_P, PSTR("%s:%ld\n"), tmpStr.c_str(), val);
            CRGB color=CRGB(val);
            myLamp.startRGB(color);
            break; 
        }
        case RA::RA_FILLMATRIX: {
            String tmpStr = value;
            if(tmpStr.indexOf(",")!=-1){
                int16_t pos = 0;
                int16_t frompos = 0;
                uint8_t val = 0;
                uint32_t res = 0;
                do {
                    frompos = pos;
                    pos = tmpStr.indexOf(",", pos);
                    if(pos!=-1){
                        val = tmpStr.substring(frompos,pos).toInt();
                        res=(res<<8)|val;
                        pos++;
                    } else if(frompos<(signed)tmpStr.length()){
                        val = tmpStr.substring(frompos,tmpStr.length()).toInt();
                        res=(res<<8)|val; 
                    }
                } while(pos!=-1);
                LOG(printf_P,PSTR("RA_FILLMATRIX: %d\n"), res);
                CRGB color=CRGB(res);
                myLamp.fillDrawBuf(color);
                break;
            }
            tmpStr.replace(F("#"), F("0x"));
            long val = strtol(tmpStr.c_str(), NULL, 0);
            LOG(printf_P, PSTR("%s:%ld\n"), tmpStr.c_str(), val);
            CRGB color=CRGB(val);
            myLamp.fillDrawBuf(color);
            break; 
        }

        case RA::RA_SEND_IP:
            myLamp.sendString(WiFi.localIP().toString().c_str(), CRGB::White);
#ifdef TM1637_CLOCK
            tm1637.setIpShow();
#endif
            break;
        case RA::RA_SEND_TIME:
            myLamp.periodicTimeHandle(value, true);
            //myLamp.sendString(String(F("%TM")).c_str(), CRGB::Green);
            break;

        case RA::RA_MODECMD:
            {
                String mode = value;
                LAMPMODE curMode = myLamp.getMode();
                // Прежде всего вернуть предыдущий режим, если текущий не MODE_NORMAL
                if(curMode!=LAMPMODE::MODE_NORMAL){
                    if(mode!=FPSTR(TCONST_00E8))
                        myLamp.startNormalMode(false,false); // не востанавливаем предыдущий эффект

                    if(curMode==LAMPMODE::MODE_DEMO){
                        myLamp.startNormalMode(false);
                        //remote_action(RA::RA_DEMO, F("0"), NULL);    
                    } else if (curMode==LAMPMODE::MODE_ALARMCLOCK){
                        remote_action(RA::RA_ALARM_OFF, NULL);
                    } else if (curMode==LAMPMODE::MODE_RGBLAMP){
                        myLamp.stopRGB();
                        httpCallback(FPSTR(TCONST_00EB), F("255,255,255"), false);
                    } else if (curMode==LAMPMODE::MODE_WHITELAMP){
                        myLamp.startNormalMode(false);
                    }
                }
                // Теперь установить новый
                if(mode==FPSTR(TCONST_00E8)){ // Normal
                    myLamp.startNormalMode();
                } else if(mode==FPSTR(TCONST_00E9)){ // Alarm
                    remote_action(RA::RA_ALARM, NULL);
                } else if(mode==FPSTR(TCONST_00EA)){ // Demo
                    remote_action(RA::RA_DEMO, F("1"), NULL);
                } else if(mode==FPSTR(TCONST_00EB)){ // RGB
                    char buf[12]; sprintf_P(buf, PSTR("%d,%d,%d"), myLamp.getRGBColor().r, myLamp.getRGBColor().g, myLamp.getRGBColor().b);
                    remote_action(RA::RA_RGB, buf, NULL);
                    httpCallback(FPSTR(TCONST_00EB), buf, false);
                } else if(mode==FPSTR(TCONST_00EC)){ // White
                    remote_action(RA::RA_WHITE_CUR, NULL);
                } else if(mode==FPSTR(TCONST_00ED)){
                    // skip
                }
// #ifdef EMBUI_USE_MQTT
//                 sendData();
// #endif
            }
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
            CALL_INTF(FPSTR(TCONST_000E), "1", set_auxflag);
            break;
        case RA::RA_AUX_OFF:
            obj[FPSTR(TCONST_000E)] = false;
            set_auxflag(nullptr, &obj);
            CALL_INTF(FPSTR(TCONST_000E), "0", set_auxflag);
            break;
        case RA::RA_AUX_TOGLE:
            AUX_toggle(!digitalRead(AUX_PIN));
            CALL_INTF(FPSTR(TCONST_000E), digitalRead(AUX_PIN) == AUX_LEVEL ? "1" : "0", set_auxflag);
            break;
#endif
        default:
            break;
    }
    doc.clear(); doc.garbageCollect(); obj = doc.to<JsonObject>();
}

String httpCallback(const String &param, const String &value, bool isset){
    String result = F("Ok");
    String cmdParam = param;
    cmdParam.toUpperCase();
    RA action = RA_UNKNOWN;
    LOG(printf_P, PSTR("HTTP: %s - %s\n"), cmdParam.c_str(), value.c_str());

    if(!isset) {
        LOG(println, F("GET"));
        if (cmdParam == FPSTR(CMD_TCONST_0000))
            { result = myLamp.isLampOn() ? "1" : "0"; }
        else if (cmdParam == FPSTR(CMD_TCONST_0001))
            { result = !myLamp.isLampOn() ? "1" : "0"; }
        else if (cmdParam == FPSTR(CMD_TCONST_0002))
            { result = myLamp.IsGlobalBrightness() ? "1" : "0"; }
        else if (cmdParam == FPSTR(CMD_TCONST_0003))
            { result = myLamp.getMode() == LAMPMODE::MODE_DEMO ? "1" : "0"; }
#ifdef MP3PLAYER
        else if (cmdParam == FPSTR(CMD_TCONST_0004)) 
            { result = myLamp.isONMP3() ? "1" : "0"; }
        else if (cmdParam == FPSTR(CMD_TCONST_0005)) 
            { result = String(mp3->getCurPlayingNb()); }
        else if (cmdParam == FPSTR(CMD_TCONST_0006)) { action = RA_MP3_PREV; remote_action(action, "1", NULL); }
        else if (cmdParam == FPSTR(CMD_TCONST_0007)) { action = RA_MP3_NEXT; remote_action(action, "1", NULL); }
#endif
#ifdef MIC_EFFECTS
        else if (cmdParam == FPSTR(CMD_TCONST_0008)) 
            { result = myLamp.isMicOnOff() ? "1" : "0"; }
#endif
        else if (cmdParam == FPSTR(CMD_TCONST_0009))
            { result = String(myLamp.effects.getCurrent());  }
        else if (cmdParam == FPSTR(CMD_TCONST_000A))
            { myLamp.showWarning(CRGB::Orange,5000,500); }
        else if (cmdParam == FPSTR(CMD_TCONST_000B)) {
                String result = myLamp.effects.geteffconfig(myLamp.effects.getCurrent(), myLamp.getNormalizedLampBrightness());
#ifdef EMBUI_USE_MQTT
                embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00AE), result, true);
#endif
                return result;
            }
        else if (cmdParam == FPSTR(CMD_TCONST_000C)) {
            LList<UIControl*>&controls = myLamp.effects.getControls();
            for(int i=0; i<controls.size();i++){
                if(value == String(controls[i]->getId())){
                    result = String(F("[")) + controls[i]->getId() + String(F(",\"")) + (controls[i]->getId()==0 ? String(myLamp.getNormalizedLampBrightness()) : controls[i]->getVal()) + String(F("\"]"));
#ifdef EMBUI_USE_MQTT
                    embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00D0), result, true);
#endif
                    return result;
                }
            }
        }
        else if (cmdParam == FPSTR(CMD_TCONST_000F))  {
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
        else if (cmdParam == FPSTR(CMD_TCONST_0010))  {
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
        else if (cmdParam == FPSTR(CMD_TCONST_0011))  {
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
        else if (cmdParam == FPSTR(CMD_TCONST_0012))  {
            String effname((char *)0);
            uint16_t effnum = String(value).toInt();
            effnum = effnum ? effnum : myLamp.effects.getCurrent();
            myLamp.effects.loadeffname(effname, effnum);
            result = String(F("["))+effnum+String(",\"")+effname+String("\"]");
        }
        else if (cmdParam == FPSTR(CMD_TCONST_0013))  {
            String effname((char *)0);
            uint16_t effnum = String(value).toInt();
            effnum = effnum ? effnum : myLamp.effects.getCurrent();
            effname = FPSTR(T_EFFNAMEID[(uint8_t)effnum]);
            result = String(F("["))+effnum+String(",\"")+effname+String("\"]");
        }
        else if (cmdParam == FPSTR(CMD_TCONST_0014)) { action = RA_EFF_NEXT;  remote_action(action, value.c_str(), NULL); }
        else if (cmdParam == FPSTR(CMD_TCONST_0015)) { action = RA_EFF_PREV;  remote_action(action, value.c_str(), NULL); }
        else if (cmdParam == FPSTR(CMD_TCONST_0016)) { action = RA_EFF_RAND;  remote_action(action, value.c_str(), NULL); }
        else if (cmdParam == FPSTR(CMD_TCONST_0017)) { action = RA_REBOOT;  remote_action(action, value.c_str(), NULL); }
        else if (cmdParam == FPSTR(CMD_TCONST_0018)) { result = myLamp.isAlarm() ? "1" : "0"; }
        else if (cmdParam == FPSTR(CMD_TCONST_0019)) { char buf[32]; sprintf_P(buf, PSTR("[%d,%d]"), WIDTH, HEIGHT);  result = buf; }
        else if (cmdParam == FPSTR(CMD_TCONST_000D)) { if(value.isEmpty()) {char buf[12]; sprintf_P(buf, PSTR("%d,%d,%d"), myLamp.getRGBColor().r, myLamp.getRGBColor().g, myLamp.getRGBColor().b); result = buf;} else {result = value.c_str();} }
        else if (cmdParam == FPSTR(CMD_TCONST_0023)) { if(myLamp.getMode()==LAMPMODE::MODE_WHITELAMP) {result = myLamp.getEffControls()[1]->getVal();} else {result = String(F("127"));} }
        else if (cmdParam == FPSTR(CMD_TCONST_000E)) { result = myLamp.getModeDesc().c_str(); }
#ifdef EMBUI_USE_MQTT        
        embui.publish(String(FPSTR(TCONST_008B)) + param, result, true);
#endif
        return result;
    } else {
        LOG(println, F("SET"));
        if (cmdParam == FPSTR(CMD_TCONST_0000)) { action = (value!="0" ? RA_ON : RA_OFF); remote_action(action, NULL, NULL); return result; }
        else if (cmdParam == FPSTR(CMD_TCONST_0001)) { action = (value!="0" ? RA_OFF : RA_ON); remote_action(action, NULL, NULL); return result; }
        else if (cmdParam == FPSTR(CMD_TCONST_0003)) action = RA_DEMO;
        else if (cmdParam == FPSTR(CMD_TCONST_001A)) action = RA_SEND_TEXT;
        else if (cmdParam == FPSTR(CMD_TCONST_0009)) action = RA_EFFECT;
        else if (cmdParam == FPSTR(CMD_TCONST_0014)) action = RA_EFF_NEXT;
        else if (cmdParam == FPSTR(CMD_TCONST_0015)) action = RA_EFF_PREV;
        else if (cmdParam == FPSTR(CMD_TCONST_0016)) action = RA_EFF_RAND;
        else if (cmdParam == FPSTR(CMD_TCONST_0017)) action = RA_REBOOT;
        else if (cmdParam == FPSTR(CMD_TCONST_0018)) action = RA_ALARM;
        else if (cmdParam == FPSTR(CMD_TCONST_0002)) action = RA_GLOBAL_BRIGHT;
        else if (cmdParam == FPSTR(CMD_TCONST_000A)) action = RA_WARNING;
        else if (cmdParam == FPSTR(CMD_TCONST_001B)) action = RA_DRAW;
        else if (cmdParam == FPSTR(CMD_TCONST_001D)) action = RA_FILLMATRIX;
        else if (cmdParam == FPSTR(CMD_TCONST_000D)) { remote_action(RA_RGB, value.c_str(), NULL); return httpCallback(FPSTR(CMD_TCONST_000D), "", false);}
        else if (cmdParam == FPSTR(CMD_TCONST_0023)) { remote_action(RA_WHITE_CUR, value.c_str(), NULL); return httpCallback(FPSTR(CMD_TCONST_0023), "", false);}
        else if (cmdParam == FPSTR(CMD_TCONST_000E)) action = RA_MODECMD;
#ifdef MP3PLAYER
        else if (cmdParam == FPSTR(CMD_TCONST_0006)) action = RA_MP3_PREV;
        else if (cmdParam == FPSTR(CMD_TCONST_0007)) action = RA_MP3_NEXT;
        else if (cmdParam == FPSTR(CMD_TCONST_0005)) action = RA_MP3_SOUND;
        else if (cmdParam == FPSTR(CMD_TCONST_0004)) action = RA_PLAYERONOFF;
#endif
#ifdef MIC_EFFECTS
        else if (cmdParam == FPSTR(CMD_TCONST_0008)) action = RA_MICONOFF;
#endif
        //else if (cmdParam.startsWith(FPSTR(TCONST_0015))) { action = RA_CONTROL; remote_action(action, cmdParam.c_str(), value.c_str(), NULL); return result; }
        else if (cmdParam == FPSTR(CMD_TCONST_000B)) {
            return httpCallback(cmdParam, "", false); // set пока не реализована
        }
        else if (cmdParam == FPSTR(CMD_TCONST_000C) || cmdParam == FPSTR(CMD_TCONST_001C)) {
            String str=value;
            DynamicJsonDocument doc(256);
            deserializeJson(doc,str);
            JsonArray arr = doc.as<JsonArray>();
            uint16_t id=0;
            String val="";

            if(arr.size()<2){ // мало параметров, т.е. это GET команда, возвращаем состояние контрола
                return httpCallback(FPSTR(CMD_TCONST_000C), value, false);
            }

            if(cmdParam == FPSTR(CMD_TCONST_001C)){ // это команда увеличения контрола на значение, соотвественно получаем текущее
                val = arr[1].as<String>().toInt();
                str = httpCallback(FPSTR(CMD_TCONST_000C), arr[0], false);
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
            remote_action(RA_CONTROL, (String(FPSTR(TCONST_0015))+id).c_str(), val.c_str(), NULL);
            //result = String(F("[")) + String(id) + String(F(",\"")) + val + String(F("\"]"));
            //embui.publish(String(FPSTR(TCONST_008B)) + FPSTR(TCONST_00D0), result, true);

            return httpCallback(FPSTR(CMD_TCONST_000C), String(id), false); // т.к. отложенный вызов, то иначе обрабатыаем
        }
        else if (cmdParam == FPSTR(CMD_TCONST_0012))  {
            String effname((char *)0);
            uint16_t effnum=String(value).toInt();
            myLamp.effects.loadeffname(effname, effnum);
            result = String(F("["))+effnum+String(",\"")+effname+String("\"]");
#ifdef EMBUI_USE_MQTT
            embui.publish(String(FPSTR(TCONST_008B)) + param, result, true);
#endif
            return result;
        }
        else if (cmdParam == FPSTR(CMD_TCONST_0013))  {
            String effname((char *)0);
            uint16_t effnum=String(value).toInt();
            effname = FPSTR(T_EFFNAMEID[(uint8_t)effnum]);
            result = String(F("["))+effnum+String(",\"")+effname+String("\"]");
#ifdef EMBUI_USE_MQTT
            embui.publish(String(FPSTR(TCONST_008B)) + param, result, true);
#endif
            return result;
        }
#ifdef OTA
        else if (cmdParam == FPSTR(CMD_TCONST_001E)) action = RA_OTA;
#endif
#ifdef AUX_PIN
        else if (cmdParam == FPSTR(CMD_TCONST_001F)) action = RA_AUX_ON;
        else if (cmdParam == FPSTR(CMD_TCONST_0020))  action = RA_AUX_OFF;
        else if (cmdParam == FPSTR(CMD_TCONST_0021))  action = RA_AUX_TOGLE;
#endif
        remote_action(action, value.c_str(), NULL);
    }
    return result;
}
