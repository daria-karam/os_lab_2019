n=$#
middle=0
for (( i = 1; i <= n; i++ ))
do
middle=$[ $middle + ${i} ]
done
middle=$[ $middle / $n ]
echo $n
echo $middle