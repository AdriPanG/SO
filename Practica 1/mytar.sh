#!/bin/bash
#Vemos si el programa esta en el dir actual y tiene permisos de ejecucion
if [[ -x mytar ]]; then
	echo Tiene permisos de ejecucion;
	if [ -d tmp ]; then
		echo "Existe el directorio tmp";
		rm -rf tmp
		echo "Se ha borrado todo";
	else
		echo "No existe el directorio";
	fi
	#Creamos el directorio tmp
	mkdir tmp;
	cd tmp;
	#Dentro de tmp creamos los tres ficheros
	echo "Hello world" > file1.txt;
	head -n 10 /etc/passwd > file2.txt;
	head -c 1024 /dev/urandom > file3.txt;
	#Ejecutamos mytar creando el tar de los tres ficheros
	../mytar -cf filetar.mtar file1.txt file2.txt file3.txt;
	#Creamos un directorio dentro de tmp
	mkdir out
	#Copiamos el tar en out
	cp filetar.mtar out
	#Nos metemos en el directorio out
	cd out
	../../mytar -xf filetar.mtar
	for ((i = 1; i <= 3; i++))
		do
			diff "file$i.txt" "../file$i.txt";
			if [[ $? != 0 ]]; then
				cd ../..;
				echo "Ha fallado la comprobación del fichero file$i.txt";
				exit 1;
			fi
		done
	cd ../..;
	echo Correct;
	exit 0;
else
	echo No existe o no tiene permisos de ejecucion;
fi
