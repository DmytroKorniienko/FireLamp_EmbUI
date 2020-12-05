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

#ifndef __TEXT_RES_H
#define __TEXT_RES_H

// Здесь строковые константы общего назначения, включая константы из интерфейса, что позволит локализировать при необходимости
// Ничего иного в данном файле не предполагается

/** набор строк с именами эффектов
 *  ссылки на эти строки собираются во флеш-массив в файле effect_types.h
 */
static const char TEFF_000[] PROGMEM = "";  // "пустышка"
static const char TEFF_001 [] PROGMEM = "Lámpara blanca";
static const char TEFF_002 [] PROGMEM = "Colores";
static const char TEFF_003 [] PROGMEM = "Arcoiris 2D";
static const char TEFF_004 [] PROGMEM = "Confeti";
static const char TEFF_005 [] PROGMEM = "";
static const char TEFF_006 [] PROGMEM = "Ventisca, Estrellas fugaces, Meteoritos";
static const char TEFF_007 [] PROGMEM = "Nuevo Matriz";
static const char TEFF_008 [] PROGMEM = "Luciérnagas";
static const char TEFF_009 [] PROGMEM = "Luciérnagas con rastro";
static const char TEFF_010 [] PROGMEM = "Cubo errante";
static const char TEFF_011 [] PROGMEM = "Pulso";
static const char TEFF_012 [] PROGMEM = "Efecto-caida";
static const char TEFF_013 [] PROGMEM = "Lámpara de Fuego";
static const char TEFF_014 [] PROGMEM = "Paintball";
static const char TEFF_015 [] PROGMEM = "Efectos de Ruido 3D";
static const char TEFF_016 [] PROGMEM = "Celdas";
static const char TEFF_017 [] PROGMEM = "Vals geométrico";
static const char TEFF_018 [] PROGMEM = "Tixy Land";
static const char TEFF_019 [] PROGMEM = "LDIRKO Land";
static const char TEFF_020 [] PROGMEM = "Oscilador";
static const char TEFF_021 [] PROGMEM = "Viento y Lluvia";
static const char TEFF_022 [] PROGMEM = "";
static const char TEFF_023 [] PROGMEM = "";
static const char TEFF_024 [] PROGMEM = "Bolas rebotando";
static const char TEFF_025 [] PROGMEM = "Sinusoide 3";
static const char TEFF_026 [] PROGMEM = "Metaesferas";
static const char TEFF_027 [] PROGMEM = "Espiral";
static const char TEFF_028 [] PROGMEM = "Arcoiris Cometa";
static const char TEFF_029 [] PROGMEM = "Tres Cometas";
static const char TEFF_030 [] PROGMEM = "Prismata";
static const char TEFF_031 [] PROGMEM = "Rebano y Depredador";
static const char TEFF_032 [] PROGMEM = "Remolino";
static const char TEFF_033 [] PROGMEM = "Deriva";
static const char TEFF_034 [] PROGMEM = "Palomitas de maíz";
static const char TEFF_035 [] PROGMEM = "Parpadeando";
static const char TEFF_036 [] PROGMEM = "Radar";
static const char TEFF_037 [] PROGMEM = "Ondas";
static const char TEFF_038 [] PROGMEM = "Fuego 2012";
static const char TEFF_039 [] PROGMEM = "";
static const char TEFF_040 [] PROGMEM = "";
static const char TEFF_041 [] PROGMEM = "ADN";
static const char TEFF_042 [] PROGMEM = "Fuego 2018";
static const char TEFF_043 [] PROGMEM = "Cerradura de código";
static const char TEFF_044 [] PROGMEM = "Cubo 2D";
static const char TEFF_045 [] PROGMEM = "Humo";
static const char TEFF_046 [] PROGMEM = "Picasso";
static const char TEFF_047 [] PROGMEM = "Picasso2";
static const char TEFF_048 [] PROGMEM = "Picasso3";
static const char TEFF_049 [] PROGMEM = "Saltadores";
static const char TEFF_050 [] PROGMEM = "Torbellinos";
static const char TEFF_051 [] PROGMEM = "Estrellas";
static const char TEFF_052 [] PROGMEM = "Deslumbramiento del agua, colores";
static const char TEFF_053 [] PROGMEM = "Fuegos artificiales";
static const char TEFF_054 [] PROGMEM = "Océano Pacífico";
static const char TEFF_055 [] PROGMEM = "Alfombra";
static const char TEFF_056 [] PROGMEM = "Ruido colorado";
static const char TEFF_057 [] PROGMEM = "Mariposas";
static const char TEFF_058 [] PROGMEM = "Sombras";
static const char TEFF_059 [] PROGMEM = "Patrones";
static const char TEFF_060 [] PROGMEM = "Flechas";
static const char TEFF_061 [] PROGMEM = "Bolas salvajes";
static const char TEFF_062 [] PROGMEM = "Atracción";
static const char TEFF_063 [] PROGMEM = "Serpientes";
static const char TEFF_064 [] PROGMEM = "Nexus";
static const char TEFF_065 [] PROGMEM = "Fuego altísimo";
static const char TEFF_066 [] PROGMEM = "Cielos sangrientos";
static const char TEFF_067 [] PROGMEM = "Serpiente arcoíris";
static const char TEFF_068 [] PROGMEM = "Florece";
static const char TEFF_069 [] PROGMEM = "Isla de Serpientes";
static const char TEFF_070 [] PROGMEM = "Sueños de los niños";
static const char TEFF_071 [] PROGMEM = "Metabolas";
static const char TEFF_072 [] PROGMEM = "Lámpara de lava";
static const char TEFF_253 [] PROGMEM = "Reloj";
static const char TEFF_254 [] PROGMEM = "Analizador de frecuencia";
static const char TEFF_255 [] PROGMEM = "Osciloscopio";


//-------------------------

/** набор строк с текстовыми константами интерфейса
 */
static const char TINTF_000[] PROGMEM = "Efectos";
static const char TINTF_001[] PROGMEM = "Representar texto";
static const char TINTF_002[] PROGMEM = "Configuracion";
static const char TINTF_003[] PROGMEM = "Presencia en lista de Efectos";
static const char TINTF_004[] PROGMEM = "Presencia en lista demo";
static const char TINTF_005[] PROGMEM = "Copiar";
static const char TINTF_006[] PROGMEM = "Eliminar";
static const char TINTF_007[] PROGMEM = "Refrescar lista de Efectos";
static const char TINTF_008[] PROGMEM = "Guardar";
static const char TINTF_009[] PROGMEM = "Control de Lista de efectos";
static const char TINTF_00A[] PROGMEM = "Efecto";
static const char TINTF_00B[] PROGMEM = "Salir";
static const char TINTF_00C[] PROGMEM = "Brillo Global";
static const char TINTF_00D[] PROGMEM = "Brillo";
static const char TINTF_00E[] PROGMEM = "Encender";
static const char TINTF_00F[] PROGMEM = "Demo";
static const char TINTF_010[] PROGMEM = "Brillo.Glob";
static const char TINTF_011[] PROGMEM = "Eventos"; 
static const char TINTF_012[] PROGMEM = "Micro.";
static const char TINTF_013[] PROGMEM = "Boton";
static const char TINTF_014[] PROGMEM = "Mas...";
static const char TINTF_015[] PROGMEM = "<<<";
static const char TINTF_016[] PROGMEM = ">>>";
static const char TINTF_017[] PROGMEM = "En espera...";
static const char TINTF_018[] PROGMEM = "Configuracion";
static const char TINTF_019[] PROGMEM = "Bajar";
static const char TINTF_01A[] PROGMEM = "Configuracion";
static const char TINTF_01B[] PROGMEM = "Crear";
static const char TINTF_01C[] PROGMEM = "Mostrar en Lampara"; 
static const char TINTF_01D[] PROGMEM = "Texto";
static const char TINTF_01E[] PROGMEM = "Color de Texto";
static const char TINTF_01F[] PROGMEM = "Enviar";
static const char TINTF_020[] PROGMEM = "Micrófono";
static const char TINTF_021[] PROGMEM = "Micrófono Activado";
static const char TINTF_022[] PROGMEM = "Correcion del Zero"; 
static const char TINTF_023[] PROGMEM = "Nivel de Ruido, und";
static const char TINTF_024[] PROGMEM = "Cancelacion de ruido";
static const char TINTF_025[] PROGMEM = "Calibrar micrófono";
static const char TINTF_026[] PROGMEM = "Encende el micrófono";
static const char TINTF_027[] PROGMEM = "... En proceso ...";
static const char TINTF_028[] PROGMEM = "WiFi";
static const char TINTF_029[] PROGMEM = "Cliente WiFi";
static const char TINTF_02A[] PROGMEM = "Conficuracion de Cliente WiFi";
static const char TINTF_02B[] PROGMEM = "Nombre de Lampara (mDNS Hostname/AP-SSID)";
static const char TINTF_02C[] PROGMEM = "WiFi SSID";
static const char TINTF_02D[] PROGMEM = "Password";
static const char TINTF_02E[] PROGMEM = "Conectarse";
static const char TINTF_02F[] PROGMEM = "WiFi AP";
static const char TINTF_030[] PROGMEM = "Muestra"; 
static const char TINTF_031[] PROGMEM = "Configuracion Punto de Acceso WiFi";
static const char TINTF_032[] PROGMEM = "En modo PA-only la lampara estara en modo punto de acceso y no se conectara a otras redes WiFI";
static const char TINTF_033[] PROGMEM = "Modo PA-only";
static const char TINTF_034[] PROGMEM = "Proteger PA con contraseña";
static const char TINTF_035[] PROGMEM = "MQTT";
static const char TINTF_036[] PROGMEM = "MQTT host";
static const char TINTF_037[] PROGMEM = "MQTT port";
static const char TINTF_038[] PROGMEM = "User";
static const char TINTF_039[] PROGMEM = "Intervalo de mqtt, sec.";
static const char TINTF_03A[] PROGMEM = "Coneccion";
static const char TINTF_03B[] PROGMEM = "Reflejo H";
static const char TINTF_03C[] PROGMEM = "Reflejo V";
static const char TINTF_03D[] PROGMEM = "Cambio de efectos sueave";
static const char TINTF_03E[] PROGMEM = "Efecto aleatorio en Demo";
static const char TINTF_03F[] PROGMEM = "Cambio de efecto en Demo. (sec)";
static const char TINTF_040[] PROGMEM = "Orden en lista de Efectos";
static const char TINTF_041[] PROGMEM = "Copias bajo original";
static const char TINTF_042[] PROGMEM = "Copias al final";
static const char TINTF_043[] PROGMEM = "Ordenar por índice"; 
static const char TINTF_044[] PROGMEM = "Ralentizar el texto";
static const char TINTF_045[] PROGMEM = "Posición del texto";
static const char TINTF_046[] PROGMEM = "Período de visualización de tiempo"; 
static const char TINTF_047[] PROGMEM = "Nunca"; 
static const char TINTF_048[] PROGMEM = "Cada hora";
static const char TINTF_049[] PROGMEM = "Cada media hora";
static const char TINTF_04A[] PROGMEM = "Cada 15 minutos";
static const char TINTF_04B[] PROGMEM = "Cada 10 minutos";
static const char TINTF_04C[] PROGMEM = "Cada 5 minutos";
static const char TINTF_04D[] PROGMEM = "Cada minuto";
static const char TINTF_04E[] PROGMEM = "Felicitacion de Año Nuevo"; 
static const char TINTF_04F[] PROGMEM = "Periodo de visualización (0 - No)";
static const char TINTF_050[] PROGMEM = "UNIX Fecha/Tiempo de Año Nuevo";
static const char TINTF_051[] PROGMEM = "Tiempo";
static const char TINTF_052[] PROGMEM = "Reglas TZSET para contabilizar el tiempo estándar/estacional (por ejemplo, 'MSK-3' para Europa / Moscú) Aquí https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h puede encontrar la frase requerida";
static const char TINTF_053[] PROGMEM = "Regla TZone (¡se recomienda configurarla!)";
static const char TINTF_054[] PROGMEM = "Servidor NTP de emnergancia (Opcional)";
static const char TINTF_055[] PROGMEM = "Fecha/Hora en formato YYYY-MM-DDThh:mm:ss (sin no hay conexion)";
static const char TINTF_056[] PROGMEM = "Actualizacion";
static const char TINTF_057[] PROGMEM = "Actualizacion ОТА-PIO";
static const char TINTF_058[] PROGMEM = "Empezar";
static const char TINTF_059[] PROGMEM = "Fichero FirmWare";
static const char TINTF_05A[] PROGMEM = "Subida";
static const char TINTF_05B[] PROGMEM = "Eventos";
static const char TINTF_05C[] PROGMEM = "Redactar";
static const char TINTF_05D[] PROGMEM = "Añadir";
static const char TINTF_05E[] PROGMEM = "Activado";
static const char TINTF_05F[] PROGMEM = "Tipo de Evento";
static const char TINTF_060[] PROGMEM = "Encender Lampara";
static const char TINTF_061[] PROGMEM = "Apagar Lampara";
static const char TINTF_062[] PROGMEM = "Encender DEMO";
static const char TINTF_063[] PROGMEM = "Despertador";
static const char TINTF_064[] PROGMEM = "Cargar Config. Lampara";
static const char TINTF_065[] PROGMEM = "Cargar Config. Efectos";
static const char TINTF_066[] PROGMEM = "Cargar Config. Eventos";
static const char TINTF_067[] PROGMEM = "Mostrar texto";
static const char TINTF_068[] PROGMEM = "Mostrar hora";
static const char TINTF_069[] PROGMEM = "Estado de Pin";
static const char TINTF_06A[] PROGMEM = "Encender AUX";
static const char TINTF_06B[] PROGMEM = "Apagar AUX";
static const char TINTF_06C[] PROGMEM = "Cambiar al revés AUX";
static const char TINTF_06D[] PROGMEM = "Fecha/Hora de Evento";
static const char TINTF_06E[] PROGMEM = "Repetir, Min";
static const char TINTF_06F[] PROGMEM = "Parar, Min";
static const char TINTF_070[] PROGMEM = "Parametros (Texto)";
static const char TINTF_071[] PROGMEM = "Repetir";
static const char TINTF_072[] PROGMEM = "Lunes";
static const char TINTF_073[] PROGMEM = "Martes";
static const char TINTF_074[] PROGMEM = "Miercoles";
static const char TINTF_075[] PROGMEM = "Jueves";
static const char TINTF_076[] PROGMEM = "Viernes";
static const char TINTF_077[] PROGMEM = "Sabado";
static const char TINTF_078[] PROGMEM = "Domingo";
static const char TINTF_079[] PROGMEM = "Actualizar";
static const char TINTF_07A[] PROGMEM = "Accion"; 
static const char TINTF_07B[] PROGMEM = "Boton activado";
static const char TINTF_07C[] PROGMEM = "OFF/ON";
static const char TINTF_07D[] PROGMEM = "Retener"; 
static const char TINTF_07E[] PROGMEM = "Presionar"; 
static const char TINTF_07F[] PROGMEM = "Una vez"; 
static const char TINTF_080[] PROGMEM = "Lampara de Fuego"; 
static const char TINTF_081[] PROGMEM = "WiFi & MQTT";
static const char TINTF_082[] PROGMEM = "Otros";
static const char TINTF_083[] PROGMEM = "Cambiando, no superponer efectos";
static const char TINTF_084[] PROGMEM = "Palitra";
static const char TINTF_085[] PROGMEM = "Alfabético (interno)";
static const char TINTF_086[] PROGMEM = "Eventos Activado ";
static const char TINTF_087[] PROGMEM = "Velocidad";
static const char TINTF_088[] PROGMEM = "Escala";
static const char TINTF_089[] PROGMEM = "Nombre de efecto";
static const char TINTF_08A[] PROGMEM = "Alfabético (Configuracion)";
static const char TINTF_08B[] PROGMEM = "Restablecer la configuración del efecto";
static const char TINTF_08C[] PROGMEM = "Prefijo de Topic";
static const char TINTF_08D[] PROGMEM = "Ordenar por micrófono";
static const char TINTF_08E[] PROGMEM = "Depuración";
static const char TINTF_08F[] PROGMEM = "Configuracion ESP";
static const char TINTF_090[] PROGMEM = "Numeracion de efectos en la lista";
static const char TINTF_091[] PROGMEM = "Símbolo de micrófono en la lista";
static const char TINTF_092[] PROGMEM = "Configuracion de Pin";
static const char TINTF_093[] PROGMEM = "Mostrar Configuracion del Sistema";
static const char TINTF_094[] PROGMEM = "Pin de Boton";
static const char TINTF_095[] PROGMEM = "Limite de corriente (mA)";
static const char TINTF_096[] PROGMEM = "Reiniciar";
static const char TINTF_097[] PROGMEM = "Pin RX Reproductor";
static const char TINTF_098[] PROGMEM = "Pin TX Reproductor";
static const char TINTF_099[] PROGMEM = "DFPlayer";
static const char TINTF_09A[] PROGMEM = "Nombre de Efecto en Demo";
static const char TINTF_09B[] PROGMEM = "Volumen";
static const char TINTF_09C[] PROGMEM = "Tiempo";
static const char TINTF_09D[] PROGMEM = "Nombre de efecto";
static const char TINTF_09E[] PROGMEM = "Efecto(reset)";
static const char TINTF_09F[] PROGMEM = "Desconectado";
static const char TINTF_0A0[] PROGMEM = "Primero";
static const char TINTF_0A1[] PROGMEM = "Aleatorio";
static const char TINTF_0A2[] PROGMEM = "MP3 Aleatorio";
static const char TINTF_0A3[] PROGMEM = "Sonido del Despertador";
static const char TINTF_0A4[] PROGMEM = "Segundo";
static const char TINTF_0A5[] PROGMEM = "Tercero";
static const char TINTF_0A6[] PROGMEM = "Cuarto";
static const char TINTF_0A7[] PROGMEM = "Quinto";
static const char TINTF_0A8[] PROGMEM = "Ecualizador";
static const char TINTF_0A9[] PROGMEM = "Normal";
static const char TINTF_0AA[] PROGMEM = "Pop";
static const char TINTF_0AB[] PROGMEM = "Rock";
static const char TINTF_0AC[] PROGMEM = "Jass";
static const char TINTF_0AD[] PROGMEM = "Clasico";
static const char TINTF_0AE[] PROGMEM = "Bass";
static const char TINTF_0AF[] PROGMEM = "Reproductor MP3";
static const char TINTF_0B0[] PROGMEM = "Cantidad de ficheros en carpeta MP3";
static const char TINTF_0B1[] PROGMEM = "Modo de reproduccion";
static const char TINTF_0B2[] PROGMEM = "Carpeta\\Numero de fichero musical(ej.: MP3\\17, 5\\19)";
static const char TINTF_0B3[] PROGMEM = "Limitar volumen del Despertador";
static const char TINTF_0B4[] PROGMEM = "Eliminar todo";
static const char TINTF_0B5[] PROGMEM = "Eliminar de la lista";
static const char TINTF_0B6[] PROGMEM = "Desconectado";
static const char TINTF_0B7[] PROGMEM = "Ejemplo 1";
static const char TINTF_0B8[] PROGMEM = "Ejemplo 2";
static const char TINTF_0B9[] PROGMEM = "Parametro";
static const char TINTF_0BA[] PROGMEM = "";
static const char TINTF_0BB[] PROGMEM = "";
static const char TINTF_0BC[] PROGMEM = "";
static const char TINTF_0BD[] PROGMEM = "";
static const char TINTF_0BE[] PROGMEM = "";
static const char TINTF_0BF[] PROGMEM = "";
static const char TINTF_0C0[] PROGMEM = "";


//-------------------------

/**
 * Набор статических строк, определяющих UI эффекта если он отличается от базового
 *
 *  GUI для эффекта по-умолчанию
 * Полный формат для пользовательского (id=3...7) Parametroа имеет вид: {\"id\":3,\"type\":0,\"val\":127,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Parametro\"}
 * @nb@ - будет заменен на реальный номер эффекта, @name@ - на дефолтное имя эффекта, @ver@ - версия, @pal@ - максимальная Palitra
 * https://community.alexgyver.ru/threads/wifi-lampa-budilnik-proshivka-firelamp_jeeui-gpl.2739/post-48813
 * https://community.alexgyver.ru/threads/wifi-lampa-budilnik-proshivka-firelamp_jeeui-gpl.2739/post-48848
 */

// Дефолтные контролы
static const char E_DEFUI[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":0,\"val\":\"127\"},{\"id\":1,\"val\":\"127\"},{\"id\":2,\"val\":\"127\"}]}";
// Добавил ко всем конфигам чекбокс "Micrófono" с индексом 7. Ввиду модифированного поведения функции myLamp.isMicOnOff(), он будет появляться только если глобально
// Micrófono включен, и эффект обладает зависимостью от Micrófonoа в принципе.
static const char E_DEFMICUI_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":0,\"val\":\"127\"},{\"id\":1,\"val\":\"127\"},{\"id\":2,\"val\":\"127\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
// Общая конфигурация для эффектов с 3-им ползунком для палитр
static const char E_3PAL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}]}";
static const char E_3PAL_MIC_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_3PAL255_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Palitra/Escala\"}]}";
// Общая конфигурация для эффектов только с 2-мя ползунками "Яркость" и "Velocidad", пример - эффект"Тихий Океан"
static const char E_2_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":48,\"val\":\"127\"}]}"; // 3*16+0 для 2 контрола
static const char E_2_MIC_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":48,\"val\":\"127\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}"; // 3*16+0 для 2 контрола

// Общая конфигурация для эффектов с 4-им ползунком для палитр
static const char E_4PAL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}]}";
static const char E_LLEND[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":0,\"min\":0,\"max\":10,\"step\":1,\"name\":\"Efecto (0 = Cíclicamente)\"}, {\"id\":3,\"type\":0,\"val\":0,\"min\":0,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}, {\"id\":4,\"type\":0,\"val\":16,\"min\":0,\"max\":32,\"step\":1,\"name\":\"Desplaz.color (0 = Ciclicamente)\"}]}";

static const char E_SPARCLES_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":16,\"val\":1,\"min\":1,\"max\":10,\"step\":1,\"name\":\"Color\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_LIGHTERS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":16,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Cantidad\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Subpixel\"}]}";
static const char E_RADAR255_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Palitra/Escala\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Subpixel\"}]}";
static const char E_WHITE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Tono\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_LEAPERS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":50,\"step\":1,\"name\":\"Tonteria!\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_BUTTERFLY_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":64,\"step\":1,\"name\":\"Cantidad / Color(luz de noche)\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Aleteo\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Luz de Noche\"}]}";
static const char E_PULS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Generador de Color\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_BBALLS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Escala\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Anillos\"}]}";
static const char E_PRIZMATA_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"},{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Difuminar\"}]}";
static const char E_AQUARIUM_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Color\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Saturación\"}, {\"id\":4,\"type\":0,\"val\":1,\"min\":0,\"max\":15,\"step\":1,\"name\":\"Destello (0:Apg. 1:Random)\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_FREQ_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":127,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Ganancia\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}, {\"id\":4,\"type\":0,\"val\":2,\"min\":1,\"max\":3,\"step\":1,\"name\":\"Modo\"}]}";
static const char E_OSC_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Escaneo\"},{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Pulso\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Ganancia\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
// Размерность ползунка "Patrón" должна быть MAX_PATTERN + 1 (patterns.h). При добавлении паттернов - менять и тут.
static const char E_PATT_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":33,\"min\":1,\"max\":65,\"step\":1,\"name\":\"Velocidad (Vert.)\"}, {\"id\":2,\"type\":0,\"val\":33,\"min\":1,\"max\":65,\"step\":1,\"name\":\"Velocidad (Horiz.)\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":39,\"step\":1,\"name\":\"Patrón (1 = aleatorio)\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Subpixel\"}]}";
static const char E_SHAD_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Velocidad\"}, {\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Sombreado\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Velocidad lineal\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_ARR_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":6,\"step\":1,\"name\":\"Modo (1 = aleatorio)\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Subpixel\"}]}";
static const char E_F2018_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Tono\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Velocidad lineal\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_NBAL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":0,\"val\":\"127\"},{\"id\":1,\"val\":\"127\"}, {\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":25,\"step\":1,\"name\":\"Parcialidad\"}]}";
static const char E_ATTRACT_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":100,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Energia\"}, {\"id\":3,\"type\":0,\"val\":100,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Peso\"}, {\"id\":4,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}]}";
static const char E_FLOCK_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Depredador\"}]}";
static const char E_WAVES_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Color/Cantidad\"}]}";
static const char E_MUNCH_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}, {\"id\":3,\"type\":0,\"val\":4,\"min\":0,\"max\":8,\"step\":1,\"name\":\"Cambio de color (0 = aleatorio)\"} ,{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_COM_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Color (1-127 aleatorio)\"}]}";
static const char E_DRIFT_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":4,\"step\":1,\"name\":\"Тип 1/2, Difuminar 3/4\"}]}";
static const char E_SMOKE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Color/Rotación\"},{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":6,\"step\":1,\"name\":\"Relleno\"}]}";
static const char E_CUBE2D_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":0,\"max\":22,\"step\":1,\"name\":\"Palitra (0 = Color aleatorio)\"},{\"id\":3,\"type\":0,\"val\":4,\"min\":1,\"max\":7,\"step\":1,\"name\":\"Tamaño (ancho)\"},{\"id\":4,\"type\":0,\"val\":4,\"min\":1,\"max\":7,\"step\":1,\"name\":\"Tamaño (altura)\"} ,{\"id\":5,\"type\":2,\"val\":\"false\",\"name\":\"Clasico\"}]}";
static const char E_FLAMP_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Color\"}]}";
static const char E_LIGHT2_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Tamaño/Color\"}]}";
static const char E_CUBE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Tamaño/Ruta(1-85-170-255)\"}]}";
static const char E_STARFAIL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":48,\"val\":\"127\"} ,{\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Nieve/Estrellas\"} ,{\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Meteoritos\"}]}"; // 3*16+0 для 2 контрола
static const char E_SNAKE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}, {\"id\":3,\"type\":0,\"val\":4,\"min\":1,\"max\":16,\"step\":1,\"name\":\"Cantidad\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Subpixel\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_NEXUS[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":5,\"min\":1,\"max\":10,\"step\":1,\"name\":\"Dencidad\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}]}";
static const char E_FWORK_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":1,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Frecuencia de tiro\"}, {\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":8,\"step\":1,\"name\":\"Cantidad de proyectiles\"}, {\"id\":3,\"type\":2,\"val\":\"false\",\"name\":\"Flashes\"}]}";
static const char E_CRAIN_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Color (1 = aleatorio, 255 = blanco)\"}, {\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Nuve\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Tormenta\"}]}";
static const char E_MATRIX[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":12,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Dencidad\"}, {\"id\":3,\"type\":0,\"val\":90,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Color (1 = random, 255 = blanco)\"}, {\"id\":4,\"type\":0,\"val\":20,\"min\":1,\"max\":60,\"step\":1,\"name\":\"Fallo\"}]}";
static const char E_FIRE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":5,\"type\":0,\"val\":1,\"min\":1,\"max\":3,\"step\":1,\"name\":\"Tipo de retardo (actual/dryrun/delay)\"}]}";
static const char E_NFIRE_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":50,\"min\":1,\"max\":100,\"step\":1,\"name\":\"Escala\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":10,\"step\":1,\"name\":\"Palitra\"}]}";
static const char E_POPCORN_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":16,\"min\":1,\"max\":32,\"step\":1,\"name\":\"Dencidad\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Difuminar\"}, {\"id\":5,\"type\":2,\"val\":\"true\",\"name\":\"Color inverso\"}]}";
static const char E_MBL_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":0,\"max\":@pal@,\"step\":1,\"name\":\"Palitra (0 = generador)\"}, {\"id\":4,\"type\":0,\"val\":0,\"min\":0,\"max\":255,\"step\":1,\"name\":\"Generador de color\"}]}";
static const char E_LIQLAM_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":0,\"max\":18,\"step\":1,\"name\":\"Palitra (0 = generador)\"}, {\"id\":4,\"type\":0,\"val\":0,\"min\":0,\"max\":255,\"step\":1,\"name\":\"Generador de color\"}, {\"id\":5,\"type\":0,\"val\":0,\"min\":0,\"max\":4,\"step\":1,\"name\":\"Filtro\"}, {\"id\":6,\"type\":2,\"val\":\"true\",\"name\":\"Fisica\"}]}";
static const char E_F2012_MIC_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":64,\"min\":1,\"max\":128,\"step\":1,\"name\":\"Escala\"},{\"id\":3,\"type\":0,\"val\":4,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_DNA_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":48,\"val\":\"127\"},{\"id\":3,\"type\":2,\"val\":\"true\",\"name\":\"Girar\"}]}";
static const char E_SNOW_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"val\":\"1\",\"min\":1,\"max\":3,\"name\":\"Suavizar\"}]}";
static const char E_SMOKEBALLS[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":16,\"step\":1,\"name\":\"Cantidad\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}]}";
static const char E_PALMICUI_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":0,\"max\":@pal@,\"step\":1,\"name\":\"Palitra (0 = blanco)\"},{\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Suavizar\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_COLORS_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":0,\"val\":\"127\"},{\"id\":1,\"val\":\"127\"},{\"id\":2,\"val\":\"127\"},{\"id\":3,\"type\":32,\"val\":\"2\",\"min\":1,\"max\":3,\"step\":1,\"name\":\"Modo\"},{\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";
static const char E_TLAND[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":25,\"min\":0,\"max\":35,\"step\":1,\"name\":\"Efecto (0 = Cíclicamente)\"}, {\"id\":3,\"type\":0,\"val\":160,\"min\":0,\"max\":255,\"step\":1,\"name\":\"Color 1 (0 = Cíclicamente)\"}, {\"id\":4,\"type\":0,\"val\":0,\"min\":0,\"max\":255,\"step\":1,\"name\":\"Color 2 (0 = Cíclicamente)\"}, {\"id\":5,\"type\":0,\"val\":1,\"min\":1,\"max\":4,\"step\":1,\"name\":\"Calidad/Velocidad\"}]}";
static const char E_FLYING[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":1,\"min\":1,\"max\":128,\"step\":1,\"name\":\"Difuminar\"}, {\"id\":3,\"type\":0,\"val\":7,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Círculos\"}]}";
static const char E_WRAIN[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":25,\"min\":1,\"max\":45,\"step\":1,\"name\":\"Dencidad\"}, {\"id\":3,\"type\":0,\"val\":4,\"min\":0,\"max\":@pal@,\"step\":1,\"name\":\"Palitra (0 = color random)\"}, {\"id\":4,\"type\":2,\"val\":\"true\",\"name\":\"Nube\"}, {\"id\":5,\"type\":2,\"val\":\"true\",\"name\":\"Tormenta\"}, {\"id\":6,\"type\":0,\"val\":1,\"min\":1,\"max\":8,\"step\":1,\"name\":\"Difuminar (1-4 con viento) \"}]}";
static const char E_CLOCK[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":4,\"min\":1,\"max\":8,\"step\":1,\"name\":\"Grosor\"}, {\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":@pal@,\"step\":1,\"name\":\"Palitra\"}]}";

// Инженерный
static const char E_TEST_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":2,\"type\":0,\"val\":8,\"min\":1,\"max\":16,\"step\":1,\"name\":\"Cantidad\"}]}";
static const char E_TEST2_CFG[] PROGMEM = "{\"nb\":@nb@,\"name\":\"@name@\",\"ver\":@ver@,\"flags\":255,\"ctrls\":[{\"id\":3,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Palitra\"}, {\"id\":4,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Parametro 1\"}, {\"id\":5,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Parametro 2\"}, {\"id\":6,\"type\":0,\"val\":1,\"min\":1,\"max\":255,\"step\":1,\"name\":\"Parametro 3\"}, {\"id\":7,\"type\":18,\"val\":\"true\",\"name\":\"Micrófono\"}]}";



#endif