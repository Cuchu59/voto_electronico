import * as ElectorModel from '../models/electorModel.js';

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