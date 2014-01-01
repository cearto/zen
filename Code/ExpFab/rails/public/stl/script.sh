for FN in *.stl.stl
do
	mv "${FN}" "${FN%.stl.stl}.stl"
done