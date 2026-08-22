# Remote Embedded Lab — Hoja de ruta MVP

## 1. Idea general

Construir una pequeña infraestructura IoT doméstica que forme parte de mi portfolio técnico.

El sistema debe demostrar:

- Desarrollo backend y frontend.
- Linux y administración de servicios.
- Redes locales.
- Comunicación MQTT.
- HTTP.
- Integración de sistemas embebidos.
- Control remoto de hardware.
- Gestión energética de nodos.
- Telemetría en tiempo real.

La idea central es que **no todos los dispositivos permanezcan encendidos permanentemente**.

Un nodo de bajo consumo permanece disponible y puede energizar otros nodos bajo demanda.

---

# 2. Arquitectura inicial

```text
                    INTERNET
                        │
                        │
                 Cloudflare Tunnel
                        │
                        ▼
                ┌───────────────┐
                │ Raspberry Pi 4│
                │               │
                │ - Web         │
                │ - API Node.js │
                │ - MQTT Broker │
                │ - Database    │
                └───────┬───────┘
                        │
              Red local │ Ethernet
                        │
          ┌─────────────┴─────────────┐
          │                           │
          ▼                           ▼
    ┌───────────┐              ┌─────────────┐
    │  ESP-01   │              │ Arduino Mega│
    │ ESP8266   │              │ + Ethernet  │
    │           │              │ + DHT11     │
    │ MQTT      │              │             │
    │           │              │ MQTT / HTTP │
    │ Relé      │──── 5V ────► │ Nodo activo │
    └───────────┘              └─────────────┘
         ▲                            │
         │                            │
         └──── Fuente 5V ─────────────┘
```

---

# 3. Roles de cada dispositivo

## Raspberry Pi 4

La Raspberry será el servidor central.

Responsabilidades:

- Servidor web.
- Backend.
- API.
- Broker MQTT.
- Base de datos.
- Comunicación con Cloudflare Tunnel.
- Gestión del estado de los nodos.
- Registro de eventos.
- Comunicación en tiempo real con el frontend.

La Raspberry será el **único punto expuesto hacia Internet**.

Los microcontroladores permanecerán dentro de la red local.

---

## ESP-01 + módulo relé

Será el **Power Controller** del sistema.

Responsabilidades:

- Permanecer conectado permanentemente a la red.
- Escuchar comandos MQTT.
- Activar o desactivar el relé.
- Controlar la alimentación de la Arduino Mega.
- Informar su propio estado al servidor.

Ejemplo de comando:

```text
lab/power/mega/command
```

Payload:

```json
{
  "action": "on"
}
```

Respuesta:

```text
lab/power/mega/status
```

```json
{
  "powered": true,
  "timestamp": "..."
}
```

---

## Arduino Mega + Ethernet Shield

Será un nodo de adquisición de datos que normalmente estará apagado.

Cuando recibe alimentación:

1. Arranca.
2. Inicializa Ethernet.
3. Obtiene una dirección IP.
4. Se conecta al broker MQTT.
5. Publica un evento de inicio.
6. Se registra en el sistema.
7. Informa sus capacidades.
8. Comienza a transmitir telemetría.

El objetivo es que la Mega **tenga una función real**, no simplemente demostrar que puede encenderse.

Su primera función será:

- Medición de temperatura.
- Posiblemente humedad.
- Uso del DHT11 como primer sensor.

Más adelante podría reemplazarse o ampliarse con otros proyectos, como el medidor de consumo eléctrico.

---

# 4. Alimentación

La idea inicial será utilizar una única fuente de 5V con capacidad suficiente.

La fuente alimentará permanentemente:

```text
Fuente 5V
   │
   ├── ESP-01 + módulo relé
   │
   └── COM del relé
          │
          └── NO ─────► 5V Arduino Mega
```

El GND puede permanecer común entre los dispositivos.

El relé únicamente conmuta el positivo de 5V hacia la Mega.

Por lo tanto:

```text
Sistema encendido
│
├── Raspberry Pi
│
└── ESP-01
       │
       └── Mega apagada
```

Cuando se solicita el nodo:

```text
ESP-01 activa relé
        │
        ▼
Mega recibe alimentación
        │
        ▼
Mega arranca desde cero
```

---

# 5. Flujo principal de la demostración

## Estado inicial

El visitante entra a la web.

Ve algo similar a:

```text
Infrastructure status

Raspberry Pi      ONLINE
MQTT Broker       ONLINE
ESP-01            ONLINE
Mega Node         OFFLINE
```

La Mega no está alimentada.

---

## Solicitud de activación

El visitante presiona:

```text
[ Initialize Mega Node ]
```

El frontend realiza una petición al backend.

El backend publica un comando MQTT.

```text
Frontend
   │
   ▼
Backend
   │
   ▼
MQTT Broker
   │
   ▼
ESP-01
   │
   ▼
Relay ON
   │
   ▼
Mega powered
```

---

## Arranque del nodo

La Mega:

1. Arranca.
2. Inicializa el Ethernet Shield.
3. Obtiene IP.
4. Conecta MQTT.
5. Publica un evento `boot`.

Ejemplo:

```json
{
  "device": "mega-01",
  "event": "boot",
  "boot_count": 27,
  "firmware": "1.0.0",
  "uptime": 0
}
```

Después anuncia sus capacidades:

```json
{
  "device": "mega-01",
  "capabilities": [
    "temperature",
    "humidity"
  ],
  "transport": "ethernet",
  "protocol": "mqtt"
}
```

---

# 6. Telemetría

Una vez iniciado, el nodo comienza a publicar datos.

Ejemplo:

```text
lab/nodes/mega-01/telemetry
```

Payload:

```json
{
  "temperature": 24.6,
  "humidity": 53.2,
  "timestamp": "..."
}
```

El backend recibe esos datos y:

- Los guarda en la base de datos.
- Los distribuye al frontend.
- Actualiza los gráficos en tiempo real.

---

# 7. Protocolos

## MQTT

Usarlo para:

- Comandos.
- Estados.
- Eventos.
- Boot.
- Heartbeats.
- Telemetría.

Ejemplo:

```text
lab/
│
├── power/
│   └── mega/
│       ├── command
│       └── status
│
└── nodes/
    └── mega-01/
        ├── boot
        ├── status
        ├── telemetry
        └── heartbeat
```

---

## HTTP

Puede utilizarse para funciones secundarias.

Por ejemplo:

```text
GET /info
```

Respuesta:

```json
{
  "device": "mega-01",
  "firmware": "1.0.0",
  "ip": "192.168.x.x",
  "mac": "...",
  "boot_count": 27
}
```

No es necesario hacer polling constantemente.

La telemetría debe llegar principalmente mediante MQTT.

---

## WebSocket

Utilizarlo entre:

```text
Backend Node.js
        │
        │ WebSocket
        ▼
Frontend
```

Esto permitirá que el usuario vea en tiempo real:

- Cambio de estado de los nodos.
- Logs.
- Eventos.
- Telemetría.
- Proceso de arranque.

---

# 8. Interfaz web

La interfaz debería mostrar el sistema como una pequeña infraestructura.

Ejemplo:

```text
┌──────────────────────────────┐
│      REMOTE EMBEDDED LAB     │
├──────────────────────────────┤
│                              │
│ Raspberry Pi       ● ONLINE  │
│ MQTT Broker        ● ONLINE  │
│ ESP-01             ● ONLINE  │
│ Mega Node          ○ OFFLINE │
│                              │
│ [ INITIALIZE NODE ]          │
│                              │
└──────────────────────────────┘
```

Después de activar el nodo:

```text
Mega Node

Status: ONLINE

Transport: Ethernet
Protocol: MQTT
Firmware: 1.0.0
Boot count: 27

Temperature
24.6 °C

Humidity
53 %

[ Live chart ]

System events:

20:14:03  Command received
20:14:04  Relay activated
20:14:06  Node booting
20:14:09  Ethernet connected
20:14:10  MQTT connected
20:14:11  Telemetry active
```

---

# 9. Primera fase de desarrollo

No construir todo al mismo tiempo.

## Paso 1 — Hardware

- [ ] Montar ESP-01 con el módulo relé.
- [ ] Alimentarlo mediante fuente de 5V.
- [ ] Verificar que el relé pueda encender y apagar la alimentación de la Mega.
- [ ] Conectar la Mega al router mediante Ethernet.
- [ ] Conectar el DHT11.

---

## Paso 2 — ESP-01

- [ ] Conectar el ESP-01 a la red Wi-Fi local.
- [ ] Conectarlo al broker MQTT.
- [ ] Crear comando para activar el relé.
- [ ] Crear comando para desactivar el relé.
- [ ] Publicar el estado del relé.

---

## Paso 3 — Arduino Mega

- [ ] Inicializar Ethernet.
- [ ] Conectar al broker MQTT.
- [ ] Crear contador de arranques.
- [ ] Publicar evento de boot.
- [ ] Publicar información del nodo.
- [ ] Leer DHT11.
- [ ] Publicar telemetría periódicamente.

---

## Paso 4 — Backend

- [ ] Configurar Mosquitto.
- [ ] Crear servicio Node.js.
- [ ] Suscribirse a los tópicos MQTT.
- [ ] Registrar eventos.
- [ ] Crear modelo de datos para telemetría.
- [ ] Crear API para consultar el estado.
- [ ] Implementar WebSocket hacia el frontend.

---

## Paso 5 — Frontend

Primera versión:

- [ ] Estado de infraestructura.
- [ ] Botón para inicializar la Mega.
- [ ] Log en tiempo real.
- [ ] Información del nodo.
- [ ] Temperatura actual.
- [ ] Gráfico histórico.

---

# 10. Objetivo del MVP

El proyecto estará considerado funcional cuando un visitante pueda realizar esta secuencia:

```text
1. Entra a la web
        ↓
2. Ve la Mega apagada
        ↓
3. Solicita activar el nodo
        ↓
4. El backend envía comando MQTT
        ↓
5. ESP-01 activa el relé
        ↓
6. Mega recibe alimentación
        ↓
7. Mega inicia Ethernet
        ↓
8. Mega se conecta al MQTT Broker
        ↓
9. La Mega se registra
        ↓
10. El frontend muestra el nodo ONLINE
        ↓
11. Comienza a llegar telemetría
        ↓
12. El usuario ve datos y gráficos en tiempo real
```

---

# 11. Principio de diseño

La idea no es demostrar:

> "Tengo una Arduino Mega leyendo un DHT11."

La idea es demostrar:

> "Puedo diseñar una infraestructura donde diferentes nodos embebidos, utilizando distintos medios de comunicación, se registran, son administrados remotamente, transmiten telemetría y pueden ser integrados en una plataforma web."

La Mega y el DHT11 son simplemente el primer nodo de esa infraestructura.

El sistema debe comenzar pequeño.

Primero hacer funcionar **este ciclo completo**.

Después se pueden agregar:

- ESP32-S3.
- USB/HID.
- AS5600.
- Medidor de consumo eléctrico.
- Nodo LoRa.
- OTA.
- Sesiones o demostraciones reservadas.
- Nuevos nodos bajo demanda.

Pero nada de eso forma parte del MVP.