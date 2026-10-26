
// ----------------------------------------------------------------------
// WorkThread - Elabora mensaje que envía a la hebra Printer usando un
//              mailbox.
//              Prioridad: WORK_THD_PRIO
//              STACK: 512 bytes
// ----------------------------------------------------------------------
static THD_FUNCTION(WorkThread, name) 
{
  int msg = 0;
  char _name[10];
  
  strncpy(_name, (char *)name, sizeof(_name)-1);
  _name[sizeof(_name)-1] = 0;

  while (true) {
    
    // Get object from memory pool.
    printerMsg_t* p = (printerMsg_t *)chPoolAlloc(&printerMemPool);
    if (!p) {
      debugPort.print(_name);
      debugPort.println(": chPoolAlloc failed");
    }
    else {
      // Form message.
      p->name = _name;
      p->msg = msg++;
      p->time_i = chVTGetSystemTime();
      
  
      // Send message.
      msg_t s = chMBPostTimeout(&printer_mailbox, (msg_t)p, TIME_IMMEDIATE);
      if (s != MSG_OK) {
        debugPort.print((char*)name);
        debugPort.println(": chMBPost failed");
      }
    }
    chThdSleepMilliseconds(1000);
  }
}
