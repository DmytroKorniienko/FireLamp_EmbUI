#!/bin/sh

echo "Упаковка ресурсов css"
cat default/css/*.css | gzip -9 > ../../../data/css/all.css.gz

echo "Упаковка ресурсов js"
cat default/js/*.js | gzip -9 > ../../../data/js/all.js.gz

echo "Упаковка ресурсов index.html"
cat default/index.html | gzip -9 > ../../../data/index.html.gz

echo "Упаковка favicon"
cat default/favicon.ico | gzip -9 > ../../../data/favicon.ico.gz
