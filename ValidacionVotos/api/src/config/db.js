import pg from 'pg';
import dotenv from 'dotenv';

// Solo necesitas llamar a config() una vez usando la instancia importada
dotenv.config();

const { Pool } = pg;

const pool = new Pool({
  user: process.env.DB_USER,
  password: process.env.DB_PASSWORD,
  host: process.env.DB_HOST,
  port: process.env.DB_PORT,
  database: process.env.DB_NAME
});

pool.on('connect', () => {
  console.log(`🔗 Conectado a PostgreSQL en AWS (${process.env.DB_HOST})`);
});

pool.on('error', (err) => {
  console.error('❌ Error inesperado en el cliente de la base de datos', err);
});

export default pool;