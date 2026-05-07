// index.js
import express from 'express';
import { createServer } from 'http';
import { Server } from 'socket.io';
import mqtt from 'mqtt';
import cors from 'cors'; // Asegúrate de tenerlo instalado: npm install cors
import * as VotoCtrl from './controllers/electorController.js';

const app = express();
app.use(cors()); // Middleware para que la web no rebote
app.use(express.json()); // <--- CRÍTICO: Para que el ESP32 pueda mandar el DNI en JSON

const httpServer = createServer(app);
const io = new Server(httpServer, { 
    cors: { 
        origin: "*", 
        methods: ["GET", "POST"] 
    } 
});

// --- RUTA PARA EL ESP32 (POST DNI) ---
app.post('/api/scan', (req, res) => VotoCtrl.procesarEscaneo(req, res, io));

// --- CONFIGURACIÓN MQTT LOCAL ---
const mqttClient = mqtt.connect('mqtt://localhost:1883');

mqttClient.on('connect', () => {
    console.log('✅ Conectado al broker MQTT en el puerto 1883');
});

// --- LÓGICA DE WEBSOCKETS ---
io.on('connection', (socket) => {
    console.log('Cliente conectado:', socket.id);

    socket.on('decision_presidente', async (data) => {
        if (data.decision === 'aprobado') {
            // CAMBIO AQUÍ: Usamos el nombre real 'habilitarVotante' 
            // y le pasamos (dni, io) como definiste en el controller
            const tokenReal = await VotoCtrl.habilitarVotante(data.dni, io);

    
            const payload = JSON.stringify({ 
                token: tokenReal, 
                dni: data.dni,
                mensaje: "Escanee para votar" 
            });
            
            mqttClient.publish('voto/mesa1/display', payload);
            socket.emit('token_enviado_al_hardware', { success: true });
        }
    });

    socket.on('validar_qr_app', (tokenEscaneado) => {
        const esValido = VotoCtrl.validarToken(tokenEscaneado);
        if (esValido) {
            socket.emit('acceso_concedido', { canVote: true });
        } else {
            socket.emit('acceso_denegado', { error: "Código inválido" });
        }
    });
});

httpServer.listen(3000, '0.0.0.0', () => {
    console.log('🚀 Server running on port 3000');
});