#!/bin/bash

MPOINT="./mount-point"
auditor="./my-fsck"
fichero1="./src/fuseLib.c"
fichero2="./src/myFS.h"
fichero3="./src/fichero3.txt"

#Borramos la carpeta temporal por si esta creada
rm -R -f ./temp
mkdir ./temp

#Copiamos los ficheros al SF y a la carpeta temporal
cp $fichero1 $MPOINT
cp $fichero2 $MPOINT
cp $fichero1 ./temp
cp $fichero2 ./temp

#Auditamos el disco, diff de los ficheros y truncamos el primer fichero con un bloque menos
$auditor virtual-disk
diff $MPOINT/fuseLib.c $fichero1
diff $MPOINT/myFs.h $fichero2
truncate -s -4096 ./temp/fuseLib.c
truncate -s -4096 $MPOINT/fuseLib.c

#Auditamos el disco y diff entre el fichero original y el truncado
$auditor virtual-disk
diff $fichero1 ./temp/fuseLib.c
diff $fichero1 $MPOINT/fuseLib.c

#Copiamos un tercer fichero al SF
echo "Este es el fichero 3" >> $fichero3
cp $fichero3 $MPOINT

#Auditamos el disco y diff entre el fichero original y el truncado
$auditor virtual-disk
diff $fichero3 $MPOINT/fichero3.txt

#Truncamos el segundo fichero para que ocupe algun bloque menos
truncate -s +5K ./temp/myFs.h
truncate -s +5K $MPOINT/myFs.h

#Auditamos el disco y diff entre el fichero original y el truncado
diff $fichero2 ./temp/myFs.h
diff $fichero2 $MPOINT/myFs.h

