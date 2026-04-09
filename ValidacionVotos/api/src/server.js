import express from 'express';
import { createServer } from 'http';
import { Server } from 'socket.io';
import cors from 'cors';
import * as VotoCtrl from './controllers/electorController.js';

const app = express();
const httpServer = createServer(app);
const io = new Server(httpServer, { cors: { origin: "*" } });

app.use(cors());
app.use(express.json());

// Ruta para que el ESP32 mande el DNI
app.post('/api/scan', (req, res) => VotoCtrl.procesarEscaneo(req, res, io));

// Lógica de WebSockets
io.on('connection', (socket) => {
  console.log('Alguien se conectó:', socket.id);

  // Cuando el presidente decide
  socket.on('decision_presidente', (data) => {
    // data = { dni: '123...', estado: 'habilitado' }
    console.log(`Presidente decidió: ${data.estado} para DNI: ${data.dni}`);
    
    // Reenviar la decisión a TODOS (o filtrar por sala de mesa)
    io.emit('cambio_estado_voto', data.estado);
  });
});

httpServer.listen(3000, () => {
  console.log('Backend corriendo en http://localhost:3000');
});