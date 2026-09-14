# 📋 Webserv — Resumen de Requisitos

## 🚨 REGLAS CRÍTICAS Y PROHIBICIONES

- El programa **no debe crashear jamás**, bajo ninguna circunstancia (incluso sin memoria) ni terminar inesperadamente → si ocurre, **nota final: 0**.
- **Prohibido usar `execve` para lanzar otro servidor web.**
- El servidor debe permanecer **no bloqueante en todo momento** y manejar correctamente desconexiones de clientes.
- Debe usarse **un único `poll()` (o equivalente)** para TODAS las operaciones de I/O entre clientes y servidor (incluyendo el listen).
- `poll()` (o equivalente) debe monitorizar **lectura y escritura simultáneamente**.
- **Prohibido hacer `read`/`write` sin pasar antes por `poll()`** (o equivalente).
- **Prohibido comprobar `errno`** para ajustar el comportamiento del servidor tras una operación de `read`/`write`.
- Excepción: no es obligatorio usar `poll()` para ficheros regulares de disco (`read()`/`write()` en ellos no requieren notificación de disponibilidad).
- **Llamar a `read`/`recv` o `write`/`send` sobre descriptores que esperan datos (sockets, pipes/FIFOs) sin notificación previa de readiness = nota 0.**
- Una petición al servidor **nunca debe quedarse colgada indefinidamente**.
- El servidor debe ser **compatible con navegadores web estándar**.
- Los **códigos de estado HTTP de la respuesta deben ser precisos**.
- Debe haber **páginas de error por defecto** si no se proporcionan.
- **`fork` solo puede usarse para CGI** (PHP, Python, etc.), no para otra cosa.
- Debe poder **servir un sitio web totalmente estático**.
- Los clientes deben poder **subir ficheros (upload)**.
- Se requieren **al menos los métodos GET, POST y DELETE**.
- Debe poder **escuchar en múltiples puertos** simultáneamente.
- **Prohibidas librerías externas y Boost.**
- (macOS) `fcntl()` solo permitido con los flags: `F_SETFL`, `O_NONBLOCK`, `FD_CLOEXEC` — **cualquier otro flag está prohibido**.
- El **feature de virtual host está fuera de scope** (opcional, no obligatorio).
- El bonus **solo se evalúa si la parte obligatoria está 100% completa y sin fallos**.
- Debe presentarse preparado para una **modificación en vivo durante la evaluación** (cambio de comportamiento/función/estructura en pocos minutos).

---

## ⚙️ COMPILACIÓN

- **Comando:** `c++`
- **Flags requeridos:** `-Wall -Wextra -Werror`
- **Versión/Estándar:** C++98 (debe compilar también añadiendo `-std=c++98`)
- **Makefile obligatorio** con reglas mínimas: `$(NAME)`, `all`, `clean`, `fclean`, `re`
- El Makefile **no debe hacer relinking innecesario**
- Preferir funciones C++ sobre C cuando exista equivalente (ej. `<cstring>` en vez de `<string.h>`), aunque se permite usar funciones C.

---

## 📁 ARCHIVOS Y NOMENCLATURA

- **Nombre del programa:** `webserv`
- **Ejecución:** `./webserv [configuration file]`
- **Ficheros a entregar:** `Makefile`, `*.{h, hpp}`, `*.cpp`, `*.tpp`, `*.ipp`, ficheros de configuración.
- **Argumentos:** archivo de configuración (obligatorio como argumento o mediante ruta por defecto).
- **Libft:** no aplica (n/a) para este proyecto.
- Se debe proveer **archivo(s) de configuración y ficheros por defecto** que demuestren que cada feature funciona durante la evaluación.
- **README.md obligatorio** en la raíz del repo:
  - Primera línea en **cursiva**: *This project has been created as part of the 42 curriculum by <login1>[, <login2>...]*
  - Sección **"Description"**
  - Sección **"Instructions"** (compilación/instalación/ejecución)
  - Sección **"Resources"** (referencias + descripción de uso de IA: en qué tareas/partes)
  - Secciones adicionales según lo requiera el proyecto
  - **Debe estar escrito en inglés**

---

## 💻 CÓDIGO Y ESTILO

- Todo debe implementarse en **C++98**.
- **Funciones externas permitidas (lista cerrada):** `execve`, `pipe`, `strerror`, `gai_strerror`, `errno`, `dup`, `dup2`, `fork`, `socketpair`, `htons`, `htonl`, `ntohs`, `ntohl`, `select`, `poll`, `epoll` (`epoll_create`, `epoll_ctl`, `epoll_wait`), `kqueue` (`kqueue`, `kevent`), `socket`, `accept`, `listen`, `send`, `recv`, `shutdown`, `chdir`, `bind`, `connect`, `getaddrinfo`, `freeaddrinfo`, `setsockopt`, `getsockname`, `getprotobyname`, `fcntl`, `close`, `read`, `write`, `waitpid`, `kill`, `signal`, `access`, `stat`, `open`, `opendir`, `readdir`, `closedir`.
- Al usar `poll()`/equivalente, se permiten macros/helpers asociados (ej. `FD_SET` para `select()`).
- **Configuración del servidor (config file) debe permitir definir:**
  - Pares `interfaz:puerto` de escucha (múltiples sitios).
  - Páginas de error por defecto.
  - Tamaño máximo permitido para el body de las peticiones.
  - Reglas por ruta/URL (sin regex):
    - Métodos HTTP aceptados por ruta.
    - Redirección HTTP.
    - Directorio raíz de la ruta (root mapping).
    - Activar/desactivar listado de directorio (directory listing).
    - Fichero por defecto servido si el recurso es un directorio.
    - Ubicación de almacenamiento para uploads.
    - Ejecución de CGI según extensión de fichero (ej. `.php`).
- **Reglas específicas de CGI:**
  - Variables de entorno correctas deben pasarse al CGI (request completo + argumentos del cliente disponibles).
  - Para requests *chunked*, el servidor debe **des-chunkear** antes de pasarlos al CGI (el CGI espera EOF como fin de body).
  - Igual para la salida del CGI: si no hay `content_length`, EOF marca el fin de los datos devueltos.
  - El CGI debe ejecutarse en el **directorio correcto** para acceso a ficheros con rutas relativas.
  - Debe soportarse **al menos un tipo de CGI** (php-CGI, Python, etc.).
- Se recomienda **probar con telnet y NGINX** antes de empezar, y comparar comportamiento/headers con NGINX.
- Se sugiere **HTTP 1.0** como referencia (no obligatorio implementar el RFC completo).
- **Bonus:** soporte de cookies/gestión de sesiones (con ejemplos simples), soporte de múltiples tipos de CGI.