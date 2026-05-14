// src/models/electorModel.js
import pool from '../config/db.js';

export const buscarPorDni = async (dni) => {
  // Aseguramos que el DNI sea numérico
  const dniNumerico = parseInt(dni, 10);

  const query = `
    SELECT 
      c.dni, 
      c.numero_tramite,
      c.apellidos,
      c.nombre, 
      c.sexo,
      c.ejemplar,
      c.fecha_nacimiento,
      c.fecha_emision,
      EXISTS(SELECT 1 FROM registro_asistencia r WHERE r.dni = c.dni) AS "haVotado",
      EXISTS(SELECT 1 FROM antecedentes_penales a WHERE a.dni = c.dni AND a.inhabilitado = TRUE) AS "antecedentes"
    FROM ciudadanos c
    WHERE c.dni = $1;
  `;

  try {
    const res = await pool.query(query, [dniNumerico]);
    
    if (res.rows.length === 0) {
      return null; // Si no hay resultados, devuelve null (como hacía el mock)
    }

    return res.rows[0]; // Devuelve el objeto estructurado
  } catch (error) {
    console.error(`Error consultando elector (DNI: ${dni}):`, error);
    throw error;
  }
};

