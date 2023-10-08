#!/bin/bash
# Написать подробные комментарии
if [ -d dirs ]
then
  rm -r dirs
fi

num=$1
x=1

mkdir dirs
cd dirs
while [ $x -le $num ]
do
  mkdir $x
  cd $x
  y=1
  while [ $y -le $num ]
  do
    mkdir $x$y
    echo "$x$y" > $x$y/$x$y
    y=$(( $y+1 ))
  done
  cd ..
  x=$(( $x+1 ))
done
