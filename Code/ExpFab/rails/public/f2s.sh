for FN in $*
do
	echo changing $FN
	echo "stl${FN//off}stl"
	./meshconv $FN -c stl -o "stl${FN//off}stl"
done