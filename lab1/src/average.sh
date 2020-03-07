n=0
middle=0

if [ 0 -ne $# ]
then
    for params in $@
    do
    let "middle += params"
    let "n += 1"
    done
else
    while read LINE
    do
    let "middle += LINE"
    let "n += 1"
    done
fi

let "middle /= n"
echo $n
echo $middle