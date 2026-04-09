// src/models/electorModel.js

// Mock de datos para pruebas
const electoresMock = [
  { dni: "12345678", nombre: "Javier", apellido: "Miguel", haVotado: false, antecedentes: false },
  { dni: "87654321", nombre: "Cuchu", apellido: "La Cámpora", haVotado: true, antecedentes: false }
];

export const buscarPorDni = async (dni) => {
  // CUANDO CONECTES AWS: Aquí irá "SELECT * FROM electores WHERE dni = ?"
  const elector = electoresMock.find(e => e.dni === dni);
  return elector || null;
};