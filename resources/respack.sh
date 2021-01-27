#!/bin/sh

# etag file
tags=etags.txt
# embui branch/tag name to fetch
embuitag="dev"

refresh_styles=0

[ -f $tags ] || touch $tags

# check github file for a new hash
freshtag(){
    local url="$1"
    etag=$(curl -sL -I $url | grep etag | awk '{print $2}')
    echo "$url $etag" >> newetags.txt
    if [ $(grep -cs $etag $tags) -eq 0 ] ; then
        #echo "new tag found for $url"
        return 0
    fi
    #echo "old tag for $url"
    return 1
}


echo "Preparing resources for FireLamp FS image into ../data/ dir" 

mkdir -p ../data/css ../data/js ../data/extras ../data/login

# собираем скрипты и стили из репозитория EmbUI (используется dev ветка, при формировании выпусков пути нужно изменить)
if freshtag https://github.com/DmytroKorniienko/EmbUI/raw/$embuitag/resources/data.zip ; then
    refresh_styles=1
    echo "EmbUI resources requires updating"
else
    echo "EmbUI is up to date"
fi

# проверяем есть ли в лампе обновленные стили для тем embui
for f in html/css/style_*.css
do
    [ $f -nt ../data/css/$( basename $f).gz ] && refresh_styles=1
done

# if any of the уьигш styles needs updatiing, than we need to repack both embui and local files
if [ $refresh_styles -eq 1 ] ; then

    echo "refreshing embui css files..."

    curl -sL https://github.com/DmytroKorniienko/EmbUI/raw/$embuitag/resources/data.zip > embui.zip
    unzip -o -d ../data/ embui.zip "css/*" "js/*"

    # append our styles to the embui
    for f in html/css/style_*.css
    do
        cat $f | gzip -9 >> ../data/css/$( basename $f).gz
    done

    rm -f embui.zip
fi

# обновляем скрипты/стили специфичные для лампы
[ ! -f ../data/css/lamp.css.gz ] || [ html/css/custom_drawing.css -nt ../data/css/lamp.css.gz ] && cat html/css/custom_drawing.css | gzip -9k >> ../data/css/lamp.css.gz
[ ! -f ../data/js/lamp.js.gz ] || [ html/js/drawing.js -nt ../data/js/lamp.js.gz ] && cat html/js/drawing.js | gzip -9k >> ../data/js/lamp.js.gz

# update static files if newer
[ ! -f ../data/index.html.gz ]  || [ html/index.html -nt ../data/index.html.gz ] && gzip -9k html/index.html && mv -f html/index.html.gz ../data/
[ ! -f ../data/favicon.ico.gz ] || [ html/favicon.ico -nt ../data/favicon.ico.gz ] &&  gzip -9k html/favicon.ico && mv -f html/favicon.ico.gz ../data/

cp -u html/.exclude.files ../data/
cp -u html/events_config.json ../data/
cp -u html/buttons_config.json ../data/

###
# обновляем ace-editor в data/extras folder
# collect all required ACE editor extensions here
if [ $(freshtag https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/ace.js) ] || \
   [ $(freshtag https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/mode-html.js) ] || \
   [ $(freshtag https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/mode-json.js) ] ; then

        echo "Updating AceEditor resources"
        curl -sL "https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/ace.js" | gzip -9 > ../data/extras/acefull.js.gz
        curl -sL "https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/mode-html.js" | gzip -9 >> ../data/extras/acefull.js.gz
        curl -sL "https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/mode-json.js" | gzip -9 >> ../data/extras/acefull.js.gz
else
    echo "AceEditor is up to date"
fi

if freshtag https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/worker-html.js ; then
    curl -sL "https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/worker-html.js" | gzip -9 > ../data/extras/worker-html.js.gz
fi
if freshtag https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/worker-css.js ; then
    curl -sL "https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/worker-css.js"  | gzip -9 > ../data/extras/worker-css.js.gz
fi
if freshtag https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/worker-javascript.js ; then
    curl -sL "https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/worker-javascript.js"  | gzip -9 > ../data/extras/worker-javascript.js.gz
fi
if freshtag https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/worker-json.js ; then
    curl -sL "https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/worker-json.js" | gzip -9 > ../data/extras/worker-json.js.gz
fi
if freshtag https://github.com/ajaxorg/ace/raw/master/doc/site/images/favicon.ico ; then
    curl -sL https://github.com/ajaxorg/ace/raw/master/doc/site/images/favicon.ico | gzip -9 > ../data/extras/ace.ico.gz
fi
[ html/extras/edit.htm -nt ../data/extras/edit.html.gz ] && gzip -9k html/extras/edit.htm && mv -f html/extras/edit.htm.gz ../data/extras/

# обновить файлы в /login/
for f in html/login/*
do
    if [ ! -f ../data/login/$( basename $f ).gz ] || [ $f -nt ../data/login/$( basename $f ).gz ] ; then
        gzip -f9k $f
        mv -f $f.gz ../data/login/
    fi
done

mv -f newetags.txt $tags

#cd ./data
#zip --filesync -r -0 --move --quiet ../data.zip .
#cd ..
#rm -r ./data
#echo "Content of data.zip file should be used to create LittleFS image"