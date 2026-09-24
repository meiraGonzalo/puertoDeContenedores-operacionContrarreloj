#ifndef USUARIO_H_INCLUDED
#define USUARIO_H_INCLUDED

void iniciarUsuario(tOperador* usuario);
int lineaValida (const char *linea);
void mostrarResumenUser (const tConfig* config, const tEstado* sis, const tOperador* usuario);

#endif // USUARIO_H_INCLUDED
