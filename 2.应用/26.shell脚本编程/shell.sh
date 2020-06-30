#!/bin/bash

a=5
b="hello world"
echo "A is $a"
echo "B is $b"

#1.1
echo "$#" 
echo "$*" 
echo "$0" 
echo "$1" 
echo "$2"

#1.2
var1=10
var2=20
var3=`expr $var1 + $var2`
echo $var3

#1.3
var4=6
if [ $var4 -gt 5 ] 
then 
   echo "var4 bigger than 5"
else
   echo "var4 smller than 5"
fi

#1.4
list="sun mon tue wed thur fri"
for day in $list
do
   echo $day
done

#1.4
var1=5
while [ $var1 -gt 0 ] 
do
  echo $var1
  var1=`expr $var1 - 1`
done 
  
   
 
bye
