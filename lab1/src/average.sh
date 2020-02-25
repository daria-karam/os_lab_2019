n=0
middle=0

if [ 0 -ne $# ]
then
while [ -n "$1" ]
do
middle=$[ middle + $1 ]
let "n += 1"
shift
done

else
while read LINE
do
middle=$[ middle + $LINE]
let "n += 1"
shift
done
fi

middle=$[ middle / n ]
echo $n
echo $middle