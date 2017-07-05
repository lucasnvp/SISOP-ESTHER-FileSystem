#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <commons/bitarray.h>
#include <commons/config.h>

#include "setup/setup.h"

static const char* PUNTO_DE_MONTAJE = "/home/utnso/Blacklist/SADICA_FS/";
static const char* TAMANIO_BLOQUE_NULL = "TAMANIO_BLOQUES=0\n";
static const char* DUMMY_CHAR_FOR_TESTS = "Hola";

int i;

uint32_t ValidarArchivo(char* path);
void CrearArchivo(char* path);
void Borrar(char* path);
char* ObtenerDatos(char* path, uint32_t offset, uint32_t size);
void GuardarDatos(char* path, uint32_t offset, uint32_t size, char* buffer);

int32_t bloque_libre(t_bitarray* auxBitArray);
char** get_bloques_array(char* path);
char* get_bloque_from_int(uint32_t nroDeBloque);

#endif /* FILESYSTEM_H_ */
