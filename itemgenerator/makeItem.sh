#!/bin/bash

#smepowercad
#Copyright (C) 2015 Smart Micro Engineering GmbH
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#GNU General Public License for more details.
#You should have received a copy of the GNU General Public License
#along with this program. If not, see <http://www.gnu.org/licenses/>.


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

cp cad_template.cpp ../items/cad_"$mainll"_"$subll".cpp
cp cad_template.h   ../items/cad_"$mainll"_"$subll".h
mkdir -p            ../icons/cad_"$mainll"
cp template.svg     ../icons/cad_"$mainll"/cad_"$mainll"_"$subll".svg

sed -i 's/mainuu/'$mainuu'/g' ../items/cad_"$mainll"_"$subll".*
sed -i 's/mainul/'$mainul'/g' ../items/cad_"$mainll"_"$subll".*
sed -i 's/mainll/'$mainll'/g' ../items/cad_"$mainll"_"$subll".*
sed -i 's/subuu/'$subuu'/g' ../items/cad_"$mainll"_"$subll".*
sed -i 's/subul/'$subul'/g' ../items/cad_"$mainll"_"$subll".*
sed -i 's/subll/'$subll'/g' ../items/cad_"$mainll"_"$subll".*
sed -i 's/subspaced/'"$subspaced"'/g' ../items/cad_"$mainll"_"$subll".*
