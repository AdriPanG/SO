#!/bin/bash
err=0;
cd mount-point
#Creamose el archivo
touch file3.txt
if [[ -f file3.txt ]]; then
	
	ls
	echo "Funcionalidad creación de archivo con mknod comprobada"
else
	echo "Hay algún error en mknod"
fi;

#Escribimos en el archivo
echo "hola grupo" >> file3.txt
#leemos lo del archivo
echo "Ahora vemos lo que hay escrito"
cat file3.txt

#Borramos el archivo que hemos creado antes
rm -R file3.txt
if [[ -f file3.txt ]]; then
	echo "Hay algún error en unlink"
else
	echo "Funcionalidad eliminación de archivo con unlink comprobada"
	ls
fi;

#Creamos un directorio
mkdir Grupo 
if [[ -d Grupo ]]; then
	echo "Funcionalidad de que se ha creado un directorio"
	
else 
	echo "Este directorio no existe "
fi;
