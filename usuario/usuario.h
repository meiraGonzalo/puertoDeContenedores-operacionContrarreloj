#ifndef USUARIO_H_INCLUDED
#define USUARIO_H_INCLUDED
#include "../globales/globales.h"

void iniciarUsuario(tOperador* usuario);
int lineaValida (const char *linea);
void mostrarResumenUser (const tConfig* config, const tEstado* sis, const tOperador* usuario);
int mostrarResumenMov (FILE* log);

#endif // USUARIO_H_INCLUDED
