import pip
import shutil
import locale
import sys
import ctypes
import os.path

def install(package):
    pip.main(['install', package])
    
try:
    from googletrans import Translator
except ImportError:
    install('googletrans==3.1.0a0')
    from googletrans import Translator

def translate_and_copy_file(original_file_path, target_language='es'):
    # Чтение оригинального файла
    with open(original_file_path, 'r', encoding='utf-8') as file:
        original_text = file.read()

    # Перевод текста с помощью Google Translate API
    translator = Translator()
    translated_text = translator.translate(original_text, dest=target_language).text  # Здесь target_language - целевой язык перевода

    # Запись переведенного текста в новый файл
    translated_file_path = (target_language+'.lang')  # Новое имя файла с переводом
    with open(translated_file_path, 'w', encoding='utf-8') as file:
        file.write(translated_text)


if __name__ == "__main__":
 
    # Получение языка ОС
    windll = ctypes.windll.kernel32
    os_language = locale.windows_locale[ windll.GetUserDefaultUILanguage() ]

    if os.path.exists((os_language+'.lang')):
    
        print('Translate exists. Nothing to do')
    
    else:

        # Использование языка ОС в качестве целевого языка, если не указан явно
        original_file_path = sys.argv[1]
        translate_and_copy_file(original_file_path, os_language)

