// src/controllers/electorController.js
import * as ElectorModel from '../models/electorModel.js';
import { parsearStringDni } from '../utils/parsearDni.js';
import { randomBytes, randomUUID } from 'crypto';

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
  const { rawScan } = req.body; 

  // 1. Parsear
  const datosDni = parsearStringDni(rawScan);
  if (!datosDni) {
    return res.status(400).json({ mensaje: "Formato de DNI inválido" });
  }

  // 2. Buscar en DB solo por DNI
  const elector = await ElectorModel.buscarPorDni(datosDni.dni);
  if (!elector) {
    return res.status(404).json({ mensaje: "Elector no encontrado" });
  }

  // 3. Contrastar datos del documento físico vs DB
  const discrepancias = contrastarDatos(datosDni, elector);

  // 4. Emitir al presidente con toda la info
  io.emit('elector_detectado', {
    elector,
    documentoValido: discrepancias.length === 0,
    discrepancias,   // [] si todo ok, o lista de campos que no coinciden
  });

  res.json({ 
    mensaje: "Datos enviados al presidente", 
    elector,
    documentoValido: discrepancias.length === 0,
    discrepancias,
  });
};

// Compara lo escaneado contra lo que está en la DB
const contrastarDatos = (escaneado, dbRecord) => {
  const discrepancias = [];

  if (escaneado.numero_tramite !== dbRecord.numero_tramite) {
    discrepancias.push('numero_tramite');
  }
  if (escaneado.apellidos.toUpperCase() !== dbRecord.apellidos.toUpperCase()) {
    discrepancias.push('apellidos');
  }
  if (escaneado.nombre.toUpperCase() !== dbRecord.nombre.toUpperCase()) {
    discrepancias.push('nombre');
  }
  if (escaneado.sexo !== dbRecord.sexo) {
    discrepancias.push('sexo');
  }
  if (escaneado.ejemplar !== dbRecord.ejemplar) {
    discrepancias.push('ejemplar');
  }


  // En contrastarDatos — las fechas DATE de Postgres llegan como objeto Date en UTC
  // Usamos UTC para evitar que el offset horario cambie el día
  const normalizarFecha = (f) => {
    if (!f) return '';
    if (typeof f === 'string') {
      // Ya viene como "DD/MM/YYYY" del scanner
      if (f.includes('/')) return f;
      // Por si acaso llega como ISO "2005-08-16"
      const [y, m, d] = f.split('-');
      return `${d}/${m}/${y}`;
    }
    // Es un objeto Date de Postgres — usar UTC para no correr el día
    const d = String(f.getUTCDate()).padStart(2, '0');
    const m = String(f.getUTCMonth() + 1).padStart(2, '0');
    const y = f.getUTCFullYear();
    return `${d}/${m}/${y}`;
  };

  if (normalizarFecha(escaneado.fecha_nacimiento) !== normalizarFecha(dbRecord.fecha_nacimiento)) {
    discrepancias.push('fecha_nacimiento');
  }
  if (normalizarFecha(escaneado.fecha_emision) !== normalizarFecha(dbRecord.fecha_emision)) {
    discrepancias.push('fecha_emision');
  }

  return discrepancias;
};