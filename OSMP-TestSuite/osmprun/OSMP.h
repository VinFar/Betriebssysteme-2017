/* OSMP.h 
 * Praktikum BS SS2017 (POSIX)
 *
 * Interface Header der OSMPlib
 *
 * R. Mentrup 5-2017
 */

#ifndef OSMPHEADER
#define OSMPHEADER

#include <sys/types.h>

/* Rueckgabewerte fuer OSMP - Routinen */
#define OSMP_SUCCESS (0)
#define OSMP_ERROR   (-1)

/* Dynamische Speicherverwaltung sehr aufwendig, daher feste Groessen vorgegeben */

/* maximale Zahl der Nachrichten pro Prozess */
#define OSMP_MAX_MESSAGES_PROC	16  

/* maximale Anzahl der Nachrichten, die insgesamt vorhanden sein duerfen */ 
#define OSMP_MAX_SLOTS		256

/* maximale Laenge der Nutzlast einer Nachricht */ 
#define OSMP_MAX_PAYLOAD_LENGTH	128

/* Funktions-Prototypen */
int OSMP_Init(int *argc, char ***argv);
int OSMP_Size(int *size); 
int OSMP_Rank(int *rank);
int OSMP_Finalize(void);

/* Die Routinen zum Senden und Empfangen von Nachrichten OSMP_Send() und OSMP_Recv () 
 sollen blockierend sein, d.h. bei der Rueckkehr des aufrufenden Prozesses aus der 
 Routine koennen alle verwendeten Puffer sofort wieder fuer andere Operationen verwendet 
 werden.*/

int OSMP_Send(const void *buf, int count, int dest);
int OSMP_Recv(void *buf, int count, int *source, int *len);
 
/* Die nicht blockierenden Routinen OSMP_Isend(),OSMP_Irecv()und OSMP_Test() kehren zurueck, 
 ohne dass die Operationen vollstaendig abgeschlossen sein muessen. Die eingesetzten Ressourcen 
 wie z.B. die Puffer duerfen zunaechst nicht fuer weitere Operationen verwendet werden. 

 Die Routine OSMP_Test() erlaubt abzufragen, ob eine vorangegangene Operation abgeschlossen 
 ist; die Routine OSMP_Wait() blockiert den aufrufenden Prozess so lange, bis die vorangegangene 
 Operation abgeschlossen ist. */

/* Requester fuer nicht blockierende Kommunikation */
typedef void* OSMP_Request;

int OSMP_Isend(const void *buf, int count, int dest, OSMP_Request *request);
int OSMP_Irecv(void *buf, int count, int *source, int *len, OSMP_Request *request);

int OSMP_Test(OSMP_Request *request, int *flag); 
int OSMP_Wait(OSMP_Request *request);


/* Makro gegen Compiler Warnings */
#define UNUSED(x) (void)(x)

#endif
