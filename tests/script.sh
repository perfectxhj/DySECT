#!/bin/bash

cp ../build/in .

n=100000000
steps=1024



for dis in bfs rwalk rwalkcyc
do
   for tl in 256 #64 2048
   do
        for bs in 8 #4 16
        do
            for cap in 1 $n
            do
                for alpha in 1.10 1.15 1.2 1.25
                do
                    echo "Parameters: -n $n -cap $cap -steps $steps -alpha $alpha -tl $tl -bs $bs -${dis} -out out/${dis}_TL${tl}_BS${bs}_al${alpha}_cap${cap}"
                    ./in -n $n -cap $cap -steps $steps -alpha $alpha -tl $tl -bs $bs -${dis} -out lg/${dis}_TL${tl}_BS${bs}_al${alpha}_cap${cap}
                done
            done
        done
    done
done