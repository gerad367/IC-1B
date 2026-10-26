/* ----------------------------------------------------------------------
 *  Ejemplo ChibiOS-2
 *    Este ejemplo muestra cómo usar los mecanismos de comunicación
 *    entre hebras (mailboxes) disponibles en ChibiOS.
 *    
 *    Requiere el uso de la librería ChRt de Bill Greiman
 *      https://github.com/greiman/ChRt
 *  Asignatura (GII-IoT)
 * ---------------------------------------------------------------------- 
 */

#include <ChRt.h>
#include "config.h"

// ----------------------------------------------------------------------
// Creamos tres hebras 
// ----------------------------------------------------------------------
void chSetup() 
{
  // Aquí asumimos que el valor de CH_CFG_ST_TIMEDELTA es siempre cero
  // para cualquier tarjeta basada en SAMD que están soportadas solo en
  // "tick mode"
  
  // Verificamos primero la configuración de ChibiOS es compatible
  // con un esquema no cooperativo, verificando el valor de CH_CFG_TIME_QUANTUM
  if (CH_CFG_TIME_QUANTUM == 0) {
    debugPort.println("You must set CH_CFG_TIME_QUANTUM to a non-zero value in");
    #if defined(__arm__)
        debugPort.print("src/<board type>/chconfig<board>.h");
    #elif defined(__AVR__)
        debugPort.print("src/avr/chconfig_avr.h"); 
    #endif 
    debugPort.println(" to enable round-robin scheduling.");
    while (true) {}
  } 

  // Vinculamos memPool con el array de objetos printerPool_t.
  chPoolLoadArray(&printerMemPool, printerPool, MB_PRINT_SLOTS);  
                      
  // Lanzamos la hebra Printer
  // Prioridad: PRINTER_THD_PRIO.
  chThdCreateStatic(wa_PrinterThread, sizeof(wa_PrinterThread),
                    PRINTER_THD_PRIO, PrinterThread, NULL);

  // Lanzamos tres hebras de trabajo (Worker)
  // Prioridad: WORK_THD_PRIO
  char name1[10] = "Worker_1";
  char name2[10] = "Worker_2";
  char name3[10] = "Worker_3";
  chThdCreateStatic(wa_WorkThread_1, sizeof(wa_WorkThread_1),
                     WORK_THD_PRIO, WorkThread, name1);
  
  chThdCreateStatic(wa_WorkThread_2, sizeof(wa_WorkThread_2),
                     WORK_THD_PRIO, WorkThread, name2);
  
  chThdCreateStatic(wa_WorkThread_3, sizeof(wa_WorkThread_3),
                     WORK_THD_PRIO, WorkThread, name3);
  
}

// ----------------------------------------------------------------------
// Set up puede usarse para activar los dispositivos que vayan a 
// emplearse
// ----------------------------------------------------------------------
void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  debugPort.begin(115200);
  
  // Blink the LED
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);

  // Wait for USB Serial.
  while (!debugPort) {;}
  debugPort.println("Starting ...");
  
  // chBegin() resetea el stack y nunca retorna.
  chBegin(chSetup);
}

// ----------------------------------------------------------------------
// El entorno Arduino exige la declaración de esta función que se trata
// como una hebra de prioridad normal (NORMALPRIO)
//
// NUNCA usar delay() para imponer un retardo. Si lo hacemos, impediremos
// la ejecución de las hebras de menor prioridad. Esto es fácil de comprobar
// en este ejemplo sustituyendo la llamada a chThdSleepMilliseconds() por
// delay(). Usar siempre chThdSleepMilliseconds() o alguna función 
// equivalente
// ----------------------------------------------------------------------
void loop() 
{ 
  debugPort.println("Loop");
  chThdSleepMilliseconds(5000);
}
