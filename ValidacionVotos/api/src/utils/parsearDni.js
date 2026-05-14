// src/utils/parsearDni.js
export const parsearStringDni = (raw) => {
  const partes = raw.trim().split('@');
  if (partes.length < 8) return null;

  return {
    numero_tramite:   parseInt(partes[0], 10),  
    apellidos:        partes[1],
    nombre:           partes[2],
    sexo:             partes[3],
    dni:              parseInt(partes[4], 10),
    ejemplar:         partes[5],
    fecha_nacimiento: partes[6], 
    fecha_emision:    partes[7],
  };
};