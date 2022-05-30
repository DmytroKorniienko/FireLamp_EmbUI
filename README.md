## People of good will, we Ukrainians appeal to you! The Russian Fuhrer sent all the military power of his Russian Reich to our country. Do not believe rushist-fascist propaganda. Ukrainians defend their home and believe that the free people of the whole world will support us in this. The Russian Reich will fall, the Russian Fuhrer will answer for nuclear threats to the whole world, victory will be ours!

## Гражданам России. В связи с тем, что ваша страна развязала полномасштабную войну против нашей страны, тех, кто поддерживает действия своего фюрера, просим собрать вместе остатки совести и не использовать этот проект, как проект авторов из ненавистной вам Украины. Тем же россиянам, кто против войны, команда проекта соболезнует, - вам, и вашим детям нести этот груз стыда и греха братоубийства. 

## Проект не закрывется, но переходит в вялотекущий режим разработки, до нашей победы! Проект "денационализирован" (убрана рус.озвучка, рус.переводы, эффекты с российской символикой). Зарубежным друзьям, кто мог раньше пользоваться русскоязычным интерфейсом, приносим искренние извинения, уверены вы понимаете нашу позицию. Друзья, используя ../include/text_res-UA.h делайте переводы на свои родные языки, и предлагайте их в issues. Слава Україні!!

## Що це?
Це є проект продвинутої лампи\панелі, на базі світлодіодів WS2812b, або аналогічних, відомих як NEOPIXEL MATRIX, з відображенням світлових композицій (далі "ефектів").
В базовому варіанті використовується матриця розміром 16x16, 256 пікселів. Загалом проект гарантуе задовільну роботу матриць довільного розміру, у яких кількість пікселів не перевищує 1024. 
Деякі відео, пов'язані з проектом, можна подивитися на [цьому](https://www.youtube.com/channel/UCMkXEbpQH7_xIXp_tFIdeag) каналі YouTube.  
   - Управління проектом за допомогою кнопки\енкодера, та повне управління з любого браузера, любого пристрою, в WebUI. 
   - Загалом понад 100 світлових композицій, як в загальному списку ефектів, так і в складі мульти-ефектів. 
   - Також в наявності є багато ефектів, які міняють поведінку, колір чи яскравість залежно від звуку. Є кілька спеціальних музичних ефектів типу "Осцилограф" та "Частотний аналізатор". 
   - Окрім цього, проект підтримує трансляції світлових композицій протоколом E1.31 з програми Jinx! та websocket з, модифікованої нами, Soulmate IDE.
   - Проект можна оснастити DFPlayer для озвучування назв ефектів, часу, будильника та програвання MP3 файлів відповідно до світлових ефектів, або як звичайний  МП3 плеєр. 
   - В проекті є програвач анімаційних файлів нашого власного формата, з підтриммкою однобайтового кодування кольору RGB332 та двухбайтового - RGB565. Файли цього формату можна отримати за допомогою написаного нами конвертора, з GIF-анімацій. Попередньо обробивши GIF в онлайн редакторі - обрізати лишнє, зменшити розрішення до розрішення лампи\панелі і таке інще. Конвертор, посилання на онлайн редактор, якийсь набір GIF, чи вже готові файли, можна знайти за посиланням внизу цього тексту. Програвач можна найти внизу списка ефектів в WebUI, за номером 251.
   - Проект також підтримує управління http-командами, та інтеграції на базі MQTT.
   - Підтримується спеціальний вихід на AUX-Реле, який управляється кліком в WebUI, або автоматично за розкладом (спрощений аналог реле Sonoff).
   - Продвинута система автоматизацій в розділі, випливаючого зліва, меню WebUI "Події" - будильники, зміна ефектів, озвучення часу, зміна яскравості, стану AUX-Реле за розкладом, etc. 
   - Підтримка датчика темеператури DS18B20 та спеціального виходу управління вентилятором обдуву матриці. Цей вихід також може бути задіяним для інших дій по перевищенню температури (це залежить тільки від Вашої фантазії та потреб). Вивід температури в WebUI та на дисплей TM1637. (Оперативні зміни лімітів з WebUI поки не реалізовані, задаються в user_config.h до компіляції проекту)

Додаткове обладнання та можливості:
   - Сенсорна кнопка типу TTP223 або рототативний енкодер з кнопкою під валом, для управління
   - 8-сегментний дисплей на чотири знакомісця TM1637 (з десятковими крапками) для відображення службової інформації, годинника та температури (при наявності DS18B20)
   - DFPlayer для озвучування
   - Температурний датчик DS18B20 для управління вентилятором
   - Мікрофонний модуль MAX9814 для реакції на звук.
   - Вихід на AUX-Реле
   - Вихід на вентилятор, або інший нагрівач\охолоджувач, залежить від датчика температури
   - Вихід MOSFET для повного відключення\включення матриці світлодіодів
   - Вихід ALARM для управління додатковими пристроями, активується на час спрацювання булильника.


![GitHub Logo](/Схема51.jpg)

# Управління

## За допомогою кнопки:
* З вимкненого стану
   - 1 дотик - увімкнути на останньому ефекті
   - 2 дотики - увімкнути в режимі ДЕМО
   - Довге утримання - включити в режимі "біла лампа" на мінімальну яскравість (нічник)
   - дотик, утримання - увімкнути в режимі "біла лампа" на максимальну яскравість
* У включеному стані
   - 1 дотик - вимкнути лампу
   - 2 дотики - наступний ефект
   - 3 дотики - попередній ефект
   - 4 дотики + 4 дотики - запуск сервісу ОТА
   - 5 дотиків - виведення IP на лампу
   - 6 дотиків - виведення поточного часу рядком, що біжить
   
   - утримання - регулювання яскравості
   - 1 дотик, утримання - регулювання "швидкості" ефекту
   - 2 дотики, утримання - регулювання "масштабу" ефекту
   
Це поведінка кнопки "за замовчуванням", але в налаштуваннях лампи її змінити. 

## За допомогою енкодера:
   - прокручування вліво\право - повільне регулювання яскравості, крок 1 одиниця з 0-255
   - швидке прокручування вліво\право - регулювання яскравості з кроком 16 одиниць
   - натиснути та прокрутити - зміна ефекту. (Треба мати на увазі, що буде вибрано слідуючий\попередній ефект, але сама зміна єфекту відбудеться тільки після відпускання енкодера. При цьому номер єфекту, який ви вибираєте, буде відображатися на лампі та дисплеї TM1637, яащо він підключений та сконфігурований в проекті.) 
   - натиснути та швидко прокрутити - швидка зміна номера ефекта, крок 16.
   - 1 натискання енкодера - включити\виключити лампу.
   - дії на 2, 3...7 натискань можна сконфігурувати до збирання прошивки в файлі user_config.h
   - натискання і утримання головки енкодера зі стану "виключено" включить режим "нічник", тобто ефект "Біла лампа" на низькій яскравості. Маніпуляції зі зміною єфектів приведе до виходу з режиму "нічник". 
   - Щоб скинути системні налаштування лампи\панелі, натисніть енкодер та подайте на прилад живлення. (Увага! Налаштування ефектів не скидується)


## Командами HTTP
команди можна посилати на лампу за допомогою браузера або утіліти curl за URL-адресою http://esp-xxxxxx/cmd?arg=param
   - /cmd?on /cmd?off /cmd?on=true /cmd?on=false - вкл/викл
   - /cmd?demo - режим ДЕМО
   - /cmd?gbright /cmd?gbright=true - перевірка/установка для глобальної яскравості
   - /cmd?msg=Hello - виведення повідомлення на лампу
   - /cmd?bright=0-255 - яскравість
   - /cmd?speed=0-255 - швидкість
   - /cmd?scale=0-255 - шкала
   - /cmd?effect=N - ефект номер N
   - /cmd?move_next - наступний ефект
   - /cmd?move_prev - попередній ефект
   - /cmd?move_rnd - випадковий ефект
   - /cmd?effect – номер поточного ефекту
   - /cmd?warning /cmd?warning=[16777215,5000,500] - неблокуючий вивід мигалки поверх ефекту (видача попереджень)
   - /cmd?alarm=true - форсувати увімкнення будильника
   - /cmd?dynCtrlX , наприклад cmd?dynCtrl5=123 - отримати/встановити значення динамічно створеного елемента управління
   - /cmd?reboot - перезавантажити лампу
   - /cmd?OTA - запустити сервер оновлення OTA через PIO (застаріло, використовуеться http-update)

також команди можна об'єднувати в ланцюжки, в одній посилці. Тобто, -  включити лампу, встановити яскравість, перейти до ефекту.
Аналогічні команди працюють і для MQTT, якщо чогось не вистачає або щось не виходить - запитуйте в issues.

Додаткові службові команди:

   - /heap - показати вільне місце на купі
   - /echo - показати ехо-відповідь (json для формування інтерфейсу)
   - /config - показати поточний конфіг (json основних налаштувань)
   - /scan - показати доступні WiFi-мережі
   - /config.json - завантажити активний конфіг лампи
   - /events_config.json - завантажити активний конфіг подій
   - /update - форма http-оновлення прошивки
   - /edit - виклик редактора конфігів (esp8266/esp8266 - логін/пасворд)

## ESP8266 vs ESP32
Основна розробка ведеться під контролер esp8266. ESP32 повністю підтримується, але має обмеженя одночасно можливого функціоналу (пов'язано з тим фактом, що компілятор формує бінарний файл надто великого розміру).

## Підготовка до збірки проекту
Проект збирається за допомогою [Platformio](https://platformio.org/)

Для збирання проекту знадобиться такі кроки:

Встановити [IDE Visual Studio Code](https://code.visualstudio.com/), і як плагін до неї встановити [Platformio](https://platformio.org/). Про те, як це зробити, можна знайти масу роликів на youtube, наприклад [цей](https://www.youtube.com/watch?v=NSljt17mg74).

Бажано ще встановити Git, та створити локальний клон проекту собі на ПК, оновлювати проект буде значно простіше.

Також, для користувачів ОС Windows 7-11 можна скористатися інсталятором Install.zip. Цей інструмент встановить все, що потрібно для збірки та оновлень проекту. Розпакуйте архів кудись в корінь любого диску. Будь ласка, не запускайте з архіву, Робочого столу та папки завантажень. Для встановлення всього потрібного для збирання проекта достатньо виконати один за одним всі кроки меню скрипта Install.bat. Увага! Для встановлення Python скрипт потрібно запустити "від Адміністратора" і потім перезапустити ПК. Все інше встановлюється в нормальному режимі. На останньому етапі буде скачано та встановлено локальний клон репозиторію, а папка з ним відкрита в Провіднику Windows.

В подальшому, для збірки та оновлення проекту, можна використовувати скрипт Builder.bat. Всі команди там описані двома мовами - англійською та українською. Для збирання проекту, засобами Builder.bat, встановлення на налаштування VSC не обов'язкове.

## Як завантажити/оновлювати проект
Найпростіший спосіб - використати команду 1 або 2 (залежить який бренч ви використовуєте Master чи Dev) в скрипті Builder.bat. 
А також...
Актуальний зріз проекту завжди можна завантажити у вигляді zip-архіву (за посиланням) (https://github.com/DmytroKorniienko/FireLamp_EmbUI/archive/master.zip), або перейшовши за посиланням останнього релізу, там же зберігаються бінарники, зібрані під цей реліз
Або можна підтримувати клон репозиторію і підтягувати оновлення перед кожним новим складанням за допомогою Git.

Відкриваємо Git-bash, клонуємо репозиторій в тукучу папку командою 'git --depth 1 --no-single-branch https://github.com/DmytroKorniienko/FireLamp_EmbUI.git'
В подальшому для того, щоб оновити репозиторій, достатньо перейти в папку проекту і виконати команду 'git pull'
Щоб переключитися на гілку для ESP32 виконуємо команду 'git checkout esp32', перейти назад на master 'git checkout master'

Після того, як ви скопіювали проект (у вигляді zip архіву або через git clone), необхідно в папці include
скопіювати файл *user_config.h.default* під новим ім'ям *user_config.h* і в ньому налаштувати складання під свої параметри матриці, пінів, функцій та додаткового обладнання, типу: кнопка\енкодер, TM1637, DFPlayer, DS18B20, вентилятор обдуву матриці, AUX-реле і таке інше.

## Як зібрати проект?
Найпростіший спосіб збирати проект в Builder.bat за допомогою команд: 4, 5, 7, 8, 7D, 8D та 6, 9 для ESP32 (DEBUG версія для ESP32 не підтримується). Або...
 * Відкриваємо папку проекту у VSCode
 * Щоб зібрати прошивку, можна скористатися кнопками в нижньому статусному рядку:
   - PlatformIO:Build - зібрати прошивку
   - PlatformIO:Upload - завантажити прошивку через USB у плату.

Platformio сам завантажить необхідні бібіотеки для проекту.

Також можна відкрити термінал за допомогою клавіш Ctrl+Shit+` та скористатися розширеними командами для складання проекту.

 - **pio run** - зібрати проект за замовчуванням (esp8266dev), dev версія під esp8266 з виведенням повідомлень налагодження через serial інтерфейс.
 - **pio run -e esp8266** - зібрати версію esp8266, буде зібрано полегшене прошивка без виведення налагоджувальних повідомлень через serial інтерфейс
Рекомендується для повсякденного використання, якщо немає потреби налагоджувати роботу лампи
 - **pio run -e esp32dev** - зібрати версію esp32dev, буде зібрано прошивку під esp32 з виведенням налагоджувальних повідомлень через serial інтерфейс
 - **pio run -t upload** - зібрати та прошити проект за замовчуванням (esp8266dev)
 - **pio run -e esp8266 -t upload** - зібрати та прошити полегшену версію
 - **pio deviсe monitor** - запустити serial-монітор для перегляду повідомлень, що видаються контролером
 
Після першої прошивки подальші оновлення можна заливати у контролер повітрям. Для цього потрібно зайти браузером на контролер за URL-адресою http://embui-xxxx/update, де xxxx це ID контролера, натиснути на кнопку 'Firmware', вибрати файл із прошивкою та завантажити його в контролер.
   Файл із прошивкою Platformio кладе у підпапки проекту:
    - **.pio/build/esp8266/firmware.bin** - звичайна версія під esp8266
    - **.pio/build/esp8266dev/firmware.bin** - debug версія під esp8266
    - **.pio/build/esp32dev/firmware.bin** - debug версія під esp32
    
    ## Важливо знати!
   - #### Для роботи прошивки також потрібно прошити в контролер образ ФС! Його можна створити методами VSC, або в Builder.bat командами b або b1 для ESP32. Рекомендується спочатку прошивати прошивку, а потім ФС.
   - Читати уважно описи команд в Builder.bat, окрім команд збірки там є команди збірки-прошивки (по кабелю)
   - В папці проекту є приклад скрипта Uploader.bat.default. Цей файл можна адаптувати для бездротового оновлення прошивки або ФС, змінивши всередені IP адресу на свою, та вказавши що саме прошивати firmware.bin чи littlefs.bin для ФС. Таких файлів можна створити безліч, важливо щоб їхня назва була по шаблону Uploader-****.bat. 
   - Кожен раз, коли ви робите збірку без прошивки, в Builder.bat, копія бінарного файла потрапляє в папку ../out_bin/, і саме звідти Uploader-****.bat буде брати файли для оновлення лампи по повітрю.
   - Доповнюється...

## Суміжні ресурси
- Репозиторій анімацій для Плеєра RGB565\332 https://github.com/kostyamat/FireLamp_EmbUI-animations Також там Ви знайдете інші доповнення до проекту лампи.
- Репозиторій фреймворку для створення WebUI для контролерів ESP8266\ESP32 https://github.com/DmytroKorniienko/EmbUI
