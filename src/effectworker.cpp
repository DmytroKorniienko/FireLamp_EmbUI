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

#include "effects.h"
#include "misc.h"

/*
 * Создаем экземпляр класса калькулятора в зависимости от требуемого эффекта
 */
void EffectWorker::workerset(uint16_t effect, const bool isCfgProceed){
  if(worker && isCfgProceed){ // сначала сохраним текущий эффект
    saveeffconfig(curEff); // пишем конфиг только если это требуется, для индекса - пропускаем, там свой механизм
  }
  if(worker)
     worker.reset(); // освободим явно, т.к. 100% здесь будем пересоздавать

  switch (static_cast<EFF_ENUM>(effect%256)) // номер может быть больше чем ENUM из-за копирований, находим эффект по модулю
  {
  case EFF_ENUM::EFF_TIME :
    worker = std::unique_ptr<EffectTime>(new EffectTime());
    break;
  case EFF_ENUM::EFF_SWIRL :
    worker = std::unique_ptr<EffectSwirl>(new EffectSwirl());
    break;
  case EFF_ENUM::EFF_RAINBOWCOMET :
  case EFF_ENUM::EFF_RAINBOWCOMET3 :
    worker = std::unique_ptr<EffectComet>(new EffectComet());
    break;
  case EFF_ENUM::EFF_FLOCK :
    worker = std::unique_ptr<EffectFlock>(new EffectFlock());
    break;
  case EFF_ENUM::EFF_PRIZMATA :
    worker = std::unique_ptr<EffectPrismata>(new EffectPrismata());
    break;
  case EFF_ENUM::EFF_SPIRO :
    worker = std::unique_ptr<EffectSpiro>(new EffectSpiro());
    break;
  case EFF_ENUM::EFF_METABALLS :
    worker = std::unique_ptr<EffectMetaBalls>(new EffectMetaBalls());
    break;
  case EFF_ENUM::EFF_SINUSOID3 :
    worker = std::unique_ptr<EffectSinusoid3>(new EffectSinusoid3());
    break;
  case EFF_ENUM::EFF_BBALS :
    worker = std::unique_ptr<EffectBBalls>(new EffectBBalls());
    break;
  case EFF_ENUM::EFF_PAINTBALL :
    worker = std::unique_ptr<EffectLightBalls>(new EffectLightBalls());
    break;
  case EFF_ENUM::EFF_FIRE :
    worker = std::unique_ptr<EffectFire>(new EffectFire());
    break;
  case EFF_ENUM::EFF_PULSE :
    worker = std::unique_ptr<EffectPulse>(new EffectPulse());
    break;
  case EFF_ENUM::EFF_CUBE :
    worker = std::unique_ptr<EffectBall>(new EffectBall());
    break;
  case EFF_ENUM::EFF_LIGHTER_TRACES :
    worker = std::unique_ptr<EffectLighterTracers>(new EffectLighterTracers());
    break;
  case EFF_ENUM::EFF_RAINBOW_2D :
    worker = std::unique_ptr<EffectRainbow>(new EffectRainbow());
    break;
  case EFF_ENUM::EFF_COLORS :
    worker = std::unique_ptr<EffectColors>(new EffectColors());
    break;
  case EFF_ENUM::EFF_WHITE_COLOR :
    worker = std::unique_ptr<EffectWhiteColorStripe>(new EffectWhiteColorStripe());
    break;
  case EFF_ENUM::EFF_MATRIX :
    worker = std::unique_ptr<EffectMatrix>(new EffectMatrix());
    break;
  case EFF_ENUM::EFF_SNOW :
    worker = std::unique_ptr<EffectSnow>(new EffectSnow());
    break;
  case EFF_ENUM::EFF_SPARKLES :
    worker = std::unique_ptr<EffectSparcles>(new EffectSparcles());
    break;
  case EFF_ENUM::EFF_EVERYTHINGFALL :
    worker = std::unique_ptr<EffectEverythingFall>(new EffectEverythingFall());
    break;
  case EFF_ENUM::EFF_FIRE2012 :
    worker = std::unique_ptr<EffectFire2012>(new EffectFire2012());
    break;
  case EFF_ENUM::EFF_SNOWSTORMSTARFALL :
    worker = std::unique_ptr<EffectStarFall>(new EffectStarFall());
    break;
  case EFF_ENUM::EFF_LIGHTERS :
    worker = std::unique_ptr<EffectLighters>(new EffectLighters());
    break;
  case EFF_ENUM::EFF_MADNESS :
  case EFF_ENUM::EFF_CLOUDS :
  case EFF_ENUM::EFF_LAVA :
  case EFF_ENUM::EFF_PLASMA :
  case EFF_ENUM::EFF_RAINBOW :
  case EFF_ENUM::EFF_RAINBOW_STRIPE :
  case EFF_ENUM::EFF_ZEBRA :
  case EFF_ENUM::EFF_FOREST :
  case EFF_ENUM::EFF_OCEAN :
    worker = std::unique_ptr<Effect3DNoise>(new Effect3DNoise());
    break;
  case EFF_ENUM::EFF_DRIFT :
  case EFF_ENUM::EFF_DRIFT2 :
    worker = std::unique_ptr<EffectDrift>(new EffectDrift());
    break;
  case EFF_ENUM::EFF_TWINKLES :
    worker = std::unique_ptr<EffectTwinkles>(new EffectTwinkles());
    break;
  case EFF_ENUM::EFF_WAVES :
    worker = std::unique_ptr<EffectWaves>(new EffectWaves());
    break;
  case EFF_ENUM::EFF_RADAR :
    worker = std::unique_ptr<EffectRadar>(new EffectRadar());
    break;
  case EFF_ENUM::EFF_SMOKE :
    worker = std::unique_ptr<EffectMStreamSmoke>(new EffectMStreamSmoke());
    break;
  case EFF_ENUM::EFF_FIRE2018 :
    worker = std::unique_ptr<EffectFire2018>(new EffectFire2018());
    break;
  case EFF_ENUM::EFF_RINGS :
    worker = std::unique_ptr<EffectRingsLock>(new EffectRingsLock());
    break;
  case EFF_ENUM::EFF_CUBE2 :
    worker = std::unique_ptr<EffectCube2d>(new EffectCube2d());
    break;
  case EFF_ENUM::EFF_RAIN :
  case EFF_ENUM::EFF_COLORRAIN :
  case EFF_ENUM::EFF_STORMYRAIN :
    worker = std::unique_ptr<EffectRain>(new EffectRain());
    break;
#ifdef MIC_EFFECTS
  case EFF_ENUM::EFF_FREQ :
    worker = std::unique_ptr<EffectFreq>(new EffectFreq());
    break;
#endif

  default:
    worker = std::unique_ptr<EffectCalc>(new EffectCalc());
  }

  if(worker){
    worker->pre_init(static_cast<EFF_ENUM>(effect%256));
    originalName = effectName = worker->getName(); // сначла заполним дефолтным именем, а затем лишь вычитаем из конфига
    if(isCfgProceed){ // читаем конфиг только если это требуется, для индекса - пропускаем
      loadeffconfig(effect);
      // окончательная инициализация эффекта тут
      worker->init(static_cast<EFF_ENUM>(effect%256), getControls()[0]->getVal().toInt(), getControls()[1]->getVal().toInt(), getControls()[2]->getVal().toInt());
    }
  }
}

void EffectWorker::clearEffectList()
{
  // удаляем весь список
  while (effects.size()) {
      EffectListElem *eff = effects.shift();
      delete eff;
  }
  effects.clear();
}

void EffectWorker::clearControlsList()
{
  // удаляем весь список
  while (controls.size()) {
      UIControl *ctrl = controls.shift();
      delete ctrl;
  }
  controls.clear();
}

void EffectWorker::initDefault()
{
  //Serial.begin(115200);
  if (LittleFS.begin()) {
      File indexFile;
      String idx;
      String filename;
      filename.concat(F("/eff_index.json"));
      DeserializationError error;
      DynamicJsonDocument doc(4096); // отожрет много памяти, но имена все равно не храним, так что хрен с ним, писать ручной парсер как-то лень

      TRYAGAIN:
      if(!LittleFS.exists(filename)){ // если индексный файл не существует, то создать
          makeIndexFile();
      }

      indexFile = LittleFS.open(filename, "r"); // PSTR("r") использовать нельзя, будет исключение!
      idx = indexFile.readString();
      indexFile.close();
      error = deserializeJson(doc, idx);
      LOG(print, idx);

      if (error) {
          LOG(print, F("Index deserialize error: "));
          LOG(println, error.c_str()); //error.code
          LittleFS.remove(filename); // пересоздаем индекс и пробуем снова
          goto TRYAGAIN;
      }

      JsonArray arr = doc.as<JsonArray>();
      if(arr.isNull() || arr.size()==0){
          LOG(print, F("Index corrupted"));
          LittleFS.remove(filename); // пересоздаем индекс и пробуем снова
          goto TRYAGAIN;
      }

      //LOG(printf_P,PSTR("Создаю список эффектов конструктор (%d): %s\n"),arr.size(),idx.c_str());
      clearEffectList();
      for (size_t i=0; i<arr.size(); i++) {
          JsonObject item = arr[i];
          effects.add(new EffectListElem(item[F("nb")].as<uint16_t>(), item[F("fl")].as<uint8_t>()));
          //LOG(printf_P,PSTR("%d : %d\n"),item[F("nb")].as<uint16_t>(), item[F("fl")].as<uint8_t>());
      }
      effects.sort([](EffectListElem *&a, EffectListElem *&b){ return a->eff_nb - b->eff_nb;}); // сортирую по eff_nb
      uint16_t chk = 0; // удаляю дубликаты
      for(int i=0; i<effects.size(); i++){
        if(effects[i]->eff_nb==chk){
          delete effects.remove(i);
          continue;
        }
        chk = effects[i]->eff_nb;
      }
  }
}

void EffectWorker::loadeffname(const uint16_t nb, const char *folder)
{
  if (LittleFS.begin()) {
      File configFile;
      String filename = geteffectpathname(nb,folder);
      String cfg_str;
      DynamicJsonDocument doc(2048);

      configFile = LittleFS.open(filename, "r"); // PSTR("r") использовать нельзя, будет исключение!
      cfg_str = configFile.readString();
      configFile.close();
      DeserializationError error = deserializeJson(doc, cfg_str);
      if (error) {
        return;
      }
      effectName = doc.containsKey(F("name")) && doc[F("name")].as<String>()!="" ? doc[F("name")].as<String>() : effectName; // перенакрываем именем из конфига, если есть
  }
}

int EffectWorker::loadeffconfig(const uint16_t nb, const char *folder)
{
  if(worker==nullptr){
    return 1;
  }
  if (LittleFS.begin()) {
      File configFile;
      String filename = geteffectpathname(nb,folder);
      String cfg_str;
      DynamicJsonDocument doc(2048);

      READALLAGAIN:

      configFile = LittleFS.open(filename, "r"); // PSTR("r") использовать нельзя, будет исключение!
      cfg_str = configFile.readString();
      configFile.close();

      // LOG(printf_P,PSTR("Попытка чтения: %s: %s\n"), filename.c_str(), cfg_str.c_str());

      if (cfg_str == F("")){
          LOG(printf_P, PSTR("Failed to open effects config file: %s\n"), filename.c_str());
          savedefaulteffconfig(nb, filename); // сохраняем дефолтный и пробуем снова
          configFile = LittleFS.open(filename, "r"); // PSTR("r") использовать нельзя, будет исключение!
          cfg_str = configFile.readString();
          configFile.close();
      }

      DeserializationError error = deserializeJson(doc, cfg_str);
      if (error) {
          LOG(print, F("deserializeJson error: "));
          LOG(println, error.code());
          savedefaulteffconfig(nb, filename);
          configFile = LittleFS.open(filename, "r"); // PSTR("r") использовать нельзя, будет исключение!
          cfg_str = configFile.readString();
          configFile.close();
          DeserializationError error = deserializeJson(doc, cfg_str);
          if (error) {
              LOG(print, F("deserializeJson error, default corrupted: "));
              LOG(println, error.code());
              return 1; // ошибка
          }
      }

      version = doc[F("ver")].as<String>();
      if(worker->getversion()!=version){
          doc.clear();
          LOG(printf_P, PSTR("Wrong version of effect, rewrite with default (%s vs %s)\n"), version.c_str(), worker->getversion().c_str());
          savedefaulteffconfig(nb, filename);
          goto READALLAGAIN;
      }

      curEff = doc[F("nb")].as<uint16_t>();
      flags.mask = doc.containsKey(F("flags")) ? doc[F("flags")].as<uint8_t>() : 255;
      effectName = doc.containsKey(F("name")) && doc[F("name")].as<String>()!="" ? doc[F("name")].as<String>() : worker->getName();
      
      LOG(printf_P, PSTR("Load MEM: %s - CFG: %s - DEF: %s\n"), effectName.c_str(), doc[F("name")].as<String>().c_str(), worker->getName().c_str());

      // вычитываею список контроллов
      // повторные - скипаем, нехватающие - создаем
      // обязательные контролы 0, 1, 2 - яркость, скорость, масштаб, остальные пользовательские

      JsonArray arr = doc[F("ctrls")].as<JsonArray>();
      clearControlsList();
      uint8_t id_tst = 0x0; // пустой
      for (size_t i = 0; i < arr.size(); i++) {
          JsonObject item = arr[i];
          uint8_t id = item[F("id")].as<uint8_t>();
          if(!(id_tst&(1<<id))){ // проверка на существование контрола
              id_tst |= 1<<item[F("id")].as<uint8_t>(); // закладываемся не более чем на 8 контролов, этого хватит более чем :)
              String name = item.containsKey(F("name")) ?
                  item[F("name")].as<String>() 
                  : id == 0 ? String(F("Яркость"))
                  : id == 1 ? String(F("Скорость"))
                  : id == 2 ? String(F("Масштаб"))
                  : String(F("Доп."))+String(id);
              String val = item.containsKey(F("val")) ?
                  item[F("val")].as<String>()
                  : id < 3 ? String(127)
                  : String();
              String min = item.containsKey(F("min")) ?
                  item[F("min")].as<String>()
                  : id < 3 ? String(1)
                  : String();
              String max = item.containsKey(F("max")) ?
                  item[F("max")].as<String>()
                  : id < 3 ? String(255)
                  : String();
              String step = item.containsKey(F("step")) ?
                  item[F("step")].as<String>()
                  : id < 3 ? String(1)
                  : String();
              controls.add(new UIControl(
                  id,             // id
                  ((id<3) ? CONTROL_TYPE::RANGE : item[F("type")].as<CONTROL_TYPE>()),     // type
                  name,           // name
                  val,            // value
                  min,            // min
                  max,            // max
                  step            // step
              ));
          }
      }
      doc.clear();
      // тест стандартных контроллов
      for(int8_t id=0;id<3;id++){
          if(!((id_tst>>id)&1)){ // не найден контрол, нужно создать
              controls.add(new UIControl(
                  id,                                     // id
                  CONTROL_TYPE::RANGE,                    // type
                  id==0 ? F("Яркость") : id==1 ? F("Скорость") : F("Масштаб"),           // name
                  String(127),                            // value
                  String(1),                              // min
                  String(255),                            // max
                  String(1)                               // step
              ));
          }
      }
      controls.sort([](UIControl *&a, UIControl *&b){ return a->getId() - b->getId();}); // сортирую по id
  }
  return 0; // успешно
}

const String EffectWorker::geteffectpathname(const uint16_t nb, const char *folder){
  uint16_t swapnb = nb>>8|nb<<8; // меняю местами 2 байта, так чтобы копии/верисии эффекта оказалась в имени файла позади
  String filename;
  char buffer[5]; 
  if (folder != nullptr) {
      filename.concat(F("/"));
      filename.concat(folder);
  }
  filename.concat(F("/eff/"));
  sprintf_P(buffer,PSTR("%04x"), swapnb);
  filename.concat(buffer);
  filename.concat(F(".json"));
  return filename;
}

void EffectWorker::savedefaulteffconfig(uint16_t nb, String &filename){
  // если конфиг не найден, то создаем дефолтный
  // при этом предполагаем, что worker указывает на нужный эффект!!!
  LOG(printf_P,PSTR("Create default config: %d %s\n"), nb, worker->getName().c_str());

  if (LittleFS.begin()) {
      File configFile;
      String  cfg = worker->defaultuiconfig();
      // workerset(nb,false); // пропускаем сохранение конфигов
      // LOG(println,worker->getName());
      cfg.replace(F("@name@"),worker->getName());
      cfg.replace(F("@ver@"),worker->getversion());
      cfg.replace(F("@nb@"), String(nb));
      configFile = LittleFS.open(filename, "w"); // PSTR("w") использовать нельзя, будет исключение!
      configFile.print(cfg);
      configFile.flush();
      configFile.close();
  }
  delay(DELAY_AFTER_FS_WRITING); // задержка после записи
}

void EffectWorker::saveeffconfig(uint16_t nb, char *folder){
  // а тут уже будем писать рабочий конфиг исходя из того, что есть в памяти
  if (LittleFS.begin()) {
      File configFile;
      String filename = geteffectpathname(nb,folder);
      configFile = LittleFS.open(filename, "w"); // PSTR("w") использовать нельзя, будет исключение!
      DynamicJsonDocument doc(2048);
      doc[F("nb")] = nb;
      doc[F("flags")] = flags.mask;
      doc[F("name")] = effectName;
      doc[F("ver")] = version;
      JsonArray arr = doc.createNestedArray(F("ctrls"));
      for (int i = 0; i < controls.size(); i++)
      {
          JsonObject var = arr.createNestedObject();
          var[F("id")]=controls[i]->getId();
          var[F("type")]=controls[i]->gettype();
          var[F("name")]=controls[i]->getName();
          var[F("val")]=controls[i]->getVal();
          var[F("min")]=controls[i]->getMin();
          var[F("max")]=controls[i]->getMax();
          var[F("step")]=controls[i]->getStep();
      }
      String cfg_str;
      serializeJson(doc, cfg_str);
      //LOG(println,cfg_str);
      configFile.print(cfg_str);
      doc.clear();
      configFile.flush();
      configFile.close();
  }
  delay(DELAY_AFTER_FS_WRITING); // задержка после записи   
}

void EffectWorker::makeIndexFile(const char *folder)
{
  if (LittleFS.begin()) {
      File indexFile;
      String idx;
      String filename;
      if (folder != nullptr) {
          filename.concat(F("/"));
          filename.concat(folder);
      }
      filename.concat(F("/eff_index.json"));

      if(LittleFS.exists(filename)){ // если индексный файл существует, то на выход
          return;
      }

      // DynamicJsonDocument doc(4096); // для списка нужно больше 2кб... будем формировать наверное ручками :)
      // JsonArray arr = doc.to<JsonArray>();
      bool firstLine = true;
      bool nameFromConfig = false;
      idx.concat(F("["));
      for (uint8_t i = ((uint8_t)EFF_ENUM::EFF_NONE+1); i < (uint8_t)EFF_ENUM::EFF_NONE_LAST; i++){ // EFF_NONE & EFF_NONE_LAST не сохраняем
          workerset(i,false); // пропускаем сохранение конфигов
          if(worker->getName()!=String(F("@name@"))){
              String cfgfilename = geteffectpathname(i, folder);
              if(!LittleFS.exists(cfgfilename)){ // если конфига эффекта не существует, создаем дефолтный
                  savedefaulteffconfig(i, cfgfilename);
                  nameFromConfig = false;
              } else { // конфиг существует, тогда читаем его
                  loadeffconfig(i, folder);
                  nameFromConfig = true;
              }

              // JsonObject var = arr.createNestedObject();
              // var[F("nb")] = i;
              // var[F("nm")] = nameFromConfig ? effectName.c_str() : worker->getName();

              if(!firstLine) idx.concat(F(",")); // собираем JSON, чтобы записать его за один раз
              idx.concat(F("{\"nb\":"));
              idx.concat(i);
              idx.concat(F(",\"fl\":"));
              idx.concat(nameFromConfig ? flags.mask : 255); // от хранения имени в индексе отказался, т.к. он становится очень большим...
              idx.concat(F("}"));

              firstLine = false; // сбрасываю признак перовой строки
          }
          ESP.wdtFeed(); // сброс вотчдога при итерациях
      }
      idx.concat(F("]"));
      indexFile = LittleFS.open(filename, "w"); // PSTR("w") использовать нельзя, будет исключение!
      indexFile.print(idx);
      // String cfg_str;
      // serializeJson(doc, cfg_str);
      // //LOG(println,cfg_str);
      // indexFile.print(cfg_str);
      // doc.clear();
      indexFile.flush();
      indexFile.close();
      // LOG(println,"");
  }
  delay(DELAY_AFTER_FS_WRITING); // задержка после записи
}

bool EffectWorker::autoSaveConfig(bool force, bool reset) {
  static unsigned long i; // getConfigSaveTimeout()
  if((i + (CFG_AUTOSAVE_TIMEOUT - 1000) > millis() || reset) && !force){  // если не пришло время - выходим из функции и сбрасываем счетчик (ожидаем бездействия в 30 секунд относительно последней записи)
      i = millis();
      return false;
  }
  LOG(printf_P,PSTR("Сохраняется конфигурация эффекта: %d\n"),curEff);
  saveeffconfig(curEff);
  //saveConfig();
  i = millis();
  return true; // сохранились
}

// конструктор копий эффектов
EffectWorker::EffectWorker(const EffectListElem* base, const EffectListElem* copy) : effects(), controls()
{
  curEff = base->eff_nb;
  if(worker==nullptr){
    workerset(base->eff_nb, false);
  }
  loadeffconfig(base->eff_nb); // вычитываем базовый конфиг
  curEff = copy->eff_nb;
  // имя формируем с базового + индекс копии
  effectName.concat(F("_"));
  effectName.concat(curEff>>8);
  saveeffconfig(copy->eff_nb); // записываем конфиг
  //updateIndexFile();
}

// конструктор текущего эффекта
EffectWorker::EffectWorker(const EffectListElem* eff, bool fast) : effects(), controls()
{
  curEff = eff->eff_nb;
  if(worker==nullptr){
    workerset(curEff, false);
  }
  if(fast){
    loadeffname(eff->eff_nb); // вычитываем только имя, если что-то не так, то используем дефолтное
  } else {
    loadeffconfig(eff->eff_nb); // вычитываем конфиг эффекта полностью, если что-то не так, то создаем все что нужно
    //updateIndexFile();
  }
  ESP.wdtFeed(); // если читается список имен эффектов перебором, то возможен эксепшен вотчдога, сбрасываем его таймер... Для есп32 надо будет этот момент отдельно поглядеть.
}

// Конструктор для отложенного эффекта, очень не желательно вызывать в цикле!
EffectWorker::EffectWorker(uint16_t delayeffnb)
{
  curEff = delayeffnb;
  if(worker==nullptr){
    workerset(curEff, false);
  }
  loadeffconfig(delayeffnb); // вычитываем конфиг эффекта полностью, если что-то не так, то создаем все что нужно
  ESP.wdtFeed(); // если читается список имен эффектов перебором, то возможен эксепшен вотчдога, сбрасываем его таймер... Для есп32 надо будет этот момент отдельно поглядеть.
}

void EffectWorker::makeIndexFileFromList(const char *folder)
{
  if (LittleFS.begin()) {
      File indexFile;
      String idx;
      String filename;
      if (folder != nullptr) {
          filename.concat(F("/"));
          filename.concat(folder);
      }
      filename.concat(F("/eff_index.json"));

      bool firstLine = true;
      idx.concat(F("["));
      for (uint8_t i = 0; i < effects.size(); i++){
              if(!firstLine) idx.concat(F(","));
              idx.concat(F("{\"nb\":"));
              idx.concat(effects[i]->eff_nb);
              idx.concat(F(",\"fl\":"));
              idx.concat(effects[i]->flags.mask);
              idx.concat(F("}"));

              firstLine = false; // сбрасываю признак первой строки
      }
      idx.concat(F("]"));
      indexFile = LittleFS.open(filename, "w"); // PSTR("w") использовать нельзя, будет исключение!
      indexFile.print(idx);
      indexFile.flush();
      indexFile.close();
  }
  LOG(println,F("Индекс эффектов обновлен!"));
  delay(DELAY_AFTER_FS_WRITING); // задержка после записи
}
// создать или обновить текущий индекс эффекта
void EffectWorker::updateIndexFile()
{
  // Я передумал, тупо пересоздадим индексный файл из списка и не морочим голову :), так будет проще и по идее надежнее, чем адресно править
  // предыдущий код пусть пока еще повисит навсякий случай
  makeIndexFileFromList();

  // if (LittleFS.begin()) {
  //     File indexFile;
  //     String idx;
  //     String filename;
  //     filename.concat(F("/eff_index.json"));

  //     if(!LittleFS.exists(filename)){ // если индексный файл не существует, то на выход
  //         return;
  //     }
  //     DynamicJsonDocument doc(4096);
      
  //     indexFile = LittleFS.open(filename, "r");
  //     idx = indexFile.readString();
  //     DeserializationError error = deserializeJson(doc,idx);

  //     if (error) {
  //         LOG(print, F("Index deserializeJson error: "));
  //         LOG(println, error.c_str()); //error.code()
  //         return;
  //     }

  //     JsonArray arr = doc.as<JsonArray>();
  //     size_t i=0;
  //     for(; i<arr.size(); i++){
  //         // ищем что проапдейтить
  //         JsonObject var = arr[i];
  //         if(var[F("nb")].as<uint16_t>()==curEff){
  //             var[F("fl")] = flags.mask;
  //         }
  //     }
  //     if(i>=arr.size()){
  //         JsonObject var = arr.createNestedObject();
  //         var[F("nb")] = curEff;
  //         var[F("fl")] = flags.mask;
  //     }
  //     idx.clear();
  //     serializeJson(doc,idx);
  //     indexFile = LittleFS.open(filename, "w");
  //     //LOG(println,idx);
  //     indexFile.print(idx);
  //     indexFile.flush();
  //     indexFile.close();
  //     doc.clear();
  // }
}

// удалить эффект из индексного файла
void EffectWorker::deleteFromIndexFile(const uint16_t effect)
{
  // Я передумал, тупо пересоздадим индексный файл из списка и не морочим голову :), так будет проще и по идее надежнее, чем адресно править
  // предыдущий код пусть пока еще повисит навсякий случай
  makeIndexFileFromList();

  // if (LittleFS.begin()) {
  //     File indexFile;
  //     String idx;
  //     String filename;
  //     filename.concat(F("/eff_index.json"));

  //     if(!LittleFS.exists(filename)){ // если индексный файл не существует, то на выход
  //         return;
  //     }
  //     DynamicJsonDocument doc(4096);
  //     indexFile = LittleFS.open(filename, "r");
  //     idx = indexFile.readString();
  //     DeserializationError error = deserializeJson(doc,idx);

  //     if (error) {
  //         LOG(print, F("Index deserializeJson error: "));
  //         LOG(println, error.c_str()); //error.code()
  //         return;
  //     }

  //     JsonArray arr = doc.as<JsonArray>();
  //     for(size_t i=0; i<arr.size(); i++){
  //         // ищем что удалить
  //         JsonObject var = arr[i];
  //         if(var[F("nb")].as<uint16_t>()==effect)
  //           arr.remove(i);
  //     }
  //     idx.clear();
  //     serializeJson(arr,idx);
  //     indexFile = LittleFS.open(filename, "w");
  //     indexFile.print(idx);
  //     indexFile.flush();
  //     indexFile.close();
  //     doc.clear();
  // }
}

// удалить эффект
void EffectWorker::deleteEffect(const EffectListElem *eff)
{
  uint16_t prevEff = EFF_ENUM::EFF_NONE;
  for(int i=0; i<effects.size(); i++){
      prevEff = effects[i]->eff_nb;
      if(effects[i]->eff_nb==eff->eff_nb){
          //deleteFromIndexFile(eff->eff_nb);
          delete effects.remove(i);
          break;
      }
  }
  directMoveBy(prevEff);
}

// копирование эффекта
void EffectWorker::copyEffect(const EffectListElem *base)
{
  EffectListElem *copy = new EffectListElem(base); // создать копию переданного эффекта
  uint16_t foundcnt=0;
  for(int i=0; i<effects.size();i++){
    if((effects[i]->eff_nb&0x00FF)==(copy->eff_nb&0x00FF)) // найдены копии
      foundcnt++;
  }
  if(foundcnt){
    copy->eff_nb=(((foundcnt+1) << 8 ) | (copy->eff_nb&0xFF)); // в старшем байте увеличиваем значение на число имеющихся копий
  }

  EffectWorker *effect = new EffectWorker(base,copy); // создать параметры для него (конфиг, индекс и т.д.)
  effects.add(copy);
  delete effect; // после того как все создано, временный экземпляр EffectWorker уже не нужен
}

// вернуть выбранный элемент списка
EffectListElem *EffectWorker::getSelectedListElement()
{ 
  EffectListElem *res = effects.size()>0? effects[0] : nullptr;
  for(int i=0; i<effects.size(); i++){
      if(effects[i]->eff_nb==selEff)
          res=effects[i];
  }
  return res;
}

// вернуть текущий элемент списка
EffectListElem *EffectWorker::getCurrentListElement()
{
  EffectListElem *res = nullptr;
  for(int i=0; i<effects.size(); i++){
      if(effects[i]->eff_nb==curEff)
          res=effects[i];
  }
  return res;
}

// вернуть выбранный элемент списка
EffectListElem *EffectWorker::getFirstEffect()
{
  if(effects.size()>0)
    return effects[0];
  else
    return nullptr; // NONE
}

// вернуть выбранный элемент списка
EffectListElem *EffectWorker::getEffect(uint16_t select){
  for (int i = 0; i < effects.size(); i++) {
      if (effects[i]->eff_nb == select) {
          return effects[i];
      }
  }
  return nullptr; // NONE
}

// вернуть следующий эффект, если передан nullptr, то возвращается первый
EffectListElem *EffectWorker::getNextEffect(EffectListElem *current){
    if(current == nullptr) return getFirstEffect();
    for (int i = 0; i < effects.size(); i++) {
        if (effects[i]->eff_nb == current->eff_nb) {
            return i+1<effects.size() ? effects[i+1] : nullptr;
        }
    }
    return nullptr; // NONE
}

// перейти на указанный
void EffectWorker::directMoveBy(uint16_t select)
{ 
  // выполняется в обход фейдера, как моментальное переключение
  setSelected(getBy(select));
  moveSelected();
}

// получить номер эффекта смещенного на количество шагов, к ближайшему большему при превышении (для DEMO)
uint16_t EffectWorker::getByCnt(byte cnt)
{
  uint16_t firstfound = curEff;
  bool found = false;
  for(int i=0; i<effects.size(); i++){
      if(curEff==effects[i]->eff_nb){
          found = true;
          continue;
      }
      if(effects[i]->isFavorite() && firstfound == curEff){
          firstfound = effects[i]->eff_nb;
      }
      if(effects[i]->isFavorite() && found  && effects[i]->eff_nb!=curEff){
            --cnt;
            if(!cnt){
                firstfound = effects[i]->eff_nb;
                break;
            }
      }
  }
  if(cnt){ // список кончился, но до сих пор не нашли... начинаем сначала
      for(int i=0; i<effects.size(); i++){
          if(effects[i]->isFavorite() && effects[i]->eff_nb!=curEff){
              --cnt;
              if(!cnt){
                  firstfound = effects[i]->eff_nb;
                  break;
              }
          }
      }
  }
  return firstfound;
}

// предыдущий эффект, кроме canBeSelected==false
uint16_t EffectWorker::getPrev()
{
  if(!isSelected()) return selEff; // если эффект в процессе смены, то возвращаем selEff

  // все индексы списка и их синхронизация - фигня ИМХО, исходим только от curEff
  uint16_t firstfound = curEff;
  bool found = false;
  for(int i=0; i<effects.size(); i++){
      if(found && firstfound!=curEff) { // нашли эффект перед собой
          break;
      } else {
          found = false; // перед текущим не нашлось подходящих, поэтому возьмем последний из canBeSelected()
      }
      if(effects[i]->eff_nb==curEff && i!=0){ // нашли себя, но не первым :)
          found = true;
          continue;
      }
      if(effects[i]->canBeSelected()){
          firstfound = effects[i]->eff_nb; // первый найденный, на случай если следующего после текущего не будет
      }
  }
  return firstfound;
  // workerset(firstfound);
  // curEff = firstfound;
}

// следующий эффект, кроме canBeSelected==false
uint16_t EffectWorker::getNext()
{
  if(!isSelected()) return selEff; // если эффект в процессе смены, то возвращаем selEff
  
  // все индексы списка и их синхронизация - фигня ИМХО, исходим только от curEff
  uint16_t firstfound = curEff;
  bool found = false;
  for(int i=0; i<effects.size(); i++){
      if(effects[i]->eff_nb==curEff){ // нашли себя
          found = true;
          continue;
      }
      if(effects[i]->canBeSelected()){
          if(firstfound == curEff)
              firstfound = effects[i]->eff_nb; // первый найденный, на случай если следующего после текущего не будет
          if(found) { // нашли эффект после себя
              firstfound = effects[i]->eff_nb;
              break;
          }
      }
  }
  return firstfound;
  // workerset(firstfound);
  // curEff = firstfound;
}

// выбор нового эффекта с отложенной сменой, на время смены эффекта читаем его список контроллов отдельно
void EffectWorker::setSelected(uint16_t effnb)
{
  if(selcontrols.size()!=controls.size() || controls.size()==0 || selcontrols[0]!=controls[0]){
    while(selcontrols.size()>0){ // очистить предыщий набор, если он только не отображен на текущий
      delete selcontrols.shift();
    }
  }

  selEff = effnb;
  //LOG(println,F("Читаю список контроллов выбранного эффекта:"));
  EffectWorker *tmpEffect = new EffectWorker(effnb);
  LList<UIControl *> fake;
  this->selcontrols = tmpEffect->controls; // копирую список контроллов, освобождать будет другой объект
  tmpEffect->controls = fake;
  delete tmpEffect;
  // for(int i=0; i<selcontrols.size(); i++){
  //   LOG(println,selcontrols[i]->getName());
  // }
}

void EffectWorker::moveSelected(){
  LOG(println,F("Синхронизация списков!"));
  workerset(selEff);
  curEff = selEff;
  clearControlsList();
  controls = selcontrols; // теперь оба списка совпадают, смена эффекта завершена
  //LOG(printf_P,PSTR("%d %d\n"),controls.size(), selcontrols.size());
}