# Report — Organizzazione della cartella di una libreria C

## Obiettivo

Questo documento riassume una struttura professionale per organizzare una libreria C riusabile, separando chiaramente:

- API pubblica
- implementazione privata
- configurazione
- test ed esempi

L'obiettivo è rendere la libreria:

- autonoma nel proprio repository
- facile da integrare in un progetto applicativo
- semplice da versionare
- chiara da mantenere nel tempo

---

## Struttura consigliata

```text
logger/
├─ include/
│  └─ logger/
│     └─ logger.h
├─ src/
│  ├─ logger.c
│  └─ logger_internal.h
├─ config/
│  └─ logger_config.h.template
├─ tests/
├─ examples/
└─ README.md
```

Questa struttura è adatta a una libreria C mantenuta in un repository separato e collegata al progetto applicativo come dipendenza versionata.

---

## Significato delle cartelle

### `include/`

È la root degli **header pubblici** della libreria.

Non contiene implementazione, ma solo ciò che deve essere visibile all'esterno.

Nel caso della libreria `logger`:

```text
include/
└─ logger/
   └─ logger.h
```

Questo consente all'applicazione di includere l'API pubblica in modo ordinato:

```c
#include <logger/logger.h>
```

### Perché usare `include/logger/` e non mettere direttamente `logger.h` in `include/`

Usare una sottocartella con il nome della libreria fornisce un namespace chiaro e riduce il rischio di collisioni con altre librerie.

Esempio corretto:

```text
include/logger/logger.h
```

Esempio meno robusto:

```text
include/logger.h
```

La prima forma è preferibile in librerie professionali e scalabili.

---

### `src/`

Contiene l'implementazione della libreria.

Esempio:

```text
src/
├─ logger.c
└─ logger_internal.h
```

Qui devono stare:

- file `.c`
- header privati
- strutture dati interne
- funzioni statiche o dettagli di implementazione

Gli header in `src/` **non devono essere inclusi dall'applicazione**.

---

### `config/`

Contiene il **template** del file di configurazione, non il file realmente usato in compilazione dall'applicazione.

Esempio:

```text
config/logger_config.h.template
```

Il template serve come:

- documentazione delle opzioni disponibili
- esempio di configurazione
- base da copiare nel progetto applicativo

Il file reale di configurazione dovrebbe stare nel repository dell'applicazione, ad esempio:

```text
cfg/logger_config.h
```

Nel codice della libreria conviene includere sempre il nome logico stabile:

```c
#include "logger_config.h"
```

Il build system dell'applicazione farà in modo che venga trovato il file corretto in `cfg/`.

---

### `tests/`

Contiene i test unitari o di integrazione della libreria.

Esempi:

```text
tests/
├─ test_logger.c
└─ test_logger_init.c
```

Questa cartella permette di validare la libreria in modo indipendente dall'applicazione finale.

---

### `examples/`

Contiene esempi minimali di utilizzo della libreria.

È utile per:

- mostrare l'uso corretto dell'API
- aiutare l'integrazione
- fornire mini-programmi di riferimento

---

### `README.md`

Dovrebbe descrivere in modo sintetico:

- scopo della libreria
- dipendenze
- modalità di integrazione
- configurazione richiesta
- esempio d'uso

---

## Separazione tra pubblico e privato

Una best practice fondamentale è separare nettamente:

### API pubblica

Va in `include/logger/`.

Qui vanno solo:

- prototipi pubblici
- tipi pubblici
- enum pubblici
- macro strettamente di interfaccia

### Implementazione privata

Va in `src/`.

Qui vanno:

- strutture interne
- funzioni di supporto
- dettagli nascosti all'utente
- header interni come `logger_internal.h`

Questa separazione evita che l'applicazione dipenda da dettagli che potrebbero cambiare.

---

## Configurazione: template nella libreria, file reale nell'app

Nel caso discusso, la libreria vive in un repository dedicato e viene collegata al progetto applicativo come dipendenza esterna.

In questo scenario è corretto che:

- il repository della libreria contenga solo il template della configurazione
- il progetto applicativo contenga la configurazione effettiva

### Esempio

Nel repo della libreria:

```text
logger/
└─ config/
   └─ logger_config.h.template
```

Nel repo dell'applicazione:

```text
app_project/
└─ cfg/
   └─ logger_config.h
```

In questo modo:

- la libreria non viene modificata direttamente nel progetto applicativo
- la configurazione resta sotto controllo dell'applicazione
- l'aggiornamento della libreria è più semplice

---

## Libreria in repository separato

Se la libreria è mantenuta in un repository autonomo, è preferibile che anche la cartella `include/` stia **dentro** la libreria.

Esempio corretto:

```text
logger/
├─ include/
├─ src/
├─ config/
└─ tests/
```

Questa scelta offre vantaggi concreti:

- la libreria è autosufficiente
- può essere testata in isolamento
- può essere riusata in altri progetti senza cambiare struttura
- l'API pubblica è chiaramente identificata

---

## Perché non usare una `include/` condivisa fuori dalla libreria

Una `include/` condivisa a livello di progetto può avere senso in un monorepo unico, ad esempio:

```text
project/
├─ include/
├─ libs/
└─ app/
```

ma nel caso di una libreria in repository separato è meno indicata, perché rende la libreria meno autonoma.

Per una libreria indipendente è più pulito mantenere tutto il contenuto pubblico sotto la propria directory.

---

## Convenzione di inclusione consigliata

Con questa struttura, il progetto applicativo aggiunge agli include path:

```text
.../logger/include
.../cfg
```

e include l'header pubblico così:

```c
#include <logger/logger.h>
```

mentre la libreria include la configurazione così:

```c
#include "logger_config.h"
```

---

## Esempio pratico finale

### Repository libreria

```text
logger/
├─ include/
│  └─ logger/
│     ├─ logger.h
│     ├─ logger_types.h
│     └─ logger_version.h
├─ src/
│  ├─ logger.c
│  └─ logger_internal.h
├─ config/
│  └─ logger_config.h.template
├─ tests/
│  └─ test_logger.c
├─ examples/
│  └─ basic_example.c
└─ README.md
```

### Repository applicazione

```text
app_project/
├─ app/
├─ cfg/
│  └─ logger_config.h
├─ external/
│  └─ logger/
└─ build/
```

---

## Regole pratiche finali

1. Mettere in `include/` solo header pubblici.
2. Mettere in `src/` implementazione e header privati.
3. Mettere in `config/` solo template o documentazione della configurazione.
4. Tenere il file reale di configurazione nel progetto applicativo.
5. Usare una sottocartella namespace sotto `include/`, ad esempio `include/logger/`.
6. Non esporre mai gli header interni all'applicazione.
7. Mantenere la libreria autosufficiente nel proprio repository.

---

## Conclusione

La struttura consigliata per una libreria C professionale è:

```text
logger/
├─ include/
│  └─ logger/
│     └─ logger.h
├─ src/
│  ├─ logger.c
│  └─ logger_internal.h
├─ config/
│  └─ logger_config.h.template
├─ tests/
├─ examples/
└─ README.md
```

È una struttura solida, leggibile e adatta a uno sviluppo professionale, soprattutto quando la libreria vive in un repository dedicato ed è integrata nell'applicazione come dipendenza esterna versionata.
