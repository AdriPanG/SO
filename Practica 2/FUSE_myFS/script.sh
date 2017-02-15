#!/bin/bash
cd mount-point
touch file1.txt
if [[ -f file1.txt ]]; then
	echo "Funcionalidad creación de archivo con mknod comprobada"
else
	echo "Hay algún error en mknod"
fi


