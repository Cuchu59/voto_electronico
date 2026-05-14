import * as ElectorModel from '../models/electorModel.js';
// Agregamos una utilidad para generar tokens cortos y únicos
import { randomBytes, randomUUID } from 'crypto';


// Un objeto temporal para guardar tokens activos (en el futuro esto va a Redis o DB)
const tokensActivos = new Map();

export const habilitarVotante = async (dni, io) => {
  // 1. Generamos un token único de 6-8 caracteres
  const tokenVoto = randomUUID()
  
  // 2. Lo vinculamos al DNI y le damos una expiración (ej. 10 minutos)
  tokensActivos.set(tokenVoto, { dni, expira: Date.now() + 600000 });

  // 3. Emitimos el evento con el TOKEN para que el presidente lo muestre como QR
  io.emit('voto_habilitado', { dni, tokenVoto });
  
  console.log(`Token generado para ${dni}: ${tokenVoto}`);
  return tokenVoto;
};

export const procesarEscaneo = async (req, res, io) => {
  const { dni } = req.body;
  const elector = await ElectorModel.buscarPorDni(dni);

  if (!elector) {
    return res.status(404).json({ mensaje: "Elector no encontrado" });
  }

  // Avisar al Presidente de Mesa vía WebSockets
  io.emit('elector_detectado', elector);
  
  res.json({ mensaje: "Datos enviados al presidente", elector });
};