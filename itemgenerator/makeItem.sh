#!/bin/bash

echo "Enter space separated main text: "
read mainspaced
echo "Enter space separated sub text: "
read subspaced

mainul=`echo $mainspaced | sed 's/ //g'`
subul=`echo $subspaced | sed 's/ //g'`
mainuu=`echo $mainul | tr "[:lower:]" "[:upper:]"`
subuu=`echo $subul | tr "[:lower:]" "[:upper:]"`
#mainul="Main"
#subul="Sub"
mainll=`echo $mainul | tr "[:upper:]" "[:lower:]"`
subll=`echo $subul | tr "[:upper:]" "[:lower:]"`
#subspaced="s u b"

cp cad_template.cpp cad_"$mainll"_"$subll".cpp
cp cad_template.h   cad_"$mainll"_"$subll".h

sed -i 's/mainuu/'$mainuu'/g' cad_"$mainll"_"$subll".*
sed -i 's/mainul/'$mainul'/g' cad_"$mainll"_"$subll".*
sed -i 's/mainll/'$mainll'/g' cad_"$mainll"_"$subll".*
sed -i 's/subuu/'$subuu'/g' cad_"$mainll"_"$subll".*
sed -i 's/subul/'$subul'/g' cad_"$mainll"_"$subll".*
sed -i 's/subll/'$subll'/g' cad_"$mainll"_"$subll".*
sed -i 's/subspaced/'"$subspaced"'/g' cad_"$mainll"_"$subll".*
