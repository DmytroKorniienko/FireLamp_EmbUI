/*
Copyright © 2020 Dmytro Korniienko (kDn)
JeeUI2 lib used under MIT License Copyright (c) 2019 Marsel Akhkamov

    This file is part of FireLamp_EmbUI.

    FireLamp_EmbUI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FireLamp_EmbUI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FireLamp_EmbUI.  If not, see <https://www.gnu.org/licenses/>.

(Цей файл є частиною FireLamp_EmbUI.

   FireLamp_EmbUI - вільна програма: ви можете перепоширювати її та/або
   змінювати її на умовах Стандартної громадської ліцензії GNU у тому вигляді,
   у якому вона була опублікована Фондом вільного програмного забезпечення;
   або версії 3 ліцензії, або (на ваш вибір) будь-якої пізнішої
   версії.

   FireLamp_EmbUI поширюється в надії, що вона буде корисною,
   але БЕЗ ВСЯКИХ ГАРАНТІЙ; навіть без неявної гарантії ТОВАРНОГО ВИГЛЯДУ
   або ПРИДАТНОСТІ ДЛЯ ВИЗНАЧЕНИХ ЦІЛЕЙ. Докладніше див. у Стандартній
   громадська ліцензія GNU.

   Ви повинні були отримати копію Стандартної громадської ліцензії GNU
   разом із цією програмою. Якщо це не так, див.
   <https://www.gnu.org/licenses/>.)
*/
#include "main.h"
#include "effectworker.h"
#include "effects.h"




/*
 * Создаем экземпляр класса калькулятора в зависимости от требуемого эффекта
 */
void EffectWorker::workerset(uint16_t effect, const bool isCfgProceed, const bool isSkipSave){
  if(worker && isCfgProceed && !isSkipSave){ // сначала сохраним текущий эффект
    saveeffconfig(curEff); // пишем конфиг только если это требуется, для индекса - пропускаем, там свой механизм
  }
  if(worker)
     worker.reset(); // освободим явно, т.к. 100% здесь будем пересоздавать

#if defined(PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED)
  HeapSelectIram ephemeral;
#endif

  switch (static_cast<EFF_ENUM>(effect%256)) // номер может быть больше чем ENUM из-за копирований, находим эффект по модулю
  {
  case EFF_ENUM::EFF_TIME :
    worker = std::unique_ptr<EffectTime>(new EffectTime());
    break;
  case EFF_ENUM::EFF_SWIRL :
    worker = std::unique_ptr<EffectSwirl>(new EffectSwirl());
    break;
  case EFF_ENUM::EFF_COMET :
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
  case EFF_ENUM::EFF_3DNOISE :
    worker = std::unique_ptr<Effect3DNoise>(new Effect3DNoise());
    break;
  case EFF_ENUM::EFF_CELL :
    worker = std::unique_ptr<EffectCell>(new EffectCell());
    break;
  case EFF_ENUM::EFF_T_LEND :
    worker = std::unique_ptr<EffectTLand>(new EffectTLand());
    break;
  case EFF_ENUM::EFF_OSCIL :
    worker = std::unique_ptr<EffectOscilator>(new EffectOscilator());
    break;
  case EFF_ENUM::EFF_WRAIN : 
    worker = std::unique_ptr<EffectWrain>(new EffectWrain());
    break;
  case EFF_ENUM::EFF_FAIRY : 
  case EFF_ENUM::EFF_FOUNT :
    worker = std::unique_ptr<EffectFairy>(new EffectFairy());
    break;
  case EFF_ENUM::EFF_CIRCLES :
    worker = std::unique_ptr<EffectCircles>(new EffectCircles());
    break;
  case EFF_ENUM::EFF_DRIFT :
    worker = std::unique_ptr<EffectDrift>(new EffectDrift());
    break;
  case EFF_ENUM::EFF_POPCORN :
    worker = std::unique_ptr<EffectPopcorn>(new EffectPopcorn());
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
  case EFF_ENUM::EFF_BENGALL :
    worker = std::unique_ptr<EffectBengalL>(new EffectBengalL());
    break;
  case EFF_ENUM::EFF_BALLS :
    worker = std::unique_ptr<EffectBalls>(new EffectBalls());
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
  case EFF_ENUM::EFF_PICASSO :
  case EFF_ENUM::EFF_PICASSO4 :
    worker = std::unique_ptr<EffectPicasso>(new EffectPicasso());
    break;
  case EFF_ENUM::EFF_STARSHIPS :
    worker = std::unique_ptr<EffectStarShips>(new EffectStarShips());
    break;
  case EFF_ENUM::EFF_FLAGS :
    worker = std::unique_ptr<EffectFlags>(new EffectFlags());
    break;
  case EFF_ENUM::EFF_LEAPERS :
    worker = std::unique_ptr<EffectLeapers>(new EffectLeapers());
    break;
  case EFF_ENUM::EFF_LIQUIDLAMP :
    worker = std::unique_ptr<EffectLiquidLamp>(new EffectLiquidLamp());
    break;
  case EFF_ENUM::EFF_AQUARIUM :
    worker = std::unique_ptr<EffectAquarium>(new EffectAquarium());
    break;
  case EFF_ENUM::EFF_WHIRL :
    worker = std::unique_ptr<EffectWhirl>(new EffectWhirl());
    break;
  case EFF_ENUM::EFF_STAR :
    worker = std::unique_ptr<EffectStar>(new EffectStar());
    break;
  case EFF_ENUM::EFF_FIREWORKS :
    worker = std::unique_ptr<EffectFireworks>(new EffectFireworks());
    break;
  case EFF_ENUM::EFF_PACIFIC :
    worker = std::unique_ptr<EffectPacific>(new EffectPacific());
    break;
  case EFF_ENUM::EFF_MUNCH :
    worker = std::unique_ptr<EffectMunch>(new EffectMunch());
    break;
  case EFF_ENUM::EFF_NOISE :
    worker = std::unique_ptr<EffectNoise>(new EffectNoise());
    break;
  case EFF_ENUM::EFF_BUTTERFLY :
    worker = std::unique_ptr<EffectButterfly>(new EffectButterfly());
    break;
  case EFF_ENUM::EFF_SHADOWS :
    worker = std::unique_ptr<EffectShadows>(new EffectShadows());
    break;
  case EFF_ENUM::EFF_PATTERNS :
    worker = std::unique_ptr<EffectPatterns>(new EffectPatterns());
    break;
  case EFF_ENUM::EFF_ARROWS :
    worker = std::unique_ptr<EffectArrows>(new EffectArrows());
    break;
  case EFF_ENUM::EFF_NBALLS :
    worker = std::unique_ptr<EffectNBals>(new EffectNBals());
    break;
  case EFF_ENUM::EFF_ATTRACT :
    worker = std::unique_ptr<EffectAttract>(new EffectAttract());
    break;
  case EFF_ENUM::EFF_SNAKE :
    worker = std::unique_ptr<EffectSnake>(new EffectSnake());
    break;
  case EFF_ENUM::EFF_NEXUS :
    worker = std::unique_ptr<EffectNexus>(new EffectNexus());
    break;
  case EFF_ENUM::EFF_MAZE :
    worker = std::unique_ptr<EffectMaze>(new EffectMaze());
    break;
  case EFF_ENUM::EFF_FRIZZLES :
    worker = std::unique_ptr<EffectFrizzles>(new EffectFrizzles());
    break;
  case EFF_ENUM::EFF_POLARL :
    worker = std::unique_ptr<EffectPolarL>(new EffectPolarL());
    break;
  case EFF_ENUM::EFF_FLOWER :
    worker = std::unique_ptr<EffectFlower>(new EffectFlower());
    break;
  case EFF_ENUM::EFF_TEST :
    worker = std::unique_ptr<EffectTest>(new EffectTest());
    break;
   case EFF_ENUM::EFF_SMOKEBALLS :
    worker = std::unique_ptr<EffectSmokeballs>(new EffectSmokeballs());
    break;
   case EFF_ENUM::EFF_RACER :
    worker = std::unique_ptr<EffectRacer>(new EffectRacer());
    break;
   case EFF_ENUM::EFF_MAGMA :
    worker = std::unique_ptr<EffectMagma>(new EffectMagma());
    break;
   case EFF_ENUM::EFF_FIRE2021 :
    worker = std::unique_ptr<EffectFire2021>(new EffectFire2021());
    break;
   case EFF_ENUM::EFF_PUZZLES :
    worker = std::unique_ptr<EffectPuzzles>(new EffectPuzzles());
    break;
   case EFF_ENUM::EFF_PILE :
    worker = std::unique_ptr<EffectPile>(new EffectPile());
    break;
   case EFF_ENUM::EFF_DNA :
    worker = std::unique_ptr<EffectDNA>(new EffectDNA());
    break;
   case EFF_ENUM::EFF_SMOKER :
    worker = std::unique_ptr<EffectSmoker>(new EffectSmoker());
    break;
  case EFF_ENUM::EFF_MIRAGE :
    worker = std::unique_ptr<EffectMirage>(new EffectMirage());
    break;
  case EFF_ENUM::EFF_WATERCOLORS :
    worker = std::unique_ptr<EffectWcolor>(new EffectWcolor());
    break;
  case EFF_ENUM::EFF_FIRE :
    worker = std::unique_ptr<EffectRadialFire>(new EffectRadialFire());
    break;
  case EFF_ENUM::EFF_SPBALS :
    worker = std::unique_ptr<EffectSplashBals>(new EffectSplashBals());
    break;
#ifdef RGB_PLAYER
  case EFF_ENUM::EEF_RGBPLAYER :
    worker = std::unique_ptr<EffectPlayer>(new EffectPlayer());
    break;
#endif
#ifdef MIC_EFFECTS
  case EFF_ENUM::EFF_VU :
    worker = std::unique_ptr<EffectVU>(new EffectVU());
    break;
  case EFF_ENUM::EFF_OSC :
    worker = std::unique_ptr<EffectOsc>(new EffectOsc());
    break;

#endif

  default:
    worker = std::unique_ptr<EffectNone>(new EffectNone()); // std::unique_ptr<EffectCalc>(new EffectCalc());
  }

  if(worker){
    worker->pre_init(static_cast<EFF_ENUM>(effect%256), this, &(getControls()), lampstate);
    originalName = effectName = FPSTR(T_EFFNAMEID[(uint8_t)effect]); // сначла заполним дефолтным именем, а затем лишь вычитаем из конфига
    if(isCfgProceed){ // читаем конфиг только если это требуется, для индекса - пропускаем
      loadeffconfig(effect);
      // окончательная инициализация эффекта тут
      worker->init(static_cast<EFF_ENUM>(effect%256), &(getControls()), lampstate);
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

void EffectWorker::initDefault(const char *folder)
{
  LOG(println,F("initDefault"));

#ifndef ESP8266
  static const char *dirs[] = {TCONST_005A,TCONST_002B,TCONST_0033,TCONST_0032,TCONST_0031,TCONST_002C,TCONST_0078};
  for(int i=0; i<sizeof(dirs)/sizeof(const char *);i++){
    String dirname = FPSTR(dirs[i]);
    dirname.remove(dirname.length()-1); // remove last '/'
    if(!LittleFS.exists(dirname)){
      LittleFS.mkdir(dirname);
      LOG(printf_P,PSTR("%s %s\n"), FPSTR(TCONST_00D6), dirname.c_str());
    }
  }
#endif

  String filename;
  if(folder && folder[0])
    filename = folder;
  else
    filename = FPSTR(TCONST_0059);

  const uint16_t buffersize = 32;
  char storage[buffersize];
  DynamicJsonDocument doc(buffersize*2+32);
  size_t size;
  uint32_t effcnt = 0, ntry=0;
  DeserializationError error;

  clearEffectList();
  do {
    File idx = LittleFS.open(filename,"r");
    //idx.seek(0, SeekMode::SeekSet);
    while((size = idx.readBytesUntil('\n',storage,buffersize))){
#ifdef ESP8266
      ESP.wdtFeed();
#elif defined ESP32
      delay(1);
#endif
      if(size<=3) continue;
      if(size && storage[size-1]==0x7D){
        storage[size]=0x00;
      }
      if(size && storage[0]==','){
        storage[0]=' ';
      }
      //LOG(println, storage);
      error = deserializeJson(doc, storage);
      if(!error){
        //LOG(printf_P, PSTR("%d - %d\n"), doc[F("n")].as<uint16_t>(), doc[F("f")].as<uint8_t>());
        effects.add(new EffectListElem(doc[F("n")].as<uint16_t>(), doc[F("f")].as<uint8_t>()));
        effcnt++;
      } else {
        LOG(printf_P, PSTR("DeserializeJson error: %d - %s\n"), error.code(), storage);
      }
      doc.clear(); doc.garbageCollect();
    }
    if(!effcnt){
      if (LittleFS.begin() && LittleFS.exists(filename))
        LittleFS.remove(filename); // пересоздаем индекс и пробуем снова
      if(ntry<2 && LittleFS.exists(FPSTR(TCONST_005A)))
        makeIndexFileFromFS(NULL,NULL,true);
      else
        makeIndexFile();
    }
    ntry++;
    idx.close();
  } while(!effcnt && ntry<3);
  if(!effcnt){
    LOG(println, F("Effect index currupted!"));
    return;
  } else {
    LOG(printf_P, PSTR("Found index for %d effects\n"),effcnt);
  }

  //LOG(printf_P,PSTR("Создаю список эффектов конструктор (%d): %s\n"),arr.size(),idx.c_str());
  effects.sort([](EffectListElem *&a, EffectListElem *&b){ return a->eff_nb - b->eff_nb;}); // сортирую по eff_nb
  int32_t chk = -1; // удаляю дубликаты
  for(int i=0; i<effects.size(); i++){
    if((int32_t)effects[i]->eff_nb==chk){
          delete effects.remove(i);
          i--;
          continue;
        }
    chk = effects[i]->eff_nb;
  }

  // for(int i=0; i<effects.size(); i++){
  //   LOG(println,effects[i]->eff_nb);
  // }

  effectsReSort();
}

void EffectWorker::removeConfig(const uint16_t nb, const char *folder)
{
  String filename = geteffectpathname(nb,folder);
  LOG(printf_P,PSTR("Remove from FS: %s\n"), filename.c_str());
  saveeffconfig(nb,folder,true); // clear
  if (isemptyconfig(nb,folder))
    LittleFS.remove(filename); // удаляем файл
}

void EffectWorker::effectsReSort(SORT_TYPE _effSort)
{
  LOG(printf_P,PSTR("*Пересортировка эффектов*: %d\n"), _effSort);
  if(_effSort==255) _effSort=effSort; // Для дефолтного - берем с конфига

  switch(_effSort){
    case SORT_TYPE::ST_BASE :
      effects.sort([](EffectListElem *&a, EffectListElem *&b){ return (((a->eff_nb&0xFF) - (b->eff_nb&0xFF))<<8) + (((a->eff_nb&0xFF00) - (b->eff_nb&0xFF00))>>8);});
      break;
    case SORT_TYPE::ST_END :
      effects.sort([](EffectListElem *&a, EffectListElem *&b){ return a->eff_nb - b->eff_nb;}); // сортирую по eff_nb
      //effects.sort([](EffectListElem *&a, EffectListElem *&b){ return ((int32_t)(((a->eff_nb&0xFF)<<8) | ((a->eff_nb&0xFF00)>>8)) - (((b->eff_nb&0xFF)<<8) | ((b->eff_nb&0xFF00)>>8)));});
      //effects.sort([](EffectListElem *&a, EffectListElem *&b){ return (a->eff_nb&0xFF00) - (b->eff_nb&0xFF00) + (((a->eff_nb&0xFF) - (b->eff_nb&0xFF))<<8) + (((a->eff_nb&0xFF00) - (b->eff_nb&0xFF00))>>8);});
      break;
    case SORT_TYPE::ST_IDX :
      effects.sort([](EffectListElem *&a, EffectListElem *&b){ return (int)(a->getMS() - b->getMS());});
      break;
    case SORT_TYPE::ST_AB2 :
      // крайне медленный вариант, с побочными эффектами, пока отключаю и использую вместо него ST_AB
      //effects.sort([](EffectListElem *&a, EffectListElem *&b){ EffectWorker *tmp = new EffectWorker((uint16_t)0); String tmp1; tmp->loadeffname(tmp1, a->eff_nb); String tmp2; tmp->loadeffname(tmp2,b->eff_nb); delete tmp; return strcmp_P(tmp1.c_str(), tmp2.c_str());});
      //break;
    case SORT_TYPE::ST_AB :
      effects.sort([](EffectListElem *&a, EffectListElem *&b){ String tmp=FPSTR(T_EFFNAMEID[(uint8_t)a->eff_nb]); return strcmp_P(tmp.c_str(), (T_EFFNAMEID[(uint8_t)b->eff_nb]));});
      break;
#ifdef MIC_EFFECTS
    case SORT_TYPE::ST_MIC :
      effects.sort([](EffectListElem *&a, EffectListElem *&b){ return ((int)(pgm_read_byte(T_EFFVER + (a->eff_nb&0xFF))&0x01) - (int)(pgm_read_byte(T_EFFVER + (b->eff_nb&0xFF))&0x01)); });
      break;
#endif
    default:
      break;
  }
}

/**
 * вычитать только имя эффекта из конфиг-файла и записать в предоставленную строку
 * в случае отсутствия/повреждения взять имя эффекта из флеш-таблицы, если есть
 * @param effectName - String куда записать результат
 * @param nb  - айди эффекта
 * @param folder - какой-то префикс для каталога
 */
void EffectWorker::loadeffname(String& _effectName, const uint16_t nb, const char *folder)
{
  String filename = geteffectpathname(nb,folder);
  const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
  //const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
  DynamicJsonDocument doc(bufsize*2);
  bool ok = deserializeFile(doc, filename.c_str(),nb);
  if (ok && doc[F("name")]){
    _effectName = doc[F("name")].as<String>(); // перенакрываем именем из конфига, если есть
  } else if(!ok) {
    _effectName = FPSTR(T_EFFNAMEID[(uint8_t)nb]);   // выбираем имя по-умолчанию из флеша если конфиг поврежден
  }
}

/**
* вычитать только имя\путь звука из конфиг-файла и записать в предоставленную строку
* в случае отсутствия/повреждения возвращает пустую строку
* @param effectName - String куда записать результат
* @param nb  - айди эффекта
* @param folder - какой-то префикс для каталога
*/
void EffectWorker::loadsoundfile(String& _soundfile, const uint16_t nb, const char *folder)
{
  String filename = geteffectpathname(nb,folder);
  const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
  //const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
  DynamicJsonDocument doc(bufsize*2);
  bool ok = deserializeFile(doc, filename.c_str(),nb);
  LOG(printf_P,PSTR("snd: %s\n"),doc[F("snd")].as<String>().c_str());
  if (ok && doc[F("snd")]){
    _soundfile = doc[F("snd")].as<String>(); // перенакрываем именем из конфига, если есть
  } else if(!ok) {
    _soundfile = "";
  }
}

/**
 *  метод загружает и пробует десериализовать джейсон из файла в предоставленный документ,
 *  возвращает true если загрузка и десериализация прошла успешно
 *  @param doc - DynamicJsonDocument куда будет загружен джейсон
 *  @param jsonfile - файл, для загрузки
 */
bool EffectWorker::deserializeFile(DynamicJsonDocument& doc, const char* filepath, int32_t nb){
  if (!filepath || !*filepath)
    return false;

  File jfile = LittleFS.open(filepath, "r");
  const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
  const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
  const uint32_t pos=nb>255?(((nb>>8)-1)%nbfiles):((nb&0xFF)%nbfiles);
  if(nb!=-1){
    jfile.seek(bufsize*pos, SeekMode::SeekSet);
  }

  DeserializationError error;
  if (jfile){
    error = deserializeJson(doc, jfile);
    jfile.close();
  } else {
    return false;
  }

  if (error) {
    if(error && ((error.code()>DeserializationError::EmptyInput) || (error.code()==DeserializationError::EmptyInput && nb==-1))) {
      LOG(printf_P, PSTR("File: failed to load json file: %s, deserializeJson error: "), filepath);
      LOG(println, error.code());
    }
    return false;
  }
  //LOG(printf_P,PSTR("File: %s deserialization took %d ms\n"), filepath, millis() - timest);
  return true;
}

int EffectWorker::loadeffconfig(const uint16_t nb, const char *folder)
{
  if(worker==nullptr){
    return 1;   // ошибка
  }

  const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
  //const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
  String filename = geteffectpathname(nb,folder);
  DynamicJsonDocument doc(bufsize*2);
  READALLAGAIN:

  if (!deserializeFile(doc, filename.c_str(), nb)){
    doc.clear();
    savedefaulteffconfig(nb, filename, true);   // пробуем перегенерировать поврежденный конфиг
    if (!deserializeFile(doc, filename.c_str(), nb))
      return 1;   // ошибка и в файле и при попытке сгенерить конфиг по-умолчанию - выходим
  }

  version = doc[F("ver")].as<uint8_t>();
  if(geteffcodeversion((uint8_t)nb) != version && nb<=255){ // только для базовых эффектов эта проверка
      doc.clear();
      LOG(printf_P, PSTR("Wrong version of effect, rewrite with default (%d vs %d)\n"), version, geteffcodeversion((uint8_t)nb));
      savedefaulteffconfig(nb, filename, true);
      goto READALLAGAIN;
  }

  curEff = doc[F("nb")].as<uint16_t>();
  //flags.mask = doc.containsKey(F("flags")) ? doc[F("flags")].as<uint8_t>() : 255;
  const char* name = doc[F("name")];
  effectName = name ? name : (String)(FPSTR(T_EFFNAMEID[(uint8_t)nb]));
  soundfile = doc.containsKey(F("snd")) ? doc[F("snd")].as<String>() : "";
  //LOG(printf_P, PSTR("Load MEM: %s - CFG: %s - DEF: %s\n"), effectName.c_str(), doc[F("name")].as<String>().c_str(), worker->getName().c_str());
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
              : id == 0 ? String(FPSTR(TINTF_00D))
              : id == 1 ? String(FPSTR(TINTF_087))
              : id == 2 ? String(FPSTR(TINTF_088))
              : String(F("Доп."))+String(id);
          String val = item.containsKey(F("val")) ? item[F("val")].as<String>() : String(1);
          String min = item.containsKey(F("min")) && id>2 ? item[F("min")].as<String>() : String(1);
          String max = item.containsKey(F("max")) && id>2 ? item[F("max")].as<String>() : String(255);
          String step = item.containsKey(F("step")) && id>2 ?  item[F("step")].as<String>() : String(1);
          CONTROL_TYPE type = item[F("type")].as<CONTROL_TYPE>();
          type = ((type & 0x0F)!=CONTROL_TYPE::RANGE) && id<3 ? CONTROL_TYPE::RANGE : type;
          min = ((type & 0x0F)==CONTROL_TYPE::CHECKBOX) ? "0" : min;
          max = ((type & 0x0F)==CONTROL_TYPE::CHECKBOX) ? "1" : max;
          step = ((type & 0x0F)==CONTROL_TYPE::CHECKBOX) ? "1" : step;
          controls.add(new UIControl(
              id,             // id
              type,           // type
              name,           // name
              val,            // value
              min,            // min
              max,            // max
              step            // step
          ));
          //LOG(printf_P,PSTR("%d %d %s %s %s %s %s\n"), id, type, name.c_str(), val.c_str(), min.c_str(), max.c_str(), step.c_str());
      }
  }
  doc.clear();
  // тест стандартных контроллов
  for(int8_t id=0;id<3;id++){
      if(!((id_tst>>id)&1)){ // не найден контрол, нужно создать
          controls.add(new UIControl(
              id,                                     // id
              CONTROL_TYPE::RANGE,                    // type
              id==0 ? FPSTR(TINTF_00D) : id==1 ? FPSTR(TINTF_087) : FPSTR(TINTF_088),           // name
              String(127),                            // value
              String(1),                              // min
              String(255),                            // max
              String(1)                               // step
          ));
      }
  }
  controls.sort([](UIControl *&a, UIControl *&b){ return a->getId() - b->getId();}); // сортирую по id
  return 0; // успешно
}

const String EffectWorker::geteffectpathname(const uint16_t nb, const char *folder){
  //const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
  const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
  uint16_t swapnb = (nb>>8)?(((nb>>8)-1)/nbfiles+1)|(((nb&0xFF)/nbfiles)<<8):((nb/nbfiles)<<8); // меняю местами 2 байта, так чтобы копии/версии эффекта оказалась в имени файла позади
  uint8_t fnb = (nb>>8)?((nb&0xFF)%nbfiles):0x00; // префікс для випадку коли декілька ефектів в однієму файлі (лише для копій ефектів)
  String filename;
  char buffer[8];
  if (folder) {
      filename.concat(F("/"));
      filename.concat(folder);
  }
  filename.concat(FPSTR(TCONST_005A));
  sprintf_P(buffer,PSTR("e%02x%04x"), fnb, swapnb);
  filename.concat(buffer);
  filename.concat(F(".json"));
  return filename;
}

void EffectWorker::savedefaulteffconfig(uint16_t nb, const String &filename, bool force){

  const String efname(FPSTR(T_EFFNAMEID[(uint8_t)nb])); // выдергиваем имя эффекта из таблицы
  LOG(printf_P,PSTR("Make default config: %d %s %s\n"), nb, efname.c_str(), filename.c_str());

  String  cfg(FPSTR(T_EFFUICFG[(uint8_t)nb]));    // извлекаем конфиг для UI-эффекта по-умолчанию из флеш-таблицы
  cfg.replace(F("@name@"), efname);
  cfg.replace(F("@ver@"), String(geteffcodeversion((uint8_t)nb)) );
  cfg.replace(F("@nb@"), String(nb));
  
  if(LittleFS.begin()){
    // якщо нема, то создаємо розміром 8000 байтів по 2000 байт на ефект
    const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
    const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
    const uint32_t pos=nb>255?(((nb>>8)-1)%nbfiles):((nb&0xFF)%nbfiles);
    uint8_t *buffer = new uint8_t[bufsize];
    memset(buffer,0,bufsize);
    if(!LittleFS.exists(filename)){
      File configFile = LittleFS.open(filename, "w");
#ifdef ESP8266
      configFile.truncate(bufsize*nbfiles);
#endif
      configFile.seek(bufsize*pos, SeekMode::SeekSet);
      sprintf((char *)buffer, "%s",cfg.c_str());
      configFile.write(buffer,bufsize);
      configFile.close();
    } else {
      File configFile = LittleFS.open(filename, "r+");
#ifdef ESP8266
      if(configFile.size()!=bufsize*nbfiles)
        configFile.truncate(bufsize*nbfiles);
#endif
      if(!force){
        configFile.seek(bufsize*pos, SeekMode::SeekSet);
        configFile.read(buffer,bufsize);
      }
      configFile.seek(bufsize*pos, SeekMode::SeekSet);
      //LOG(printf_P,PSTR("pos: %d, char: %d\n"), configFile.position(), buffer[0]);
      if(buffer[0]!='{' || force){
        sprintf((char *)buffer, "%s",cfg.c_str());
        configFile.write(buffer,bufsize);
      }
      configFile.close();
    }
    delete[] buffer;
  }
}

String EffectWorker::getfseffconfig(uint16_t nb)
{
  String cfg_str;
  String filename = geteffectpathname(nb);
  const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
  const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
  const uint32_t pos=nb>255?(((nb>>8)-1)%nbfiles):((nb&0xFF)%nbfiles);
  File jfile = LittleFS.open(filename, "r");
  if(nb!=-1){
    jfile.seek(bufsize*pos, SeekMode::SeekSet);
  }
  if(jfile)
    cfg_str = jfile.readStringUntil(0);//jfile.readString();
  jfile.close();

  //LOG(println,cfg_str);
  return cfg_str;
}

void EffectWorker::geteffconfig(uint16_t nb, uint8_t replaceBright, char *buffer)
{
  // конфиг текущего эффекта
  String storage;
  geteffconfig(nb,replaceBright,storage);
  snprintf((char *)buffer,EFF_BUFFER_SIZE,"%s",storage.c_str());
}

String &EffectWorker::geteffconfig(uint16_t nb, uint8_t replaceBright, String &str)
{
  // конфиг текущего эффекта
  const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
  //const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
  DynamicJsonDocument doc(bufsize*2);
  EffectListElem *eff = getEffect(nb);
  EffectWorker *tmp=this;
  bool isFader = (curEff != nb);
  if(isFader){ // работает фейдер, нужен новый экземпляр
#if defined(PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED)
    HeapSelectIram ephemeral;
#endif
    tmp=new EffectWorker(eff);
  }

  doc[F("nb")] = nb;
  doc[F("flags")] = eff ? eff->flags.mask : SET_ALL_EFFFLAGS;
  doc[F("name")] = tmp->effectName;
  doc[F("ver")] = tmp->version;
  doc[F("snd")] = tmp->soundfile;
  JsonArray arr = doc.createNestedArray(F("ctrls"));
  for (int i = 0; i < tmp->controls.size(); i++) {
    JsonObject var = arr.createNestedObject();
    var[F("id")]=tmp->controls[i]->getId();
    var[F("type")]=tmp->controls[i]->getType();
    var[F("name")]=tmp->controls[i]->getName();
    var[F("val")]=(tmp->controls[i]->getId()==0 && replaceBright) ? String(replaceBright) : tmp->controls[i]->getVal();
    var[F("min")]=tmp->controls[i]->getMin();
    var[F("max")]=tmp->controls[i]->getMax();
    var[F("step")]=tmp->controls[i]->getStep();
  }
  if(tmp!=this)
    delete tmp;

  serializeJson(doc, str);
  doc.clear();
  return str;
}

bool EffectWorker::isemptyconfig(uint16_t nb, const char *folder){
  String filename = geteffectpathname(nb,folder);
  const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
  const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
  //const uint32_t pos=nb>255?(((nb>>8)-1)%nbfiles):((nb&0xFF)%nbfiles);
  File configFile;
  configFile = LittleFS.open(filename, "r");
  int8_t cnt=0;
  for(uint8_t i=0; i<nbfiles; i++){
      uint8_t chk;
      configFile.seek(bufsize*i, SeekMode::SeekSet);
      configFile.read(&chk,1);
      //LOG(printf_P,PSTR("pos: %d, char: %d\n"), configFile.position(), buffer[0]);
      if(chk!='{'){
        cnt++;
      }
  }
  configFile.close();
  if(cnt==nbfiles) return true; else return false;
}

void EffectWorker::saveeffconfig(uint16_t nb, const char *folder, bool clear){
  // а тут уже будем писать рабочий конфиг исходя из того, что есть в памяти
  if(millis()<10000) return; // в первые десять секунд после рестарта запрещаем запись
  if(tConfigSave)
    tConfigSave->cancel(); // если оказались здесь, и есть отложенная задача сохранения конфига, то отменить ее
  
  String filename = geteffectpathname(nb,folder);
  const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
  const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
  const uint32_t pos=nb>255?(((nb>>8)-1)%nbfiles):((nb&0xFF)%nbfiles);
  uint8_t *buffer = new uint8_t[bufsize];
  memset(buffer,0,bufsize);
  File configFile;
  if(LittleFS.exists(filename))
    configFile = LittleFS.open(filename, "r+");
  else
    configFile = LittleFS.open(filename, "w");
#ifdef ESP8266
  if(configFile.size()!=bufsize*nbfiles)
    configFile.truncate(bufsize*nbfiles);
#endif
  configFile.seek(bufsize*pos, SeekMode::SeekSet);
  if(!clear)
    geteffconfig(nb,0,(char *)buffer);
  configFile.write(buffer,bufsize);
  configFile.close();
  delete[] buffer;
}

/**
 * проверка на существование "дефолтных" конфигов для всех статичных эффектов
 *
 */
void recreateoptionsTask(bool isCancelOnly);
void EffectWorker::chkdefconfigs(const char *folder){
//   uint16_t tst = 0;
//   for (uint16_t i = ((uint16_t)EFF_ENUM::EFF_NONE); i < (uint16_t)256; i++){ // всего 254 базовых эффекта, 0 - служебный, 255 - последний
//     if (!strlen_P(T_EFFNAMEID[i]) && i!=0)   // пропускаем индексы-"пустышки" без названия, кроме EFF_NONE
//       continue;

// #ifndef MIC_EFFECTS
//     if(i>EFF_ENUM::EFF_TIME) continue; // пропускаем эффекты для микрофона, если отключен микрофон
// #endif
//     String cfgfilename = geteffectpathname(i, folder);
//     if(!LittleFS.exists(cfgfilename) || tst>i){ // если конфига эффекта не существует, создаем дефолтный
//       savedefaulteffconfig(i, cfgfilename);
//       if(!tst)
//         tst = i + EFF_NB_PER_FILE;
//     } else {
//       tst = 0;
//     }
// #ifdef ESP8266
//     ESP.wdtFeed();
// #endif
//   }

  DynamicJsonDocument doc(512);
  JsonObject data = doc.to<JsonObject>();
  data[FPSTR(TCONST_00EF)] = folder ? folder : "";
  data[FPSTR(TCONST_00F0)] = EFF_ENUM::EFF_NONE;

  LOG(println,F("Create chkdefconfigs task"));
  String scurEff = embui.param(FPSTR(TCONST_0016)); // текущий создадим вне очереди
  uint16_t ce = scurEff.toInt();
  String cfgfilename = geteffectpathname(ce, folder);
  if(getfseffconfig(ce).isEmpty()){ // если конфига эффекта не существует, создаем дефолтный
    savedefaulteffconfig(ce, cfgfilename, true);
  }

  Task *_t = new JsonTask(&data, 1000, TASK_FOREVER, [this](){
    JsonTask *task = (JsonTask *)ts.getCurrentTask();
    JsonObject storage = task->getData();
    JsonObject *data = &storage; // task->getData();

    //LOG(println,F("chkdefconfigs task"));

    String folder=(*data)[FPSTR(TCONST_00EF)];
    uint16_t i=(*data)[FPSTR(TCONST_00F0)].as<uint16_t>();

    if(!(i < (uint16_t)256)){
      //LOG(println,F("chkdefconfigs task canceled"));
      task->cancel();
      return;
    }

    uint16_t chk=i+1;
    while(!strlen_P(T_EFFNAMEID[chk]) && chk<256){   // пропускаем индексы-"пустышки" без названия, кроме EFF_NONE
      chk++;
    }
    (*data)[FPSTR(TCONST_00F0)]=chk;
#ifndef MIC_EFFECTS
    if(i>=254U) {return;} // пропускаем эффекты для микрофона, если отключен микрофон EFF_ENUM::EFF_VU +
#endif
#ifndef RGB_PLAYER
    if(i==251U) {return;} // пропускаем плеер, если отключен
#endif
    String cfgfilename = geteffectpathname(i, folder.isEmpty() ? NULL : folder.c_str());
    if((strlen_P(T_EFFNAMEID[i]) && getfseffconfig(i).isEmpty()) || !i){ // если конфига эффекта не существует или 0 эффект, создаем дефолтный
      recreateoptionsTask(true);
      savedefaulteffconfig(i, cfgfilename, true);
    } else {
      i=chk;
      while(!getfseffconfig(i).isEmpty() && i<256 && i<=chk+10){ // знайти наступний порожній
#ifdef ESP8266
        ESP.wdtFeed(); // если читается список имен эффектов перебором, то возможен эксепшен вотчдога, сбрасываем его таймер...
#elif defined ESP32
        delay(1);
#endif
        recreateoptionsTask(true);
        //LOG(println,i);
        i++;
      }
      (*data)[FPSTR(TCONST_00F0)]=i;
    }
  }, &ts, false, nullptr, [this](){TASK_RECYCLE;});
  _t->enable();
}

void EffectWorker::autoSaveConfig(bool force) {
    if (force){
        if(tConfigSave)
          tConfigSave->cancel();
        saveeffconfig(curEff);
        fsinforenew();
        LOG(printf_P,PSTR("Force save effect config: %d\n"), curEff);
    } else {
        if(!tConfigSave){ // task for delayed config autosave
          tConfigSave = new Task(CFG_AUTOSAVE_TIMEOUT, TASK_ONCE, [this](){
            saveeffconfig(curEff);
            fsinforenew();
            LOG(printf_P,PSTR("Autosave effect config: %d\n"), curEff);
          }, &ts, false, nullptr, [this](){TASK_RECYCLE; tConfigSave=nullptr;});
          tConfigSave->enableDelayed();
        } else {
          tConfigSave->restartDelayed();
        }
    }
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
    loadeffname(effectName, eff->eff_nb); // вычитываем только имя, если что-то не так, то используем дефолтное
  } else {
    loadeffconfig(eff->eff_nb); // вычитываем конфиг эффекта полностью, если что-то не так, то создаем все что нужно
    //updateIndexFile();
  }
}

// Конструктор для отложенного эффекта, очень не желательно вызывать в цикле!
EffectWorker::EffectWorker(uint16_t delayeffnb)
{
  curEff = delayeffnb;
  if(worker==nullptr){
    workerset(curEff, false);
  }
  loadeffconfig(delayeffnb); // вычитываем конфиг эффекта полностью, если что-то не так, то создаем все что нужно
#ifdef ESP8266
  ESP.wdtFeed(); // если читается список имен эффектов перебором, то возможен эксепшен вотчдога, сбрасываем его таймер...
#elif defined ESP32
  delay(1);
#endif
}

/**
 * процедура открывает индекс-файл на запись в переданный хендл,
 * возвращает хендл
 */
File& EffectWorker::openIndexFile(File& fhandle, const char *folder){

  String filename;

  if (folder && folder[0]){ // если указан каталог и первый символ не пустой, то берем как есть
    filename.concat(folder);
    LOG(print, F("index:"));
    LOG(println, filename.c_str());
  } else
    filename.concat(FPSTR(TCONST_0059));
  
  fhandle = LittleFS.open(filename, "w");
  return fhandle;
}


/**
 *  процедура содания индекса "по-умолчанию" на основе "вшитых" в код enum/имен эффектов
 *
 */
void EffectWorker::makeIndexFile(const char *folder, const bool forcechkdef)
{
  uint32_t timest = millis();

  // LittleFS глючит при конкуретном доступе к файлам на запись, разносим разношерстные операции во времени
  // сначала проверяем наличие дефолтных конфигов для всех эффектов
  if(!LittleFS.exists(FPSTR(TCONST_005A)) || forcechkdef)
    chkdefconfigs(folder);

  File indexFile;

  LOG(println, F("Rebuilding Index file..."));
  bool firstLine = true;
  openIndexFile(indexFile, folder);
  indexFile.print(F("[\n"));

  EffectListElem *eff;
  uint8_t flags = SET_ALL_EFFFLAGS;
  for (uint16_t i = ((uint16_t)EFF_ENUM::EFF_NONE); i < (uint16_t)256; i++){
    if (!strlen_P(T_EFFNAMEID[i]) && i!=0)   // пропускаем индексы-"пустышки" без названия, кроме 0 "EFF_NONE"
      continue;

#ifndef RGB_PLAYER
    if(i==251U) continue; // пропускаем плеер, если отключен
#endif
#ifndef MIC_EFFECTS
    if(i>254U) continue; // пропускаем эффекты для микрофона, если отключен микрофон
#endif

    eff = getEffect(i);
    if(eff)
      flags = eff->flags.mask;

    indexFile.printf_P(PGidxtemplate, firstLine ? " " : ",", i, i ? flags : 0); // дефолтный флаг SET_ALL_EFFFLAGS для любого эффекта, кроме 0
    firstLine = false; // сбрасываю признак перовой строки
#ifdef ESP8266
    ESP.wdtFeed();
#elif defined ESP32
    delay(1);
#endif
  }
  indexFile.print("]");
  indexFile.close();

  LOG(printf_P,PSTR("rebuilding took %ld ms\n"), millis() - timest);

}

void EffectWorker::removeLists(){
  LOG(println, F("removeLists"));
  LittleFS.remove(FPSTR(TCONST_0082));
  LittleFS.remove(FPSTR(TCONST_0083));
  LittleFS.remove(FPSTR(TCONST_0086));
  LittleFS.remove(FPSTR(TCONST_0084));
  listsuffix = time(NULL);
}

void EffectWorker::makeIndexFileFromList(const char *folder, bool forceRemove)
{
  File indexFile;

  if(forceRemove)
    removeLists();

  openIndexFile(indexFile, folder);
  effectsReSort(SORT_TYPE::ST_IDX); // сброс сортировки перед записью

  bool firstLine = true;
  indexFile.print(F("[\n"));
  for (uint8_t i = 0; i < effects.size(); i++){
    indexFile.printf_P(PGidxtemplate, firstLine ? " " : ",", effects[i]->eff_nb, effects[i]->flags.mask);
    firstLine = false; // сбрасываю признак первой строки
#ifdef ESP8266
    ESP.wdtFeed();
#elif defined ESP32
    delay(1);
#endif
  }
  indexFile.print("]");
  indexFile.close();
  LOG(println,F("Индекс эффектов обновлен!"));
  effectsReSort(); // восстанавливаем сортировку
}

void EffectWorker::makeIndexFileFromFS(const char *fromfolder,const char *tofolder, const bool skipInit, const bool forcechkdef)
{
  File indexFile;
  String sourcedir;
  makeIndexFile(tofolder,forcechkdef); // создать дефолтный набор прежде всего

  removeLists();

  if (fromfolder) {
      sourcedir.concat(F("/"));
      sourcedir.concat(fromfolder);
  }
  sourcedir.concat(FPSTR(TCONST_005A));

#ifdef ESP8266
  Dir dir = LittleFS.openDir(sourcedir);
#endif

#ifdef ESP32
  File dir = LittleFS.open(sourcedir);
  if (!dir || !dir.isDirectory()){
    LOG(print, F("Can't open dir: ")); LOG(println, sourcedir);
    return;
  }
#endif

  String fn;
  openIndexFile(indexFile, tofolder);
  bool firstLine = true;
  indexFile.print(F("[\n"));

  const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
  const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
  DynamicJsonDocument doc(bufsize*2);

#ifdef ESP8266
  while (dir.next()) {
      fn=sourcedir + "/" + dir.fileName();
#endif
#ifdef ESP32
  File _f = dir.openNextFile();
  while(_f){
      fn = sourcedir + _f.name();
#endif
      for(uint32_t i=0;i<nbfiles;i++){
        if (!deserializeFile(doc, fn.c_str(), i)) {
          continue;
        }
        uint16_t nb = doc[F("nb")].as<uint16_t>();
        uint8_t flags = doc[F("flags")].as<uint8_t>();
        doc.clear(); doc.garbageCollect();
        EffectListElem *eff = getEffect(nb);
        if(eff)
          flags = eff->flags.mask;
        indexFile.printf_P(PGidxtemplate, firstLine ? " " : ",", nb, flags);
        firstLine = false; // сбрасываю признак первой строки
#ifdef ESP8266
        ESP.wdtFeed();
#elif defined ESP32
        delay(1);
#endif
      }
#ifdef ESP8266
  ESP.wdtFeed();
#elif defined ESP32
  delay(1);
  _f = dir.openNextFile();
#endif
  }
  indexFile.print("]");
  indexFile.close();

  LOG(println,F("Индекс эффектов создан из FS!"));
  if(!skipInit)
    initDefault(tofolder); // перечитаем вновь созданный индекс
}

// создать или обновить текущий индекс эффекта
void EffectWorker::updateIndexFile()
{
  makeIndexFileFromList();
}

// удалить эффект из индексного файла
void EffectWorker::deleteFromIndexFile(const uint16_t effect)
{
  makeIndexFileFromList();
}

// створення резервної копії ефектів
void EffectWorker::saveEffectsBackup(const char *filename)
{
  if(!filename) return;
  String buffer;

  File bkp = LittleFS.open(filename, "w");
  bool firstLine = true;
  bkp.print(F("[\n"));
  for(int i=0; i<effects.size(); i++){
    buffer = getfseffconfig(effects[i]->eff_nb);
    if(!buffer.isEmpty()){
      if(firstLine)
        buffer=String(" ")+buffer;
      else
        buffer=String(",")+buffer;
      bkp.println(buffer);
      firstLine = false;
    }
  }
  bkp.print(F("]"));
  bkp.close();
}

// очищення папки з конфігураціями ефектів
void EffectWorker::clearEffDir()
{
  LOG(println, F("Clear /eff folder"));
  String effdir = FPSTR(TCONST_005A);
#ifdef ESP8266
  Dir dir = LittleFS.openDir(effdir);
#endif

#ifdef ESP32
  File dir = LittleFS.open(effdir);
  if (!dir || !dir.isDirectory()){
    LOG(print, F("Can't open dir: ")); LOG(println, effdir);
    return;
  }
#endif

  String fn;
#ifdef ESP8266
  while (dir.next()) {
      fn=effdir + "/" + dir.fileName();
#endif
#ifdef ESP32
  File _f = dir.openNextFile();
  while(_f){
      fn = _f.name();
#endif
      LittleFS.remove(fn);
#ifdef ESP8266
  ESP.wdtFeed();
#elif defined ESP32
  delay(1);
  _f = dir.openNextFile();
#endif
  }
  //LittleFS.rmdir(effdir);
}

// завантаження резервної копії ефектів
void EffectWorker::loadEffectsBackup(const char *filename, bool clear)
{
  if(!filename) return;
//   const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
//   const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
//   DynamicJsonDocument doc(bufsize*2);
//   File bkp = LittleFS.open(filename, "r");
//   File configFile;
//   uint8_t *buffer = new uint8_t[bufsize];
//   uint32_t pos=0, nb=0;
//   String cfilename;

//   if(clear)
//     clearEffDir();

//   size_t size;
//   memset(buffer,0,bufsize);
//   while((size = bkp.readBytesUntil('\n',buffer,bufsize))){
//     if(size<=3) continue;
//     String storage = (char *)buffer;
//     DeserializationError err = deserializeJson(doc, storage);
//     if (err) {
// 			LOG(print, F("deserializeJson error: "));
// 			LOG(println, err.code());
//       LOG(println, (char *)buffer);
//     } else {
//       if(size && buffer[size-1]==0x7D){
//         buffer[size]=0x00;
//       }
//       if(size && buffer[0]==','){
//         buffer[0]=' ';
//       }
//       nb=doc[F("nb")].as<uint16_t>();
//       LOG(printf_P, PSTR("%d.%s "),nb,doc[F("name")].as<String>().c_str());
//       doc.clear(); doc.garbageCollect(); storage=String();
//       cfilename = geteffectpathname(nb);
//       pos=nb>255?(((nb>>8)-1)%nbfiles):((nb&0xFF)%nbfiles);
//       if(LittleFS.exists(cfilename))
//         configFile = LittleFS.open(cfilename, "r+");
//       else
//         configFile = LittleFS.open(cfilename, "w");
//       if(configFile.size()!=bufsize*nbfiles)
//         configFile.truncate(bufsize*nbfiles);
//       configFile.seek(bufsize*pos, SeekMode::SeekSet);
//       size_t size = configFile.write(buffer,bufsize);
//       LOG(println, size);
//       configFile.close();
//     }
//     memset(buffer,0,bufsize);
// #ifdef ESP8266
//     ESP.wdtFeed(); // long term operation...
// #elif defined ESP32
//     delay(1);
// #endif
//   }
//   delete[] buffer;
//   bkp.close();

  DynamicJsonDocument doc(256);
  JsonObject data = doc.to<JsonObject>();
  data[FPSTR(TCONST_002A)] = filename ? filename : "";
  data[FPSTR(TCONST_005B)] = 2;

  if(clear)
    clearEffDir();

  LOG(println,F("Create loadEffectsBackup task"));

  Task *_t = new JsonTask(&data, 1000, TASK_FOREVER, [this](){
    JsonTask *task = (JsonTask *)ts.getCurrentTask();
    JsonObject jstorage = task->getData();
    JsonObject *data = &jstorage; // task->getData();

    const uint32_t bufsize=EFF_BUFFER_SIZE; // повинно бути кратно 4
    const uint32_t nbfiles=EFF_NB_PER_FILE; // EFF_BUFFER_SIZE*EFF_NB_PER_FILE десь 4-32кб
    DynamicJsonDocument doc(bufsize*2);
    File bkp = LittleFS.open((*data)[FPSTR(TCONST_002A)].as<String>(), "r");
    bkp.seek((*data)[FPSTR(TCONST_005B)].as<size_t>(), SeekMode::SeekSet);
    uint8_t *buffer = new uint8_t[bufsize];
    memset(buffer,0,bufsize);
    size_t size = bkp.readBytesUntil('\n',buffer,bufsize);
    uint32_t pos=0, nb=0;
    String cfilename;
    File configFile;

    if(size<=3) {
      if(bkp.position()>=bkp.size()){
        makeIndexFileFromFS();
        setSelected(getSelected(),true);
        LOG(println, F("loadEffectsBackup done"));
        task->cancel();
      }
      else
        (*data)[FPSTR(TCONST_005B)] = bkp.position();
      bkp.close();
      delete[] buffer;
      return;
    }
    if(size && buffer[size-1]==0x7D){
      buffer[size]=0x00;
    }
    if(size && buffer[0]==','){
      buffer[0]=' ';
    }

    String storage = (char *)buffer;
    DeserializationError err = deserializeJson(doc, storage);
    if (err) {
			LOG(print, F("deserializeJson error: "));
			LOG(println, err.code());
      LOG(println, (char *)buffer);
    } else {
      nb=doc[F("nb")].as<uint16_t>();
      LOG(printf_P, PSTR("%d.%s "),nb,doc[F("name")].as<String>().c_str());
      doc.clear(); doc.garbageCollect(); storage=String();
      cfilename = geteffectpathname(nb);
      pos=nb>255?(((nb>>8)-1)%nbfiles):((nb&0xFF)%nbfiles);
      if(LittleFS.exists(cfilename))
        configFile = LittleFS.open(cfilename, "r+");
      else
        configFile = LittleFS.open(cfilename, "w");
#ifdef ESP8266
      if(configFile.size()!=bufsize*nbfiles)
        configFile.truncate(bufsize*nbfiles);
#endif        
      configFile.seek(bufsize*pos, SeekMode::SeekSet);
      size_t size = configFile.write(buffer,bufsize);
      LOG(println, size);
      configFile.close();
    }
    (*data)[FPSTR(TCONST_005B)] = bkp.position();
    delete[] buffer;
    bkp.close();
  }, &ts, false, nullptr, [this](){TASK_RECYCLE;});
  _t->enable();
}

// удалить эффект
uint16_t EffectWorker::deleteEffect(const EffectListElem *eff, bool isCfgRemove)
{
  uint16_t result = 0;
  for(int i=0; i<effects.size(); i++){
      if(effects[i]->eff_nb==eff->eff_nb){
          if(isCfgRemove)
            removeConfig(eff->eff_nb);
          if(i>0)
            result = effects[i-1]->eff_nb;
          delete effects.remove(i);
          break;
      }
  }
  return result;
}

// копирование эффекта
uint16_t EffectWorker::copyEffect(const EffectListElem *base)
{
  EffectListElem *copy = new EffectListElem(base); // создать копию переданного эффекта
  uint16_t maxfoundnb=base->eff_nb;
  for(int i=0; i<effects.size();i++){
    if(effects[i]->eff_nb>255 && ((effects[i]->eff_nb&0x00FF)==(copy->eff_nb&0x00FF))){ // найдены копии
      if(maxfoundnb<effects[i]->eff_nb)
        maxfoundnb=effects[i]->eff_nb;
        //LOG(printf_P,PSTR("maxfoundnb=%d\n"),maxfoundnb);
    }
  }
  copy->eff_nb=(((((maxfoundnb&0xFF00)>>8)+1) << 8 ) | (copy->eff_nb&0xFF)); // в старшем байте увеличиваем значение на число имеющихся копий

  EffectWorker *effect = new EffectWorker(base,copy); // создать параметры для него (конфиг, индекс и т.д.)
  effects.add(copy);
  delete effect; // после того как все создано, временный экземпляр EffectWorker уже не нужен
  return copy->eff_nb;
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
  //LOG(println,F("------"));
  for (int i = 0; i < effects.size(); i++) {
      //LOG(println,effects[i]->eff_nb);
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
}

// выбор нового эффекта с отложенной сменой, на время смены эффекта читаем его список контроллов отдельно
void EffectWorker::setSelected(uint16_t effnb, const bool move)
{
  if(controls.size()==0 || selcontrols[0]!=controls[0] || !effnb){
    while(selcontrols.size()>0){ // очистить предыщий набор, если он только не отображен на текущий
      delete selcontrols.shift();
    }
    selcontrols.clear();
  }

  selEff = effnb;
  //LOG(println,F("Читаю список контроллов выбранного эффекта:"));
#if defined(PIO_FRAMEWORK_ARDUINO_MMU_CACHE16_IRAM48_SECHEAP_SHARED)
  HeapSelectIram ephemeral;
#endif
  EffectWorker *tmpEffect = new EffectWorker(effnb);
  LList<UIControl *> fake;
  this->selcontrols = tmpEffect->controls; // копирую список контроллов, освобождать будет другой объект
  tmpEffect->controls = fake;
  delete tmpEffect;
  if(move)
    moveSelected(true);
}

void EffectWorker::moveSelected(bool force){
  LOG(printf_P,PSTR("Синхронизация списков! Эффект: %d\n"), selEff);
  if(curEff != selEff || force){
    workerset(selEff, true, force);
    curEff = selEff;
    clearControlsList();
    controls = selcontrols; // теперь оба списка совпадают, смена эффекта завершена
  }
  //LOG(printf_P,PSTR("%d %d\n"),controls.size(), selcontrols.size());
}


/**
 * получить версию эффекта из "прошивки" по его ENUM
 */
const uint8_t EffectWorker::geteffcodeversion(const uint8_t id){
    uint8_t ver = pgm_read_byte(T_EFFVER + id);
    return ver;
}



void EffectCalc::init(EFF_ENUM _eff, LList<UIControl*>* controls, LAMPSTATE *_lampstate){
  effect=_eff;
  lampstate = _lampstate;

  isMicActive = isMicOnState();
  for(int i=0; i<controls->size(); i++){
    setDynCtrl((*controls)[i]);
  }

  active=true;
  load();
}

/*
 *  первоначальная загрузка эффекта, автозапускается из init()
 */
void EffectCalc::load(){}

bool EffectCalc::run(CRGB* ledarr, EffectWorker *opt){
  return false;
}

/**
 * проверка на холостой вызов для эффектов с доп. задержкой
 */
bool EffectCalc::dryrun(float n, uint8_t delay){
  //if((millis() - lastrun - EFFECTS_RUN_TIMER) < (unsigned)(255-speed)/n){
  if((millis() - lastrun - delay) < (unsigned)(float(255 - speed) / n)) {
    active=false;
  } else {
    lastrun = millis();
    active=true;
  }

  return !active;
}

/**
 * status - статус воркера, если работает и загружен эффект, отдает true
 */
bool EffectCalc::status(){return active;}

/**
 * setDynCtrl - была смена динамического контрола, idx=3+
 * вызывается в UI, для реализации особого поведения (палитра и т.д.)...
 */
String EffectCalc::setDynCtrl(UIControl*_val){
  if(!_val)
    return String();
  String ret_val = _val->getVal();
  //LOG(printf_P, PSTR("ctrlVal=%s\n"), ret_val.c_str());
  if (usepalettes && _val->getName().startsWith(FPSTR(TINTF_084))==1){ // Начинается с Палитра
    if(isRandDemo()){
      paletteIdx = random(_val->getMin().toInt(),_val->getMax().toInt()+1);
    } else
      paletteIdx = ret_val.toInt();
    palettemap(palettes, paletteIdx, _val->getMin().toInt(), _val->getMax().toInt());
    isCtrlPallete = true;
  }

  if(_val->getId()==7 && _val->getName().startsWith(FPSTR(TINTF_020))==1){ // Начинается с микрофон и имеет 7 id
    isMicActive = (ret_val.toInt() && isMicOnState()) ? true : false;
#ifdef MIC_EFFECTS
    if(lampstate!=nullptr)
      lampstate->setMicAnalyseDivider(isMicActive);
#endif
  } else {
    if(isRandDemo()){ // для режима рандомного ДЕМО, если это не микрофон - то вернуть рандомное значение в пределах диапазона значений
      ret_val = String(random(_val->getMin().toInt(), _val->getMax().toInt()+1));
    }
  }

  switch(_val->getId()){
    case 0: brightness = getBrightness(); break; // яркость всегда как есть, без рандома, но с учетом глобальности :) //LOG(printf_P,PSTR("brightness=%d\n"), brightness);
    case 1: speed = ret_val.toInt(); speedfactor = getSpeedFactor()*SPEED_ADJ; break; // LOG(printf_P,PSTR("speed=%d, speedfactor=%2.2f\n"), speed, speedfactor);
    case 2: scale = ret_val.toInt(); break;
    default: break;
  }

  return ret_val;
}

// Load palletes into array
void EffectCalc::palettesload(){
  palettes.reserve(FASTLED_PALETTS_COUNT);
  palettes.push_back(&AuroraColors_p/*RainbowStripeColors_p*/);
  palettes.push_back(&ForestColors_p);
  palettes.push_back(&NormalFire_p);
  palettes.push_back(&LavaColors_p);
  palettes.push_back(&OceanColors_p);
  palettes.push_back(&PartyColors_p);
  palettes.push_back(&RainbowColors_p);
  palettes.push_back(&HeatColors_p);
  palettes.push_back(&CloudColors_p);
  palettes.push_back(&EveningColors_p);
  palettes.push_back(&LithiumFireColors_p);
  palettes.push_back(&WoodFireColors_p);
  palettes.push_back(&SodiumFireColors_p);
  palettes.push_back(&CopperFireColors_p);
  palettes.push_back(&AlcoholFireColors_p);
  palettes.push_back(&RubidiumFireColors_p);
  palettes.push_back(&PotassiumFireColors_p);
  palettes.push_back(&AutumnColors_p);
  palettes.push_back(&AcidColors_p);
  palettes.push_back(&StepkosColors_p);
  palettes.push_back(&HolyLightsColors_p);
  palettes.push_back(&WaterfallColors_p);

  usepalettes = true; // активируем "авто-переключатель" палитр при изменении scale/R
  scale2pallete();    // выставляем текущую палитру
}

/**
 * palletemap - меняет указатель на текущую палитру из набора в соответствие с "ползунком"
 * @param _val - байт "ползунка"
 * @param _pals - набор с палитрами
 */
void EffectCalc::palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val, const uint8_t _min,  const uint8_t _max){
  if (!_pals.size() || _val>_max) {
    LOG(println,F("No palettes loaded or wrong value!"));
    return;
  }
  ptPallete = (_max+0.1)/_pals.size();     // сколько пунктов приходится на одну палитру; 255.1 - диапазон ползунка, не включая 255, т.к. растягиваем только нужное :)
  palettepos = (uint8_t)(_max ? (float)_val/ptPallete : 0);
  curPalette = _pals.at(palettepos);
  palettescale = _val-ptPallete*(palettepos); // разбиваю на поддиапазоны внутри диапазона, будет уходить в 0 на крайней позиции поддиапазона, ну и хрен с ним :), хотя нужно помнить!
  
  LOG(printf_P,PSTR("Mapping value to pallete: Psize=%d, POS=%d, ptPallete=%4.2f, palettescale=%d\n"), _pals.size(), palettepos, ptPallete, palettescale);
}

/**
 * метод выбирает текущую палитру '*curPalette' из набора дотупных палитр 'palettes'
 * в соответствии со значением "бегунка" шкалы. В случае если задана паременная rval -
 * метод использует значение R,  иначе используется значение scale
 * (палитры меняются автоматом при изменении значения шкалы/дин. ползунка, метод оставлен для совместимости
 * и для первоначальной загрузки эффекта)
 */
void EffectCalc::scale2pallete(){
  if (!usepalettes)
    return;

  LOG(println, F("Reset all controls"));
  for(int i=0;i<ctrls->size();i++){
    setDynCtrl((*ctrls)[i]);
  }
}
