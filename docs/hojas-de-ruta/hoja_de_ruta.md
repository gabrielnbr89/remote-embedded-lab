# 🚀 Portfolio Vivo IoT: Hoja de Ruta & Arquitectura de Sistema

Este documento define la planificación y diseño técnico para el desarrollo, integración y despliegue del portfolio profesional alojado localmente en un nodo **Raspberry Pi 4 (4GB)** expuesto de forma segura mediante **Cloudflare Tunnel** (CG-NAT bypass).

---

## 🎯 Objetivos Principales
* **Demostración de Integración Vertical:** Mostrar solvencia técnica conectando hardware (C/C++), redes/Linux, broker MQTT, base de datos relacional, backend API/WebSockets y frontend reactivo.
* **Proyectos Físicos y Reales:** Cero simulaciones; interacciones directas con procesos del sistema Linux y nodos de borde (*edge devices*).
* **Entorno de Administración Remota:** Configuración de túnel TCP para desarrollo continuo vía VS Code SSH desde cualquier ubicación.

---

## 📍 Hoja de Ruta por Fases

### [FASE 1: Infraestructura Base & Entorno Linux]
- [X] Instalación y *hardening* de **Raspberry Pi OS Lite (64-bit)**.
- [X] Configuración de **Cloudflare Tunnel (`cloudflared`)**:
  - Túnel SSH TCP (`ssh.tu-dominio.com`) para administración remota.
  - Túnel HTTP/HTTPS para el servicio web en producción.
- [X] Despliegue de entorno virtualizado con **Docker & Docker Compose**:
  - **Nginx / Caddy:** Reverse Proxy.
  - **Mosquitto MQTT Broker:** Comunicación de sensores (Puerto 1883).
  - **MariaDB:** Persistencia relacional de telemetría y mensajes.

### [FASE 2: Pipeline IoT & Backend (Express)]
- [ ] Firmware C++ para nodo de borde (**ESP-01 / ESP8266**):
  - Lectura de sensores (**DHT11/DHT22** + **LDR**).
  - Publicación periódica vía MQTT (`PubSubClient`).
- [ ] Servicio de Backend con **Node.js + Express**:
  - Suscripción MQTT a los tópicos del ESP-01.
  - Servidor de **WebSockets (Socket.io)** para actualización *real-time* en el frontend.
  - Inserción programada de datos de telemetría en **MariaDB**.
  - Endpoint de telemetría del servidor Linux (`/proc`, RAM, Temp SoC vía `vcgencmd`).
  - Módulo de notificaciones asíncronas (**Bot de Telegram**).

### [FASE 3: Frontend Interactivo (Vite + React)]
- [ ] UI/UX con concepto de **"Dashboard de Control / Nodo de Ingeniería"**.
- [ ] **Widget Animado "El Clima de mi Taller":**
  - Ilustración dinámica (ESP-01 + cielo en *pixel art* / vectorial).
  - Cambio de estados visuales según variables físicas (Día, Noche, Nublado, Tormenta).
- [ ] **Panel de Gráficos Históricos:**
  - Renderizado de curvas de Temperatura, Humedad y Luz de las últimas 24hs (Chart.js / Recharts).
- [ ] **Monitor de Telemetría Hardware (Raspi):**
  - Métrica de recursos en vivo del servidor alojado.
- [ ] **Muro de Contacto Persistente & CLI Terminal:**
  - Guardado en MariaDB + cálculo de latencia + alerta push a Telegram.
  - Consola interactiva sanitizada para recorrer el CV en modo terminal.

### [FASE 4: Seguridad, Optimización & Deploy Final]
- [ ] Sanitización estricta de entradas y **Rate Limiting** en Express.
- [ ] Manejo de reconexión automática en reconexiones MQTT y WebSockets.
- [ ] Optimización de uso de RAM/CPU en el SoC ARM de la Raspi 4.
- [ ] Verificación del flujo completo: `ESP-01 -> Mosquitto -> Express -> MariaDB -> React -> Cloudflare`.

---

## 🌤️ Detalle de Módulo: "El Clima de mi Taller en Tiempo Real"

### Matriz de Determinación de Estado Visual:
| Lectura LDR (Luz) | Lectura Humedad / Temp | Estado Determinado | Render en Widget |
| :--- | :--- | :--- | :--- |
| **Alta** | Humedad Normal | **Día Despejado** | Sol animado, cielo azul, ESP-01 activo |
| **Media / Baja** | Humedad Alta / Caída Temp | **Nublado / Lluvia** | Nubes animadas, cielo gris, gotas de lluvia |
| **Cero / Muy Baja**| Indiferente | **Noche** | Luna, estrellas, ESP-01 iluminado / "durmiendo" |

---

## 🛠️ Stack Tecnológico
* **Hardware:** Raspberry Pi 4 (4GB), ESP-01 / ESP8266, DHT11/DHT22, LDR.
* **OS & Infra:** Raspberry Pi OS Lite (64-bit), Docker, Cloudflare Zero Trust.
* **Broker & Protocols:** Mosquitto MQTT, WebSockets (Socket.io), SSH.
* **Backend:** Node.js, Express, MariaDB.
* **Frontend:** React, Vite, CSS Variables / Tailwind, Chart.js / Recharts.