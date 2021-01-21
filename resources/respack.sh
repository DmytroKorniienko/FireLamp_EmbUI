#!/bin/sh

# etag file
tags=etags.txt
# embui branch/tag name to fetch
embuitag="dev"


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
if [ ! -f embui.zip ] || freshtag https://github.com/DmytroKorniienko/EmbUI/raw/$embuitag/resources/data.zip ; then
    echo "Updating EmbUI resources"
    curl -sL https://github.com/DmytroKorniienko/EmbUI/raw/$embuitag/resources/data.zip > embui.zip
    unzip -uo -d ../data/ embui.zip "css/*" "js/*"
    #rm embui.zip
else
    echo "EmbUI is up to date"
fi

# добавляем скрипты/стили лампы
cat html/css/custom_drawing.css | gzip -9 > ../data/css/lamp.css.gz
cat html/js/drawing.js | gzip -9 > ../data/js/lamp.js.gz

cp -u html/css/pure-min_*.css ../data/css
unzip -o -d ../data/ embui.zip "css/style_*.gz"
gzip -d ../data/css/style_*.gz
cat ../data/css/*_dark.css | gzip -9 > ../data/css/style_dark.css.gz
cat ../data/css/*_light.css | gzip -9 > ../data/css/style_light.css.gz
rm ../data/css/*_dark.css
rm ../data/css/*_light.css

# update static files if newer
[ html/index.html -nt ../data/index.html.gz ] && gzip -9k html/index.html && mv -f html/index.html.gz ../data/
[ html/favicon.ico -nt ../data/favicon.ico ] &&  gzip -9k html/favicon.ico && mv -f html/favicon.ico.gz ../data/

cp -u html/.exclude.files ../data/
cp -u html/events_config.json ../data/
cp -u html/buttons_config.json ../data/

###
# обновляем ace-editor в data/extras folder
# collect all required ACE editor extensions here
if  freshtag https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/ace.js || \
    freshtag https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/mode-html.js || \
    freshtag https://github.com/ajaxorg/ace-builds/raw/master/src-min-noconflict/mode-json.js ; then

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