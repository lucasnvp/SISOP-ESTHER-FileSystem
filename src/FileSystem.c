/*
 ============================================================================
 Name        : FileSystem.c
 Author      : Lucas Visser
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "FileSystem.h"

int main(void) {
	puts("FileSystem\n"); /* prints Config */

	punto_montaje_setup(PUNTO_DE_MONTAJE);

	//Prueba de creacion de archivos
	bool testbit;

	testbit = bitarray_test_bit(bitarray, 0);
	printf("Testbit: %i\n", testbit);

	//Test CrearArchivo
	CrearArchivo("/home/utnso/Blacklist/SADICA_FS/Archivos/prueba.txt");
	testbit = bitarray_test_bit(bitarray, 0);
	printf("Testbit: %i\n", testbit);

	//Test guardar dato
	GuardarDatos("/home/utnso/Blacklist/SADICA_FS/Archivos/prueba.txt", 0, sizeof(DUMMY_CHAR_FOR_TESTS), DUMMY_CHAR_FOR_TESTS);

	//Test leer dato
	char* datoObtenido = ObtenerDatos("/home/utnso/Blacklist/SADICA_FS/Archivos/prueba.txt", 0, sizeof(DUMMY_CHAR_FOR_TESTS));
	printf("Dato obtenido: %s\n", datoObtenido);

	//Test borrar archivo
	Borrar("/home/utnso/Blacklist/SADICA_FS/Archivos/prueba.txt");
	testbit = bitarray_test_bit(bitarray, 0);
	printf("Testbit: %i\n", testbit);

	return EXIT_SUCCESS;
}

uint32_t ValidarArchivo(char* path){
	uint32_t archivo_existente;
	if(access(path, F_OK) != -1){
		archivo_existente = true;
	} else{
		archivo_existente = false;
	}
	return archivo_existente;
}

void CrearArchivo(char* path){
	if(ValidarArchivo(path) == false){
		FILE * newFD;
		newFD = fopen(path, "w+");
		fwrite(TAMANIO_BLOQUE_NULL,1,strlen(TAMANIO_BLOQUE_NULL),newFD);
		//Agregar el bloque
	 	int32_t bloque_to_add = bloque_libre(bitarray);
		if(bloque_to_add != -1){
			char* bloquesDelArchivo = string_new();
			string_append(&bloquesDelArchivo, "BLOQUES=[");
			string_append(&bloquesDelArchivo, string_itoa(bloque_to_add));
			string_append(&bloquesDelArchivo, "]\n");
			fwrite(bloquesDelArchivo,1,strlen(bloquesDelArchivo),newFD);

			bitarray_set_bit(bitarray, bloque_to_add);
		} else{
			//No hay bloques
		}
		fclose(newFD);
	}
}

int32_t bloque_libre(t_bitarray* auxBitArray){
	bool testBit;
	for(i = 0; i <= bitarray_get_max_bit(auxBitArray); i++){
		if(bitarray_test_bit(auxBitArray, i) == false){
			return i;
		}
	}
	//No hay bloques
	return -1;
}

void Borrar(char* path){
	char** bloquestodelete = get_bloques_array(path);
	for(i=0 ; bloquestodelete[i] != NULL ; i++){
		bitarray_clean_bit(bitarray, atoi(bloquestodelete[i]));
	}
	free(bloquestodelete);

	if(remove(path) == 0){
		printf("Se elimino el archivo exitosamente\n");
	} else{
		printf("Error al eliminar el archivo\n");
	}
}

char* ObtenerDatos(char* path, uint32_t offset, uint32_t size){
	char** bloquestoread = get_bloques_array(path);
	char* pathDato = strdup(get_bloque_from_int(atoi(bloquestoread[offset])));
	FILE * fileToRead = fopen(pathDato,"r");
	char* datosObtenidos = malloc( sizeof(char) * size);
	fgets(datosObtenidos,size + 1,fileToRead);
	fclose(fileToRead);
	return datosObtenidos;
}

void GuardarDatos(char* path, uint32_t offset, uint32_t size, char* buffer){
	char** bloquestosave = get_bloques_array(path);
	//en el offset tiene que ir el resultado de una formula para calcular en que bloque esta.
	char* pathDato = strdup(get_bloque_from_int(atoi(bloquestosave[offset])));
//	printf("%s\n", pathDato);
	FILE * fileToWrite = fopen(pathDato, "w+b");
	fwrite(buffer, sizeof(char), size, fileToWrite);
	fclose(fileToWrite);
	free(bloquestosave);
}

char** get_bloques_array(char* path){
	t_config* filetogetbloques = config_create(path);
	char** bloquesarray = config_get_array_value(filetogetbloques, "BLOQUES");
	config_destroy(filetogetbloques);
	return bloquesarray;
}

char* get_bloque_from_int(uint32_t nroDeBloque){
	char* nroBloque = string_new();
	string_append(&nroBloque, montajeBloques);
	string_append(&nroBloque, string_itoa(nroDeBloque));
	string_append(&nroBloque, ".bin");
	return nroBloque;
}
