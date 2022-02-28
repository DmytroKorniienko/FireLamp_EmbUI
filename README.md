## Гражданам России. В связи с тем, что ваша страна развязала полномасштабную войну против моей страны, тех, кто поддерживает действия своего фурера, просим собрать остатки совести и не использовать этот проект, как проект авторов из ненавистной вам Украины. Тем, же, кто против войны, команда проекта соболезнует, - вам, и вашим детям нести этот стыд. Проект не закрывется, но переносится на после нашей победы! Проект "денационализирован" (убрана рус.озвучка, рус.переводы, эффекты с российской символикой). Зарубежным же друзьям, кто мог раньше пользоваться русскоязычным интерфейсов, приносим искренние извинения, думаю вы нас поймете. Друзья, используя ../include/text_res-UA.h делайте переводы на свои национальные языки, и предлагайте в issues. Слава Україні!!

## Що це
Обговорення та підтримка даної прошивки триває [на форумі] Трохи згодом.

![GitHub Logo](/Схема51.jpg)

# Управління

## За кнопкою:
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
   
Це дефолтний стан для кнопки, але будь-яке налаштування можна змінити

## За HTTP
команди можна посилати на лампу через браузер або curl за URL-адресою http://esp-xxxxxx/cmd?arg=param
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
   - /cmd?warning /cmd?warning=[16777215,5000,500] - неблокуючий висновок мигалки поверх ефекту (видача попереджень)
   - /cmd?alarm=true - форсувати увімкнення будильника
   - /cmd?dynCtrlX , наприклад cmd?dynCtrl5=123 - отримати/встановити значення дин. контролю
   - /cmd?reboot - перезавантажити лампу
   - /cmd?OTA - запустити сервер оновлення OTA через PIO

так само команди можна поєднувати в ланцюжки в одній посилці, тобто. допустимо включити лампу, встановити яскравість, перейти до ефекту
аналогічні команди працюють і для MQTT, якщо чогось не вистачає або щось не виходить - запитуйте у темі

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
Основна технологія ведеться під контролер esp8266. ESP32 повністю підтримується лише на рівні движка, але має обмежений функціонал (переважно пов'язаний із сторонніми бібліотеками).

Складання під ESP32 виконується окремо.
```pio run -e esp32```

## Як зібрати проект

Проект збирається за допомогою [Platformio](https://platformio.org/)

Для складання проекту знадобиться таке:

Встановити [IDE Visual Studio Code](https://code.visualstudio.com/), і як плагін до неї встановити [Platformio](https://platformio.org/). Про те, як це зробити, можна знайти масу роликів на youtube, наприклад [цей](https://www.youtube.com/watch?v=NSljt17mg74).

Бажано ще встановити Git, оновлювати проект буде значно простіше

## Як завантажити/оновлювати проект

Актуальний зріз проекту завжди можна завантажити у вигляді zip-архіву (за посиланням) (https://github.com/DmytroKorniienko/FireLamp_JeeUI/archive/master.zip), або перейшовши за посиланням останнього релізу, там же зберігаються бінарники, зібрані під цей реліз
Або можна підтримувати клон репозиторію і підтягувати оновлення перед кожним новим складанням
Відкриваємо Git-bash, клонуємо репозиторій в тукучу папку командою 'git --depth 1 --no-single-branch https://github.com/DmytroKorniienko/FireLamp_JeeUI.git'
В подальшому для того, щоб оновити репозиторій, достатньо перейти в папку проекту і виконати команду 'git pull'
Щоб переключитися на гілку для ESP32 виконуємо команду 'git checkout esp32', перейти назад на master 'git checkout master'

Після того, як ви скопіювали проект (у вигляді zip архіву або через git clone), необхідно в папці include
скопіювати файл *user_config.h.default* під новим ім'ям *user_config.h* і в ньому налаштувати складання під свої параметри матриці, номерів висновків та потрібних функцій.

## Складання

 * Відкриваємо папку проекту у VSCode
 * Щоб зібрати прошивку, можна скористатися кнопками в нижньому статусному рядку:
   - PlatformIO:Build - зібрати прошивку
   - PlatformIO:Upload - завантажити прошивку через USB у плату.

Platformio сам завантажить необхідні бібіотеки для збирання проекту.

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
