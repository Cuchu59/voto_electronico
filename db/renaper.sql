CREATE TABLE ciudadanos (
                            dni               INTEGER      PRIMARY KEY,
                            numero_tramite    INTEGER      NOT NULL,
                            apellidos         VARCHAR(50)  NOT NULL,
                            nombre            VARCHAR(50)  NOT NULL,
                            sexo              CHAR(1)      NOT NULL CHECK (sexo IN ('M', 'F', 'X')),
                            ejemplar          CHAR(1)      NOT NULL,
                            fecha_nacimiento  DATE         NOT NULL,
                            fecha_emision     DATE         NOT NULL,
                            control           VARCHAR(15)
);

-- 2. Antecedentes penales (Inhabilitaciones)
CREATE TABLE antecedentes_penales (
                                      dni           INTEGER      PRIMARY KEY REFERENCES ciudadanos(dni),
                                      descripcion   VARCHAR(200) NOT NULL,
                                      fecha_condena DATE         NOT NULL,
                                      inhabilitado  BOOLEAN      NOT NULL DEFAULT TRUE
);

-- 3. Infraestructura Electoral
CREATE TABLE sedes (
                       id        INTEGER      PRIMARY KEY,
                       nombre    VARCHAR(100) NOT NULL,
                       direccion VARCHAR(200) NOT NULL
);

CREATE TABLE mesas (
                       id_mesa         INTEGER PRIMARY KEY,
                       sede_id         INTEGER NOT NULL REFERENCES sedes(id)
);

-- 4. Padrón Electoral (Asignación estática)
CREATE TABLE habilitados (
                             dni             INTEGER PRIMARY KEY REFERENCES ciudadanos(dni),
                             id_mesa         INTEGER NOT NULL REFERENCES mesas(id_mesa),
                             numero_de_orden INTEGER NOT NULL
);

-- 5. Oferta Electoral
CREATE TABLE candidatos (
                            numero_de_lista      INTEGER      PRIMARY KEY,
                            nombre_del_partido   VARCHAR(100) NOT NULL,
                            nombre_del_candidato VARCHAR(100) NOT NULL
);

---
--- PROCESO ELECTORAL (Tablas desacopladas para garantizar anonimato)
---

-- Registro de ASISTENCIA: Solo para saber quién ya votó y evitar fraude.
-- No se vincula con la tabla de resultados.
CREATE TABLE registro_asistencia (
                                     dni             INTEGER PRIMARY KEY REFERENCES ciudadanos(dni),
                                     fecha_voto      TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Conteo de VOTOS: Acumulador por mesa y lista.
-- El voto en blanco se representa con numero_de_lista como NULL.
CREATE TABLE resultados_mesa (
                                 id_mesa          INTEGER NOT NULL REFERENCES mesas(id_mesa),
                                 numero_de_lista  INTEGER NOT NULL REFERENCES candidatos(numero_de_lista),
                                 cantidad_votos   INTEGER NOT NULL DEFAULT 0,
                                 PRIMARY KEY (id_mesa, numero_de_lista)
);