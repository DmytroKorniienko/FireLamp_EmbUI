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

#ifndef __TEXT_RES_H
#define __TEXT_RES_H
#include <Arduino.h>

// Здесь строковые константы общего назначения, включая константы из интерфейса, что позволит локализировать при необходимости
// Ничего иного в данном файле не предполагается

/** набор строк с именами эффектов
 *  ссылки на эти строки собираются во флеш-массив в файле effect_types.h
 */
static const char TEFF_000 [] PROGMEM = "";  // "пустышка"
static const char TEFF_001 [] PROGMEM = "Lámpara blanca";
static const char TEFF_002 [] PROGMEM = "Colores";
static const char TEFF_003 [] PROGMEM = "Arcoiris 2D";
static const char TEFF_004 [] PROGMEM = "Confeti";
static const char TEFF_005 [] PROGMEM = "M&M`s";
static const char TEFF_006 [] PROGMEM = "Ventisca, Estrellas fugaces, Meteoritos";
static const char TEFF_007 [] PROGMEM = "Nuevo Matriz";
static const char TEFF_008 [] PROGMEM = "Acuarela";
static const char TEFF_009 [] PROGMEM = "Luciérnagas con rastro";
static const char TEFF_010 [] PROGMEM = "Cubo errante";
static const char TEFF_011 [] PROGMEM = "Pulso";
static const char TEFF_012 [] PROGMEM = "Efecto-caida";
static const char TEFF_013 [] PROGMEM = "Zarza ardiente";
static const char TEFF_014 [] PROGMEM = "Paintball";
static const char TEFF_015 [] PROGMEM = "Efectos de Ruido 3D";
static const char TEFF_016 [] PROGMEM = "Celdas y mas";
static const char TEFF_017 [] PROGMEM = "Quince Fichas";
static const char TEFF_018 [] PROGMEM = "Tixy Land";
static const char TEFF_019 [] PROGMEM = "N.Tesla";
static const char TEFF_020 [] PROGMEM = "Oscilador";
static const char TEFF_021 [] PROGMEM = "Viento y Lluvia";
static const char TEFF_022 [] PROGMEM = "Hada";
static const char TEFF_023 [] PROGMEM = "Fuente";
static const char TEFF_024 [] PROGMEM = "Bolas rebotando";
static const char TEFF_025 [] PROGMEM = "Sinusoide";
static const char TEFF_026 [] PROGMEM = "Metaesferas";
static const char TEFF_027 [] PROGMEM = "Espiral";
static const char TEFF_028 [] PROGMEM = "Cometas, Fuegos y mas";
static const char TEFF_029 [] PROGMEM = "BulBulator";
static const char TEFF_030 [] PROGMEM = "Prismata";
static const char TEFF_031 [] PROGMEM = "Rebano y Depredador";
static const char TEFF_032 [] PROGMEM = "Remolino";
static const char TEFF_033 [] PROGMEM = "Deriva";
static const char TEFF_034 [] PROGMEM = "Palomitas de maíz";
static const char TEFF_035 [] PROGMEM = "Parpadeando";
static const char TEFF_036 [] PROGMEM = "Radar";
static const char TEFF_037 [] PROGMEM = "Ondas";
static const char TEFF_038 [] PROGMEM = "Fuego 2012";
static const char TEFF_039 [] PROGMEM = "Luces de Bengala";
static const char TEFF_040 [] PROGMEM = "Bolas";
static const char TEFF_041 [] PROGMEM = "ADN";
static const char TEFF_042 [] PROGMEM = "Fuego 2018";
static const char TEFF_043 [] PROGMEM = "Cerradura de código";
static const char TEFF_044 [] PROGMEM = "Cubo 2D";
static const char TEFF_045 [] PROGMEM = "Magma";
static const char TEFF_046 [] PROGMEM = "Picasso";
static const char TEFF_047 [] PROGMEM = "Flota Estelar";
static const char TEFF_048 [] PROGMEM = "Banderas";
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
static const char TEFF_065 [] PROGMEM = "Laberinto";
static const char TEFF_066 [] PROGMEM = "Colores rizados";
static const char TEFF_067 [] PROGMEM = "Aurora boreal";
static const char TEFF_068 [] PROGMEM = "Florece";
static const char TEFF_069 [] PROGMEM = "Isla de Serpientes";
static const char TEFF_070 [] PROGMEM = "Sueños de los niños";
static const char TEFF_071 [] PROGMEM = "Metabolas";
static const char TEFF_072 [] PROGMEM = "Lámpara de lava";
static const char TEFF_073 [] PROGMEM = "Corredor Espacial";
static const char TEFF_074 [] PROGMEM = "Humo";
static const char TEFF_075 [] PROGMEM = "Lampara de Fuego";
static const char TEFF_076 [] PROGMEM = "Espejismo";
static const char TEFF_250 [] PROGMEM = "Reloj";
static const char TEFF_251 [] PROGMEM = "Reproductor RGB 332/565";
static const char TEFF_254 [] PROGMEM = "Analizador de frecuencia";
static const char TEFF_255 [] PROGMEM = "Osciloscopio";


// -------------------------
#define DFTINTF_00A "Efecto"
#define DFTINTF_020 "Micrófono"
#define DFTINTF_084 "Paleta"
#define DFTINTF_088 "Escala"
#define DFTINTF_0D0 "Color"
#define DFTINTF_0D1 "Paleta/Escala"
#define DFTINTF_0D2 "Efecto (0 = cíclico)"
#define DFTINTF_0D3 "Desplazamiento de color (0: cíclico)"
#define DFTINTF_0D4 "Subpíxel"
#define DFTINTF_0D5 "Cantidad"
#define DFTINTF_0D6 "Generador de color"
#define DFTINTF_0D7 "Tinte"
#define DFTINTF_0D8 "¡Tonteria!"
#define DFTINTF_0D9 "Cantidad/Color (lampara nocturna)"
#define DFTINTF_0DA "Saturación"
#define DFTINTF_0DB "Ganancia"
#define DFTINTF_0DC "Barrido"
#define DFTINTF_0DD "Onda"
#define DFTINTF_0DE "Desenfocar"
#define DFTINTF_0DF "Flutter"
#define DFTINTF_0E0 "Luz nocturna"
#define DFTINTF_0E1 "Anillos"
#define DFTINTF_0E2 "Deslumbramiento (0: Apagado)"
#define DFTINTF_0E3 "Modo"
#define DFTINTF_0E4 "Velocidad lineal"
#define DFTINTF_0E5 "Modo (1: aleatorio)"
#define DFTINTF_0E6 "Desplazamiento"
#define DFTINTF_0E7 "Cantidad/Dirección/Posición"
#define DFTINTF_0E8 "Depredador"
#define DFTINTF_0E9 "Masa"
#define DFTINTF_0EA "Velocidad (vertical)"
#define DFTINTF_0EB "Velocidad (horizontal)"
#define DFTINTF_0EC "Patrón (0: aleatorio)"
#define DFTINTF_0ED "Sombreado"
#define DFTINTF_0EE "Energía"
#define DFTINTF_0EF "Densidad"
#define DFTINTF_0F0 "Color/Rotación"
#define DFTINTF_0F1 "Tamaño/Cola (1-85-170-255)"
#define DFTINTF_0F2 "Frecuencia de inicio"
#define DFTINTF_0F3 "Rotar"
#define DFTINTF_0F4 "Paleta (0: generador)"
#define DFTINTF_0F5 "Suavizado"
#define DFTINTF_0F6 "Relleno"
#define DFTINTF_0F7 "Espesor"
#define DFTINTF_0F9 "Comportamiento: 1/2"
#define DFTINTF_0FA "Color: 1/2"
#define DFTINTF_0FB "Paleta (0: sin color)"
#define DFTINTF_0FC "Color (1: aleatorio, 255: blanco)"
#define DFTINTF_0FD "Nevada/Starfall/Meteors"
#define DFTINTF_0FE "Cantidad"
#define DFTINTF_0FF "Tipo 1/2, Desenfoque 3/4"
#define DFTINTF_100 "Paleta (0: color aleatorio)"
#define DFTINTF_101 "Color 1 (0: cíclico)"
#define DFTINTF_102 "Nube"
#define DFTINTF_103 "Desenfocado (0: no)"
#define DFTINTF_104 "Deslizar"
#define DFTINTF_105 "Filtro"
#define DFTINTF_106 "Física"
#define DFTINTF_107 "Círculos"
#define DFTINTF_108 "Opción 1/2"
#define DFTINTF_109 "Parpadea"
#define DFTINTF_10A "Fallo"
#define DFTINTF_10B "Tormenta"
#define DFTINTF_10C "Cantidad de proyectiles"
#define DFTINTF_10D "Tamaño (ancho)"
#define DFTINTF_10E "Cambio de color (0: aleatorio)"
#define DFTINTF_10F "Tamaño (altura)"
#define DFTINTF_110 "Clásico"
#define DFTINTF_111 "Un color"
#define DFTINTF_112 "Colores inversos"
#define DFTINTF_113 "Color 2 (0: cíclico)"
#define DFTINTF_114 "Calidad / Velocidad"
#define DFTINTF_115 "Desenfocar (1-4 con viento)"
#define DFTINTF_116 "Desplazamiento sinusoidal"
#define DFTINTF_117 "Direccion (0: cíclico)"
#define DFTINTF_118 "Umbral de señal"
#define DFTINTF_11A "Color (1: cíclico)"

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
static const char TINTF_00A[] PROGMEM = DFTINTF_00A;
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
static const char TINTF_020[] PROGMEM = DFTINTF_020;
static const char TINTF_021[] PROGMEM = "Micrófono Activado";
static const char TINTF_022[] PROGMEM = "Correcion del Zero"; 
static const char TINTF_023[] PROGMEM = "Nivel de Ruido, und";
static const char TINTF_024[] PROGMEM = "Cancelacion de ruido";
static const char TINTF_025[] PROGMEM = "Calibrar micrófono";
static const char TINTF_026[] PROGMEM = "Encende el micrófono";
static const char TINTF_027[] PROGMEM = "... En proceso ...";
static const char TINTF_028[] PROGMEM = "WiFi";
static const char TINTF_029[] PROGMEM = "Cliente (STA)";
static const char TINTF_02A[] PROGMEM = "Conficuracion de Cliente WiFi";
static const char TINTF_02B[] PROGMEM = "Nombre de Lampara (mDNS Hostname/AP-SSID)";
static const char TINTF_02C[] PROGMEM = "WiFi SSID";
static const char TINTF_02D[] PROGMEM = "Password";
static const char TINTF_02E[] PROGMEM = "Conectarse";
static const char TINTF_02F[] PROGMEM = "Punto de Acceso (PA)";
static const char TINTF_030[] PROGMEM = "Muestra"; 
static const char TINTF_031[] PROGMEM = "Configuracion WiFi & Punto de Acceso";
static const char TINTF_032[] PROGMEM = "En modo PA la lampara estara en modo punto de acceso y no se conectara a otras redes WiFI";
static const char TINTF_033[] PROGMEM = "Modo WiFi";
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
static const char TINTF_044[] PROGMEM = "Velocidad del texto";
static const char TINTF_045[] PROGMEM = "Posición del texto";
static const char TINTF_046[] PROGMEM = "Mezclado (PA+STA)"; 
static const char TINTF_047[] PROGMEM = "Nunca"; 
static const char TINTF_048[] PROGMEM = "Cada hora";
static const char TINTF_049[] PROGMEM = "Cada media hora";
static const char TINTF_04A[] PROGMEM = "Cada 15 minutos";
static const char TINTF_04B[] PROGMEM = "Cada 10 minutos";
static const char TINTF_04C[] PROGMEM = "Cada 5 minutos";
static const char TINTF_04D[] PROGMEM = "Cada minuto";
static const char TINTF_04E[] PROGMEM = "Felicitacion de Año Nuevo"; 
static const char TINTF_04F[] PROGMEM = "Periodo de visualización (0 - No)";
static const char TINTF_050[] PROGMEM = "Fecha/Tiempo de Año Nuevo en formato YYYY-MM-DDThh:mm";
static const char TINTF_051[] PROGMEM = "Fecha / Hora / Zona horaria";
static const char TINTF_052[] PROGMEM = "Las reglas de cambio de zona horaria / horario de verano se aplican automáticamente, sin necesidad de ajustarlas manualmente.";
static const char TINTF_053[] PROGMEM = "Zona horaria";
static const char TINTF_054[] PROGMEM = "Servidor NTP de emergancia (Opcional)";
static const char TINTF_055[] PROGMEM = "Fecha/Hora en formato YYYY-MM-DDThh:mm:ss (si no hay conexion)";
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
static const char TINTF_062[] PROGMEM = "Modo DEMO";
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
#if defined(EMBUI_USE_FTP) && defined(EMBUI_USE_MQTT)
static const char TINTF_081[] PROGMEM = "WiFi, MQTT & FTP";
#elif defined(EMBUI_USE_MQTT)
static const char TINTF_081[] PROGMEM = "WiFi & MQTT";
#elif defined(EMBUI_USE_FTP)
static const char TINTF_081[] PROGMEM = "WiFi & FTP";
#else
static const char TINTF_081[] PROGMEM = "WiFi";
#endif
static const char TINTF_082[] PROGMEM = "Otros";
static const char TINTF_083[] PROGMEM = "Cambiando, no superponer efectos";
static const char TINTF_084[] PROGMEM = DFTINTF_084;
static const char TINTF_085[] PROGMEM = "Alfabético (interno)";
static const char TINTF_086[] PROGMEM = "Eventos Activado ";
static const char TINTF_087[] PROGMEM = "Velocidad";
static const char TINTF_088[] PROGMEM = DFTINTF_088;
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
static const char TINTF_0B4[] PROGMEM = "Por defecto";
static const char TINTF_0B5[] PROGMEM = "Eliminar de la lista";
static const char TINTF_0B6[] PROGMEM = "Desconectado";
static const char TINTF_0B7[] PROGMEM = "Ejemplo 1";
static const char TINTF_0B8[] PROGMEM = "Ejemplo 2";
static const char TINTF_0B9[] PROGMEM = "Parametro";
static const char TINTF_0BA[] PROGMEM = "Despertador Amanecer";
static const char TINTF_0BB[] PROGMEM = "Duración del Amanecer";
static const char TINTF_0BC[] PROGMEM = "Brillar después del amanecer";
static const char TINTF_0BD[] PROGMEM = "<";
static const char TINTF_0BE[] PROGMEM = ">";
static const char TINTF_0BF[] PROGMEM = "<+5";
static const char TINTF_0C0[] PROGMEM = "+5>";
static const char TINTF_0C1[] PROGMEM = "segundos";
static const char TINTF_0C2[] PROGMEM = "minutos";
static const char TINTF_0C3[] PROGMEM = "horas";
static const char TINTF_0C4[] PROGMEM = "dias";
static const char TINTF_0C5[] PROGMEM = "dias";
static const char TINTF_0C6[] PROGMEM = "día";
static const char TINTF_0C7[] PROGMEM = "horas";
static const char TINTF_0C8[] PROGMEM = "hora";
static const char TINTF_0C9[] PROGMEM = "El Azar.";
static const char TINTF_0CA[] PROGMEM = "Oscurecer el fondo";
static const char TINTF_0CB[] PROGMEM = "Advertencia";
static const char TINTF_0CC[] PROGMEM = "minutos";
static const char TINTF_0CD[] PROGMEM = "minuto";
static const char TINTF_0CE[] PROGMEM = "Dibujo";
static const char TINTF_0CF[] PROGMEM = "Llenar";
static const char TINTF_0D0[] PROGMEM = DFTINTF_0D0;
static const char TINTF_0D1[] PROGMEM = "Sonido primero";
static const char TINTF_0D2[] PROGMEM = "Limita el volumen";
static const char TINTF_0D3[] PROGMEM = "Coef. velocidad (1.0 por defecto)";
static const char TINTF_0D4[] PROGMEM = "Configuraciones de pantalla";
static const char TINTF_0D5[] PROGMEM = "Brillo (0 - OFF)";
static const char TINTF_0D6[] PROGMEM = "Brillo con OFF lámpara (0 - OFF)";
static const char TINTF_0D7[] PROGMEM = "Formato de 24 horas";
static const char TINTF_0D8[] PROGMEM = "Líder 0";
static const char TINTF_0D9[] PROGMEM = "Claro";
static const char TINTF_0DA[] PROGMEM = "Escanear";
static const char TINTF_0DB[] PROGMEM = "FTP";
static const char TINTF_0DC[] PROGMEM = "Control";
static const char TINTF_0DD[] PROGMEM = "Tipo de escala";
static const char TINTF_0DE[] PROGMEM = "Escala color";
static const char TINTF_0DF[] PROGMEM = "Texto color";
static const char TINTF_0E0[] PROGMEM = "Mostrar temperatura";
static const char TINTF_0E1[] PROGMEM = "¿Está seguro?";
static const char TINTF_0E2[] PROGMEM = "Transmisión";
static const char TINTF_0E3[] PROGMEM = "Modo";
static const char TINTF_0E4[] PROGMEM = "E1.31";
static const char TINTF_0E5[] PROGMEM = "SoulMate";
static const char TINTF_0E6[] PROGMEM = "Efecto bloque";
static const char TINTF_0E7[] PROGMEM = "Mapang";
static const char TINTF_0E8[] PROGMEM = "Universo primero";
static const char TINTF_0E9[] PROGMEM = "Cargar Config. Botons";
static const char TINTF_0EA[] PROGMEM = "Luz de noche";
static const char TINTF_0EB[] PROGMEM = "Lámpara brillante";
static const char TINTF_0EC[] PROGMEM = "De todos modos";
static const char TINTF_0ED[] PROGMEM = "Dice el tiempo";
static const char TINTF_0EE[] PROGMEM = "Discapacitada";
static const char TINTF_0EF[] PROGMEM = "Vertical";
static const char TINTF_0F0[] PROGMEM = "Horizontal";

static const char NY_MDG_STRING1[] PROGMEM = "Hasta que se quede el año nuevo %d %s";
static const char NY_MDG_STRING2[] PROGMEM = "Feliz año nuevo %d!";
//-------------------------

#endif