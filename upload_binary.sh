
MingwBin="/c/mingw/bin"
QtBin="/c/Qt/4.7.4/bin"
QtPlugins="/c/Qt/4.7.4/plugins"
UsrBin="/c/lib/bin"

cd release

mkdir -p picture_search/plugins/imageformats
mkdir -p picture_search/plugins/sqldrivers

cp $QtBin/QtCore4.dll picture_search
cp $QtBin/QtGui4.dll picture_search
cp $QtBin/QtSql4.dll picture_search

cp $QtPlugins/sqldrivers/qsqlite4.dll picture_search/plugins/sqldrivers
cp $QtPlugins/sqldrivers/qsqlmysql4.dll picture_search/plugins/sqldrivers
cp $QtPlugins/imageformats/*.dll picture_search/plugins/imageformats
rm picture_search/plugins/imageformats/*d4.dll

cp $MingwBin/libgcc_s_dw2-1.dll picture_search
cp $MingwBin/mingwm10.dll picture_search
cp $MingwBin/libstdc++-6.dll picture_search

cp $UsrBin/libmysql.dll picture_search

cp picture_search.exe picture_search
cp ../conf/qt.conf picture_search

rm -f picture_search.zip

7z a picture_search.zip picture_search

scp picture_search.zip picture_search.exe root@mugiseyebrows.ru:/var/www/download/picture_search/

cd ../