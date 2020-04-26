#!/bin/sh

echo "Упаковвка ресурсов css"
cat default/css/*.css | gzip -9 > ../../../data/css/all.css.gz

echo "Упаковвка ресурсов js"
cat default/js/*.js | gzip -9 > ../../../data/js/all.js.gz

echo "Упаковвка ресурсов index.html"
cat default/index.html | gzip -9 > ../../../data/index.html.gz

echo "Упаковвка favicon"
cat default/favicon.ico | gzip -9 > ../../../data/favicon.ico.gz
