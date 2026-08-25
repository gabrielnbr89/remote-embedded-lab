# ESP01 Power Controller

Firmware para un ESP-01 basado en ESP8266 que controla un relé mediante MQTT. El dispositivo se conecta a una red Wi-Fi, recibe comandos para cambiar el estado del relé y publica eventos, estado y heartbeat en formato JSON.

## Hardware

- Placa: ESP-01 / ESP8266
- Relé: GPIO2
- Lógica del relé: activa en `LOW`
- Estado inicial: apagado

## Funcionamiento

`setup()` inicializa, en este orden, el relé, la EEPROM, Wi-Fi y MQTT. En cada iteración de `loop()` se ejecutan las actualizaciones de Wi-Fi, MQTT y logger.

El firmware no bloquea el ciclo principal mientras espera conexiones:

- Reintenta Wi-Fi cada 10 segundos.
- Reintenta MQTT cada 5 segundos cuando hay conexión Wi-Fi.
- Publica un heartbeat cada 30 segundos.
- Si MQTT permanece desconectado durante 60 segundos, apaga el relé como medida de seguridad.
- El contador de arranques se guarda en EEPROM y se incrementa en cada conexión MQTT inicial.

## MQTT

Host, credenciales y Wi-Fi se configuran en `include/secrets.h`. Los tópicos del dispositivo son:

| Tópico | Dirección | Función |
| --- | --- | --- |
| `lab/devices/esp01-power/command` | Entrada | Cambia el relé |
| `lab/devices/esp01-power/state/request` | Entrada | Solicita el estado actual |
| `lab/devices/esp01-power/state` | Salida | Publica el estado del relé |
| `lab/devices/esp01-power/events` | Salida | Publica eventos del dispositivo |
| `lab/devices/esp01-power/heartbeat` | Salida | Publica estado periódico |
| `lab/devices/esp01-power/availability` | Reservado | Definido, pero todavía no se publica |

### Comando

Publicar en `command`:

```json
{"command":true}
```

El valor `true` enciende el relé y `false` lo apaga. Tras un comando válido se publica el nuevo estado y un evento `relay_command`.

### Estado

```json
{"relay":true}
```

El mismo mensaje se publica al recibir un comando válido o una solicitud en `state/request`.

### Eventos y heartbeat

Eventos de arranque y reconexión incluyen los intentos de conexión. El evento de arranque también informa el número de arranques, MAC, IP y RSSI:

```json
{"event":"startup","device_id":"esp01-power","boot_count":1,"wifi":{"attempts":1,"mac":"...","ip":"...","rssi":-55},"mqtt":{"attempts":1}}
```

El heartbeat se publica cada 30 segundos:

```json
{"device_id":"esp01-power","relay":false}
```

## Módulos

- `main.cpp`: inicialización y ciclo principal.
- `wifi.cpp`: modo estación, conexión y reconexión Wi-Fi, MAC, IP, RSSI y contador de intentos.
- `mqtt.cpp`: conexión MQTT, suscripciones, recepción de comandos y temporizador de seguridad.
- `relay.cpp`: configuración del GPIO, lectura, escritura y publicación del estado.
- `storage.cpp`: inicialización de EEPROM y contador persistente de arranques.
- `logger.cpp`: eventos de arranque, reconexión, comandos y heartbeat.
- `config.h`: GPIO, intervalos, tamaño de payload, identificador y tópicos.
- `secrets.h`: datos privados de Wi-Fi y MQTT.

## Configuración y compilación

1. Copiar `include/secrets.example.h` como `include/secrets.h`.
2. Completar SSID, contraseña Wi-Fi, host MQTT, usuario y contraseña.
3. Compilar y cargar con PlatformIO:

```bash
pio run
pio run --target upload
pio device monitor --baud 115200
```

También se puede ejecutar desde VS Code con la extensión PlatformIO.

## Dependencias

- Arduino framework para ESP8266
- `knolleary/PubSubClient`
- `bblanchon/ArduinoJson`

## Estructura

```text
include/   Interfaces, configuración y credenciales
src/       Implementación del firmware
lib/       Librerías locales del proyecto
test/      Pruebas y documentación de pruebas
platformio.ini
```
