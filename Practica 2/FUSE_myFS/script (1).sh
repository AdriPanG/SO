#!/bin/bash
cd mount-point
touch file1.txt
if [[ -f file1.txt ]]; then
	
	ls
	echo "Funcionalidad creación de archivo con mknod comprobada"
	err = 0
else
	echo "Hay algún error en mknod"
	err = 1
fi;
if [[ $err -eq 0 ]]; then
	unlink file1.txt
	if [[ -f file1.txt ]]; then
		echo "Hay algún error en unlink"
	else
		echo "Funcionalidad eliminación de archivo con unlink comprobada"
	fi;
fi;

