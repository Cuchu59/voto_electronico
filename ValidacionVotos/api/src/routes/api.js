// src/routes/api.js
import express from 'express';
import * as VotoCtrl from '../controllers/electorController.js';

const router = express.Router();

// Esta es la ruta que llamará el ESP32
// El objeto 'io' se pasa desde server.js
export const setupRoutes = (io) => {
    router.post('/scan', (req, res) => VotoCtrl.procesarEscaneo(req, res, io));
    return router;
};