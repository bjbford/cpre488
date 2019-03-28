#!/bin/ash
while  [ 1 = 1 ]
do  
    swt=$(devmem 0x41220000 b)
    btn=$(devmem 0x41210000 b)
	count="0";
	i="0";
    while [ $i -lt 8 ]
    do
        #echo "$i"
        if [ $(( $btn & $((1<<$i)) )) = $((1<<$i)) ]; then
        count=$((count + 1))
        #echo "match found"
        fi
        i=$((i + 1))
    done 
    #echo "Count = $count"
    
    if [ $count = 0 ]; then
        #echo "leds = swt"
        leds=$swt
    elif [ $count = 1 ]; then
        #echo "leds = ~swt"
        leds=$((~$swt & 0xFF))
    else
        #echo "leds = 0xFF"
        leds="0xFF"
    fi
    devmem 0x41200000 b $leds
    #echo "$leds"
    
done