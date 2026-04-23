"""
insertar_ciudadanos.py
----------------------
Adaptado para el esquema 'sistema_electoral'.
"""

import argparse
import sys
import re
from datetime import datetime
from pathlib import Path

# ─────────────────────────────────────────────
#  Constantes de validación
# ─────────────────────────────────────────────

IDX_TRAMITE   = 0
IDX_APELLIDO  = 1
IDX_NOMBRE    = 2
IDX_SEXO      = 3
IDX_DNI       = 4
IDX_EJEMPLAR  = 5
IDX_FECHA_NAC = 6
IDX_FECHA_EMI = 7

CAMPOS_ESPERADOS   = 9
SEXOS_VALIDOS      = {"M", "F", "X"}
EJEMPLARES_VALIDOS = {"A", "B", "C", "D", "E"} # Ampliado por si hay ejemplares nuevos
FORMATO_FECHA      = "%d/%m/%Y"  # <--- Cambiado de %Y%m%d a %d/%m/%Y
FORMATO_FECHA_SQL  = "%Y-%m-%d"
REGEX_DNI          = re.compile(r"^\d{7,8}$")
REGEX_TRAMITE      = re.compile(r"^[A-Z0-9]{5,20}$")

def parsear_fecha(valor: str) -> datetime | None:
    try:
        return datetime.strptime(valor.strip(), FORMATO_FECHA)
    except ValueError:
        return None

def parsear_linea(linea: str) -> list[str]:
    return [campo.strip() for campo in linea.split("@")]

def validar_fila(campos: list[str], numero: int) -> list[str]:
    errores = []
    if len(campos) < CAMPOS_ESPERADOS:
        errores.append(f"Línea {numero}: se esperaban {CAMPOS_ESPERADOS} campos, se encontraron {len(campos)}")
        return errores

    sexo = campos[IDX_SEXO].upper()
    dni = campos[IDX_DNI]
    ejemplar = campos[IDX_EJEMPLAR].upper()
    
    if not REGEX_DNI.match(dni):
        errores.append(f"Línea {numero}: DNI inválido '{dni}'")
    if sexo not in SEXOS_VALIDOS:
        errores.append(f"Línea {numero}: sexo inválido '{sexo}'")
    
    fn = parsear_fecha(campos[IDX_FECHA_NAC])
    fe = parsear_fecha(campos[IDX_FECHA_EMI])
    if not fn: errores.append(f"Línea {numero}: fecha_nacimiento inválida")
    if not fe: errores.append(f"Línea {numero}: fecha_emision inválida")

    return errores

def escapar(valor: str) -> str:
    return valor.replace("'", "''")

def generar_insert(campos: list[str]) -> str:
    # Adaptado a la tabla 'ciudadanos' de tu script SQL
    dni      = campos[IDX_DNI]
    tramite  = escapar(campos[IDX_TRAMITE].upper())
    apellido = escapar(campos[IDX_APELLIDO].upper())
    nombre   = escapar(campos[IDX_NOMBRE].upper())
    sexo     = campos[IDX_SEXO].upper()
    ejemplar = campos[IDX_EJEMPLAR].upper()
    
    fn_sql = parsear_fecha(campos[IDX_FECHA_NAC]).strftime(FORMATO_FECHA_SQL)
    fe_sql = parsear_fecha(campos[IDX_FECHA_EMI]).strftime(FORMATO_FECHA_SQL)

    return (
        f"INSERT INTO ciudadanos (dni, numero_tramite, apellidos, nombre, sexo, ejemplar, fecha_nacimiento, fecha_emision) "
        f"VALUES ({dni}, '{tramite}', '{apellido}', '{nombre}', '{sexo}', '{ejemplar}', '{fn_sql}', '{fe_sql}') "
        f"ON CONFLICT (dni) DO NOTHING;"
    )

def procesar_archivo(ruta: Path) -> tuple[list[str], list[str], list[str]]:
    inserts, errores, omitidos = [], [], []
    dnis_vistos = set()

    with ruta.open(encoding="utf-8-sig") as f:
        for i, linea in enumerate(f, start=1):
            linea = linea.strip()
            if not linea: continue
            
            campos = parsear_linea(linea)
            dni = campos[IDX_DNI] if len(campos) > IDX_DNI else ""

            if dni in dnis_vistos:
                omitidos.append(f"Línea {i}: DNI {dni} duplicado")
                continue
            if dni: dnis_vistos.add(dni)

            errs = validar_fila(campos, i)
            if errs:
                errores.extend(errs)
            else:
                inserts.append(generar_insert(campos))
    return inserts, errores, omitidos

def escribir_sql(inserts: list[str], ruta_salida: Path) -> None:
    with ruta_salida.open("w", encoding="utf-8") as f:
        f.write("-- Carga de ciudadanos\n")
        f.write("SET search_path TO public;\n\n")
        f.write("BEGIN;\n\n")
        f.write("\n".join(inserts))
        f.write("\n\nCOMMIT;\n")

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--input", "-i", required=True)
    parser.add_argument("--output", "-o", default="inserts_ciudadanos.sql")
    parser.add_argument("--solo-validos", action="store_true")
    args = parser.parse_args()

    ruta_csv = Path(args.input)
    if not ruta_csv.exists():
        print(f"Error: No existe {ruta_csv}")
        sys.exit(1)

    inserts, errores, omitidos = procesar_archivo(ruta_csv)

    if errores and not args.solo_validos:
        for e in errores: print(e)
        print("\nEjecutá con --solo-validos para ignorar errores.")
        sys.exit(1)

    escribir_sql(inserts, Path(args.output))
    print(f"Finalizado. Generados {len(inserts)} inserts en {args.output}")

if __name__ == "__main__":
    main()