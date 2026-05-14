-- Carga de ciudadanos
SET search_path TO public;

BEGIN;

INSERT INTO ciudadanos (dni, numero_tramite, apellidos, nombre, sexo, ejemplar, fecha_nacimiento, fecha_emision) VALUES (46483866, 00670655129, 'RECIO OJEDA', 'MATIAS', 'M', 'A', '2005-08-16', '2021-08-16') ON CONFLICT (dni) DO NOTHING;
INSERT INTO ciudadanos (dni, numero_tramite, apellidos, nombre, sexo, ejemplar, fecha_nacimiento, fecha_emision) VALUES (46991423, 00624714941, 'CESIO', 'FERNANDO JOSE', 'M', 'A', '2005-12-05', '2020-01-13') ON CONFLICT (dni) DO NOTHING;
INSERT INTO ciudadanos (dni, numero_tramite, apellidos, nombre, sexo, ejemplar, fecha_nacimiento, fecha_emision) VALUES (47170977, 00169165392, 'AMODEI', 'PAZ MARIA', 'F', 'A', '2006-03-06', '2013-02-21') ON CONFLICT (dni) DO NOTHING;
INSERT INTO ciudadanos (dni, numero_tramite, apellidos, nombre, sexo, ejemplar, fecha_nacimiento, fecha_emision) VALUES (47091856, 00647158271, 'ZAMPELLA', 'LUCIO', 'M', 'A', '2005-11-23', '2021-06-09') ON CONFLICT (dni) DO NOTHING;
INSERT INTO ciudadanos (dni, numero_tramite, apellidos, nombre, sexo, ejemplar, fecha_nacimiento, fecha_emision) VALUES (47335086, 00652152155, 'AGUILAR', 'THIAGO AGUSTIN', 'M', 'A', '2006-05-12', '2021-03-10') ON CONFLICT (dni) DO NOTHING;
INSERT INTO ciudadanos (dni, numero_tramite, apellidos, nombre, sexo, ejemplar, fecha_nacimiento, fecha_emision) VALUES (96243966, 00735180800, 'ROSALES TORRES', 'MARIA JOSE', 'F', 'B', '2002-02-13', '2025-05-20') ON CONFLICT (dni) DO NOTHING;
INSERT INTO ciudadanos (dni, numero_tramite, apellidos, nombre, sexo, ejemplar, fecha_nacimiento, fecha_emision) VALUES (46565876, 00607378657, 'MAINE', 'FRANCISCO', 'M', 'A', '2005-07-08', '2019-08-28') ON CONFLICT (dni) DO NOTHING;
INSERT INTO ciudadanos (dni, numero_tramite, apellidos, nombre, sexo, ejemplar, fecha_nacimiento, fecha_emision) VALUES (47128310, 00650724330, 'BREGOLI', 'TOMAS', 'M', 'A', '2006-05-04', '2021-02-26') ON CONFLICT (dni) DO NOTHING;
INSERT INTO ciudadanos (dni, numero_tramite, apellidos, nombre, sexo, ejemplar, fecha_nacimiento, fecha_emision) VALUES (26118751, 00136072682, 'MAINE', 'MARTIN ARIEL', 'M', 'A', '1977-09-09', '2012-09-13') ON CONFLICT (dni) DO NOTHING;
INSERT INTO ciudadanos (dni, numero_tramite, apellidos, nombre, sexo, ejemplar, fecha_nacimiento, fecha_emision) VALUES (47076977, 00641995673, 'GARCIA', 'IGNACIO', 'M', 'A', '2006-04-19', '2020-11-28') ON CONFLICT (dni) DO NOTHING;
INSERT INTO ciudadanos (dni, numero_tramite, apellidos, nombre, sexo, ejemplar, fecha_nacimiento, fecha_emision) VALUES (46917267, 00702077082, 'CAPORASO', 'TOMAS MARTIN', 'M', 'B', '2006-01-27', '2023-03-14') ON CONFLICT (dni) DO NOTHING;

INSERT INTO sedes (id, nombre, direccion) VALUES
                                              (1, 'Universidad Austral', 'Mariano Acosta 1611, Pilar'),
                                              (2, 'Club Social y Deportivo Don Torcuato', 'Estrada 550, Tigre'),
                                              (3, 'Universidad Nacional de Córdoba', 'Av. Haya de la Torre, Córdoba'),
                                              (4, 'Sociedad de Fomento San Martín', 'Av. Colón 123, Mar del Plata');

INSERT INTO mesas (id_mesa, sede_id) VALUES
                                                           (1001, 1),
                                                           (1002, 1),
                                                           (1003, 2),
                                                           (1004, 3),
                                                           (1005, 4);

INSERT INTO candidatos (numero_de_lista, nombre_del_partido, nombre_del_candidato) VALUES
                                                                                       (0, 'Voto en blanco', 'Voto en blanco'),
                                                                                       (10, 'La Libertad Avanza', 'Javier Milei'),
                                                                                       (20, 'Frente de Todos', 'Sergio Massa'),
                                                                                       (30, 'Frente de Izquierda', 'Nicolas del Caño');



INSERT INTO antecedentes_penales (dni, descripcion, fecha_condena, inhabilitado)  VALUES
                                                                            (46917267, 'Robo a mano armada', '1936-09-1', False),
                                                                            (47335086, 'Secuestro de personas', '1810-05-25', False)
                                                                           ;
INSERT INTO habilitados (dni, id_mesa, numero_de_orden) VALUES
                                                            (46483866, 1001, 1),
                                                            (46991423, 1001, 2),
                                                            (47170977, 1001, 3),
                                                            (47335086, 1001, 4),
                                                            (96243966, 1001, 5),
                                                            (46565876, 1001, 6),
                                                            (47128310, 1001, 7),
                                                            (26118751, 1001, 8),
                                                            (47076977, 1001, 9),
                                                            (46917267, 1001, 10),

                                                            (47091856, 1003, 1)
;


COMMIT;
