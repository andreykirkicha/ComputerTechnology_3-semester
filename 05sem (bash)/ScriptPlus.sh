#!/bin/bash
# Написать подробные комментарии
tl='|'
for ((i=$2; $i; i--))
do
	tl=`echo "$tl-"`
done
line=`ls $1`

for word in {ls -m}
do		
	word=${word%,}
	echo "$tl$word%,"
	if [ $2 > 0 ]&&[ -d $word ];
	then 
		nextlev=$(($2 - 1))
		bash $0 $1/$word $nextlev
	fi
done
