#include "../headers/debugger.h"

/**
 * Inzializza il debugger.
 * @param log_filename Nome del file di log.
 */
void init_debugger(const char *log_filename)
{
    log_file = fopen(log_filename, "a");

    if (!log_file)
    {
        perror("Failed to open log file");
        exit(1);
    }
}

/**
 * Imposta il livello di log.
 * @param level Livello di log.
 */
void set_log_level(int level)
{
    log_level = level;
}

/**
 * Scrive un messaggio di log.
 * @param level Livello di log.
 * @param format Formato del messaggio.
 * @param ... Argomenti del messaggio.
*/
void log_message(int level, const char *format, ...)
{
    if (level >= log_level)
    {
        va_list args;
        va_start(args, format);

        vfprintf(log_file, format, args);
        fprintf(log_file, "\n");
        fflush(log_file);

        va_end(args);
    }
}

/**
 * Chiude il debugger.
*/
void close_debugger()
{
    if (log_file)
    {
        fclose(log_file);
        log_file = NULL;
    }
}
