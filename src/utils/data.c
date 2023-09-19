#include "../headers/data.h"

/**
 * Imposta la modalità di esecuzione.
 * @param exm   La modalità di esecuzione.
*/
void set_exm(ExecutionMode new_mode)
{
    exm = new_mode;
}

/**
 * Restituisce la modalità di esecuzione.
 * @return  La modalità di esecuzione.
*/
ExecutionMode get_exm()
{
    return exm;
}